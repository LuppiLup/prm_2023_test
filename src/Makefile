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

-include depend.mk
