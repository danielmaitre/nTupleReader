/*
 * NtupleInfo.h
 *
 *  Created on: Jan 12, 2012
 *      Author: daniel
 */

#ifndef NTUPLEINFO_H_
#define NTUPLEINFO_H_

#include "TChain.h"

#define MAX_NBR_PARTICLES 100


template <int N> struct NtupleInfo {
	double wgt;
	double wgt2;
	double muF;
	double muR;
	double muF2;
	double muR2;
	Int_t id;
	float px[N];
	float py[N];
	float pz[N];
	float E[N];
	double pxD[N];
	double pyD[N];
	double pzD[N];
	double ED[N];
	int kf[N];
	int nparticle;
	int id1,id2;
	double x1,x2;
	double x1p,x2p;
	double me_wgt,me_wgt2;
	int numgt;
	double usr_wgts[18];
  double alphas;
  bool hasDoublePrecisionMomenta;
  int ncount;
  enum { maxNbrParticles=N };
  virtual void Assign(TChain* t,bool doublePrecision=false,bool withNcount=false);
  virtual void AssignNoAlpha(TChain* t,bool doublePrecision=false,bool withNcount=false);
	void print(std::ostream& os);
};

#ifdef SWIG
%template(NtupleInfo100) NtupleInfo<100>;
#endif


#endif /* NTUPLEINFO_H_ */
