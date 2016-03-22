/*
 * HepMC_utils.h
 *
 *  Created on: 8 Jun 2015
 *      Author: daniel
 */

#ifndef NTUPLESREADER_HEPMC_UTILS_H_
#define NTUPLESREADER_HEPMC_UTILS_H_

#ifdef USE_HEPMC

#include "HepMC/GenEvent.h"
#include "nTupleReader.h"


struct HepMCcoll {
	colliderType d_colliderType;
	HepMC::FourVector d_beam1;
	HepMC::FourVector d_beam2;
	HepMCcoll(double CMS_energy=7000.0,colliderType ct=pp);
	void setCMSEnergy(double CMS_energy);
};


#endif /* USE_HEPMC */


#endif /* NTUPLESREADER_HEPMC_UTILS_H_ */
