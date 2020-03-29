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

# Common makefile definitions for C projects that result in libraries.

# The executable that is built by this makefile.
LIB         = $(dlctargetfile)

# Default make rule 
all: $(LIB)

# The (space-separated) list of object files in the project
OBJS = $(dlcobjs)

# Rule for building the library
$(LIB):	$(OBJS)
	$(AR) -cr $@ $(OBJS)
	$(RANLIB) $@

include $(UXDICE)/lang/ocl/config/proj/c.mk

