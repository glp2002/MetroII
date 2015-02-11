// Definitions of required/provided ports and read/write ports
// Authors: Abhijit Davare, Guang Yang, Trevor Meyerowitz, Qi Zhu

#ifndef M2_PORTS_H
#define M2_PORTS_H

#include "m2_base.h"
#include "m2_interface.h"

namespace m2_core { // begin namespace m2_core 

    //******************************************************************************
    // MetroII required port definition 
    //******************************************************************************
    template <typename Interface_Type>
        class m2_required_port : public Interface_Type 
    {
      public:

        m2_required_port():Interface_Type()
        {
        }

        Interface_Type* operator -> () {
            return this;
        }

        Interface_Type* getThisInterface() {
            return this;
        }
    };

    //******************************************************************************
    // MetroII provided port definition
    //******************************************************************************
    template <typename Interface_Type>
        class m2_provided_port : public Interface_Type 
    {
      public:

        m2_provided_port():Interface_Type()
        {
        }

        Interface_Type* operator -> () {
            return this;
        }

        Interface_Type* getThisInterface() 
        {
            return this;
        }
    };

    //******************************************************************************
    // MetroII read port - derived from provided port, for reading data from medium
    //******************************************************************************
    template <typename Interface_Type>
        class m2_read_port : public m2_provided_port<Interface_Type> 
    {
    };


    //******************************************************************************
    // MetroII write port - derived from provided port, for writing data to medium
    //******************************************************************************
    template <typename Interface_Type>
        class m2_write_port : public m2_provided_port<Interface_Type>
    {
    };

} // end namespace m2_core

#endif
