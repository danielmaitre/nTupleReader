/*
 * weightFunctions.h
 *
 *  Created on: Jan 13, 2012
 *      Author: daniel
 */

#ifndef WEIGHTFUNCTIONS_H_
#define WEIGHTFUNCTIONS_H_

#include "NtupleInfo.h"
#include "pdf.h"

#ifdef LHAPDF_NEW_VERSION
#define ADDPDF(d1) d1,LHAPDF::PDF* pdf
#define ADDPDF2(d1,d2) d1,d2,LHAPDF::PDF* pdf
#define ADDPDF3(d1,d2,d3) d1,d2,d3,LHAPDF::PDF* pdf
#define ADDPDF4(d1,d2,d3,d4) d1,d2,d3,d4,LHAPDF::PDF* pdf

#define ADDPDFARG(d1) d1,pdf
#define ADDPDFARG2(d1,d2) d1,d2,pdf
#define ADDPDFARG3(d1,d2,d3) d1,d2,d3,pdf
#define ADDPDFARG4(d1,d2,d3,d4) d1,d2,d3,d4,pdf
#else
#define ADDPDF(d1) d1
#define ADDPDF2(d1,d2) d1,d2
#define ADDPDF3(d1,d2,d3) d1,d2,d3
#define ADDPDF4(d1,d2,d3,d4) d1,d2,d3,d4

#define ADDPDFARG(d1) d1
#define ADDPDFARG2(d1,d2) d1,d2
#define ADDPDFARG3(d1,d2,d3) d1,d2,d3
#define ADDPDFARG4(d1,d2,d3,d4) d1,d2,d3,d4
#endif

double computeWithLogCoefficients(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower));
double computeWithLogCoefficientsAF(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor));

double computeBornWeight(ADDPDF3(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor));
double computeBornWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower));

double computeLoopWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower));
double computeLoopWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor));

double computeVsubWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower));
double computeVsubWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor));

double computeRealWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower));
double computeRealWeight(ADDPDF3(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor));

double computeRealWeightWithProperStat(ADDPDF3(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor));
double computeRealWeightWithProperStat(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower));

#endif /* WEIGHTFUNCTIONS_H_ */
