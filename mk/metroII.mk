# Makefile for Metropolis II makefile variables determined by configure 
#
# @version: $Id: metroII.mk.in,v 1.1 2007/03/08 04:57:44 cxh Exp $
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
# 						PT_COPYRIGHT_VERSION_2
# 						COPYRIGHTENDKEY

# NOTE: Don't edit this file if it is called metroII.mk, instead
# edit metroII.mk.in, which is read by configure.

# C compiler, such as gcc
METROII_CC =      /usr/bin/gcc

# C++ compiler, such as g++
METROII_CXX =     g++

# C OPTIMIZER - The setting for the optimizer, usually -O2
C_OPTIMIZER = -O2 -g

# C_WARNINGS  - Flags that print warnings.
C_WARNINGS = -w

# Flags used by gcov:
GCC_COVERAGE_FLAGS =  -fprofile-arcs -ftest-coverage

# C_MAKEFILE_FLAGS - Metropolis makefiles may optionally set this for
#   flags that are included on a per makefile basis.
#
# C_COVERAGE_FLAGS is used by gcov for code coverage, try
# make C_COVERAGE_FLAGS="-fprofile-arcs -ftest-coverage
#
# C_USERFLAGS - Metropolis makefiles never set this, but the user can set it.

C_FLAGS = $(C_OPTIMIZER) $(C_WARNINGS) $(C_MAKEFILE_FLAGS) $(C_COVERAGE_FLAGS) $(C_USERFLAGS)


# CXX OPTIMIZER - The setting for the optimizer, usually -O2
CXX_OPTIMIZER = -O2 -g

# CXX_WARNINGS  - Flags that print warnings.
CXX_WARNINGS = -Wno-deprecated -Wall

# CXX_MAKEFILE_FLAGS - Metropolis makefiles may optionally set this for
#   flags that are included on a per makefile basis.
#
# Flags used by gcov 
# CXX_COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
#
# CXX_USERFLAGS - Metropolis makefiles never set this, but the user can set it.

CXX_INCLUDE_FLAGS = -I$(SYSTEMC)/include -I$(ROOT)/include

CXX_FLAGS = $(CXX_OPTIMIZER) $(CXX_WARNINGS) $(CXX_INCLUDE_FLAGS) $(CXX_MAKEFILE_FLAGS) $(CXX_COVERAGE_FLAGS) $(CXX_USERFLAGS)

# Set to h.264-decoder and used in example/makefile if SDL is found.
METROII_H264_DIR = 

# Location of the Simple Directmedia Layer (SDL), used by the h.264 example
METROII_SDL = 

# Location of the SystemC directory, for example /usr/local/systemc-2.0.1
SYSTEMC =	/usr/local/systemc-2.1

# Location of the SystemC libraries, i.e lib-cygwin
SYSTEMC_LIB =	lib-linux
