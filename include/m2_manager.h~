// MetroII manager, controlling the execution 
// Authors: Abhijit Davare, Guang Yang, Trevor Meyerowitz, Qi Zhu

#ifndef M2_MANAGER_H
#define M2_MANAGER_H

//******************************************************************************
// Required Header Files
//******************************************************************************
#include "m2_base.h"
#include "m2_debug.h"
#include "m2_event.h"
#include "m2_component.h"
#include "m2_interface.h"
#include "m2_ports.h"
#include "m2_constraints.h"
#include "m2_ann_sched.h"

namespace m2_core { //begin namespace m2_core 

    //******************************************************************************
    // Manager of the system, handles switching between phases
    //******************************************************************************
    // does not handle multiple proposed events per process and terminating processes
    class m2_manager : public sc_module 
    {
        std::vector <m2_event *> events; 

        int procs_ready_to_switch;
        int total_procs;
        int total_adaptors;
        sc_event e_activate_manager;

      public:

        m2_constraint_solver* c_solver;
        std::vector <m2_annotator *> annotator_list;
        std::vector <m2_scheduler *> scheduler_list;

        SC_HAS_PROCESS(m2_manager);

        m2_manager(sc_module_name name)
        {
            procs_ready_to_switch = 0;
            total_adaptors = 0;

            c_solver = new m2_constraint_solver();

#if SWITCH_PHASES == 1   
            SC_THREAD(main);
#endif

        }

        virtual const char* kind() const
        {
            return "m2_manager";
        }

        void set_number_of_processes_in_system(int p)
        {
            total_procs = p;
        }

        void increment_num_adaptors()
        {
            total_adaptors++;
        }

        void propose_events(m2_event& e)
        {
#if SWITCH_PHASES == 1

            M2_DEBUG2("Propose Event: " << e.get_full_name());
            e.set_status(M2_EVENT_PROPOSED);
            events.push_back(&e);
            increment_procs_ready_to_switch();
            wait(e);

#endif
        }

        void increment_procs_ready_to_switch()
        {
#if SWITCH_PHASES == 1

            procs_ready_to_switch++;
            M2_DEBUG2("Procs ready to switch incremented to " << procs_ready_to_switch);
            if (procs_ready_to_switch == total_procs)
            {
                e_activate_manager.notify();
            }

#endif
        }

        void decrement_procs_ready_to_switch()
        {
#if SWITCH_PHASES == 1
            M2_DEBUG2("Procs ready to switch decremented to " << procs_ready_to_switch);
            procs_ready_to_switch--;
#endif
        }

        void decrement_total_procs()
        {
            total_procs--;
            cout << "Total processes " << total_procs << endl;
            if (total_procs == total_adaptors)
                sc_stop();
            M2_DEBUG2("Total procs decremented to " << total_procs);
            if (procs_ready_to_switch == total_procs)
            {
                e_activate_manager.notify();
            }

        }

        m2_constraint_solver* get_constraint_solver()
        {
            return c_solver;
        }

        void add_constraint_solver(m2_constraint_solver* _c_solver)
        {
            c_solver = _c_solver; 
        }

        void add_annotator(m2_annotator* _annotator)
        {
            annotator_list.push_back(_annotator);
        }

        void add_scheduler(m2_scheduler* _scheduler)
        {
            scheduler_list.push_back(_scheduler);
        }

        void main()
        {
            std::vector<m2_event *> tmp_events;
            while(true)
            {
                M2_DEBUG1("------------- Start simulation iteration --------------");

                M2_DEBUG3("# of events: " << events.size());

                // phase 1: base model execution
                M2_DEBUG1("Phase1: Base Model Execution");
                wait(e_activate_manager); // wait to switch

                // phase 2: annotation
                M2_DEBUG1("Phase2: Annotation");
                for (unsigned i = 0; i < annotator_list.size(); i++)
                    annotator_list[i]->annotate();

                // phase 3: constraint resolution
                bool statusChange = true;
                std::vector<char> eventStatus;
                while (statusChange)
                {
                    M2_DEBUG3("testing status change...");

                    statusChange = false;

                    // phase 3: constraint solver
                    M2_DEBUG1("Phase3.1: Constraint Solving");
                    c_solver->resolve();

                    if (!c_solver->is_stable())
                    {
                        statusChange = true;
                    }

                    // phase 3: schedulers
                    M2_DEBUG1("Phase3.2: Scheduling");
                    for (unsigned i = 0; i < scheduler_list.size(); i++) {
                        scheduler_list[i]->schedule();
                        if (!scheduler_list[i]->is_stable())
                        {
                            statusChange = true;
                        }
                    }					
                }

                // post_schedule of the schedulers
                for (unsigned i = 0; i < scheduler_list.size(); i++) {
                    scheduler_list[i]->post_schedule();
                }
                
                // post_resolve of the solvers should be after schedulers if for instance
                // the time annotation need to be passed between sync. events
                c_solver->post_resolve();

                // phase 3: enable/disable events
                M2_DEBUG1("Phase3.3: Enable/disable events");
                tmp_events.clear();

                M2_DEBUG3("# of events after phases: " << events.size());

                for (unsigned i = 0; i < events.size(); i++)
                {
                    if (events[i]->get_status() == (char)M2_EVENT_PROPOSED) {
                        events[i]->set_status((char)M2_EVENT_INACTIVE);
                        events[i]->notify(SC_ZERO_TIME);
                        procs_ready_to_switch--;
                    }
                    else {
                        tmp_events.push_back(events[i]);
                    }
                    M2_DEBUG2("event " << events[i]->get_full_name() << " status " << events[i]->string_status());
                }

                events.clear();
                events = tmp_events;

                M2_DEBUG1("------------- End simulation iteration --------------");
            }
        }
    };

    //******************************************************************************
    // Macros that hide all of the complexity
    //******************************************************************************

#define M2_COMPONENT(user_module_name)      \
    class user_module_name : public m2_component

#define M2_INTERFACE(interface) \
    class interface : public m2_interface

#define M2_CONNECT(req_obj, req_port, prov_obj, prov_port) \
    (req_obj).req_port.connect((typeof ((req_obj).req_port.getThisInterface())) &(prov_obj)); \
    (prov_obj).prov_port.connect((typeof ((prov_obj).prov_port.getThisInterface())) &(req_obj));

    //Limitation:
    //      1. restrict to single function argument and single return value
    //      2. cannot use void as return value
    //
    // Possible solutions to the M2_FUNCTION macro that may solve limitation 1.
    //#define M2_FUNCTION(return_type, name, ...) name(__VA_ARGS__)
    //#define M2_FUNCTION(return_type, name, argument_type ...) name(argument_type)
    // limitation 2 is still open

#define make_string(s) #s

#define M2_MAP(func_component, func_method, arch_component, arch_method) \
    m2_mapping_constraint* func_component##func_method##arch_component##arch_method##cons1 = new m2_mapping_constraint(func_component.func_method##_event_beg, arch_component.arch_method##_event_beg);\
    m2_mapping_constraint* func_component##func_method##arch_component##arch_method##cons2 = new m2_mapping_constraint(func_component.func_method##_event_end, arch_component.arch_method##_event_end);\
    mapping_constraints->addConstraint(func_component##func_method##arch_component##arch_method##cons1);\
    mapping_constraints->addConstraint(func_component##func_method##arch_component##arch_method##cons2);

// new version of M2_MAP
// identify the events also by port name
#define M2_MAP2(func_component, func_event_beg, func_event_end, arch_component, arch_event_beg, arch_event_end) \
    m2_mapping_constraint* func_component##func_event_beg##arch_component##arch_event_beg= \
	    new m2_mapping_constraint(func_component.func_event_beg, arch_component.arch_event_beg);\
    m2_mapping_constraint* func_component##func_event_end##arch_component##arch_event_end= \
	    new m2_mapping_constraint(func_component.func_event_end, arch_component.arch_event_end);\
    mapping_constraints->addConstraint(func_component##func_event_beg##arch_component##arch_event_beg);\
    mapping_constraints->addConstraint(func_component##func_event_end##arch_component##arch_event_end);

#define M2_NOARG_FUNCTION(return_type, name) \
    virtual return_type name() { \
        return_type __ret; \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        if (get_connected_obj()) __ret = ((typeof this)get_connected_obj())->name(); \
        else __ret = 0;\
        manager.propose_events(name(M2_EVENT_END)); \
        return __ret; \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

#define M2_ONEARG_FUNCTION(return_type, name, argument_type1) \
    virtual return_type name(argument_type1 __arg1) { \
        return_type __ret; \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        if (get_connected_obj()) __ret = ((typeof this)get_connected_obj())->name(__arg1); \
        else __ret = 0;\
        manager.propose_events(name(M2_EVENT_END)); \
        return __ret; \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };
	
//this is a function with unlimited number of passed arguments with one mandatory type element of type	argument_type1
#define M2_FUNCTION(return_type, name, argument_type1, ...) \
    virtual return_type name(argument_type1 __arg1, ...) { \
	return_type __ret; \
	va_list pointer;\
	va_start(pointer, __arg1);\
     __ret = name(__arg1, pointer);   \
	 va_end(pointer);\
        return __ret; \
    }; \
    \
	virtual return_type name(argument_type1 __arg1, va_list & listPointer) { \
        return_type __ret; \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
		\
        if (get_connected_obj()) __ret = ((typeof this)get_connected_obj())->name(__arg1, listPointer); \
        else __ret = 0;\
		\
        manager.propose_events(name(M2_EVENT_END)); \
        return __ret; \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };	

#define M2_NOARG_PROCEDURE(name) \
    virtual void name() { \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        if (get_connected_obj()) ((typeof this)get_connected_obj())->name(); \
        manager.propose_events(name(M2_EVENT_END)); \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

#define M2_ONEARG_PROCEDURE(name, argument_type1) \
    virtual void name(argument_type1 __arg1) { \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        if (get_connected_obj()) ((typeof(this))get_connected_obj())->name(__arg1); \
        manager.propose_events(name(M2_EVENT_END)); \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

#define M2_TWOARG_PROCEDURE(name, argument_type1, argument_type2) \
    virtual void name(argument_type1 __arg1, argument_type2 __arg2) { \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        if (get_connected_obj()) ((typeof(this))get_connected_obj())->name(__arg1, __arg2); \
        manager.propose_events(name(M2_EVENT_END)); \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

    // _EVENT for test, to be remove

#define M2_TWOARG_PROCEDURE_EVENT(name, argument_type1, argument_type2) \
    virtual void name(argument_type1 __arg1, argument_type2 __arg2) { \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        m2_event_info* tmp = new m2_event_info(name(M2_EVENT_BEGIN));\
        if (get_connected_obj()) ((typeof(this))get_connected_obj())->name(__arg1, __arg2); \
        tmp->copy_info_to_event(name(M2_EVENT_END));\
        manager.propose_events(name(M2_EVENT_END)); \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

#define M2_THREEARG_PROCEDURE(name, argument_type1, argument_type2, argument_type3) \
    virtual void name(argument_type1 __arg1, argument_type2 __arg2, argument_type3 __arg3) { \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        if (get_connected_obj()) ((typeof(this))get_connected_obj())->name(__arg1, __arg2, __arg3); \
        manager.propose_events(name(M2_EVENT_END)); \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

#define M2_THREEARG_PROCEDURE_EVENT(name, argument_type1, argument_type2, argument_type3) \
    virtual void name(argument_type1 __arg1, argument_type2 __arg2, argument_type3 __arg3) { \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        m2_event_info* tmp = new m2_event_info(name(M2_EVENT_BEGIN));\
        if (get_connected_obj()) ((typeof(this))get_connected_obj())->name(__arg1, __arg2, __arg3); \
        tmp->copy_info_to_event(name(M2_EVENT_END));\
        manager.propose_events(name(M2_EVENT_END)); \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

#define M2_FOURARG_PROCEDURE(name, argument_type1, argument_type2, argument_type3, argument_type4) \
    virtual void name(argument_type1 __arg1, argument_type2 __arg2, argument_type3 __arg3, argument_type4 __arg4) { \
        manager.propose_events(name(M2_EVENT_BEGIN)); \
        if (get_connected_obj()) ((typeof this)get_connected_obj())->name(__arg1, __arg2, __arg3, __arg4); \
        manager.propose_events(name(M2_EVENT_END)); \
    }; \
    \
    virtual m2_event& name(M2_Event_Types _type, char* event_name = NULL, sc_process_handle thread = \
            sc_get_current_process_handle()) { \
        thread_event_map_t* temap; \
        if (__m2_func_event_map.find(#name) == __m2_func_event_map.end()) { \
            temap = new thread_event_map_t; \
            __m2_func_event_map[#name] = temap; \
        } else { \
            temap = __m2_func_event_map[#name]; \
        } \
        event_pair_t * ep; \
        if (temap->find(thread) == temap->end()) { \
	    char* event_beg_name; \
	    char* event_end_name; \
	    if (event_name != NULL) { \
		    STR_CAT(event_beg_name, event_name, "_b"); \
		    STR_CAT(event_end_name, event_name, "_e"); \
            } \
	    else { \
		    STR_CAT(event_beg_name, #name, "_b"); \
		    STR_CAT(event_end_name, #name, "_e"); \
            } \
            ep = new event_pair_t(new m2_event(event_beg_name), \
                    new m2_event(event_end_name));\
            (*temap)[thread] = ep; \
        } else { \
            ep = (*temap)[thread]; \
        } \
        return (_type == M2_EVENT_BEGIN)? \
        *(ep->first) : *(ep->second); \
    };

    extern m2_manager manager;
    extern void scan_hierarchy(std::vector<m2_component *> *, sc_object * );
    extern void m2_start();
    extern void m2_wait( const sc_event &, sc_simcontext* = sc_get_curr_simcontext());
    extern void m2_wait(double, sc_time_unit); //new wait 
    extern void m2_wait(sc_event_or_list&); //new wait 
	extern void m2_wait(sc_event_and_list&);
    extern void m2_wait();
    extern void register_constraint_solver(m2_constraint_solver* _c_solver);
    extern void register_annotator(m2_annotator* _annotator);
    extern void register_scheduler(m2_scheduler* _scheduler);
    extern void m2_end(sc_process_handle proc);



} //end namespace m2_core 

#endif
