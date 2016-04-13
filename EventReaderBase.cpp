#include "EventReaderBase.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TChain.h"
#include "TLeaf.h"
#include "TList.h"
#include "TFile.h"


//#include "debug.h"


using namespace std;



#define NEW_TREE_NAME      1
#define DOUBLE_PRECISION   2
#define WITH_NCOUNT        4


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
  std::cout << "Version " << version << std::endl;
  f.Close();
  return version;
}


void RootFileReaderBase::initStorage(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& fileName){
  if (!d_initialised){
    int version=nTupleVersion(fileName);
    string treeName;
    bool doublePrecision;
    bool withNcount;
    if ( ( version & NEW_TREE_NAME ) == NEW_TREE_NAME  ){
      treeName=std::string("BHSntuples");
    } else {
      treeName=std::string("t3");
    }
    if ( ( version & DOUBLE_PRECISION ) == DOUBLE_PRECISION  ){
      doublePrecision=true;
      std::cout << "Using double precision momenta..." << std::endl;
    } else {
      std::cout << "Using simple precision momenta..." << (version & DOUBLE_PRECISION)<< std::endl;
      doublePrecision=false;
    }
    if ( ( version & WITH_NCOUNT ) == WITH_NCOUNT  ){
      withNcount=true;
    } else {
      withNcount=false;
    }
    init(NI,treeName.c_str(),doublePrecision,withNcount);
  }  
}


void RootFileReaderBase::addFiles(NtupleInfo<MAX_NBR_PARTICLES>& NI,std::vector<string> fileNames){
	std::vector<std::string>::iterator end=fileNames.end();
	for (std::vector<std::string>::iterator it=fileNames.begin();it!=end;++it){
	  addFile(NI,*it);
	}
	d_endEvent=d_fin->GetEntries();
}

void RootFileReaderBase::addFile(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& fileName){
  if (!d_initialised){
    initStorage(NI,fileName);
  }
  d_fin->Add(fileName.c_str());
		long nEntries=(long)d_fin->GetEntries();
		d_endEvent=nEntries;
}

RootFileReaderBase::RootFileReaderBase(): d_fin(0){
	d_startEvent=0;
	//no files are loaded so far
	d_endEvent=0;
	d_iEntry = d_startEvent;
	resetEventCounter();
	d_maxTrueEvent=0;
	d_startTrueEvent=0;
}

RootFileReaderBase::RootFileReaderBase(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& treeName,bool doublePrecision,bool withNcount){
  //init(NI,treeName.c_str(),doublePrecision,withNcount);
	d_startEvent=0;
	//no files are loaded so far
	d_endEvent=0;
	d_iEntry = 0;
	resetEventCounter();
	d_maxTrueEvent=0;
	d_startTrueEvent=0;
	d_initialised=false;
	d_storage=&NI;
}

void RootFileReaderBase::init(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& treeName,bool doublePrecision,bool withNcount){
	d_fin = new TChain(treeName.c_str());

	NI.Assign(d_fin,doublePrecision,withNcount);

}


long RootFileReaderBase::getNumberOfEntries(){return (long)d_fin->GetEntries();};


RootFileReaderBase::~RootFileReaderBase(){ delete d_fin;}

bool RootFileReaderBase::readNextEntry(NtupleInfo<MAX_NBR_PARTICLES>& NI){
	while (d_iEntry < d_endEvent){
	    if (d_fin->GetEntry(d_iEntry)==0){
	    	//happens if entry cannot be read
	    	return false;
	    };
		d_iEntry++;
		if (NI.id!=d_lastID){ d_lastID=NI.id; d_iEvent++; }
		if (d_maxTrueEvent>0 && d_iEvent >= d_maxTrueEvent + d_startTrueEvent ){
			return false;
		}
		if (d_startTrueEvent>=0 && d_iEvent < d_startTrueEvent ){
			continue;
		}
		return true;
	}
	return false;
}

void RootFileReaderBase::getEntry(long i){
	d_fin->GetEntry(i-1); // now one-based call
	d_iEntry=i-1;
	//getting ready for the next point
	d_iEntry++;
};

bool RootFileReaderBase::getEntry(long i,NtupleInfo<MAX_NBR_PARTICLES>& NI){
	d_fin->GetEntry(i-1); // now one-based call
	d_iEntry=i-1;
	//getting ready for the next point
	d_iEntry++;
	if (NI.id!=d_lastID){ d_lastID=NI.id; d_iEvent++; }
	if (d_maxTrueEvent>0 && d_iEvent >= d_maxTrueEvent + d_startTrueEvent ){
		return false;
	}
	return true;
};

void RootFileReaderBase::setMaxEvent(long count){
	d_maxTrueEvent=count;
	//need to reset d_endEntry
	long nEntries=(long)d_fin->GetEntries();
	d_endEvent=nEntries;
};


