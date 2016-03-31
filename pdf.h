#ifndef _H_PDF_H
#define _H_PDF_H

#include "LHAPDF/LHAPDF.h"

#ifdef LHAPDF_MAJOR_VERSION
#if LHAPDF_MAJOR_VERSION == 6
#define LHAPDF_NEW_VERSION
#endif
#endif

#include <vector>

namespace Pdf {

struct initialState {
  static int s_id1;
  static int s_id2;
  static void setInitialState(int id1,int id2);
} ;



enum pdgCodes { GLUON=21  };

struct pdfArray {
		double tbar;
		double bbar;
		double cbar;
		double sbar;
		double ubar;
		double dbar;
		double gluon;
		double d;
		double u;
		double s;
		double c;
		double b;
		double t;
		double x;
		double operator[](int n) const {return *(&tbar+n);};
};

double pdf(const pdfArray& X, int PDGcode,int initialState);


// works for u,d,s,c,b,t,g
#ifdef LHAPDF_NEW_VERSION
double pdf(double x, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF);
double pdf_vsub1(double x, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF);
double pdf_vsub2(double x, double xp, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF);
double pdf_vsub3(double x, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF);
double pdf_vsub4(double x, double xp, double Q, int PDGcode,int initialState,LHAPDF::PDF* PDF);
void LHAComputePdf(double x,double Q,pdfArray& pa,LHAPDF::PDF* PDF);
#endif
double pdf(double x, double Q, int PDGcode,int initialState);
double pdf_vsub1(double x, double Q, int PDGcode,int initialState);
double pdf_vsub2(double x, double xp, double Q, int PDGcode,int initialState);
double pdf_vsub3(double x, double Q, int PDGcode,int initialState);
double pdf_vsub4(double x, double xp, double Q, int PDGcode,int initialState);
void LHAComputePdf(double x,double Q,pdfArray& pa);



double pdf_vsub1(const pdfArray& X, int PDGcode,int initialState);
double pdf_vsub2(const pdfArray& XoverXp, double x, int PDGcode,int initialState);
double pdf_vsub3(const pdfArray& X, int PDGcode,int initialState);
double pdf_vsub4(const pdfArray& XoverXp, double xp, int PDGcode,int initialState);

#ifdef LHAPDF_NEW_VERSION
class current {
public:
	static LHAPDF::PDF* s_PDF;
	static LHAPDF::PDFSet* s_PDFSet;
	static void init();
	static std::vector<LHAPDF::PDF*> s_PDFs;
};

#endif

} /* Pdf */

#endif /* _H_PDF_H */
