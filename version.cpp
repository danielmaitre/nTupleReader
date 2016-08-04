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
  std::cout << "Version " << version << std::endl;
  f.Close();
  return version;
}


