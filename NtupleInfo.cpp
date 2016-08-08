/*
 * NtupleInfo.cpp
 *
 *  Created on: Jan 12, 2012
 *      Author: daniel
 */

#include "NtupleInfo.h"
#include "TChain.h"
#include "TLeaf.h"
#include <iostream>
#include "version.h"

template <int N> void NtupleInfo<N>::Assign(TChain* fin, int version){
  AssignNoAlpha(fin,version);
  fin->SetBranchAddress("alphas" ,&alphas);
}


template <int N> void NtupleInfo<N>::AssignNoAlpha(TChain* fin,int version){


	bool doublePrecision=nTupleHasDoublePrecision(version);
	bool withNcount=nTupleHasNcount(version);
	bool withMinlo=nTupleHasMinlo(version);
	bool withHadron1=nTupleHasHadron1(version);
	bool withHadron2=nTupleHasHadron2(version);

	fin->SetBranchAddress("id",&id);
	if (!doublePrecision){
	  fin->SetBranchAddress("px",px);
	  fin->SetBranchAddress("py",py);
	  fin->SetBranchAddress("pz",pz);
	  fin->SetBranchAddress("E" ,E);
	  hasDoublePrecisionMomenta=false;
	} else {
	  fin->SetBranchAddress("px",pxD);
	  fin->SetBranchAddress("py",pyD);
	  fin->SetBranchAddress("pz",pzD);
	  fin->SetBranchAddress("E" ,ED);
	  hasDoublePrecisionMomenta=true;
	}
	if (withNcount){
	  fin->SetBranchAddress("ncount" ,&ncount);
	} else {
	  ncount=1;   //this makes sure the default is set right
	}

	fin->SetBranchAddress("weight" ,&wgt);
	fin->SetBranchAddress("weight2" ,&wgt2);
	fin->SetBranchAddress("kf" ,kf);
	fin->SetBranchAddress("nparticle", &nparticle);

	if (withHadron1){
		fin->SetBranchAddress("id1",&id1);
		fin->SetBranchAddress("x1",&x1);
		fin->SetBranchAddress("x1p",&x1p);
	}
	if (withHadron2){
		fin->SetBranchAddress("id2",&id2);
		fin->SetBranchAddress("x2",&x2);
		fin->SetBranchAddress("x2p",&x2p);
	}
	fin->SetBranchAddress("me_wgt" ,&me_wgt);
	fin->SetBranchAddress("me_wgt2" ,&me_wgt2);
	fin->SetBranchAddress("fac_scale" ,&muF);
	fin->SetBranchAddress("ren_scale" ,&muR);
	if ( nTupleHasUserWeights(version) ){
		fin->SetBranchAddress("nuwgt" ,&numgt);
		fin->SetBranchAddress("usr_wgts" ,usr_wgts);
	} else {
		numgt=0;  // this is important so that loops over the user weights are correct
	}
	if (withMinlo){
		fin->SetBranchAddress("id1p",&id1p);
		fin->SetBranchAddress("id2p",&id2p);
	}

}

template <int N> void NtupleInfo<N>::print(std::ostream& os){
	os <<  "Id: " << id << '\n';
	os <<  "Weight: " << wgt  << '\n';
	os <<  "ME weight: " << me_wgt  << '\n';
	if (wgt!=wgt2){
		os <<  "Weight 2: " << wgt2 << '\n';
		os <<  "ME weight 2: " << me_wgt2  << '\n';
	}

	os <<  "Fac scale: " << muF << '\n';
	os <<  "Ren scale: " << muR << '\n';
	os <<  "Ren scale: " << muR << '\n';
	os << "Alpha_s: " << alphas<< '\n';
	os <<  "Nbr particles: " << nparticle<< '\n';
	for (int i=0;i<nparticle;i++){
		os <<  "  "<< i << ": kf:" << kf[i] << "\tP=(" << E[i] << "," << px[i]<< ","<<py[i]<<","<< pz[i] << ")\n";
	}
	os <<  "I1: " << id1<< '\n';
	os <<  "I2: " << id2<< '\n';

	os <<  "x1: " << x1<< '\n';
	os <<  "x2: " << x2<< '\n';
	if (x1p!=0 && x1p!=1){
		os <<  "x1p: " << x1p<< '\n';
	}
	if (x2p!=0&& x2p!=1){
		os <<  "x2p: " << x2p<< '\n';
	}
	os <<  "id: " << id<< '\n';

	if (numgt!=0){
		os << "Nbr user weights: " << numgt;
		for (int i=0;i<numgt;i++){
			os << "user weight["<<i<<"]: " << usr_wgts[i]<< '\n';
		}
	}

}

template void NtupleInfo<MAX_NBR_PARTICLES>::Assign(TChain* fin,int version);
template void NtupleInfo<MAX_NBR_PARTICLES>::AssignNoAlpha(TChain* fin,int version);
template void NtupleInfo<MAX_NBR_PARTICLES>::print(std::ostream& os);

