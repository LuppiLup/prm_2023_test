THISDIR=$(shell pwd)

ALL_C	=$(wildcard $(THISDIR)/*$(SOURCEEXTENSION))
ALL_TEMP=$(subst $(THISDIR),$(BUILDDIR),$(ALL_C))
ALL_O	=$(ALL_TEMP:$(SOURCEEXTENSION)=.o)

ULIB=$(LIBDIR)/libUserSrc.so
LIBSO=libUserSrc.so

InThisDirectory:
	@echo "Dont do make in this directory!"

.PHONY: lib
lib: $(ULIB)

$(ULIB): depend.mk $(ALL_O)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Making library \"$(LIBSO)\" at $(LIBDIR)/ with:"; fi
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo $(ALL_O); fi
	$(CXX) -shared $(FLAGS) -o $(ULIB) $(ALL_O)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo; fi

$(BUILDDIR)/%.o: ./%$(SOURCEEXTENSION)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Compiling: " $<; fi
	$(CXX) $(FLAGS) $(INCLUDE) -c $< -o $@

depend.mk: $(ALL_H)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Create dependencies file: "$@; fi
	rm -f depend.mk
	for f in *$(SOURCEEXTENSION) ; do \
		$(CXX) $(FLAGS) $(INCLUDE) -MQ $(BUILDDIR)/$$(basename "$$f" $$SOURCEEXTENSION).o $$f -MM >> depend.mk ; \
	done

UCint$(SOURCEEXTENSION): $(ROOTALL_H) LinkDef.h
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Generating:  ROOT dictionary UCint$(SOURCEEXTENSION)"; fi
	rootcint -f $@ -c $(INCLUDE) $(ROOTALL_H) LinkDef.h
	mv UCint_rdict.pcm ../lib/

LinkDef.h: $(ROOTALL_H)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Generating: " $@; fi
	rm -f  $@
	touch  $@
	echo "#ifdef __CINT__"  >> $@
	echo "#pragma link off all globals;"  >> $@
	echo "#pragma link off all classes;"  >> $@
	echo "#pragma link off all functions;"  >> $@
	for f in $(ROOTALL_H) ; do \
		echo "#pragma link C++ defined_in $$f;"  >> $@; \
	done
	echo "#endif"  >> $@




$(BUILDDIR)/UCint.o: UCint$(SOURCEEXTENSION)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Compiling:  ROOT dictionary UCint$(SOURCEEXTENSION)"; fi
	$(CXX) $(FLAGS) $(INCLUDE) -c $< -o $@

-include depend.mk
