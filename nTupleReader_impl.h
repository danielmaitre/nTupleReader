/*
 * nTupleReader_impl.h
 *
 *  Created on: 8 Jun 2015
 *      Author: daniel
 */

/*
 * nTupleReader.h
 *
 *  Created on: Feb 13, 2012
 *      Author: daniel
 */

#ifndef NTUPLEREADER_IMPL_H_
#define NTUPLEREADER_IMPL_H_

#include "EventReaderBase.h"
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include "pdf.h"
#ifdef USE_HEPMC
#include "HepMC/GenEvent.h"
#include "HepMC_utils.h"
#endif
#include "nTupleReader.h"   //for colliderType

class nTR_OutOfBound: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Maximum particle number exceeded.";
  }
} ;


#if SWIG
%exception nTupleReader_impl::getE {
   try {
      $action
   } catch (nTR_OutOfBound &e) {
      PyErr_SetString(PyExc_SystemError, "nTupleReader error: out of bounds.");
      return NULL;
   }
}
%exception nTupleReader_impl::getEnergy {
   try {
      $action
   } catch (nTR_OutOfBound &e) {
      PyErr_SetString(PyExc_SystemError, "nTupleReader error: out of bounds.");
      return NULL;
   }
}
%exception nTupleReader_impl::getX {
   try {
      $action
   } catch (nTR_OutOfBound &e) {
      PyErr_SetString(PyExc_SystemError, "nTupleReader error: out of bounds.");
      return NULL;
   }
}
%exception nTupleReader_impl::getY {
   try {
      $action
   } catch (nTR_OutOfBound &e) {
      PyErr_SetString(PyExc_SystemError, "nTupleReader error: out of bounds.");
      return NULL;
   }
}
%exception nTupleReader_impl::getZ {
   try {
      $action
   } catch (nTR_OutOfBound &e) {
      PyErr_SetString(PyExc_SystemError, "nTupleReader error: out of bounds.");
      return NULL;
   }
}

#endif


class nTupleReader_impl : public RootFileReaderBase {
	int d_member;
	short d_alphasPower;
	Char_t d_alphasPowerChar;
	char d_part[2];
	bool convertFromChar;
	#ifdef LHAPDF_NEW_VERSION
		LHAPDF::PDFSet d_pdfset;
		std::vector<LHAPDF::PDF*> d_pdfs;
		LHAPDF::PDF* d_pdf;
 	 #endif
	double d_totalWeights;
	long d_nEvents;
	double d_totalWeightsSquared;
	long d_lastID;
	double d_eventWeight;
public:
	NtupleInfo<MAX_NBR_PARTICLES> d_NI;
	//! Constructor for a nTupleReader object
	nTupleReader_impl();
	nTupleReader_impl(const std::string&  treeName);
	/**
	Reads the next entry and returns true upon success, false otherwise
	(including when the end of the file is reached).
	*/
	bool nextEntry();
	/** Sets the pdf set to be used.
	\param name is the name of the file to be loaded by \\textsc{LHAPDF}, for example {\\tt CT10.LHgrid\/}.
	*/
	void setPDF(const std::string& name);
	/** Sets the pdf member number to be used.
	\param member is an integer labeling the member;
	0 is typically used to denote the central value.
	*/
	void setPDFmember(int member);
	/** Returns the ID of the current event.
	 *
	*/
	int getID(){return d_NI.id;}
	/** Returns the number of final state particles in the current entry.
	*/
	int getParticleNumber(){return d_NI.nparticle;}
	/** Returns the energy of the $i^{\rm th}$ particle in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getEnergy(int i){if (i<d_NI.nparticle) {return d_NI.hasDoublePrecisionMomenta?d_NI.ED[i]:d_NI.E[i];} else throw nTR_OutOfBound();};
	/** Returns the $x$ component of the $i^{\rm th}$ particle's momentum in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getX(int i){if (i<d_NI.nparticle) {return d_NI.hasDoublePrecisionMomenta?d_NI.pxD[i]:d_NI.px[i];} else throw nTR_OutOfBound();};
	/** Returns the $y$ component of the $i^{\rm th}$ particle's momentum in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getY(int i){if (i<d_NI.nparticle) {return d_NI.hasDoublePrecisionMomenta?d_NI.pyD[i]:d_NI.py[i];} else throw nTR_OutOfBound();};
	/** Returns the $z$ component of the $i^{\rm th}$ particle's momentum in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getZ(int i){if (i<d_NI.nparticle) {return d_NI.hasDoublePrecisionMomenta?d_NI.pzD[i]:d_NI.pz[i];} else throw nTR_OutOfBound();};
	/** Returns the PDG code of the $i^{\rm th}$ particle in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	int getPDGcode(int i){if (i<d_NI.nparticle) {return d_NI.kf[i];} else throw nTR_OutOfBound();}
	/** Returns the momentum fraction $x_1$ in the current entry.
	*/
	double getX1(){return d_NI.x1;};
	/** Returns the momentum fraction $x_2$ in the current entry.
	*/
	double getX2(){return d_NI.x2;};
	/** Returns the PDG code for the first (forward) incoming parton in the current entry.
	*/
	double getId1(){return d_NI.id1;};
	/** Returns the PDG code for the second (backward) incoming parton in the current entry.
	*/
	double getId2(){return d_NI.id2;};
	/** Returns the power of the strong coupling constant in the current entry.
	*/
	short getAlphasPower(){return d_alphasPower;}
	/** Returns the renormalization scale used to compute the weights for the current entry.
	*/
	double getRenormalizationScale(){return d_NI.muR;}
	/** Returns the factorization scale used to compute the weights for the current entry.
	*/
	double getFactorizationScale(){return d_NI.muF;}
	/** Returns the weight ({\\tt weight\/}) for the current entry.
	*/
	double getWeight(){return d_NI.wgt;}
	/** Returns the secondary weight ({\\tt weight2\/}) for the current entry, to be used
	 * as described in \\sect{NTupleUseSection} to obtain the correct estimate of the
	 * statistical uncertainty.
	*/
	double getWeight2(){return d_NI.wgt2;}
	/** Returns the weight for the current entry omitting pdf factors.
	*/
	double getMEWeight(){return d_NI.me_wgt;}
	/** Returns the secondary weight for the current entry omitting the pdf factors, to be used
	 * as described in \\sect{NTupleUseSection} to obtain the correct estimate of the statistical
	 * uncertainty.
	*/
	double getMEWeight2(){return d_NI.me_wgt2;}
	/** Returns the type of the current entry, `B' standing for born, `I' for integrated subtraction, `V' for the virtual,
and `R' for the subtracted real emission.
	*/
	char getType(){return d_part[0];}
	/** Returns the weight ({\\tt weight\/}) of the current entry
	 * recomputed for the new scales, using the current pdf member number in the current pdf set.
	 * \param newFactorizationScale is the new factorization scale (in GeV)
	 * \param newRenormalisationScale is the new renormalization scale (in GeV)
	*/
	double computeWeight(double newFactorizationScale,double newRenormalizationScale);
	/** Returns the secondary weight ({\\tt weight2\/}) of the current entry
	 * recomputed for the new scales, using the current pdf member number in the current pdf set.
	 * One should use this weight for the real part in order to take into account the correlation between the entry and counter entries.
	 * \param newFactorizationScale is the new factorization scale (in GeV)
	 * \param newRenormalisationScale is the new renormalization scale (in GeV)
	*/
	double computeWeight2(double newFactorizationScale,double newRenormalizationScale);
	/** Sets the initial state to proton\--proton. This is the default if no calls to {\\tt setPP()} or {\\tt setPPbar\(\)} are issued. This routine should only be invoked before using files generated for proton\--proton colliders.
	 * */
	void setPP();
	/** Sets the initial state to proton\--antiproton. This routine should only be invoked before using files generated for proton\--antiproton colliders.
	 * */
	void setPPbar();
	//! Destructor
	virtual ~nTupleReader_impl();
	virtual void addFile(const std::string& filename);
	virtual void addFiles(const std::vector<std::string>& files);

#ifdef USE_HEPMC
	HepMCcoll d_collider;
	void fillHepMC(HepMC::GenEvent& evt,double xsec,double err);
	void fillHepMC(HepMC::GenEvent& evt);
	void setCMSEnergy(double CMS_energy);
	void setColliderType(colliderType ct);

#endif


#ifdef LHAPDF_NEW_VERSION
private:
	void clearPDFs();
#endif
public:
void resetCrossSection();
double getCrossSection();
double getCrossSectionError();
private:
void updateCrossSection();


};



#endif /* NTUPLEREADER_IMPL_H_ */



