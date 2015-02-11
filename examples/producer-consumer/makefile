# Metropolis II makefile for producer/consumer mapping example
#
# @Version: $Id: makefile,v 1.4 2007/12/05 10:48:37 zhuqi Exp $
#
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
#						METROPOLIS_COPYRIGHT_VERSION_2
#						COPYRIGHTENDKEY
##########################################################################

# Current directory relative to top
ME =		examples/producer-consumer-complete

# Root of Metro directory
ROOT =		../..

# Compiler options
FLAGS = -g

# Get configuration info
CONFIG =	$(ROOT)/mk/metroII.mk
include $(CONFIG)

DIRS =

CPP_SRCS = producer-consumer-complete.cpp

H_SRCS =

OBJS = $(CPP_SRCS:%.cpp=%.o)

EXTRA_SRCS = $(CPP_SRCS) $(H_SRCS)

# Sources that may or may not be present, but if they are present, we don't
# want make checkjunk to report an error on them.
MISC_FILES = \
	$(DIRS)

# make checkjunk will not report OPTIONAL_FILES as trash
# make distclean removes OPTIONAL_FILES
OPTIONAL_FILES =

LIBDIR		= -L$(SYSTEMC)/$(SYSTEMC_LIB) -L$(ROOT)/src
LIBS		= $(ROOT)/src/metroII.o -lsystemc
TARGET		= producer-consumer-complete

all: $(TARGET)
	@echo "To run the example, run ./$(TARGET)"

install: all

$(TARGET) : $(OBJS)
	$(METROII_CXX) $(FLAGS) -o $(TARGET) $(LIBDIR) $(OBJS) $(LIBS) 

# 'make clean' removes KRUFT
KRUFT = $(TARGET)

# Get the rest of the rules
include $(ROOT)/mk/metroIIcommon.mk
