// Mapper definition, currently not used
// Author: Abhijit Davare

#ifndef M2_MAPPER_H
#define M2_MAPPER_H

namespace m2_core { // begin namespace m2_core 

    template <typename Func_Interface, typename Arch_Interface>
        class m2_mapper : public sc_module, public Func_Interface
    {
      public:

        m2_mapper() {}

        m2_provided_port<Func_Interface> in_port;
        m2_required_port<Arch_Interface> out_port;

        m2_mapper(sc_module_name name) : sc_module(name)
        {
        }

        virtual const char* kind() const
        {
            return "m2_mapper";
        }

    };

} // end namespace m2_core

#endif
