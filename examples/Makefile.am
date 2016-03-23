FC=gfortran
NTR_PATH=$(builddir)/../

all: nTupleReaderExample.o nTupleReaderExample

clean-local:
	rm -f *.o nTupleReaderExample

nTupleReaderExample.o:	nTupleReaderExample.cpp
	$(CXX) -c -g   $$(sh $(NTR_PATH)/nTupleReader-config --cxxflags) -o $@ $<



nTupleReaderExample:	nTupleReaderExample.o	
	$(CXX) -o $@ $< $$(sh $(NTR_PATH)/nTupleReader-config --rpath) $$(sh $(NTR_PATH)/nTupleReader-config --ldflags) $$(sh $(NTR_PATH)/nTupleReader-config --libs)  


EXTRA_dist = 

examplesdir = $(pkgdatadir)/examples

dist_examples_DATA = nTupleReaderExample.cpp nTupleReaderExample.py
