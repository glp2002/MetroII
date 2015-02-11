// Base includes files and definitions used by all core metroII files
// Authors: Abhijit Davare, Guang Yang, Trevor Meyerowitz, Qi Zhu 

#ifndef M2_BASE_H
#define M2_BASE_H

#include <systemc.h>
#include <map>
#include <vector>
#include <list>
#include <cstring>
#include <algorithm>
#include <deque>

#define SWITCH_PHASES (1)

#define STR_CAT(dst, src1, src2) \
    dst = (char *)malloc(strlen(src1) + strlen(src2) + 1); \
strcpy(dst, src1); \
strcat(dst, src2); 

// temporary, need a type system for val and tag
#define NONDET -100000

namespace m2_core { //begin namespace m2_core 


    //******************************************************************************
    // Utilities class types
    //******************************************************************************

    //struct ltprochandle 
    typedef struct {
        bool operator()(const sc_process_handle& s1, const sc_process_handle& s2)
        {
            return (((sc_process_b*) (sc_process_handle) s1) <
                    ((sc_process_b*) (sc_process_handle) s2));
        }
    } ltprochandle;

    typedef struct 
    {
        bool operator()(const char* s1, const char* s2) const
        {
            return strcmp(s1, s2) < 0;
        }
    } ltstr;

    typedef struct 
    {
        bool operator()(const char* s1, const char* s2) const
        {
            return strncmp(s1, s2, strlen(s1) - 1) < 0;
        }
    } ltstrn;

    class m2_event;
    typedef std::pair<m2_event*, m2_event*> event_pair_t;
    typedef std::map<sc_process_handle, event_pair_t*, ltprochandle> thread_event_map_t;
    typedef std::map<const char*, thread_event_map_t*> func_event_map_t;
}

#endif


