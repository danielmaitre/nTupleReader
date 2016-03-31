#SUBDIRS = test examples
SUBDIRS = test 

ACLOCAL_AMFLAGS=-I m4

AM_CPPFLAGS += $(PYTHON_INCLUDE) -I$(PYTHON_INCLUDE_DIR)

bin_PROGRAMS= getInfo
lib_LTLIBRARIES=libNTR.la libInfo.la

libNTR_la_SOURCES = \
	pdf.cpp pdf.h \
	NtupleInfo.h NtupleInfo.cpp \
	EventReaderBase.cpp EventReaderBase.h \
	weightFunctions.cpp weightFunctions.h \
	nTupleReader.cpp nTupleReader.h \
	nTupleReader_impl.cpp nTupleReader_impl.h 

if USE_HEPMC
libNTR_la_SOURCES += HepMC_utils.cpp HepMC_utils.h
endif

InfoStructDict.cpp:	InfoStruct.cpp InfoStruct.h InfoStructLinkDef.h
	echo ${PATH} 
	$(ROOTCINT) -f InfoStructDict.cpp -c $(srcdir)/InfoStruct.h $(srcdir)/InfoStructLinkDef.h

InfoStructDict.h:	InfoStructDict.cpp
	@

CLEANFILES = InfoStructDict.h InfoStructDict.cpp 

libInfo_la_SOURCES = InfoStruct.cpp InfoStruct.h InfoStructLinkDef.h 
nodist_libInfo_la_SOURCES = InfoStructDict.h InfoStructDict.cpp 

if PYTHONINTERFACE
AM_CPPFLAGS += $(PYTHON_INCLUDE) -I$(PYTHON_INCLUDE_DIR)

lib_LTLIBRARIES+=libPyNTR.la

libPyNTR_la_SOURCES =  NTRInterface.cpp

NTRInterface.d:	nTupleReader.i 
	swig $(AM_CPPFLAGS) -M -Wall -python -c++ -o NTRInterface.cpp $(srcdir)/nTupleReader.i 

@am__include@ ./NTRInterface.d

xml/index.xml:	nTupleReader.h
	doxygen Doxyfile

swig_doc.i:	xml/index.xml
	xsltproc swig.xsl xml/index.xml > temp_doc.i
	cat temp_doc.i | sed 's/"/\\"/g' | sed 's/__QuOtE__/"/g' | sed 's/rm th\}/\\rm th\}/g' > swig_doc.i
	rm temp_doc.i


NTRInterface.cpp:	nTupleReader.i  swig_doc.i 
	swig $(AM_CPPFLAGS) -Wall -python -c++ -o NTRInterface.cpp $(srcdir)/nTupleReader.i 

CLEANFILES += NTRInterface.cpp

endif


getInfo_SOURCES = getInfo.cpp

#include HepMC first because HepMC is also installed at the same 
#place as other libraries, but with the wrong version
AM_LDFLAGS =  $(FASTJET_LDFLAGS)
AM_LDFLAGS += $(ROOT_LIBS)
AM_LDFLAGS += $(LHAPDF_LDFLAGS)
if USE_HEPMC
AM_LDFLAGS += $(HEPMC_LDFLAGS)
endif

libInfo_la_LDFLAGS = $(ROOT_LDFLAGS)

AM_CXXFLAGS =  $(FASTJET_CXXFLAGS)
AM_CXXFLAGS += $(ROOT_CXXFLAGS)
AM_CXXFLAGS += $(LHAPDF_CXXFLAGS)
if USE_HEPMC
AM_CXXFLAGS += $(HEPMC_CXXFLAGS)
endif


getInfo_LDADD = libInfo.la 


libPyNTR_la_LIBADD = libNTR.la



if PYTHONINTERFACE
uninstall-hook:
	rm -f  $(DESTDIR)$(libdir)/_nTupleReader.so
	if test -f  $(DESTDIR)$(libdir)/nTupleReader.py; then rm -f $(DESTDIR)$(libdir)/nTupleReader.py; fi



install-exec-hook:
	ls -l ${prefix}/lib
#	echo prefix=${prefix}
#	echo prefix=${install-prefix}
#	echo libdir=${libdir}
#	echo destdir=${DESTDIR}
	ln -fs $(DESTDIR)$(libdir)/libPyNTR.so $(DESTDIR)$(libdir)/_nTupleReader.so
	if test -f $(top_builddir)/nTupleReader.py; then \
	cp $(top_builddir)/nTupleReader.py $(DESTDIR)$(libdir); \
	else \
	cp $(srcdir)/nTupleReader.py $(DESTDIR)$(libdir)	;	\
	fi
	if test -f $(top_builddir)/InfoStructDict_rdict.pcm; then \
	cp $(top_builddir)/InfoStructDict_rdict.pcm $(DESTDIR)$(libdir); \
	fi


CLEANFILES += nTupleReader.py NTRInterface.d

EXTRA_DIST=nTupleReader.i nTupleReader.py

endif

bin_SCRIPTS=nTupleReader-config

pkginclude_HEADERS = nTupleReader.h EventReaderBase.h NtupleInfo.h pdf.h weightFunctions.h InfoStruct.h

@DX_RULES@

MOSTLYCLEANFILES=$(DX_CLEANFILES)
