/*
   Copyright (c) 2007 The Regents of the University of California.
   All rights reserved.

   Permission is hereby granted, without written agreement and without
   license or royalty fees, to use, copy, modify, and distribute this
   software and its documentation for any purpose, provided that the
   above copyright notice and the following two paragraphs appear in all
   copies of this software and that appropriate acknowledgments are made
   to the research of the Metropolis group.

   IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
   FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
   ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
   THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE.

   THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
   PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
   CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
   ENHANCEMENTS, OR MODIFICATIONS.

   METROPOLIS_COPYRIGHT_VERSION_1
   COPYRIGHTENDKEY
   */

// MetroII core source file
// Authors: Abhijit Davare, Guang Yang, Trevor Meyerowitz, Qi Zhu

#include "metroII.h"

namespace m2_core { // begin namespace m2_core 


    m2_manager manager("Manager"); // instantiate the manager

    //******************************************************************************
    // set up the manager and start the simulation
    //******************************************************************************
    void scan_hierarchy(int * total, sc_object * obj) 
    {
        std::vector<sc_object*> children = obj->get_child_objects();

        std::cout << obj->name() << " " << obj->kind() << std::endl ; // Print the name and kind
        if ( strcmp(obj->kind(),"sc_thread_process") == 0 )
        {
            (*total)++;
        }

        if (strcmp(obj->kind(),"m2_manager") != 0) // don't investigate the manager
        {
            for (unsigned i = 0; i < children.size(); i++)
            {
                if (children[i])
                {
                    scan_hierarchy(total, children[i]);
                }
            }
        }
    };


    void m2_start() 
    {
        // gather all components in the design
        int total_num_processes = 0;

        std::vector<sc_object*> tops = sc_get_top_level_objects();

        for ( unsigned i = 0; i < tops.size(); i++ )
        {
            if ( tops[i] )
            {
                scan_hierarchy(&total_num_processes, tops[i]); // Traverse the object hierarchy below
            }
        }

        manager.set_number_of_processes_in_system(total_num_processes);
        cout << "Total processes = " << total_num_processes << endl;

        sc_start();
    }

    void m2_wait( const sc_event & m, sc_simcontext * s)
    {
        manager.increment_procs_ready_to_switch();
        wait(m, s);
        manager.decrement_procs_ready_to_switch();
    }

	

    void m2_wait(double v, sc_time_unit tu)
    {
        manager.increment_procs_ready_to_switch();
        wait(v, tu);
        manager.decrement_procs_ready_to_switch();
    }

	void m2_wait( sc_event_and_list& list)
    {
        manager.increment_procs_ready_to_switch();
        wait(list);
        manager.decrement_procs_ready_to_switch();
    }
	
    void m2_wait(sc_event_or_list& list)
    {
        manager.increment_procs_ready_to_switch();
        wait(list);
        manager.decrement_procs_ready_to_switch();
    }

    void m2_wait()
    {
        manager.increment_procs_ready_to_switch();
        wait();
        manager.decrement_procs_ready_to_switch();
    }

    void register_constraint_solver(m2_constraint_solver* c_solver)
    {
        manager.add_constraint_solver(c_solver);
    }

    void register_annotator(m2_annotator* _annotator)
    {
        manager.add_annotator(_annotator);
    }

    void register_scheduler(m2_scheduler* _scheduler)
    {
        manager.add_scheduler(_scheduler);
    }

    void m2_end(sc_process_handle proc)
    {
        for (unsigned int i=0; i<manager.scheduler_list.size(); i++)
        {
            if (manager.scheduler_list[i]->type >= 1)
            {
                manager.scheduler_list[i]->update_end_process(proc);
            }
        }
        manager.decrement_total_procs();
    }

} //end namespace m2_core 

