#############################################################################
# @ddblock_begin copyright
#
# Copyright (c) 1997-2015
# Maryland DSPCAD Research Group, The University of Maryland at College Park 
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the above
# copyright notice and the following two paragraphs appear in all copies
# of this software.
# 
# IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
# THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
# 
# THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
# MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
# ENHANCEMENTS, OR MODIFICATIONS.
#
# @ddblock_end copyright
#############################################################################

# Common makefile definitions for compiling C-OpenCL code.
# @author Kyunghun Lee, Yanzhou Liu

# Settings for standard makefile variables based on the current dlc project
# configuration.
CMISCFLAGS     = $(dlcmiscflags)
CMISCLDFLAGS   = $(dlcmiscldflags)
CINCLUDEPATH   = $(dlcincludepath)
CLIBPATH       = $(dlclibpath)
CLIBS          = $(dlclibs)

# Standard dlc settings for project arameters
CFLAGS         = -Wall -O -g -pedantic $(CINCLUDEPATH) $(CMISCFLAGS)
LDFLAGS        = -Wall -O -pedantic $(CINCLUDEPATH) $(CMISCLDFLAGS) $(CLIBPATH) 
DEPEND         = $(CC) -MM $(CINCLUDEPATH)

# Using PHONY targets. This is limited to GNU Make
.PHONY: all

# OBJS defined in individual Makefile
# Derive SRCS and DEPS from this
SRCS = $(OBJS:%.o=%.c)

# Depend files for each src file - hello.c -> hello.d
DEPS = $(OBJS:%.o=%.d)

# include common makefile definitions
include $(UXDICELANG)/common/config/proj/common.mk

# Rule for compiling C files. 
%.o: 
	$(CC) -c $(CFLAGS) $< $(OCL_FG)

# deps will be created for each c file
%.d: %.c
	set -e; $(DEPEND) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@
#	set -e; mkdir -p deps; echo -n $@ " "> $@; \
#	$(DEPEND) $< > $@; \
#	[ -s $@ ] || rm -f $@; 

-include $(DEPS)
