#include "pdf.h"
#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <numeric>

using namespace std;

namespace Pdf {

int initialState::s_id1=1;
int initialState::s_id2=1;

#ifdef LHAPDF_NEW_VERSION

LHAPDF::PDF* current::s_PDF=0;
LHAPDF::PDFSet* current::s_PDFSet=0;
  std::vector<LHAPDF::PDF*> current::s_PDFs;
void current::init(){
  static bool done=false;
  if (!done) {
    s_PDFs=s_PDFSet->mkPDFs();    
    done=true;
  }

}

#endif

void initialState::setInitialState(int id1,int id2){
	initialState::s_id1=id1;
	initialState::s_id2=id2;
};


// works for u,d,s,c,b,t,g
#ifdef LHAPDF_NEW_VERSION
double pdf(double x, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF){

	if (abs(PDGcode)<22){
	int id;
	if ( initialState == 1 ){
		id=(PDGcode%21);
	} else {
		id=-(PDGcode%21);
	}
	return PDF->xfxQ(id,x,Q)/x;
	} else {
		// these are special combinations for NNLO ntuples
		vector<double> xfs; xfs.resize(13);
		PDF->xfxQ(x, Q, xfs);
		switch(PDGcode){
			case  51: return std::accumulate(xfs.begin()+7,xfs.end(),0.0)/(5.0*x);
			case  59: return std::accumulate(xfs.begin(),xfs.begin()+6,0.0)/(5.0*x);
			case  10: return (std::accumulate(xfs.begin(),xfs.end(),0.0)-xfs[6])/(10.0*x);
		}

	}
}


double pdfConvolution(double x1,double x2, double Q, int PDGcode1,int PDGcode2,int initialState1,int initialState2,LHAPDF::PDF* PDF){
	int code=PDGcode1*100+PDGcode2;

	switch (code) {
	case 5252: break;
	case 5258: break;
	case 5852: break;
	case 5858: break;
	default: return pdf(x1,Q,PDGcode1,initialState1,PDF)*pdf(x2,Q,PDGcode2,initialState2,PDF);
	}
	vector<double> xfs1,xfs2;
	xfs1.resize(13);
	xfs2.resize(13);
	PDF->xfxQ(x1, Q, xfs1);
	PDF->xfxQ(x2, Q, xfs2);
	double res=0;
	for (int ii=0;ii<6;ii++){
		switch (code) {
		case 5252: res+=xfs1[ii+7]*xfs2[ii+7]; break;
		case 5258: res+=xfs1[ii+7]*xfs2[5-ii]; break;
		case 5852: res+=xfs1[5-ii]*xfs2[ii+7]; break;
		case 5858: res+=xfs1[5-ii]*xfs2[5-ii]; break;
		}
	}
	return res/(5.0*x1*x2);

}


double pdf(double x, double Q, int PDGcode,int initialState){
	return pdf(x,Q,PDGcode,initialState,current::s_PDF);
}

double pdf_vsub1(double x, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF){
if (PDGcode == GLUON){
	vector<double> results(13);
	PDF->xfxQ(x,Q,results);
	return (results[0]+results[1]+results[2]+results[3]+results[4]+results[5]+results[7]+results[8]+results[9]+results[10]+results[11]+results[12])/x;
} else {
	return pdf(x,Q,PDGcode,initialState,PDF);
}
}
double pdf_vsub1(double x, double Q, int PDGcode,int initialState){
	return pdf_vsub1(x,Q,PDGcode,initialState,current::s_PDF);
}

double pdf_vsub2(double x,double xp, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF){
  //only divide by x because xfx returns pdf(y)*y and here y=x/xp
	if (PDGcode == GLUON){
		vector<double> results(13);
		PDF->xfxQ(x/xp,Q,results);
		double allQuarks=results[0]+results[1]+results[2]+results[3]+results[4]+results[5]+results[7]+results[8]+results[9]+results[10]+results[11]+results[12];
		return (allQuarks)/x;
	} else {

		if ( initialState == 1 ){
			return PDF->xfxQ(PDGcode,x/xp,Q)/x;
		} else {
			return PDF->xfxQ(-PDGcode,x/xp,Q)/x;
		}
	}
}

double pdf_vsub2(double x,double xp, double Q, int PDGcode,int initialState){
	return pdf_vsub2(x,xp,Q,PDGcode,initialState,current::s_PDF);
}

double pdf_vsub3(double x, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF){
	return PDF->xfxQ(GLUON,x,Q)/x;
}

double pdf_vsub3(double x, double Q, int PDGcode,int initialState){
	return pdf_vsub3(x,Q,PDGcode,initialState,current::s_PDF);
}
double pdf_vsub4(double x,double xp, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF){
  //only divide by x because xfx returns pdf(y)*y and here y=x/xp
	return PDF->xfxQ(GLUON,x/xp,Q)/x;
}
double pdf_vsub4(double x,double xp, double Q, int PDGcode,int initialState){
	return pdf_vsub4(x,xp,Q,PDGcode,initialState,current::s_PDF);
}

void LHAComputePdf(double x,double Q,pdfArray& pa,LHAPDF::PDF* PDF){
	static vector<double> ret(13);
	PDF->xfxQ(x,Q,ret);
	std::copy(ret.begin(),ret.end(),&pa.tbar);
	pa.x=x;
	pa.u/=x;
	pa.ubar/=x;
	pa.d/=x;
	pa.dbar/=x;
	pa.s/=x;
	pa.sbar/=x;
	pa.c/=x;
	pa.cbar/=x;
	pa.b/=x;
	pa.bbar/=x;
	pa.t/=x;
	pa.tbar/=x;
	pa.gluon/=x;
}

void LHAComputePdf(double x,double Q,pdfArray& pa){
	LHAComputePdf(x,Q,pa,current::s_PDF);
}
#else

double pdf(double x, double Q, int PDGcode,int initialState){
		double results[13];
		LHAPDF::xfx(x,Q,results);
	//	if (Q>1e4){ throw;}
		if ( initialState == 1 ){
			return results[6+(PDGcode%21)]/x;
		} else {
			return results[6-(PDGcode%21)]/x;
		}
}

double pdf_vsub1(double x, double Q, int PDGcode,int initialState){
	double results[13];
	LHAPDF::xfx(x,Q,results);
/*	cout << " PDF: " << results[0] << " ";
	cout << results[1] << " ";
	cout << results[2] << " ";
	cout << results[3] << " ";
	cout << results[4] << " ";
	cout << results[5] << " ";
	cout << results[6] << " ";
	cout << results[7] << " ";
	cout << results[8] << " ";
	cout << results[9] << " ";
	cout << results[10] << " ";
	cout << results[11] << " ";
	cout << results[12] << " " <<endl;*/
if (PDGcode == GLUON){
		return (results[0]+results[1]+results[2]+results[3]+results[4]+results[5]+results[7]+results[8]+results[9]+results[10]+results[11]+results[12])/x;
	} else {
		if ( initialState == 1 ){
			return results[6+(PDGcode%21)]/x;
		} else {
			return results[6-(PDGcode%21)]/x;
		}
	}

}

double pdf_vsub2(double x,double xp, double Q, int PDGcode,int initialState){
  //only divide by x because xfx returns pdf(y)*y and here y=x/xp
	double results[13];
	LHAPDF::xfx(x/xp,Q,results);
	if (PDGcode == GLUON){
		double allQuarks=results[0]+results[1]+results[2]+results[3]+results[4]+results[5]+results[7]+results[8]+results[9]+results[10]+results[11]+results[12];
		return (allQuarks)/x;
	} else {
		if ( initialState == 1 ){
			return results[6+(PDGcode%21)]/x;
		} else {
			return results[6-(PDGcode%21)]/x;
		}
	}
}

double pdf_vsub3(double x, double Q, int PDGcode,int initialState){
	return pdf(x,Q,GLUON,initialState);
}

double pdf_vsub4(double x,double xp, double Q, int PDGcode,int initialState){
  //only divide by x because xfx returns pdf(y)*y and here y=x/xp
	return pdf(x/xp,Q,GLUON,initialState)/x;
}

void LHAComputePdf(double x,double Q,pdfArray& pa){
	LHAPDF::xfx(x,Q,&pa.tbar);
	pa.x=x;
	pa.u/=x;
	pa.ubar/=x;
	pa.d/=x;
	pa.dbar/=x;
	pa.s/=x;
	pa.sbar/=x;
	pa.c/=x;
	pa.cbar/=x;
	pa.b/=x;
	pa.bbar/=x;
	pa.t/=x;
	pa.tbar/=x;
	pa.gluon/=x;
}

#endif




double pdf(const pdfArray& X, int PDGcode,int initialState){
	if (initialState == 1){
		return X[6+(PDGcode%21)];
	} else {
		return X[6-(PDGcode%21)];
	}
};


double pdf_vsub1(const pdfArray& X, int PDGcode,int initialState){
if (PDGcode == GLUON){
		return (X.t+X.b+X.c+X.s+X.d+X.u+X.ubar+X.dbar+X.sbar+X.cbar+X.bbar+X.tbar);
	} else {
		if (initialState == 1){
			return X[6+(PDGcode%21)];
		}
		else {
			return X[6-(PDGcode%21)];
		}
	}
}

double pdf_vsub2(const pdfArray& XoverXp, double xp, int PDGcode,int initialState){
	if (PDGcode == GLUON){
		double allQuarks=(XoverXp.t+XoverXp.b+XoverXp.c+XoverXp.s+XoverXp.d+XoverXp.u+XoverXp.ubar+XoverXp.dbar+XoverXp.sbar+XoverXp.cbar+XoverXp.bbar+XoverXp.tbar);
		return allQuarks/xp;
	} else {
		if (initialState == 1){
			return XoverXp[6+(PDGcode%21)]/xp;
		}
		else {
			return XoverXp[6-(PDGcode%21)]/xp;
		}
	}
}

double pdf_vsub3(const pdfArray& X, int PDGcode,int initialState){
	return X.gluon;
}

double pdf_vsub4(const pdfArray& XoverXp, double xp, int PDGcode,int initialState){
	return XoverXp.gluon/xp;
}



} /* Pdf */
