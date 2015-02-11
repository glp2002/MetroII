// Definitions of component and its derived objects
// Authors: Abhijit Davare, Guang Yang, Trevor Meyerowitz, Qi Zhu 

#ifndef M2_COMPONENT_H
#define M2_COMPONENT_H

#include "m2_base.h"


namespace m2_core { // begin namespace m2_core 

    //******************************************************************************
    // MetroII component base class
    //******************************************************************************
    class m2_component : public sc_module 
    {
      public:

        m2_component(sc_module_name name) : sc_module(name)
        {
            func_arch_flag = -1;
        }

        m2_component(sc_module_name name, int _func_arch_flag) : sc_module(name)
        {
            func_arch_flag = _func_arch_flag;
        }

        void set_func_arch_flag(int _func_arch_flag)
        {
            func_arch_flag = _func_arch_flag;		
        }

        virtual const char* kind() const
        {
            return "m2_component";
        }

      protected:

        int func_arch_flag; // 0: function component, 1: architecture component, -1: not defined
    };

#define M2_CTOR(user_module_name)                                             \
    typedef user_module_name SC_CURRENT_USER_MODULE;                          \
    user_module_name( sc_module_name  name) : m2_component(name)

    //******************************************************************************
    // MetroII channel 
    //******************************************************************************
    class m2_channel : public m2_component
    {
        // m2_channel is used as communication medium between other components.
        // It does not have thread. Currently it is a trivial derived class from
        // m2_component, and used for tools to identify it is a channel. 
        
      public:

        m2_channel(sc_module_name name) : m2_component(name)
        {
        } 

        m2_channel(sc_module_name name, int _func_arch_flag) : m2_component(name, _func_arch_flag)
        {
        } 
    };
} // end namespace m2_core

#endif
