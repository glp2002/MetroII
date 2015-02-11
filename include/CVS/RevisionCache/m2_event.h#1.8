// Definitions of event and event_info (used in adaptors)
// Authors: Abhijit Davare, Guang Yang, Trevor Meyerowitz, Qi Zhu

#ifndef M2_EVENT_H
#define M2_EVENT_H

#include "m2_base.h"

namespace m2_core { // begin namespace m2_core 

    //******************************************************************************
    // Constants that denote the event status
    //******************************************************************************

    enum M2_Event_Types 
    {
        M2_EVENT_BEGIN,
        M2_EVENT_END,
        M2_EVENT_GENERIC
    };

    enum M2_Event_Status 
    {
        M2_EVENT_INACTIVE,
        M2_EVENT_PROPOSED,

        M2_EVENT_WAITING,
        M2_EVENT_NOTIFIED,

        M2_EVENT_DISABLED
    };

    //******************************************************************************
    // MetroII event definition
    //******************************************************************************
    class m2_event : public sc_event 
    {
      private:
        const char* _name;
        const char* _full_name;
        char _status;
        sc_process_handle _owner;

      public:
        //map<string, double> tag;
        //map<string, double> val;

        double tag;
        double val;

        m2_event()
        {
            char * _temp;
            _name = "unknown";
            _status = (char) M2_EVENT_INACTIVE;
            _owner = sc_get_current_process_handle();
            STR_CAT(_temp, _owner.name(), _name);
            _full_name = _temp;
            val = NONDET;
        }

        m2_event(const char * name)
        {
            char * _temp;
            _name = name;
            _status = (char) M2_EVENT_INACTIVE;
            _owner  = sc_get_current_process_handle();
            STR_CAT(_temp, _owner.name(), _name);
            _full_name = _temp;
            val = NONDET;
        }

        m2_event(const char * name, sc_process_handle owner)
        {
            char * _temp;
            _name = name;
            _status = (char) M2_EVENT_INACTIVE;
            _owner  = owner;
            STR_CAT(_temp, _owner.name(), _name);
            _full_name = _temp;
            val = NONDET;
        }

        m2_event& clone(sc_process_handle owner) {
            m2_event* n = new m2_event(_name);
            n->set_status(_status);
            n->set_owner(owner);
            n->tag = tag;
            n->val = val;
            return (*n);
        }

        m2_event& clone() {
            return clone(_owner);
        }

        void set_status(char status)
        {
            _status = status;
        }

        char get_status()
        {
            return _status;
        }

        void set_owner(sc_process_handle owner)
        {
            _owner = owner;
        }

        sc_process_handle get_owner()
        {
            return _owner;
        }

        const char * get_full_name()
        {
            return _full_name;
        }

        const char * name()
        {
            return _name;
        }

        const char * string_status()
        {
            char * message;
            message = "";
            if (_status == (char)M2_EVENT_INACTIVE)
                message = "Inactive";
            if (_status == (char)M2_EVENT_PROPOSED)
                message = "Proposed";
            if (_status == (char)M2_EVENT_WAITING)
                message = "Waiting";
            if (_status == (char)M2_EVENT_NOTIFIED)
                message = "Notified";
            if (_status == (char)M2_EVENT_DISABLED)
                message = "Disabled";
            return message;
        }

    };

    //******************************************************************************
    // MetroII event info definition
    //******************************************************************************
    class m2_event_info
    {
      public:
        const char* name;
        double tag;
        double val;
        char status;

        m2_event_info()
        {
            name = "unknown";
            status = (char) M2_EVENT_INACTIVE;
            tag = 0;
            val = NONDET;
        }

        m2_event_info(m2_event& e)
        {
            char * temp;
            temp = (char *)malloc(strlen(e.get_full_name()) + 1);
            name = temp;
            tag = e.tag;
            val = e.val;
            status = e.get_status(); 
        }

        void copy_info_to_event(m2_event& e)
        {
            e.tag = tag;
            e.val = val;
            e.set_status(status);
        }

        ~m2_event_info()
        {
            free((void *)name);
        }
    };

} // begin namespace m2_core 

#endif
