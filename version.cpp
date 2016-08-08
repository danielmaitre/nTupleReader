#include "version.h"
#include "TChain.h"
#include "TLeaf.h"
#include "TList.h"
#include "TFile.h"
#include <iostream>

bool nTupleHasNewTree(int version){
	return ( ( version & NEW_TREE_NAME ) == NEW_TREE_NAME  );
}
bool nTupleHasDoublePrecision(int version){
	return ( ( version & DOUBLE_PRECISION ) == DOUBLE_PRECISION  );
}
bool nTupleHasNcount(int version){
	return ( ( version & WITH_NCOUNT ) == WITH_NCOUNT  );
}
bool nTupleHasMinlo(int version){
	return  ( ( version & WITH_MINLO ) == WITH_MINLO  );
}
bool nTupleHasHadron1(int version){
	return  ( ( version & WITH_HADRON1 ) == WITH_HADRON1  );
}
bool nTupleHasHadron2(int version){
	return  ( ( version & WITH_HADRON2 ) == WITH_HADRON2  );
}
bool nTupleHasUserWeights(int version){
	return  ( ( version & WITH_USERWEIGHTS ) == WITH_USERWEIGHTS  );
}

int nTupleVersion(const std::string& filename){
  int version=0;
  TFile f(filename.c_str(),"READONLY");
  TList* l=f.GetListOfKeys();
  TTree* t;

  if (l->FindObject("BHSntuples")){
    version |=  NEW_TREE_NAME ;
    t=dynamic_cast<TTree*>(f.Get("BHSntuples"));
  } else {
    t=dynamic_cast<TTree*>(f.Get("t3"));
  }
  TBranch* b=t->FindBranch("px");
  TLeaf* leaf=b->GetLeaf("px");
  if ( std::string(leaf->GetTypeName()) == "Double_t"){
    version |=  DOUBLE_PRECISION ;
  } else if ( std::string(leaf->GetTypeName()) != "Float_t"){
    std::cerr << "Could not determine the precision of the momenta! got type: '" <<leaf->GetTypeName() <<"'"   << std::endl;
  }

  if (t->FindBranch("ncount")){
    version |= WITH_NCOUNT;
  }
  if (t->FindBranch("id1p")){
    version |= WITH_MINLO;
  }

  if (t->FindBranch("id1") and t->FindBranch("x1")){
    version |= WITH_HADRON1;
  }
  if (t->FindBranch("id2") and t->FindBranch("x2")){
    version |= WITH_HADRON2;
  }

  if (t->FindBranch("nuwgt") and t->FindBranch("usr_wgts")){
    version |= WITH_USERWEIGHTS;
  }


  std::cout << "Version " << version << std::endl;
  f.Close();
  return version;
}


