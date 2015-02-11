// Definitions and libraries of constraints and constraint solver, including
// rendezvous constraint and mapping constraints
// Author: Qi Zhu

#ifndef M2_CONSTRAINTS_H
#define M2_CONSTRAINTS_H

#include "m2_base.h"
#include "m2_event.h"


namespace m2_core { // begin namespace m2_core 

    enum M2_Constraint_Types
    {
        M2_LTL_CONSTRAINT,
        M2_LOC_CONSTRAINT,
        M2_RENDEZ_CONSTRAINT,
        UNKNOWN
    };


    //**************************************************************
    // MetroII constraints base class 
    //**************************************************************    
    class m2_constraint
    {
      protected:
        const char* _name;
        M2_Constraint_Types _type;

      public:
        m2_constraint()
        {
            _name = "unknown";
            _type = UNKNOWN;
        }

        m2_constraint(M2_Constraint_Types type)
        {
            _type = type;
        }

        m2_constraint(const char* name, M2_Constraint_Types type)
        {
            _name = name;
            _type = type;
        }
        virtual ~m2_constraint()
        {
        }

        virtual bool isSatisfied() = 0; 
        virtual void solveConstraint() {};
        virtual bool is_stable() = 0;
        virtual void post_resolve() = 0;
    };


    //**************************************************************
    // MetroII rendezvous constraints 
    //**************************************************************    
    class m2_rendez_constraint : public m2_constraint
    {
      protected:
        m2_event *_m1, *_m2;
        bool stable;

      public:
        m2_rendez_constraint(m2_event* m1, m2_event* m2)
            : m2_constraint(M2_RENDEZ_CONSTRAINT)
        {
            _m1 = m1;
            _m2 = m2;
            stable = true;
        }

        m2_rendez_constraint(const char* name, m2_event* m1, m2_event* m2)
            : m2_constraint(name, M2_RENDEZ_CONSTRAINT)
        {
            _m1 = m1;
            _m2 = m2;
            stable = true;
        }

        bool isSatisfied()
        {
            M2_DEBUG3("mapping constr: " << _m1->get_full_name() << " " << _m1->string_status() 
                    << " and" << _m2->get_full_name() << " " << _m2->string_status());
            return (((_m1->get_status() == (char)M2_EVENT_PROPOSED) || (_m1->get_status() == (char)M2_EVENT_WAITING))
                    && ((_m2->get_status() == (char)M2_EVENT_PROPOSED) || (_m2->get_status() == (char)M2_EVENT_WAITING)));
        }

        void solveConstraint()
        {
            char tmp1 = (char)_m1->get_status();
            char tmp2 = (char)_m2->get_status();

            if (isSatisfied()) {
                _m1->set_status((char)M2_EVENT_PROPOSED);
                _m2->set_status((char)M2_EVENT_PROPOSED);
            }
            else {
                if ((_m1->get_status() == (char)M2_EVENT_PROPOSED)
                        || (_m1->get_status() == (char)M2_EVENT_WAITING))
                {
                    _m1->set_status((char)M2_EVENT_DISABLED);
                }
                if ((_m2->get_status() == (char)M2_EVENT_PROPOSED)
                        || (_m2->get_status() == (char)M2_EVENT_WAITING))
                {
                    _m2->set_status((char)M2_EVENT_DISABLED);
                }
            }

            if ((tmp1 == (char)_m1->get_status()) && (tmp2 == (char)_m2->get_status()))
            {
                stable = true;
            }
            else {
                stable = false;
            }
        }

        bool is_stable()
        {
            return stable;		
        }

        void post_resolve()
        {
            if (_m1->get_status() == (char)M2_EVENT_DISABLED)
            {
                _m1->set_status((char)M2_EVENT_WAITING);
            }
            if (_m2->get_status() == (char)M2_EVENT_DISABLED)
            {
                _m2->set_status((char)M2_EVENT_WAITING);
            }
        }
    };

    //**************************************************************
    // MetroII mapping constraints 
    //**************************************************************    
    class m2_mapping_constraint : public m2_rendez_constraint
    {
      public:
        m2_mapping_constraint(m2_event* m1, m2_event* m2)
            : m2_rendez_constraint(m1, m2)
        {
        }

        m2_mapping_constraint(const char* name, m2_event* m1, m2_event* m2)
            : m2_rendez_constraint(name, m1, m2)
        {
        }

        void solveConstraint()
        {
            char tmp1 = (char)_m1->get_status();
            char tmp2 = (char)_m2->get_status();

            if (isSatisfied()) {
                _m1->set_status((char)M2_EVENT_PROPOSED);
                _m2->set_status((char)M2_EVENT_PROPOSED);
           }
            else {
                if ((_m1->get_status() == (char)M2_EVENT_PROPOSED)
                        || (_m1->get_status() == (char)M2_EVENT_WAITING))
                {
                    _m1->set_status((char)M2_EVENT_DISABLED);
                }
                if ((_m2->get_status() == (char)M2_EVENT_PROPOSED)
                        || (_m2->get_status() == (char)M2_EVENT_WAITING))
                {
                    _m2->set_status((char)M2_EVENT_DISABLED);
                }
            }

            if ((tmp1 == (char)_m1->get_status()) && (tmp2 == (char)_m2->get_status()))
            {
                stable = true;
            }
            else {
                stable = false;
            }
        }

        void post_resolve()
        {
            m2_rendez_constraint::post_resolve();
            if ((_m1->get_status() == (char)M2_EVENT_PROPOSED)
                    && (_m2->get_status() == (char)M2_EVENT_PROPOSED))
            {
                // tentative way to represent value passing
                // need more sophisticated data structure for values, including
                // the notion of nondeterminism
                if ((_m1->val == NONDET) && (_m2->val != NONDET))
                {
                    _m1->val = _m2->val;
                }
                else if ((_m1->val != NONDET) && (_m2->val == NONDET))
                {
                    _m2->val = _m1->val;
                }
            }
        }
    };

    //**************************************************************
    // MetroII constraint solver 
    //**************************************************************    
    class m2_constraint_solver
    {
      private:
        std::vector<m2_constraint *> _constraint_list;

      public:
        m2_constraint_solver()
        {
        }

        m2_constraint_solver(const std::vector<m2_constraint *> constraint_list)
        {
            m2_constraint_solver();
            _constraint_list = constraint_list;
        }

        void addConstraint(m2_constraint* c)
        {
            _constraint_list.push_back(c);
        }

        void resolve()
        {
            for (unsigned i = 0; i < _constraint_list.size(); i ++)
            {
                _constraint_list[i]->solveConstraint();
            }
        }

        bool is_stable()
        {
            bool stable = true;
            for (unsigned i = 0; i < _constraint_list.size(); i ++)
            {
                if (!_constraint_list[i]->is_stable())
                {
                    stable = false;
                }
            }

            return stable;
        }

        void post_resolve()
        {
            for (unsigned i = 0; i < _constraint_list.size(); i ++)
            {
                _constraint_list[i]->post_resolve();
            }		
        }
    };

} // end namespace m2_core

#endif
