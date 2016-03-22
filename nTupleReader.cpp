/*
 * nTupleReader.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: daniel
 */




#include "nTupleReader.h"
#include "nTupleReader_impl.h"
#include <string>
#include <vector>
#include <iostream>
#include "weightFunctions.h"
#include "pdf.h"
#include "LHAPDF/LHAPDF.h"
#include "TLeaf.h"

nTupleReader::nTupleReader(): d_impl(new nTupleReader_impl()){
	};

nTupleReader::nTupleReader(const std::string& treeName): d_impl(new nTupleReader_impl(treeName)) {
};



bool nTupleReader::nextEntry(){return d_impl->nextEntry();};

int nTupleReader::getParticleNumber(){return d_impl->getParticleNumber();}

double nTupleReader::getEnergy(int i){return d_impl->getEnergy(i);};
double nTupleReader::getX(int i){return d_impl->getX(i);};
double nTupleReader::getY(int i){return d_impl->getY(i);};
double nTupleReader::getZ(int i){return d_impl->getZ(i);};
int nTupleReader::getPDGcode(int i){return d_impl->getPDGcode(i);}
double nTupleReader::getX1(){return d_impl->getX1();};
double nTupleReader::getX2(){return d_impl->getX2();};
double nTupleReader::getId1(){return d_impl->getId1();};
double nTupleReader::getId2(){return d_impl->getId2();};
short nTupleReader::getAlphasPower(){return d_impl->getAlphasPower();}
double nTupleReader::getRenormalizationScale(){return d_impl->getRenormalizationScale();}
double nTupleReader::getFactorizationScale(){return d_impl->getFactorizationScale();}
double nTupleReader::getWeight(){return d_impl->getWeight();}
double nTupleReader::getWeight2(){return d_impl->getWeight2();}
double nTupleReader::getMEWeight(){return d_impl->getMEWeight();}
double nTupleReader::getMEWeight2(){return d_impl->getMEWeight2();}
char nTupleReader::getType(){return d_impl->getType();}

int nTupleReader::getID(){return d_impl->getID();}





void nTupleReader::addFile(const std::string& fileName){
	d_impl->addFile(fileName);
}

void nTupleReader::addFiles(const std::vector<std::string>& files){
	d_impl->addFiles(files);
}

void nTupleReader::setPDF(const std::string& name){
	d_impl->setPDF(name);
}

void nTupleReader::setPDFmember(int member){
	d_impl->setPDFmember(member);
}

double nTupleReader::computeWeight(double newFacScale,double newRenScale){
	d_impl->computeWeight(newFacScale,newRenScale);
};

double nTupleReader::computeWeight2(double newFacScale,double newRenScale){
	d_impl->computeWeight2(newFacScale,newRenScale);
};


void nTupleReader::setPP(){
	d_impl->setPP();
}
void nTupleReader::setPPbar(){
	d_impl->setPPbar();
}



nTupleReader::~nTupleReader(){
	delete d_impl;
}

#ifdef USE_HEPMC
	void nTupleReader::fillHepMC(HepMC::GenEvent& evt){d_impl->fillHepMC(evt);};
	void nTupleReader::fillHepMC(HepMC::GenEvent& evt,double xsec,double err){d_impl->fillHepMC(evt,xsec,err);};
	void nTupleReader::setCMSEnergy(double CMS_energy){d_impl->setCMSEnergy(CMS_energy);};
	void nTupleReader::setColliderType(colliderType ct){d_impl->setColliderType(ct);};
#endif

	void nTupleReader::resetCrossSection(){d_impl->resetCrossSection();};
	double nTupleReader::getCrossSection(){return d_impl->getCrossSection();};
	double nTupleReader::getCrossSectionError(){return d_impl->getCrossSectionError();};


