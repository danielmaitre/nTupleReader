/*
 * nTupleReader_impl.cpp
 *
 *  Created on: 8 Jun 2015
 *      Author: daniel
 */


#include "nTupleReader_impl.h"
#include "nTupleReader.h"
#include <string>
#include <vector>
#include <iostream>
#include "weightFunctions.h"
#include "pdf.h"
#include "LHAPDF/LHAPDF.h"
#include "TLeaf.h"
#ifdef USE_HEPMC
#include "HepMC/GenEvent.h"
using namespace HepMC;
#endif


nTupleReader_impl::nTupleReader_impl(): RootFileReaderBase() {
	init(d_NI,"BHSntuples");
	d_member=-1;
	d_fin->SetBranchAddress("alphasPower",&d_alphasPower);
	d_fin->SetBranchAddress("part",d_part);
	resetCrossSection();
};

nTupleReader_impl::nTupleReader_impl(const std::string& treeName): RootFileReaderBase(){
	init(d_NI,treeName);
	d_member=-1;
	std::string alphaType;
	//alphasPower branch will be added when files are loaded
	d_fin->SetBranchAddress("part",d_part);
	resetCrossSection();
};

void nTupleReader_impl::addFile(const std::string& fileName){
	RootFileReaderBase::addFile(fileName);
	std::string alphaType;
	TBranch* b=d_fin->GetBranch("alphasPower");
	if (b){
		TLeaf* l=b->GetLeaf("alphasPower");
		if (l){
			alphaType=std::string(l->GetTypeName());
		} else {
			std::cout << "could not find leaf" << std::endl;
		}
	} else {
		std::cout << "could not find branch alphasPower" << std::endl;
	}
	//std::cout << "alphaType: \'" << alphaType << "\'" << std::endl;
	if (alphaType == "Char_t"){
		std::cout << "This appears to be a file with the wrong type for alphasPower, trying to correct..." << std::endl;
		d_fin->SetBranchAddress("alphasPower",&d_alphasPowerChar);
		convertFromChar=true;
	} else {
		d_fin->SetBranchAddress("alphasPower",&d_alphasPower);
		convertFromChar=false;
	}
}

void nTupleReader_impl::setPDF(const std::string& name){
#ifdef LHAPDF_NEW_VERSION
	d_pdfset = LHAPDF::PDFSet(name);
	clearPDFs();
	d_pdfs = d_pdfset.mkPDFs();
	d_pdf=d_pdfs[0];
#else
	static bool hasDoneInit=false;
	if (!hasDoneInit){
		LHAPDF::initLHAPDF();
		hasDoneInit=true;
	}
	LHAPDF::initPDFSetByName(name);
	LHAPDF::initPDF(0);
#endif
}

void nTupleReader_impl::setPDFmember(int member){
	if (member!=d_member){
#ifdef LHAPDF_NEW_VERSION
		d_pdf=d_pdfs[member];
#else
		LHAPDF::initPDF(member);
#endif
		d_member=member;
	}
}

double nTupleReader_impl::computeWeight(double newFacScale,double newRenScale){
	if (convertFromChar) {
		d_alphasPower=static_cast<short>(d_alphasPowerChar);
		//std::cout <<  d_alphasPower << d_alphasPowerChar << std::endl;
	}
#ifdef LHAPDF_NEW_VERSION
	switch ( getType() ){
	case 'B': return computeBornWeight(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	case 'I':  return computeVsubWeight(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	case 'V': case 'L': return computeLoopWeight(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	case 'R': return computeBornWeight(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	}
#else
	switch ( getType() ){
	case 'B': return computeBornWeight(d_NI,newFacScale,newRenScale,getAlphasPower());
	case 'I':  return computeVsubWeight(d_NI,newFacScale,newRenScale,getAlphasPower());
	case 'V': case 'L': return computeLoopWeight(d_NI,newFacScale,newRenScale,getAlphasPower());
	case 'R': return computeBornWeight(d_NI,newFacScale,newRenScale,getAlphasPower());
	}
#endif
};

double nTupleReader_impl::computeWeight2(double newFacScale,double newRenScale){
#ifdef LHAPDF_NEW_VERSION
	switch ( getType() ){
	case 'B': return computeBornWeight(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	case 'I':  return computeVsubWeight(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	case 'V': case 'L': return computeLoopWeight(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	case 'R': return computeRealWeightWithProperStat(d_NI,newFacScale,newRenScale,getAlphasPower(),d_pdf);
	}
#else
	switch ( getType() ){
	case 'B': return computeBornWeight(d_NI,newFacScale,newRenScale,getAlphasPower());
	case 'I':  return computeVsubWeight(d_NI,newFacScale,newRenScale,getAlphasPower());
	case 'V': case 'L': return computeLoopWeight(d_NI,newFacScale,newRenScale,getAlphasPower());
	case 'R': return computeRealWeightWithProperStat(d_NI,newFacScale,newRenScale,getAlphasPower());
	}
#endif
};


void nTupleReader_impl::setPP(){
	Pdf::initialState::setInitialState(1,1);
}
void nTupleReader_impl::setPPbar(){
	Pdf::initialState::setInitialState(1,-1);
}

#ifdef LHAPDF_NEW_VERSION
void nTupleReader_impl::clearPDFs(){
	for (int i =0;i<d_pdfs.size();i++){
			delete d_pdfs[i];
	}
}
#endif


nTupleReader_impl::~nTupleReader_impl(){
#ifdef LHAPDF_NEW_VERSION
	clearPDFs();
#endif
}

#ifdef USE_HEPMC

void nTupleReader_impl::fillHepMC(GenEvent& evt,double xsec,double err){
	evt.use_units(Units::GEV, Units::MM);
	evt.weights().push_back(d_NI.wgt2);

	GenVertex* v = new GenVertex();
	evt.add_vertex(v);
	GenParticle* gp1;
	GenParticle* gp2;
	switch (d_collider.d_colliderType){
	case pp: {
		gp1 = new GenParticle(d_collider.d_beam1, 2212, 4);
		gp2 = new GenParticle(d_collider.d_beam2, 2212, 4);
	} ; break;
	case ppbar: {
		gp1 = new GenParticle(d_collider.d_beam1, 2212, 4);
		gp2 = new GenParticle(d_collider.d_beam2, -2212, 4);
	} ; break;
	}
	v->add_particle_in(gp1);
	v->add_particle_in(gp2);
	evt.set_beam_particles(gp1, gp2);

	for (int i = 0; i < d_NI.nparticle; ++i) {
	  FourVector p(d_NI.px[i], d_NI.py[i],d_NI.pz[i],d_NI.E[i]);
	  GenParticle* gp = new GenParticle(p, d_NI.kf[i], 1);
	  gp->set_generated_mass(0);
	  v->add_particle_out(gp);
	}

    HepMC::GenCrossSection Gxsec;
    Gxsec.set_cross_section(xsec, err);
    evt.set_cross_section(Gxsec);

    evt.set_event_number(d_NI.id);
}


void nTupleReader_impl::fillHepMC(GenEvent& evt){
	evt.use_units(Units::GEV, Units::MM);
	evt.weights().push_back(d_NI.wgt2);

	GenVertex* v = new GenVertex();
	evt.add_vertex(v);
	GenParticle* gp1;
	GenParticle* gp2;
	switch (d_collider.d_colliderType){
	case pp: {
		gp1 = new GenParticle(d_collider.d_beam1, 2212, 4);
		gp2 = new GenParticle(d_collider.d_beam2, 2212, 4);
	} ; break;
	case ppbar: {
		gp1 = new GenParticle(d_collider.d_beam1, 2212, 4);
		gp2 = new GenParticle(d_collider.d_beam2, -2212, 4);
	} ; break;
	}
	v->add_particle_in(gp1);
	v->add_particle_in(gp2);
	evt.set_beam_particles(gp1, gp2);

	for (int i = 0; i < d_NI.nparticle; ++i) {
	  FourVector p(d_NI.px[i], d_NI.py[i],d_NI.pz[i],d_NI.E[i]);
	  GenParticle* gp = new GenParticle(p, d_NI.kf[i], 1);
	  gp->set_generated_mass(0);
	  v->add_particle_out(gp);
	}

    HepMC::GenCrossSection Gxsec;
    Gxsec.set_cross_section(getCrossSection(), getCrossSectionError());
    evt.set_cross_section(Gxsec);

    evt.set_event_number(d_NI.id);
}


void nTupleReader_impl::setCMSEnergy(double energy){
	d_collider.setCMSEnergy(energy);
}
void nTupleReader_impl::setColliderType(colliderType ct){
	d_collider.d_colliderType=ct;
}
#endif

void nTupleReader_impl::resetCrossSection(){
	d_lastID=-1;
	d_totalWeights=0;
	d_totalWeightsSquared=0;
	d_nEvents=0;
	d_eventWeight=0;
}

void nTupleReader_impl::updateCrossSection(){
	d_totalWeights+=getWeight2();


	if ( getID()!=d_lastID){
		d_lastID=getID();
		d_nEvents++;
		d_totalWeightsSquared+=d_eventWeight*d_eventWeight;
		d_eventWeight=0;
	}

	d_eventWeight+=getWeight2();
}

bool nTupleReader_impl::nextEntry(){
	bool res = readNextEntry(d_NI);
	if (res) {
		updateCrossSection();
	};
	return res;
};

double nTupleReader_impl::getCrossSection(){
      if (d_nEvents>0){
    	  return d_totalWeights/d_nEvents;
      } else {
    	 return  d_totalWeights;
      }
}

double nTupleReader_impl::getCrossSectionError(){
	if (d_nEvents>2){
		return sqrt((d_totalWeightsSquared/d_nEvents -(d_totalWeights*d_totalWeights)/d_nEvents/d_nEvents)/d_nEvents);
	} else {
		return d_totalWeights;
}
}


