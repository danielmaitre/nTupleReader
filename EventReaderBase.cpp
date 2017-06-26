#include "EventReaderBase.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TChain.h"
#include "TLeaf.h"
#include "TList.h"
#include "TFile.h"
#include "version.h"

//#include "debug.h"


using namespace std;



#define NEW_TREE_NAME      1
#define DOUBLE_PRECISION   2
#define WITH_NCOUNT        4
#define WITH_MINLO         8



void RootFileReaderBase::initStorage(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& fileName){
  if (!d_initialised){
    d_version=nTupleVersion(fileName);
    if ( nTupleHasDoublePrecision(d_version) ){
      std::cout << "Using double precision momenta..." << std::endl;
    } else {
      std::cout << "Using simple precision momenta..." <<  std::endl;
    }
    if ( nTupleHasNcount(d_version) ){

    } else {

    }
    if ( nTupleHasMinlo(d_version) ){
    } else {
    }
    init(NI,d_version);
  }
}


void RootFileReaderBase::addFiles(NtupleInfo<MAX_NBR_PARTICLES>& NI,std::vector<string> fileNames){
	std::vector<std::string>::iterator end=fileNames.end();
	for (std::vector<std::string>::iterator it=fileNames.begin();it!=end;++it){
	  addFile(NI,*it);
	}
	d_endEvent=(long)d_fin->GetEntries();
	//std::cout << " nEntries after addFiles in RootFileReader: " << (long)d_fin->GetEntries()<< std::endl;
}

void RootFileReaderBase::addFile(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& fileName){
  if (!d_initialised){
    initStorage(NI,fileName);
    d_initialised=true;
  }
  d_fin->Add(fileName.c_str());
		long nEntries=(long)d_fin->GetEntries();
		d_endEvent=nEntries;
		cout << "nEntries so far: " << d_endEvent << "(" << d_fin->GetEntries() <<")"<< std::endl;
}

RootFileReaderBase::RootFileReaderBase(): d_fin(0){
	d_startEvent=0;
	//no files are loaded so far
	d_endEvent=0;
	d_iEntry = d_startEvent;
	resetEventCounter();
	d_maxTrueEvent=0;
	d_startTrueEvent=0;
	d_initialised=false;
}

RootFileReaderBase::RootFileReaderBase(NtupleInfo<MAX_NBR_PARTICLES>& NI,int version){
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

void RootFileReaderBase::init(NtupleInfo<MAX_NBR_PARTICLES>& NI,int version){
	if ( nTupleHasNewTree(d_version)  ){
		d_fin = new TChain("BHSntuples");
	} else {
		d_fin = new TChain("t3");
	}
	NI.Assign(d_fin,version);

}


long RootFileReaderBase::getNumberOfEntries(){return (long)d_fin->GetEntries();};


RootFileReaderBase::~RootFileReaderBase(){ delete d_fin;}

void RootFileReaderBase::transferFromCache(NtupleInfo<MAX_NBR_PARTICLES>& NI){
	bool doublePrecision=nTupleHasDoublePrecision(d_version);
	for (int ii=0;ii<NI.nparticle;ii++){
		if (doublePrecision){
			NI.pxD[ii]=NI.pxDCache[ii];
			NI.pyD[ii]=NI.pyDCache[ii];
			NI.pzD[ii]=NI.pzDCache[ii];
			NI.ED[ii]=NI.EDCache[ii];
		} else {
			NI.px[ii]=NI.pxCache[ii];
			NI.py[ii]=NI.pyCache[ii];
			NI.pz[ii]=NI.pzCache[ii];
			NI.E[ii]=NI.ECache[ii];
		}
		NI.kf[ii]=NI.kfCache[ii];
		NI.nparticle=NI.nparticleCache;
	}
}

bool RootFileReaderBase::readNextEntry(NtupleInfo<MAX_NBR_PARTICLES>& NI){
	bool isNNLO=nTupleHasLogCoefficients(d_version);
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
		if (isNNLO){
			if (NI.nparticleCache!=0){
				transferFromCache(NI);
			}
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

bool RootFileReaderBase::hasNcount(){
	return nTupleHasNcount(d_version);
};

