dist_datadir= data

sampledir = $(pkgdatadir)/


dist_sample_DATA = \
	data/sample.root \
	data/RS_s1.root  \
	data/RS_s2.root 
 
if PYTHONINTERFACE 
check_SCRIPTS = all.py

TESTS = all.py

EXTRA_DIST = all.py.in StartAndEnd.txt OutOfBounds.txt

CLEANFILES = all.py

all.py:	all.py.in
	rm -f _NtuplesReader.so
	if [ -f ${abs_top_builddir}/.libs/libPyNTR.dylib ] ; then ln -s ${abs_top_builddir}/.libs/libPyNTR.dylib _NtuplesReader.so;fi
	if [ -f ${abs_top_builddir}/.libs/libPyNTR.so ] ; then	ln -sf ${abs_top_builddir}/.libs/libPyNTR.so _nTupleReader.so;fi
	echo "#!/usr/bin/env python" > all.py	
	echo "SRCDIR='@srcdir@'" >> all.py
	echo "BUILDDIR='@abs_top_builddir@'" >> all.py
	cat ${srcdir}/all.py.in >> all.py
	chmod +x all.py




endif
