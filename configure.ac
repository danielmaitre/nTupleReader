AC_INIT([nTupleReader], [1.0], [daniel.maitre@durham.ac.uk])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_DISABLE_STATIC
AC_PROG_CXX
AM_PATH_PYTHON
m4_ifdef([AM_PROG_AR],[AM_PROG_AR])
AC_PROG_LIBTOOL
AC_CONFIG_FILES([
    Makefile
    test/Makefile
    examples/Makefile
    nTupleReader-config
    examples/nTupleReaderExample.cpp
    examples/nTupleReaderExample.py
	swig.xsl
	Doxyfile
])


m4_include([m4Macros/m4_ax_python.m4])

AX_PYTHON


AC_ARG_WITH([lhapdf-path], [AS_HELP_STRING([--with-lhapdf-path],
          [location of the lhapdf installation])],
          [],
          [with_lhapdf_path=yes])

AS_IF([test "x$with_lhapdf_path" != xyes],
          [ 
	    
			AC_SUBST([LHAPDF_CXXFLAGS], ["`$with_lhapdf_path/bin/lhapdf-config --cppflags`"])
			AC_SUBST([LHAPDF_LDFLAGS], ["`$with_lhapdf_path/bin/lhapdf-config --ldflags`"])
			LHAPDFINCDIR=`$with_lhapdf_path/bin/lhapdf-config --incdir`
	  ],
        [
		AC_SUBST([LHAPDF_CXXFLAGS], [`lhapdf-config --cppflags`])
	   AC_SUBST([LHAPDF_LDFLAGS], ["`lhapdf-config --ldflags`"])
			LHAPDFINCDIR=`lhapdf-config --incdir`
])

oldCPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS -I$LHAPDFINCDIR"
AC_LANG([C++])
AC_HEADER_STDC
AC_CHECK_HEADERS([LHAPDF/LHAPDF.h],[],[AC_ERROR([Need LHAPDF header, please specifiy the path to the LHAPDF installation with the --with-lhapdf-path option.])])
CPPFLAGS=$oldCPPFLAGS



AC_ARG_WITH([root-path], [AS_HELP_STRING([--with-root-path],
          [location of the root installation])],
          [],
          [with_root_path=yes])

AS_IF([test "x$with_root_path" != xyes],
          [AC_SUBST([ROOT_CXXFLAGS], ["`$with_root_path/bin/root-config --cflags`"])
	   AC_SUBST([ROOT_LDFLAGS], ["`$with_root_path/bin/root-config --libs`"])
	   AC_SUBST([ROOTCINT], ["`$with_root_path/bin/root-config --prefix`/bin/rootcint"])
	   ROOTINCDIR=`$with_root_path/bin/root-config --incdir`
	  ],
          [AC_SUBST([ROOT_CXXFLAGS], [`root-config --cflags`])
	   AC_SUBST([ROOT_LDFLAGS], ["`root-config --ldflags`"])
	   AC_SUBST([ROOT_LIBS], ["`root-config --libs`"])
	   AC_SUBST([ROOTCINT], ["`root-config --prefix`/bin/rootcint"])
	   ROOTINCDIR=`root-config --incdir`
])


oldCPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS -I$ROOTINCDIR"
AC_LANG([C++])
AC_CHECK_HEADERS([TBuffer.h TChain.h TFile.h TH1F.h TKey.h TList.h TObject.h TString.h TSystem.h TTree.h
],[],[AC_ERROR([Need ROOT headers, please specifiy the path to the ROOT installation with the --with-root-path option. This can also fail if you are using ROOT >= 6.0 and do not the c++11 standards are not supported by default, consider using CXXFLAGS=-std=c++11])])
CPPFLAGS=$oldCPPFLAGS


AC_ARG_WITH([hepmc-path], [AS_HELP_STRING([--with-hepmc-path],
          [location of the hepmc installation])],
          [use_hepmc_path=true
          AC_SUBST([HEPMC_CXXFLAGS], ["-I$with_hepmc_path/include -DUSE_HEPMC"])
	      AC_SUBST([HEPMC_LDFLAGS], ["-L$with_hepmc_path/lib/ -lHepMC -Wl,-rpath -Wl,$with_hepmc_path"])
	      ])
AM_CONDITIONAL([USE_HEPMC], [test "x$use_hepmc_path" == "xtrue" ])



AC_ARG_ENABLE([debug],
     [  --enable-debug    Turn on debugging],
     [case "${enableval}" in
       yes) debug=true ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
     esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

if test x$debug = xtrue ; then
    AM_CPPFLAGS="$AM_CPPFLAGS -DDEBUG_ON "
    echo "Using debugging"
else
    AM_CPPFLAGS="$AM_CPPFLAGS  "
    echo "Not using debugging"
fi

AC_SUBST([AM_CPPFLAGS])



AC_ARG_ENABLE(pythoninterface,
[  --enable-pythoninterface    compiles the python interface],
[case "${enableval}" in
  yes) pythoninterface=true ;;
  no)  pythoninterface=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-pythoninterface) ;;
esac],[pythoninterface=true])
AM_CONDITIONAL([PYTHONINTERFACE], [test x$pythoninterface = xtrue])


if test x$pythoninterface = xtrue ; then
	PYTHON_INCLUDE=`python-config --includes `
	depfile=${PWD}/NTRInterface.d
    AC_MSG_NOTICE('$depfile')
    echo Creating ${depfile}
    test -f $depfile || echo "# dummy" >> $depfile
fi

AC_SUBST([PYTHON_INCLUDE])


if test x$pythoninterface = xtrue ; then
	echo "Use python interface"
fi

echo chmod +x $ac_top_builddir/test/all.py

AC_OUTPUT
