THISDIR=$(shell pwd)

MAIN_C	=$(THISDIR)/$(MAIN)$(SOURCEEXTENSION)
MAIN_TEMP  =$(subst $(THISDIR),$(BUILDDIR),$(MAIN_C))
MAIN_O  =$(MAIN_TEMP:$(SOURCEEXTENSION)=.o)

ULIB=$(LIBDIR)/libUserMain.so
LIBSO=libUserMain.so

InThisDirectory:
	@echo "Dont do make in this directory!"

.PHONY: lib
lib: $(ULIB)

$(ULIB): depend.mk $(MAIN_O)
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo "Making library \"$(LIBSO)\" at $(LIBDIR)/ with:"; fi
	@if [ ${VERBOSEHERE} -gt 1 ]; then echo $(MAIN_O); fi
	$(CXX) -shared $(FLAGS) -o $(ULIB) $(MAIN_O)
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
