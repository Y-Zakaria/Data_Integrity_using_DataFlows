###############################################################################
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
###############################################################################

EPS_SRC = $(wildcard $(FIG_DIR)/*.eps)
PS_SRC = $(wildcard $(FIG_DIR)/*.ps)
DOT_SRC = $(wildcard $(FIG_DIR)/*.dot)
PPT_SRC = $(wildcard $(FIG_DIR)/*.ppt)
PPT_SRC += $(wildcard $(FIG_DIR)/*.pptx)

PDF_EPS_TGT = $(EPS_SRC:.eps=.pdf)
PDF_PS_TGT = $(PS_SRC:.ps=.pdf)
EPSI_PPT_TGT = 
EPSI_PPT_TGT += 
EPS_TGT = $(DOT_SRC:.dot=.epsi)

PS_PPT_TGT = 
#$(PPT_SRC:.ppt=.ps)

DATE = $(shell /bin/date +\%m\%d\%y)
FIG_DIR = .
PDF_DIR = .
TEX_DIR = .

#TEX_SRC = $(wildcard $(TEX_DIR)/*.tex)
TEX_SRC = $(wildcard *.tex)
BIB_SRC = $(wildcard *.bib)
BBL_SRC = $(wildcard *.bbl)

OSTYPE=$(shell uname | sed -e 's/-.*//g')

ifeq ($(OSTYPE),Linux) 
else 
ifeq ($(OSTYPE),Darwin)
else
#else we're cygwin
	EPSI_PPT_TGT = $(PPT_SRC:.ppt=.epsi)
	EPSI_PPT_TGT += $(PPT_SRC:.pptx=.epsi)
endif
endif

ifeq ($(strip $(BBL_SRC)),) 

$(dllatoplevel).pdf : $(EPS_TGT) $(EPSI_PPT_TGT) $(PS_TGT) $(PS_PPT_TGT) $(TEX_SRC) $(STYLEFILE) 
	pdflatex -output-format=dvi $(TEX_DIR)/$(dllatoplevel)
	bibtex $(dllatoplevel)

else

$(dllatoplevel).pdf : $(EPS_TGT) $(EPSI_PPT_TGT) $(PS_TGT) $(PS_PPT_TGT) $(TEX_SRC) $(STYLEFILE) 
	pdflatex -output-format=dvi $(TEX_DIR)/$(dllatoplevel)

endif

ifeq ($(dllaappend),date)
	pdflatex -output-format=dvi $(TEX_DIR)/$(dllatoplevel)
	pdflatex -output-format=dvi $(TEX_DIR)/$(dllatoplevel)
	dvips -t letter -o $(PDF_DIR)/$(dllatoplevel)-$(DATE).ps $(dllatoplevel).dvi
	@bash -c "ps2pdf $(dllatoplevel)-$(DATE).ps"
else
	pdflatex -output-format=dvi $(TEX_DIR)/$(dllatoplevel)
	pdflatex -output-format=dvi $(TEX_DIR)/$(dllatoplevel)
	dvips -t letter -o $(PDF_DIR)/$(dllatoplevel).ps $(dllatoplevel).dvi
	@bash -c "ps2pdf $(dllatoplevel).ps"
endif

%.pdf : %.eps
	ps2pdf14 -dEPSCrop $< $*.pdf

%.epsi : %.ps
	ps2epsi $< $*.epsi

%.ps : %.dot
	dot -Tps $< -o $*.ps

%.ps : %.ppt
	/cygdrive/c/Program\ Files\ \(x86\)/Microsoft\ Office/Office12/POWERPNT.exe /PT "PostScript Printer"  $<
	sleep 5; 
	cp /cygdrive/c/FileSpooler/PostScript.ps $*.ps

%.ps : %.pptx
	/cygdrive/c/Program\ Files\ \(x86\)/Microsoft\ Office/Office12/POWERPNT.exe /PT "PostScript Printer"  $< ; 
	sleep 5; 
	cp /cygdrive/c/FileSpooler/PostScript.ps $*.ps

