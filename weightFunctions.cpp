/*
 * weightFunctions.cpp
 *
 *  Created on: Jan 13, 2012
 *      Author: daniel
 */

#include "weightFunctions.h"
#include "pdf.h"
#include <cmath>
#include "LHAPDF/LHAPDF.h"
#include "pdf.h"



#ifdef LHAPDF_NEW_VERSION
double computeAlphaFactor(ADDPDF3(NtupleInfo<MAX_NBR_PARTICLES>& NI,double newScale,int alphasPower)) {
	double oldAlpha=NI.alphas;
	double newAlpha=pdf->alphasQ(newScale);
	double alphasFactor = std::pow(newAlpha/oldAlpha,alphasPower);
	//std::cout << "old alpha: " << oldAlpha << " new: " << newAlpha << " power: " << alphasPower << std::endl;
	return alphasFactor;
}



double computeBornWeight(ADDPDF3(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor)){
	//double newPdfWgtold = Pdf::pdf(ADDPDFARG4(Ev.x1,newFacScale,Ev.id1,Pdf::initialState::s_id1))*Pdf::pdf(ADDPDFARG4(Ev.x2,newFacScale,Ev.id2,Pdf::initialState::s_id2)) ;
	double newPdfWgt = Pdf::pdfConvolution(ADDPDFARG7(Ev.x1,Ev.x2,newFacScale,Ev.id1,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2)) ;
	//std::cout << "old: " << newPdfWgtold << " new: " << newPdfWgt << std::endl;
	return Ev.me_wgt*alphaFactor*newPdfWgt;
};


double computeBornWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower)){
	double alphaFactor=computeAlphaFactor(ADDPDFARG3(Ev,newRenScale,alphasPower));
	double newPdfWgt = Pdf::pdfConvolution(ADDPDFARG7(Ev.x1,Ev.x2,newFacScale,Ev.id1,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2)) ;
	return Ev.me_wgt*alphaFactor*newPdfWgt;
}


double computeLoopWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower)){
	double alphaFactor=computeAlphaFactor(ADDPDFARG3(Ev,newRenScale,alphasPower));
	return computeLoopWeight(ADDPDFARG4(Ev,newFacScale,newRenScale,alphaFactor));
}

double computeLoopWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor)){
	double newPdfWgt = Pdf::pdfConvolution(ADDPDFARG7(Ev.x1,Ev.x2,newFacScale,Ev.id1,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2)) ;

	double L=2.0*log(newRenScale/Ev.muR);
	double res2=
		newPdfWgt*(
			Ev.me_wgt
			+Ev.usr_wgts[0]*L
			+0.5*Ev.usr_wgts[1]*L*L
		);
	return res2*alphaFactor;
}


double computeVsubWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower)){
	double alphaFactor=computeAlphaFactor(ADDPDFARG3(Ev,newRenScale,alphasPower));
	return computeVsubWeight(ADDPDFARG4(Ev,newFacScale,newRenScale,alphaFactor));
}

double computeVsubWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor)){
	double r1=Ev.x1/Ev.x1p;
	double r2=Ev.x2/Ev.x2p;
	if (r1>1 and r1<1.0000001){
		r1=0.99999999999999;
	}
	if (r2>1 and r2<1.0000001){
		r2=0.99999999999999;
	}
	Pdf::pdfArray X1,X2,X1overX1p,X2overX2p;
	Pdf::LHAComputePdf(ADDPDFARG3(Ev.x1,newFacScale,X1));
	Pdf::LHAComputePdf(ADDPDFARG3(Ev.x2,newFacScale,X2));
	Pdf::LHAComputePdf(ADDPDFARG3(r1,newFacScale,X1overX1p));
	Pdf::LHAComputePdf(ADDPDFARG3(r2,newFacScale,X2overX2p));

	double L=2.0*log(newRenScale/Ev.muR);
	double LF=2.0*log(newFacScale/Ev.muF);

/*	double res2=
		Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1)*Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2) *
		(
			Ev.me_wgt
			+Ev.usr_wgts[0]*L
			+0.5*Ev.usr_wgts[1]*L*L
		)
	 +
		(
		pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[2]+Ev.usr_wgts[10]*LF)
		+pdf_vsub2(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[3]+Ev.usr_wgts[11]*LF)
		+pdf_vsub3(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[4]+Ev.usr_wgts[12]*LF)
		+pdf_vsub4(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[5]+Ev.usr_wgts[13]*LF)
		)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2) +
		(
		pdf_vsub1(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[6]+Ev.usr_wgts[14]*LF)
		+pdf_vsub2(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[7]+Ev.usr_wgts[15]*LF)
		+pdf_vsub3(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[8]+Ev.usr_wgts[16]*LF)
		+pdf_vsub4(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[9]+Ev.usr_wgts[17]*LF)
		)* Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1) ;
*/
		double pdfconvo11=(Ev.id1==21?10.0:1.0)*Pdf::pdfConvolution(
				X1,X2,(Ev.id1==21?10:Ev.id1),Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2);

		double pdfconvo12=(Ev.id1==21?10.0:1.0)*Pdf::pdfConvolution(
				X1overX1p,X2,(Ev.id1==21?10:Ev.id1),Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2)/Ev.x1p;

		double pdfconvo13=Pdf::pdfConvolution(
				X1,X2,21,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2);

		double pdfconvo14=Pdf::pdfConvolution(
				X1overX1p,X2,21,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2)/Ev.x1p;

		double pdfconvo21=(Ev.id2==21?10.0:1.0)*Pdf::pdfConvolution(
				X1,X2,Ev.id1,(Ev.id2==21?10:Ev.id2),Pdf::initialState::s_id1,Pdf::initialState::s_id2);

		double pdfconvo22=(Ev.id2==21?10.0:1.0)*Pdf::pdfConvolution(
				X1,X2overX2p,Ev.id1,(Ev.id2==21?10:Ev.id2),Pdf::initialState::s_id1,Pdf::initialState::s_id2)/Ev.x2p;

		double pdfconvo23=Pdf::pdfConvolution(
				X1,X2,Ev.id1,21,Pdf::initialState::s_id1,Pdf::initialState::s_id2);

		double pdfconvo24=Pdf::pdfConvolution(
				X1,X2overX2p,Ev.id1,21,Pdf::initialState::s_id1,Pdf::initialState::s_id2)/Ev.x2p;


		double resConvo=
				Pdf::pdfConvolution(X1,X2,Ev.id1,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2) *
				(
					Ev.me_wgt
					+Ev.usr_wgts[0]*L
					+0.5*Ev.usr_wgts[1]*L*L
				)
			 +
				(
				pdfconvo11*(Ev.usr_wgts[2]+Ev.usr_wgts[10]*LF)
				+pdfconvo12*(Ev.usr_wgts[3]+Ev.usr_wgts[11]*LF)
				+pdfconvo13*(Ev.usr_wgts[4]+Ev.usr_wgts[12]*LF)
				+pdfconvo14*(Ev.usr_wgts[5]+Ev.usr_wgts[13]*LF)
				) +
				(
				pdfconvo21*(Ev.usr_wgts[6]+Ev.usr_wgts[14]*LF)
				+pdfconvo22*(Ev.usr_wgts[7]+Ev.usr_wgts[15]*LF)
				+pdfconvo23*(Ev.usr_wgts[8]+Ev.usr_wgts[16]*LF)
				+pdfconvo24*(Ev.usr_wgts[9]+Ev.usr_wgts[17]*LF)
				);

/*
 		double vsub11=pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2);
		double vsub12=pdf_vsub2(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2);
		double vsub13=pdf_vsub3(X1,Ev.id1,Pdf::initialState::s_id1)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2);
		double vsub14=pdf_vsub4(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2);

 		std:: cout << "sub11:" << (pdfconvo11-vsub11)/(pdfconvo11+vsub11)<< std::endl;
		std:: cout << "sub12:" << (pdfconvo12-vsub12)/(pdfconvo12+vsub12)<< std::endl;
		std:: cout << "sub13:" << (pdfconvo13-vsub13)/(pdfconvo13+vsub13)<< std::endl;
		std:: cout << "sub14:" << (pdfconvo14-vsub14)/(pdfconvo14+vsub14)<< std::endl;

		std:: cout << "ratio:" << (res2-resConvo)/(res2+resConvo)<< std::endl;

		std::cout <<
				"vsub1:"
		<< pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2)
		<<" = " << pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1) << "*" << Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2)
		<< " convo: "
		<< pdfconvo11
		 << std::endl;
		 */
	/*
		std::cout << "contributions to the new weight:" << std::endl;
		std::cout << Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1)*Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2) *
			(
				Ev.me_wgt
				+Ev.usr_wgts[0]*L
				+0.5*Ev.usr_wgts[1]*L*L
			 ) << " pdf: " << Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1)*Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2) << std::endl;
	std::cout << pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[2]+Ev.usr_wgts[10]*LF)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< " pdf:"
			 << pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2)<<std::endl;
	std::cout <<+pdf_vsub2(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[3]+Ev.usr_wgts[11]*LF)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< std::endl;
	std::cout <<+pdf_vsub3(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[4]+Ev.usr_wgts[12]*LF)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< std::endl;
	std::cout <<+pdf_vsub4(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[5]+Ev.usr_wgts[13]*LF)* Pdf::pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< std::endl;
	std::cout <<pdf_vsub1(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[6]+Ev.usr_wgts[14]*LF)* Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1) << std::endl;
	std::cout <<+pdf_vsub2(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[7]+Ev.usr_wgts[15]*LF)* Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1)<< std::endl ;
	std::cout <<+pdf_vsub3(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[8]+Ev.usr_wgts[16]*LF)* Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1) << std::endl;
	std::cout <<+pdf_vsub4(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[9]+Ev.usr_wgts[17]*LF)* Pdf::pdf(X1,Ev.id1,Pdf::initialState::s_id1)<< std::endl ;

	
	std::cout << "alphaFactor: " << alphaFactor << std::endl ; 
	*/



	return resConvo*alphaFactor;
}

double computeRealWeight(ADDPDF3(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor)){
	double newPdfWgt = Pdf::pdfConvolution(ADDPDFARG7(Ev.x1,Ev.x2,newFacScale,Ev.id1,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2)) ;
	return Ev.me_wgt*alphaFactor*newPdfWgt;
}

double computeRealWeight(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower)){
	double alphaFactor=computeAlphaFactor(ADDPDFARG3(Ev,newRenScale,alphasPower));
	return computeRealWeight(ADDPDFARG3(Ev,newFacScale,alphaFactor));
}

double computeRealWeightWithProperStat(ADDPDF3(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor)){
	double newPdfWgt = Pdf::pdfConvolution(ADDPDFARG7(Ev.x1,Ev.x2,newFacScale,Ev.id1,Ev.id2,Pdf::initialState::s_id1,Pdf::initialState::s_id2)) ;
	return Ev.me_wgt2*alphaFactor*newPdfWgt;
}

double computeRealWeightWithProperStat(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower)){
	double alphaFactor=computeAlphaFactor(ADDPDFARG3(Ev,newRenScale,alphasPower));
	return computeRealWeightWithProperStat(ADDPDFARG3(Ev,newFacScale,alphaFactor));
}


double computeWithLogCoefficientsAF(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor)){
	double L=log(newRenScale);
	double Ltothen=1.0;
	double total=0.0;
	for (int mui=0;mui<Ev.nwgts;mui++){
		double newPdfWgt = Pdf::pdfConvolution(ADDPDFARG7(Ev.x1s[mui],Ev.x2s[mui],newFacScale,Ev.id1s[mui],Ev.id2s[mui],Pdf::initialState::s_id1,Pdf::initialState::s_id2)) ;
		total+=Ev.wgts[mui]*Ltothen*newPdfWgt;
		Ltothen*=L;
	}
	return total*alphaFactor;
}

double computeWithLogCoefficients(ADDPDF4(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower)){
	double alphaFactor=computeAlphaFactor(ADDPDFARG3(Ev,newRenScale,alphasPower));
	double newAlpha=pdf->alphasQ(newRenScale);
	double alphasFactor = std::pow(newAlpha,alphasPower);
	return alphasFactor*computeWithLogCoefficientsAF(ADDPDFARG4(Ev,newFacScale,newRenScale,alphaFactor));
}


#else

double computeAlphaFactor(NtupleInfo<MAX_NBR_PARTICLES>& NI,double newScale,int alphasPower) {
	double oldAlpha=NI.alphas;
	double newAlpha=LHAPDF::alphasPDF(newScale);
	double alphasFactor = std::pow(newAlpha/oldAlpha,alphasPower);
	//std::cout << "old alpha: " << oldAlpha << " new: " << newAlpha << " power: " << alphasPower << std::endl;
	return alphasFactor;
}

double computeBornWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor){
	double newPdfWgt = Pdf::pdf(Ev.x1,newFacScale,Ev.id1,Pdf::initialState::s_id1)*Pdf::pdf(Ev.x2,newFacScale,Ev.id2,Pdf::initialState::s_id2) ;
	return Ev.me_wgt*alphaFactor*newPdfWgt;
};


double computeBornWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower){
	double alphaFactor=computeAlphaFactor(Ev,newRenScale,alphasPower);
	double newPdfWgt = Pdf::pdf(Ev.x1,newFacScale,Ev.id1,Pdf::initialState::s_id1)*Pdf::pdf(Ev.x2,newFacScale,Ev.id2,Pdf::initialState::s_id2) ;
	return Ev.me_wgt*alphaFactor*newPdfWgt;
}


double computeLoopWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower){
	double alphaFactor=computeAlphaFactor(Ev,newRenScale,alphasPower);
	return computeLoopWeight(Ev,newFacScale,newRenScale,alphaFactor);
}

double computeLoopWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor){
	Pdf::pdfArray X1,X2,X1overX1p,X2overX2p;
	Pdf::LHAComputePdf(Ev.x1,newFacScale,X1);
	Pdf::LHAComputePdf(Ev.x2,newFacScale,X2);

	double L=2.0*log(newRenScale/Ev.muR);
	double res2=
		pdf(X1,Ev.id1,Pdf::initialState::s_id1)*pdf(X2,Ev.id2,Pdf::initialState::s_id2) *
		(
			Ev.me_wgt
			+Ev.usr_wgts[0]*L
			+0.5*Ev.usr_wgts[1]*L*L
		);
	return res2*alphaFactor;
}


double computeVsubWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower){
	double alphaFactor=computeAlphaFactor(Ev,newRenScale,alphasPower);
	return computeVsubWeight(Ev,newFacScale,newRenScale,alphaFactor);
}

double computeVsubWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,double alphaFactor){
	double r1=Ev.x1/Ev.x1p;
	double r2=Ev.x2/Ev.x2p;
	if (r1>1 and r1<1.0000001){
		r1=0.99999999999999;
	}
	if (r2>1 and r2<1.0000001){
		r2=0.99999999999999;
	}
	Pdf::pdfArray X1,X2,X1overX1p,X2overX2p;
	Pdf::LHAComputePdf(Ev.x1,newFacScale,X1);
	Pdf::LHAComputePdf(Ev.x2,newFacScale,X2);
	Pdf::LHAComputePdf(r1,newFacScale,X1overX1p);
	Pdf::LHAComputePdf(r2,newFacScale,X2overX2p);

	double L=2.0*log(newRenScale/Ev.muR);
	double LF=2.0*log(newFacScale/Ev.muF);

	double res2=
		pdf(X1,Ev.id1,Pdf::initialState::s_id1)*pdf(X2,Ev.id2,Pdf::initialState::s_id2) *
		(
			Ev.me_wgt
			+Ev.usr_wgts[0]*L
			+0.5*Ev.usr_wgts[1]*L*L
		)
	 +
		(
		pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[2]+Ev.usr_wgts[10]*LF)
		+pdf_vsub2(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[3]+Ev.usr_wgts[11]*LF)
		+pdf_vsub3(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[4]+Ev.usr_wgts[12]*LF)
		+pdf_vsub4(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[5]+Ev.usr_wgts[13]*LF)
		)* pdf(X2,Ev.id2,Pdf::initialState::s_id2) +
		(
		pdf_vsub1(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[6]+Ev.usr_wgts[14]*LF)
		+pdf_vsub2(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[7]+Ev.usr_wgts[15]*LF)
		+pdf_vsub3(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[8]+Ev.usr_wgts[16]*LF)
		+pdf_vsub4(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[9]+Ev.usr_wgts[17]*LF)
		)* pdf(X1,Ev.id1,Pdf::initialState::s_id1) ;

	/*
	std::cout << "contributions to the new weight:" << std::endl;
	std::cout << pdf(X1,Ev.id1,Pdf::initialState::s_id1)*pdf(X2,Ev.id2,Pdf::initialState::s_id2) *
			(
				Ev.me_wgt
				+Ev.usr_wgts[0]*L
				+0.5*Ev.usr_wgts[1]*L*L
			) << " pdf: " <<  pdf(X1,Ev.id1,Pdf::initialState::s_id1)*pdf(X2,Ev.id2,Pdf::initialState::s_id2) << std::endl;
	std::cout << pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[2]+Ev.usr_wgts[10]*LF)* pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< " pdf:"
			 << pdf_vsub1(X1,Ev.id1,Pdf::initialState::s_id1)* pdf(X2,Ev.id2,Pdf::initialState::s_id2)<<std::endl;
	std::cout <<+pdf_vsub2(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[3]+Ev.usr_wgts[11]*LF)* pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< std::endl;
	std::cout <<+pdf_vsub3(X1,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[4]+Ev.usr_wgts[12]*LF)* pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< std::endl;
	std::cout <<+pdf_vsub4(X1overX1p,Ev.x1p,Ev.id1,Pdf::initialState::s_id1)*(Ev.usr_wgts[5]+Ev.usr_wgts[13]*LF)* pdf(X2,Ev.id2,Pdf::initialState::s_id2)<< std::endl;
	std::cout <<pdf_vsub1(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[6]+Ev.usr_wgts[14]*LF)* pdf(X1,Ev.id1,Pdf::initialState::s_id1) << std::endl;
	std::cout <<+pdf_vsub2(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[7]+Ev.usr_wgts[15]*LF)* pdf(X1,Ev.id1,Pdf::initialState::s_id1)<< std::endl ;
	std::cout <<+pdf_vsub3(X2,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[8]+Ev.usr_wgts[16]*LF)* pdf(X1,Ev.id1,Pdf::initialState::s_id1) << std::endl;
	std::cout <<+pdf_vsub4(X2overX2p,Ev.x2p,Ev.id2,Pdf::initialState::s_id2)*(Ev.usr_wgts[9]+Ev.usr_wgts[17]*LF)* pdf(X1,Ev.id1,Pdf::initialState::s_id1)<< std::endl ;

	std::cout << "alphaFactor: " << alphaFactor << std::endl;
*/

	return res2*alphaFactor;
}

double computeRealWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor){
	double newPdfWgt = Pdf::pdf(Ev.x1,newFacScale,Ev.id1,Pdf::initialState::s_id1)*Pdf::pdf(Ev.x2,newFacScale,Ev.id2,Pdf::initialState::s_id2) ;
	return Ev.me_wgt*alphaFactor*newPdfWgt;
}

double computeRealWeight(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower){
	double alphaFactor=computeAlphaFactor(Ev,newRenScale,alphasPower);
	return computeRealWeight(Ev,newFacScale,alphaFactor);
}

double computeRealWeightWithProperStat(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double alphaFactor){
	double newPdfWgt = Pdf::pdf(Ev.x1,newFacScale,Ev.id1,Pdf::initialState::s_id1)*Pdf::pdf(Ev.x2,newFacScale,Ev.id2,Pdf::initialState::s_id2) ;
	return Ev.me_wgt2*alphaFactor*newPdfWgt;
}

double computeRealWeightWithProperStat(NtupleInfo<MAX_NBR_PARTICLES>& Ev,double newFacScale,double newRenScale,int alphasPower){
	double alphaFactor=computeAlphaFactor(Ev,newRenScale,alphasPower);
	return computeRealWeightWithProperStat(Ev,newFacScale,alphaFactor);
}
#endif
