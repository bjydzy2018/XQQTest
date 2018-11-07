
%.o: %.c
	@echo -e '\e[1;34mCompiling $<...\e[0m'
	@$(MAKEDEPEND_C); \
	cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d
	@$(CC) -c $(CFLAGS) -o $@ $<

-include $(SRCS_C:.c=.P)

%.o: %.cpp
	@echo -e '\e[1;34mCompiling $<...\e[0m'
	@$(MAKEDEPEND_CXX); \
	cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d
	@$(CXX) -c $(CXXFLAGS) -o $@ $<

-include $(SRCS_CXX:.cpp=.P)

ifdef PCH_H
  $(PCH):$(PCH_H)
	@echo -e '\e[1;34mPrepare precompiled header $@...\e[0m'
	@$(CC) $(CFLAGS) -x c-header -c -o $@ $<
endif

ifdef PCH_CXX_H
$(PCH_CXX):$(PCH_CXX_H)
	@echo -e '\e[1;34mPrepare precompiled header $@...\e[0m'
	@$(CXX) $(CXXFLAGS) -x c++-header -c -o $@ $<
endif
