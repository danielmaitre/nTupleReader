/*
 * nTupleReader.h
 *
 *  Created on: Feb 13, 2012
 *      Author: daniel
 */

#ifndef NTUPLEREADER_H_
#define NTUPLEREADER_H_

#include <string>
#include <vector>
#include <iostream>


enum colliderType {pp,ppbar};


namespace HepMC {
class GenEvent;
}

class nTupleReader_impl;

class nTupleReader {
	nTupleReader_impl *d_impl;
public:
	//! Constructor for a nTupleReader object
	nTupleReader();
	nTupleReader(const std::string&  treeName);
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
	int getID();
	/** Returns the number of final state particles in the current entry.
	*/
	int getParticleNumber();
	/** Returns the energy of the $i^{\rm th}$ particle in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getEnergy(int i);;
	/** Returns the $x$ component of the $i^{\rm th}$ particle's momentum in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getX(int i);
	/** Returns the $y$ component of the $i^{\rm th}$ particle's momentum in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getY(int i);
	/** Returns the $z$ component of the $i^{\rm th}$ particle's momentum in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	double getZ(int i);
	/** Returns the PDG code of the $i^{\rm th}$ particle in the current entry.
		\param i is a 0-based index; an argument equal to or larger than the number of final state particles will throw
an {\\tt nTR\\\_OutOfBound\/} exception.
	*/
	int getPDGcode(int i);
	/** Returns the momentum fraction $x_1$ in the current entry.
	*/
	double getX1();
	/** Returns the momentum fraction $x_2$ in the current entry.
	*/
	double getX2();
	/** Returns the PDG code for the first (forward) incoming parton in the current entry.
	*/
	double getId1();
	/** Returns the PDG code for the second (backward) incoming parton in the current entry.
	*/
	double getId2();
	/** Returns the power of the strong coupling constant in the current entry.
	*/
	short getAlphasPower();
	/** Returns the renormalization scale used to compute the weights for the current entry.
	*/
	double getRenormalizationScale();
	/** Returns the factorization scale used to compute the weights for the current entry.
	*/
	double getFactorizationScale();
	/** Returns the weight ({\\tt weight\/}) for the current entry.
	*/
	double getWeight();
	/** Returns the secondary weight ({\\tt weight2\/}) for the current entry, to be used
	 * as described in \\sect{NTupleUseSection} to obtain the correct estimate of the
	 * statistical uncertainty.
	*/
	double getWeight2();
	/** Returns the weight for the current entry omitting pdf factors.
	*/
	double getMEWeight();
	/** Returns the secondary weight for the current entry omitting the pdf factors, to be used
	 * as described in \\sect{NTupleUseSection} to obtain the correct estimate of the statistical
	 * uncertainty.
	*/
	double getMEWeight2();
	/** Returns the type of the current entry, `B' standing for born, `I' for integrated subtraction, `V' for the virtual,
and `R' for the subtracted real emission.
	*/
	char getType();
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
	virtual ~nTupleReader();
	virtual void addFile(const std::string& filename);
	virtual void addFiles(const std::vector<std::string>& files);
	void fillHepMC(HepMC::GenEvent& evt,double xsec,double err);
	void fillHepMC(HepMC::GenEvent& evt);
	void setCMSEnergy(double CMS_energy);
	void setColliderType(colliderType ct);
	void resetCrossSection();
	double getCrossSection();
	double getCrossSectionError();
        nTupleReader_impl* get_impl(){return d_impl;};


#ifdef LHAPDF_NEW_VERSION
private:
	void clearPDFs();
#endif


};



#endif /* NTUPLEREADER_H_ */
