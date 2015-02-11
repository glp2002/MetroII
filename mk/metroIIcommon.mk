# Copyright (c) 2007 The Regents of the University of California.
# All rights reserved.
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in all
# copies of this software and that appropriate acknowledgments are made
# to the research of the Metropolis group.
# 
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
# THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
# CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
# ENHANCEMENTS, OR MODIFICATIONS.
#
#
# 						METROPOLIS_COPYRIGHT_VERSION_1
# 						COPYRIGHTENDKEY
#
# Common makefile of the Metropolis meta-model compiler.
# 
# To install, use 'make install'.
# To compile , use 'make' 
#               or 'make all'.
# To get docs, use 'make docs'.
# To clean up, use 'make clean'.
# To test, use 'make test'.


# Rule for compiling C++ files
.cpp.o:
	$(METROII_CXX) $(CXX_FLAGS) $(CXX_INCLUDES) -c $<

# Rule for compiling with cc
.c.o:
	$(METROII_CC) $(C_FLAGS) $(C_INCLUDES) -c $<

# Under no circumstances should this makefile include 'all', 'install'
# or 'depend' rules.  These rules should go in the makefile that
# includes this makefile, or into no-compile.mk
# The reason is that we want to avoid duplicate 'all', 'install'
# and 'depend' rules without using the possibly unportable double-colon
# makefile convention.

# Run make all in the subdirs
suball:
	@if [ "x$(DIRS)" != "x" ]; then \
		set $(DIRS); \
		for x do \
		    if [ -w $$x ] ; then \
			( cd $$x ; \
			echo making all in $(ME)/$$x `date "+%h %e %T"`; \
			$(MAKE) $(MFLAGS) $(MAKEVARS) all ;\
			) \
		    fi ; \
		done ; \
	fi

# Run make install in the subdirs
subinstall:
	@if [ "x$(DIRS)" != "x" ]; then \
		set $(DIRS); \
		for x do \
		    if [ -w $$x ] ; then \
			( cd $$x ; \
			echo making install in $(ME)/$$x `date "+%h %e %T"` ; \
			$(MAKE) $(MFLAGS) $(MAKEVARS) install ;\
			) \
		    fi ; \
		done ; \
	fi


# Quickly attempt to build the tree
# 'make fast' is a hack.  If it does not work for you, either fix it or
# don't use it and use 'make' instead.
# Run make $(EXTRA_TARGETS) first so that we create
# metamodel.MetaModelVisitor before going in to nodetypes.
# $(EXTRA_TARGETS2) is used in metropolis/metamodel/frontend/parser/makefile
# so that we build MetaModelParser.java first before building the tables.
fast:
	@if [ "x$(EXTRA_TARGETS)" != "x" ]; then \
		$(MAKE) $(EXTRA_TARGETS); \
	fi
	@if [ "x$(DIRS)" != "x" ]; then \
		set $(DIRS); \
		for x do \
		    if [ -w $$x ] ; then \
			( cd $$x ; \
			echo making fast in $(ME)/$$x `date "+%h %e %T"`; \
			$(MAKE) $(MFLAGS) $(MAKEVARS) fast ;\
			) \
		    fi ; \
		done ; \
	fi
	@if [ "x$(CPP_SRCS)" != "x" ]; then \
		echo "fast build with 'make sources' in `pwd`"; \
		$(MAKE) sources; \
		echo "fast build with '"$(METROII_CXX)" $(CXX_FLAGS) $(CXX_INCLUDES) -c $(CPP_SRCS)' in `pwd`"; \
		"$(METROII_CXX)" $(CXX_FLAGS) $(CXX_INCLUDES) -c $(CPP_SRCS); \
	fi
	@if [ "x$(C_SRCS)" != "x" ]; then \
		echo "fast build with '"$(METROII_CC)" $(C_FLAGS) $(C_INCLUDES) -c $(C_SRCS)' in `pwd`"; \
		"$(METROII_CC)" $(C_FLAGS) $(C_INCLUDES) -c $(C_SRCS); \
		$(MAKE) all; \
	fi
	@if [ "x$(TARGET)" != "x" ]; then \
		$(MAKE) $(TARGET); \
	fi
	@if [ "x$(EXTRA_TARGETS2)" != "x" ]; then \
		$(MAKE) $(EXTRA_TARGETS2); \
	fi

	@if [ "x$(METROII_LIBS)" != "x" ]; then \
		$(MAKE) $(METROII_LIBS); \
	fi

# "make sources" will do SCCS get on anything where SCCS file is newer.
sources::	$(SRCS) $(EXTRA_SRCS) $(HDRS) $(MISC_FILES) makefile
	@if [ "x$(DIRS)" != "x" ]; then \
		set $(DIRS); \
		for x do \
		    if [ -w $$x ] ; then \
			( cd $$x ; \
			echo making $@ in $(ME)/$$x ; \
			$(MAKE) $(MFLAGS) $(MAKEVARS) $@ ;\
			) \
		    fi ; \
		done ; \
	fi


# Script used to find files that shold not be shipped
CHKEXTRA =	$(METRO)/util/testsuite/chkextra
checkjunk:
	@"$(CHKEXTRA)" $(SRCS) $(HDRS) $(EXTRA_SRCS) $(MISC_FILES) \
		$(OPTIONAL_FILES) $(JSRCS) alljtests.tcl makefile SCCS CVS \
		$(JCLASS) $(OBJS) $(LIBR) $(PTDISTS) \
		$(PTCLASSJAR) $(PTCLASSALLJAR) $(PTAUXALLJAR) \
		$(EXTRA_TARGETS)
	@if [ "x$(DIRS)" != "x" ]; then \
		set $(DIRS); \
		for x do \
		    if [ -w $$x ] ; then \
			( cd $$x ; \
			echo making $@ in $(ME)/$$x ; \
			$(MAKE) $(MFLAGS) $(MAKEVARS) $@ ;\
			) \
		    fi ; \
		done ; \
	fi

##############
# Rules for cleaning

DERIVED_JAVA_SRCS =  $(MMM_SRCS:%.mmm=%.java)

CRUD=.*.ast .constr*.loc .trace *.o *.so core *~ *.bak ,* LOG* *.class \
	*.bb *.bbg *.bin *.da *.gcov \
	alljtests.tcl manifest.tmp .lastgen \
	$(DERIVED_JAVA_SRCS) \
	$(JCLASS) $(KRUFT)

clean:
	@if [ -f systemc_sim.mk ]; then \
	    $(MAKE) -f systemc_sim.mk sim_clean; \
	fi
	rm -rf $(CRUD)
	if [ "x$(DIRS)" != "x" ]; then \
		set $(DIRS); \
		for x do \
		    if [ -w $$x ] ; then \
			( cd $$x ; \
			echo making $@ in $(ME)/$$x ; \
			$(MAKE) $(MFLAGS) $(MAKEVARS) $@ ;\
			) \
		    fi ; \
		done ; \
	fi

# Cleaner than 'make clean'
# Remove the stuff in the parent directory after processing
# the child directories incase something in the child depends on
# something we will be removing in the parent
realclean:
	@if [ "x$(DIRS)" != "x" ]; then \
		set $(DIRS); \
		for x do \
		    if [ -w $$x ] ; then \
			( cd $$x ; \
			echo making $@ in $(ME)/$$x ; \
			$(MAKE) $(MFLAGS) $(MAKEVARS) $@ ;\
			) \
		    fi ; \
		done ; \
	fi
	rm -f $(CRUD) configure config.status config.log config.cache
	-rm -f doc/codeDoc/* $(OPTIONAL_FILES)
