%module nTupleReader

%include "stl.i"
%include "std_string.i"

%include "std_vector.i"

%include "carrays.i"
%array_functions(float, floatArray);

%feature("autodoc", "3");
%include "swig_doc.i"

%{
#include "nTupleReader.h"
#include "nTupleReader_impl.h"
%}

namespace std {
   %template(vector_str) vector<string>;
   %template(vector_d) vector<double>;
   %template(vector_i) vector<int>;
}; 

%include <typemaps.i>
%apply bool & INOUT { bool& isValid };

%include "EventReaderBase.h"
%include "nTupleReader.h"
	
