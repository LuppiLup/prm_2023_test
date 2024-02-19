# Default target executed when no arguments are given to make.
default_target: default

# Suppress display of executed commands.
# Call make with VERBOSE=0 to enable all outputs
$(VERBOSE).SILENT:

# ------------------------------------------------------------
# ---------------------- BEGIN OF TODOs ----------------------
# ------------------------------------------------------------

ifndef EXE
# TODO: EDIT THIS ON FIRST USAGE if you want to change the name of the executable
EXE = Program
endif

ifndef MAIN
# TODO: EDIT THIS ON FIRST USAGE if you want to change the name of the main source file
MAIN = main
endif

ifndef VERBOSEHERE
# TODO: EDIT THIS ON FIRST USAGE if you want to supress echo outputs put 1. If you want to supress Warnings and echos put 0.
VERBOSEHERE=2
endif
export VERBOSEHERE

ROOTLIB = $(shell root-config --libs)
# TODO: EDIT THIS ON FIRST USAGE if you want to use a different compiler
CXX=g++
# TODO: EDIT THIS ON FIRST USAGE if you want to use different compiler flags
FLAGS=-fPIC -std=c++14 -O2 -pthread -m64

ifneq (${VERBOSEHERE},0)
FLAGS+= -Wall
endif

# TODO: EDIT THIS ON FIRST USAGE if you want to use a different extension for your source files
SOURCEEXTENSION=.cpp
# TODO: EDIT THIS ON FIRST USAGE if you want to use a different extension for your header files
HEADEREXTENSION=.h

PROGRAMDIR=$(shell pwd)
# TODO: EDIT THESE ON FIRST USAGE if you use a different folder structure
SRCDIR=$(PROGRAMDIR)/src
INCDIR=$(PROGRAMDIR)/include
ROOTINCDIR=$(INCDIR)/ROOT_include
BUILDDIR=$(PROGRAMDIR)/build
LIBDIR=$(PROGRAMDIR)/lib
MAINDIR=$(PROGRAMDIR)/main

# TODO: COMMENT THE FOLLOWING LINE AFTER ALL ADJUSTMENTS ABOVE ARE DONE!!!
#$(info )
#$(info WARNING: Some things have to be set up before usage. Please look into the base Makefile located at ${PROGRAMDIR} and work yourself through the TODOs at the beginning of this file!)
#$(info )

# ------------------------------------------------------------
# ----------------------- END OF TODOs -----------------------
# ------------------------------------------------------------

OBJECTFILES=$(wildcard $(BUILDDIR)/*.o)
LIBUSER=$(wildcard $(LIBDIR)/*.so)

ALL_H=$(wildcard $(INCDIR)/*$(HEADEREXTENSION))
ROOTALL_H=$(wildcard $(ROOTINCDIR)/*$(HEADEREXTENSION))
ALL_H+=$(ROOTALL_H)

INCLUDE= -I$(shell root-config --incdir) -I$(INCDIR) -I$(ROOTINCDIR)

export MAIN
export ALL_H
export ROOTALL_H
export SOURCEEXTENSION
export HEADEREXTENSION
export INCDIR
export BUILDDIR
export LIBDIR
export CXX
export FLAGS
export INCLUDE
export ROOTLIB


.PHONY: default
default:  UCintTest Src mainfolder
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Move to $(PROGRAMDIR)/"; fi
	$(MAKE) $(EXE)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Execute the Program with ./$(EXE)"; fi

.PHONY: UCintTest
UCintTest:
	@if [ ! -e "$(SRCDIR)/UCint$(SOURCEEXTENSION)" ] ; then touch $(SRCDIR)/UCint$(SOURCEEXTENSION) && make new; fi

#-Wl, targets the linker
#--start-group and --end-group say to the linker that he needs to sort them by himself
$(EXE): $(LIBUSER)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Create executable: $(EXE)"; fi
	$(CXX) $(FLAGS) $(INCLUDE) -o $(EXE) -Wl,--start-group $(LIBUSER) -Wl,--end-group $(ROOTLIB)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo; fi

.PHONY: Src
Src:
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Move to $(SRCDIR)/"; fi
	$(MAKE) -C $(SRCDIR) lib
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo; fi

.PHONY: mainfolder
mainfolder:
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Move to $(MAINDIR)/"; fi
	$(MAKE) -C $(MAINDIR) lib
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo; fi

.PHONY: clean
clean:
	$(MAKE) cleanEXE
	$(MAKE) cleanlib
	$(MAKE) cleanO
	$(MAKE) cleanmk
	$(MAKE) cleanUCint

.PHONY: cleanEXE
cleanEXE:
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Removing executable $(EXE)."; fi
	rm -f $(EXE)

.PHONY: cleanlib
cleanlib:
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Removing user created library files."; fi
	rm -f $(LIBUSER)

.PHONY: cleanO
cleanO:
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Removing user created object files."; fi
	rm -f $(OBJECTFILES)

.PHONY: cleanmk
cleanmk:
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Removing dependency files."; fi
	rm -f $(SRCDIR)/depend.mk
	rm -f $(MAINDIR)/depend.mk

.PHONY: cleanUCint
cleanUCint:
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Removing LinkDef.h"; fi
	rm -f $(SRCDIR)/LinkDef.h
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Removing Cint_rdict.pcm";fi
	rm -f $(LIBDIR)/Cint_rdict.pcm

.PHONY:new
new:
	$(MAKE) clean
	$(MAKE) default

.PHONY: help
help:
	@echo "-- If you want to add new files,"
	@echo "   add a $(SOURCEEXTENSION) file in $(SRCDIR) and"
	@echo "   a $(HEADEREXTENSION) file in $(INCDIR)"
	@echo "-- Do \"make EXE=x\", if you want to name the Executable x instead of Program or"
	@echo "   rename the variable in the Makefile."
	@echo "-- Do \"make new\" if you want to compile everything from scratch"
	@echo "-- If you get the following error message:"
	@echo "     \"g++: fatal error: no input files\""
	@echo "   you might have to adjust some things in the base Makefile."
	@echo "-- If you get a root compile error try deleting"
	@echo "   $(SRCDIR)/UCint$(SOURCEEXTENSION) and make again."
	@echo "-- If you want to supress all outputs,"
	@echo "   try make VERBOSEHERE=1"
	@echo "   make VERBOSEHERE=0, if you want to supress Warnings too."
