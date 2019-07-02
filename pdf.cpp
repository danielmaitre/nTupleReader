#include "pdf.h"
#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <numeric>

using namespace std;

namespace Pdf {

int initialState::s_id1=1;
int initialState::s_id2=1;

initialState::channel initialState::s_channel=initialState::all;

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

void initialState::setChannel(initialState::channel chan){
	initialState::s_channel=chan;
};

initialState::channel initialState::reverse(initialState::channel ch){
	switch (ch){
	case initialState::gg:   return initialState::gg;
	case initialState::gq:   return initialState::qg;
	case initialState::gqb:  return initialState::qbg;
	case initialState::qg:   return initialState::gq;
	case initialState::qbg:  return initialState::gqb;
	case initialState::qq:   return initialState::qq;
	case initialState::qqb:  return initialState::qbq;
	case initialState::qbq:  return initialState::qqb;
	case initialState::qbqb: return initialState::qbqb;
	case all:                return initialState::all;
	}
};

// works for u,d,s,c,b,t,g
#ifdef LHAPDF_NEW_VERSION
double pdf(double x, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF){

	if (abs(PDGcode)<22 && PDGcode!=10){
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

double getPDFsingleChannel(const Pdf::pdfArray& X,int PDGcode,int initialState,initialState::channel ch){
	switch (PDGcode){
	case 21: {
		if ( not(   ch==initialState::gg
				 or ch==initialState::gq
			     or ch==initialState::gqb )
				){
			return 0.0;
		} else {
			return pdf(X,21,initialState);
		}
	} break;
	case -5: case -4: case -3 : case -2 : case -1: {
		if ( not(   ch==initialState::qbg
				 or ch==initialState::qbq
			     or ch==initialState::qbqb )
				){
			return 0.0;
		} else {
			return pdf(X,PDGcode,initialState);
		}
	} break;
	case 1: case 2: case 3: case 4: case 5: {
		if ( not(   ch==initialState::qg
				 or ch==initialState::qq
			     or ch==initialState::qqb )
				){
			return 0.0;
		} else {
			return pdf(X,PDGcode,initialState);
		}
	} break;
	case 10: {
		if ( not(   ch==initialState::qg
				 or ch==initialState::qq
			     or ch==initialState::qqb
			     or ch==initialState::qbg
			     or ch==initialState::qbq
			     or ch==initialState::qbqb
				 )
				){
			return 0.0;
		} else {
			if (  ch==initialState::qg
					 or ch==initialState::qq
				     or ch==initialState::qqb
				     ){
				// factor of 2 is to take into account that PDGcode=10 is normalised by 10 while 51 and 59 are normalised by 5
				return pdf(X,51,initialState)/2;
			} else {
				return pdf(X,59,initialState)/2;
			}
		}
	} break;
	case 51: {
		if (  ch==initialState::qg
				or ch==initialState::qq
				or ch==initialState::qqb
		){
				return pdf(X,51,initialState);
			} else  {
				return 0;
			}
	} break;
	case 59: {
		if (  ch==initialState::qbg
				or ch==initialState::qbq
				or ch==initialState::qbqb
		){
				return pdf(X,59,initialState);
			} else  {
				return 0;
			}
	} break;

	}
	cerr << "No PDF value for this PID " << PDGcode << "!" << std::endl;
	throw exception();
}

double pdfConvolution(const Pdf::pdfArray &X1,const Pdf::pdfArray &X2, int PDGcode1,int PDGcode2,int initialState1,int initialState2){
	int code = PDGcode1*1000+PDGcode2;

	switch (code) {
	case 52052: break;
	case 52058: break;
	case 58052: break;
	case 58058: break;

	case 101104: break;
	case 104101: break;
	case 102103: break;
	case 103102: break;

	case 101103: break;
	case 103101: break;
	case 102104: break;
	case 104102: break;

	case 101102: break;
	case 102101: break;
	case 104103: break;
	case 103104: break;

	default: {
		//std::cout << "pdfconvo (" << PDGcode1 <<","<< PDGcode2 << "): pdf1: " << pdf(X1,PDGcode1,initialState1)<< " pdf2: " <<  pdf(X2,PDGcode2,initialState2) << std::endl;
		if (initialState::s_channel==initialState::all){
			return pdf(X1,PDGcode1,initialState1)*pdf(X2,PDGcode2,initialState2);
		} else {

			double pdf1=getPDFsingleChannel(X1,PDGcode1,initialState1,initialState::s_channel);
			std::cout <<  PDGcode1 << ": " << pdf1 << std::endl;
			if (pdf1==0.0){return 0.0;}
			double pdf2=getPDFsingleChannel(X2,PDGcode2,initialState2,initialState::reverse(initialState::s_channel));
			std::cout <<  PDGcode1 <<","<< PDGcode2 << ": " << pdf1<< " " << pdf2 << std::endl;
			return pdf1*pdf2;

		}
	}
	}

	double res=0;
	switch (code) {
	case 52052: case 101101: case 101102: case 102101: case 102102:{
		if (!initialState::s_channel==initialState::qq) return 0.0;
	}; break;
	case 52058: case 101104: case 102103: case 101103: case 102104: {
		if (!initialState::s_channel==initialState::qqb) return 0.0;
	}; break;
	case 58052: case 104101: case 103102: case 104102: case 103101: {
		if (!initialState::s_channel==initialState::qbq) return 0.0;
	}; break;
	case 58058: case 103103: case 103104: case 104103: case 104104:{
		if (!initialState::s_channel==initialState::qbqb) return 0.0;
	}; break;

	}

	switch (code){
	case 52052: case 52058: case 58052: case 58058: {
	  for (int ii=0;ii<6;ii++){
	    switch (code) {
	    case 52052: res+=X1[ii+7]*X2[ii+7]; break;
	    case 52058: res+=X1[ii+7]*X2[5-ii]; break;
	    case 58052: res+=X1[5-ii]*X2[ii+7]; break;
	    case 58058: res+=X1[5-ii]*X2[5-ii]; break;
	    }
	  }
	  return res/(5.0);
	} break;
	case 101104: return (X1[8]*X2[4] + X1[10]*X2[2])/2.0;
	case 104101: return (X1[4]*X2[8] + X1[2]*X2[10])/2.0;
	case 102103: return (X1[7]*X2[5] + X1[9]*X2[3]+ X1[11]*X2[1])/3.0;
	case 103102: return (X1[5]*X2[7] + X1[3]*X2[9]+ X1[1]*X2[11])/3.0;	    
	
	case 101103: return (X1[8] + X1[10]) * (X2[1] + X2[3]+ X2[5])/6.0;
	case 103101: return (X2[8] + X2[10]) * (X1[1] + X1[3]+ X1[5])/6.0;
	case 102104: return (X1[7] + X1[9] + X1[11]) * (X2[2] + X2[4])/6.0;
	case 104102: return (X2[7] + X2[9] + X2[11]) * (X1[2] + X1[4])/6.0;

	case 101101: return (X1[8] + X1[10]) * (X2[8] + X2[10])/4.0;
	case 102102: return (X1[1] + X1[3]+ X1[5]) * (X2[1] + X2[3]+ X2[5])/9.0;
	case 103103: return (X1[7] + X1[9] + X1[11]) * (X1[7] + X1[9] + X1[11])/9.0;
	case 104104: return (X1[2] + X1[4]) * (X2[2] + X2[4])/4.0;

	case 101102: return (X1[8] + X1[10]) * (X2[7] + X2[9] + X2[11])/6.0;
	case 102101: return (X2[8] + X2[10]) * (X1[7] + X1[9] + X1[11])/6.0;
	case 103104: return (X1[1] + X1[3]+ X1[5]) * (X2[2] + X2[4])/6.0;
	case 104103: return (X1[2] + X1[4]) * (X2[1] + X2[3]+ X2[5]) /6.0;


	} 
	
}


double pdfConvolution(double x1,double x2, double Q, int PDGcode1,int PDGcode2,int initialState1,int initialState2,LHAPDF::PDF* PDF){
	Pdf::pdfArray X1,X2;
	LHAComputePdf(x1,Q,X1,PDF);
	LHAComputePdf(x2,Q,X2,PDF);
	return pdfConvolution(X1,X2, PDGcode1, PDGcode2, initialState1, initialState2);

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
	if ( abs(PDGcode) < 100) {
		LHAPDF::xfx(x,Q,results);
	//	if (Q>1e4){ throw;}
		if ( initialState == 1 ){
			return results[6+(PDGcode%21)]/x;
		} else {
			return results[6-(PDGcode%21)]/x;
		}
	else {
		switch (PDGcode){
			case 101: return (results[8] + results[10])/2.0; 
			case 102: return (results[7] + results[9] + results[11])/3.0; 
			case 103: return (results[1] + results[3] + results[5])/3.0; 
			case 104: return (results[2] + results[4])/2.0; 
			default: {
				cerr << "No pdf for PDG code " << PDGcode << std::endl;
				throw;
			}
		} 		
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
	if (abs(PDGcode)<22 && PDGcode!=10){
		if (initialState == 1){
			return X[6+(PDGcode%21)];
		} else {
			return X[6-(PDGcode%21)];
		}
	} else {
		switch(PDGcode){
			case  51: return std::accumulate(&X.tbar+7,&X.t+1,0.0)/(5.0);
			case  59: return std::accumulate(&X.tbar,&X.tbar+6,0.0)/(5.0);
			case  10: return (std::accumulate(&X.tbar,&X.t+1,0.0)-X.gluon)/(10.0);
			case 101: return (X.u + X.c)/2.0; 
			case 102: return (X.d + X.s + X.b)/3.0; 
			case 103: return (X.dbar + X.sbar + X.bbar)/3.0; 
			case 104: return (X.ubar + X.cbar)/2.0; 
			default: {
				cerr << "No pdf for PDG code " << PDGcode << std::endl;
				throw;
			}

		}

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
