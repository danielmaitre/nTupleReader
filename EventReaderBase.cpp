#include "EventReaderBase.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TChain.h"
//#include "debug.h"


using namespace std;



void RootFileReaderBase::addFiles(std::vector<string> fileNames){
	std::vector<std::string>::iterator end=fileNames.end();
	for (std::vector<std::string>::iterator it=fileNames.begin();it!=end;++it){
		d_fin->Add((*it).c_str());
	}
	d_endEvent=d_fin->GetEntries();
}

void RootFileReaderBase::addFile(const std::string& fileName){
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
  init(NI,treeName.c_str(),doublePrecision,withNcount);
	d_startEvent=0;
	//no files are loaded so far
	d_endEvent=0;
	d_iEntry = 0;
	resetEventCounter();
	d_maxTrueEvent=0;
	d_startTrueEvent=0;
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


