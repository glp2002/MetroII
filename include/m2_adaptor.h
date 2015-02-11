// Base classes and libraries of adaptor and adaptor channel, 
// including trivial copy adaptor and adaptors between DF and FSM semantics
// Author: Qi Zhu

#ifndef M2_ADAPTOR_H
#define M2_ADAPTOR_H

#include "m2_base.h"
#include "m2_event.h"
#include "m2_component.h"
#include "m2_ports.h"
#include "m2_manager.h"

#include <deque>
#include <time.h>

namespace m2_core{

    class i_ac_write : public m2_interface
    {
      public:
        M2_ONEARG_PROCEDURE(write_event_info, m2_event_info*); // interface for components
        virtual void write_event_info_direct(m2_event_info* e_info)
        {
            // interface for adaptor (without proposing events)
            if (get_connected_obj())
                ((typeof(this))get_connected_obj())->write_event_info(e_info);
        }
    };

    class i_ac_read : public m2_interface
    {
      public:
        M2_NOARG_FUNCTION(m2_event_info*, read_event_info);	// interface for components
        virtual m2_event_info* read_event_info_direct()
        {
            // interface for adaptor
            m2_event_info* ret;
            if (get_connected_obj())
                ret = ((typeof(this))get_connected_obj())->read_event_info();
            else
                ret = NULL;
            return ret;
        }
    };

    class adaptor_channel : public i_ac_write, public i_ac_read
    {
      protected:
        int maxSize;
        std::deque<m2_event_info*> event_info_list;

      public:
        m2_provided_port<i_ac_write> write_port;
        m2_provided_port<i_ac_read> read_port;

        adaptor_channel()
        {
            maxSize = -1;
        }

        adaptor_channel(int _maxSize)
        {
            maxSize = _maxSize;
        }

        void write_event_info(m2_event_info* e_info)
        {
            // If channel is full, the event will be lost.
            // Another possibility is the last event in the channel will be
            // overwritten. 
            if ((int)event_info_list.size() == maxSize)
            {
                M2_DEBUG1("event channel is full while writing");
                return;
            }
            M2_DEBUG1("write event to ac channel with tag " << e_info->tag);
            event_info_list.push_back(e_info);
        }

        m2_event_info* read_event_info()
        {
            if (event_info_list.empty()){
                M2_DEBUG1("event channel is empty while reading");
                return NULL;
            }
            else {
                m2_event_info* tmp = event_info_list.front();
                event_info_list.pop_front();
                M2_DEBUG1("read event from ac channel with tag " << tmp->tag);
                return tmp;
            }
        }

    };

    class adaptor : public m2_component
    {
      protected:
        std::deque<m2_event_info*> internal_event_info_list;

      public:
        m2_required_port<i_ac_write> write_port;
        m2_required_port<i_ac_read> read_port;

        sc_process_handle this_thread;

        SC_HAS_PROCESS(adaptor);

        adaptor(sc_module_name n) : m2_component(n)
        {
            SC_THREAD(main);
        }

        virtual void read_events() = 0;

        virtual void write_events() = 0;

        virtual void transform_events() = 0;

        void main()
        {
            manager.increment_num_adaptors();
            while (true)
            {
                m2_event e;
                manager.propose_events(e);                
                read_events();
                transform_events();
                write_events();
            }    
        }
    }; 

    class copy_adaptor : public adaptor
    {
      public:
        copy_adaptor(sc_module_name n) : adaptor(n)
        {
        }

        void read_events()
        {
            M2_DEBUG1("-----read events in adaptor-----");
            m2_event_info* tmp;
            tmp = read_port->read_event_info_direct(); 
            while (tmp != NULL)
            {
                internal_event_info_list.push_back(tmp);
                tmp = read_port->read_event_info_direct();
            }
            M2_DEBUG1("-----end of read events in adaptor-----");
        }

        void transform_events()
        {
            M2_DEBUG1("-----transform events in adaptor-----");
        }

        void write_events()
        {
            M2_DEBUG1("-----write events in adaptor-----");
            while (!internal_event_info_list.empty())
            {
                write_port->write_event_info_direct(internal_event_info_list.front());
                internal_event_info_list.pop_front();
            }            
            M2_DEBUG1("-----end of write events in adaptor-----");
        }
    };

    class df_fsm_adaptor : public adaptor
    {
      public:
        df_fsm_adaptor(sc_module_name n) : adaptor(n)
        {
            timeTag = 0;
            range = 10;
            srand( time(NULL) );
        }

        df_fsm_adaptor(sc_module_name n, int _range) : adaptor(n)
        {
            timeTag = 0;
            range = _range;
            srand( time(NULL) );
        }

        void set_range(int _range)
        {
            range = _range;
        }

        void read_events()
        {
            M2_DEBUG1("-----read events in adaptor-----");
            m2_event_info* tmp;
            tmp = read_port->read_event_info_direct(); 
            while (tmp != NULL)
            {
                internal_event_info_list.push_back(tmp);
                tmp = read_port->read_event_info_direct();
            }
            M2_DEBUG1("-----end of read events in adaptor-----");
        }

        void transform_events()
        {
            M2_DEBUG1("-----transform events in adaptor-----");
            for (unsigned int i=0; i<internal_event_info_list.size(); i++)
            {
                int addTime = rand() % range + 1;
                timeTag += addTime;
                internal_event_info_list[i]->tag = timeTag;
            }
        }

        void write_events()
        {
            M2_DEBUG1("-----write events in adaptor-----");
            while (!internal_event_info_list.empty())
            {
                write_port->write_event_info_direct(internal_event_info_list.front());
                internal_event_info_list.pop_front();
            }            
            M2_DEBUG1("-----end of write events in adaptor-----");
        }

      protected:

        int timeTag;
        int range;
    };

    class fsm_df_adaptor : public adaptor
    {
        // current implementation is the same as a copy adaptor
      public:
        fsm_df_adaptor(sc_module_name n) : adaptor(n)
        {
        }

        void read_events()
        {
            M2_DEBUG1("-----read events in adaptor-----");
            m2_event_info* tmp;
            tmp = read_port->read_event_info_direct(); 
            while (tmp != NULL)
            {
                internal_event_info_list.push_back(tmp);
                tmp = read_port->read_event_info_direct();
            }
            M2_DEBUG1("-----end of read events in adaptor-----");
        }

        void transform_events()
        {
            M2_DEBUG1("-----transform events in adaptor-----");
        }

        void write_events()
        {
            M2_DEBUG1("-----write events in adaptor-----");
            while (!internal_event_info_list.empty())
            {
                write_port->write_event_info_direct(internal_event_info_list.front());
                internal_event_info_list.pop_front();
            }            
            M2_DEBUG1("-----end of write events in adaptor-----");
        }
    };

}

#endif
