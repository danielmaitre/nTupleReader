
#ifndef _H_VERSION_H_
#define _H_VERSION_H_

#define NEW_TREE_NAME      		1
#define DOUBLE_PRECISION   		2
#define WITH_NCOUNT        		4
#define WITH_MINLO         		8
#define WITH_HADRON1      		16
#define WITH_HADRON2      		32
#define WITH_USERWEIGHTS  		64
#define WITH_LOGCOEFFICIENTS 	128
#include <string>

bool nTupleHasNewTree(int version);
bool nTupleHasDoublePrecision(int version);
bool nTupleHasNcount(int version);
bool nTupleHasMinlo(int version);
bool nTupleHasHadron1(int version);
bool nTupleHasHadron2(int version);
bool nTupleHasUserWeights(int version);
bool nTupleHasLogCoefficients(int version);


int nTupleVersion(const std::string& filename);




#endif /* _H_VERSION_H_  */
									       
