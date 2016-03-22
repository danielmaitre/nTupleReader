/*
 * HepMC_utils.cpp
 *
 *  Created on: 8 Jun 2015
 *      Author: daniel
 */


#ifdef USE_HEPMC

#include "HepMC_utils.h"


using namespace HepMC;

HepMCcoll::HepMCcoll(double CMS_energy,colliderType ct): d_colliderType(ct) {
	setCMSEnergy(CMS_energy);
}


void HepMCcoll::setCMSEnergy(double CMS_energy) {
	d_beam1=FourVector(0, 0, CMS_energy/2.0, CMS_energy/2.0);
	d_beam2=FourVector(0, 0, -CMS_energy/2.0, CMS_energy/2.0);
}


#endif

