
#ifndef _H_VERSION_H_
#define _H_VERSION_H_

#define NEW_TREE_NAME      1
#define DOUBLE_PRECISION   2
#define WITH_NCOUNT        4
#define WITH_MINLO         8

#include <string>

bool nTupleHasNewTree(int version);
bool nTupleHasDoublePrecision(int version);
bool nTupleHasNcount(int version);
bool nTupleHasMinlo(int version);


int nTupleVersion(const std::string& filename);




#endif /* _H_VERSION_H_  */
									       
