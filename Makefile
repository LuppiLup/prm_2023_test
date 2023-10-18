# Default target executed when no arguments are given to make.
default_target: default

# Suppress display of executed commands.
# Call make with VERBOSE=0 to enable all outputs
$(VERBOSE).SILENT:

ifndef EXE
EXE = Program
endif

ifndef MAIN
MAIN = main
endif

ifndef VERBOSEHERE
VERBOSEHERE=2
endif
export VERBOSEHERE

CXX=g++
FLAGS=-fPIC -std=c++14 -O2 -pthread -m64
#INCLUDE=-I/usr/local/src/root_v5.34-36/lib/
#LIBPATH=/usr/local/src/root_v5.34-36/lib/
#LIBS= -libTree.so




ifneq (${VERBOSEHERE},0)
FLAGS+= -Wall
endif

SOURCEEXTENSION=.cpp
HEADEREXTENSION=.h

PROGRAMDIR=$(shell pwd)
SRCDIR=$(PROGRAMDIR)/src
INCDIR=$(PROGRAMDIR)/include
BUILDDIR=$(PROGRAMDIR)/build
LIBDIR=$(PROGRAMDIR)/lib
MAINDIR=$(PROGRAMDIR)/main

OBJECTFILES=$(wildcard $(BUILDDIR)/*.o)
LIBUSER=$(wildcard $(LIBDIR)/*.so)

ALL_H=$(wildcard $(INCDIR)/*$(HEADEREXTENSION))

#You can use ROOT
INCLUDE= -I$(INCDIR) -I$(shell root-config --incdir)
ROOTLIB= $(shell root-config --glibs)

export MAIN
export ALL_H
export SOURCEEXTENSION
export HEADEREXTENSION
export INCDIR
export BUILDDIR
export LIBDIR
export CXX
export FLAGS
export INCLUDE


.PHONY: default
default:  Src mainfolder
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Move to $(PROGRAMDIR)/"; fi
	$(MAKE) $(EXE)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Execute the Program with ./$(EXE)"; fi

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
	@echo "-- If you want to supress all outputs,"
	@echo "   try make VERBOSEHERE=1"
	@echo "   make VERBOSEHERE=0, if you want to supress Warnings too."
