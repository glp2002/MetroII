// Definitions of interface
// Authors: Abhijit Davare, Guang Yang, Trevor Meyerowitz, Qi Zhu

#ifndef M2_INTERFACE_H
#define M2_INTERFACE_H

#include "m2_base.h"


namespace m2_core { // begin namespace m2_core 

    //******************************************************************************
    // MetroII interface base class
    //******************************************************************************
    class m2_interface 
    {
      public:
        func_event_map_t __m2_func_event_map;
        m2_interface * __connected_obj;

        // initialize connected objects
        m2_interface()
        {
            __connected_obj = NULL;
        }

        virtual ~m2_interface() {}

        void connect(m2_interface * dest) 
        {
            __connected_obj = dest;
        }

        m2_interface * get_connected_obj() 
        {
            return __connected_obj;
        }

    };

} // end namespace m2_core

#endif
