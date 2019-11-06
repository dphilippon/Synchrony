/*
 * poultrytype.cpp
 *
 *  Created on: Nov 1, 2019
 *      Author: Damien
 */

#include "poultrytype.h"
#include "parameters.h"
#include<string>
#include <vector>
#include <map>
#include <iostream>

int poultry_type::nb_types = 0;
map<int,poultry_type*> poultry_type::types;

poultry_type::poultry_type(int id,string label, int initial_S, int initial_E,
		int initial_I, int initial_R, int initial_N, float life_expectancty, float min_phi,
		float max_phi, bool renewal, float time_renewal, float sigma, float gamma,
		float ksi, float rho)
{
	this->label = label;
	this->initial_S = initial_S;
	this->initial_E = initial_E;
	this->initial_I = initial_I;
	this->initial_R = initial_R;
	this->initial_N = initial_N;
	this->life_expectancy = life_expectancty;
	this->min_phi = min_phi;
	this->max_phi = max_phi;
	this->renewal = renewal;
	this->time_renewal = time_renewal;
	this->sigma = sigma;
	this->gamma = gamma;
	this->ksi = ksi;
	this->id = id;
	this->rho = rho;
	poultry_type::nb_types = poultry_type::getNbTypes()+1;
	this->nb_bands = 0;
	this->total_S =0;
	this->total_E =0;
	this->total_I =0;
	this->total_R =0;
	this->total_N =0;
}
poultry_type::~poultry_type()
{

}

//***********************************
//		    Static methods
//***********************************
//Static function returning the number of band type
int poultry_type::getNbTypes()
{
	return nb_types;
}
//Static function returning the types of band
map<int,poultry_type*> poultry_type::getTypes()
{
	return poultry_type::types;
}
//Return a poultry type from a given ID of type
poultry_type* poultry_type::getType(int id)
{
	if(poultry_type::types.count(id)>0)
	{
		return poultry_type::types.find(id)->second;
	}
	else
	{
		parameters::print_debug("No type found for that ID");
		return NULL;
	}
}

//***********************************
//		    Instance methods
//***********************************
//Return the ID of the band
int poultry_type::getId()
{
	return this->id;
}
//Return the label of the type of band
string poultry_type::getLabel()
{
	return this->label;
}
//Return the beta_to_type of the type of band
map<int,float> poultry_type::getBetaToType()
{
	return this->beta_to_type;
}
//Return the beta_to_type of the type of band for a given type of band
float poultry_type::getBetaToTypeBand(int id_band_type)
{
	float result = -1.0;
	if(this->beta_to_type.empty()==false)
	{
		if(this->beta_to_type.count(id_band_type)==0)
		{
			parameters::print_debug("Poultry type beta not found");
		}
		else
		{
			result = this->beta_to_type.find(id_band_type)->second;
		}
	}
	else
	{
		parameters::print_debug("No poultry type beta initialized");
	}
	return result;
}
//Return the beta_to_type of the type of band
map<int,float> poultry_type::getCouplingToType()
{
	return this->coupling_to_type;
}
//Return the beta_to_type of the type of band for a given type of band
float poultry_type::getCouplingToTypeBand(int id_band_type)
{
	float result = -1.0;
	if(this->coupling_to_type.empty()==false)
	{
		if(this->coupling_to_type.count(id_band_type)==0)
		{
			parameters::print_debug("Poultry type coupling not found");
		}
		else
		{
			result = this->coupling_to_type.find(id_band_type)->second;
		}
	}
	else
	{
		parameters::print_debug("No poultry type coupling initialized");
	}
	return result;
}
//Return a type of poultry for a given ID
void poultry_type::addType(poultry_type* aType)
{
	poultry_type::types.insert(pair<int,poultry_type*>(aType->getId(),aType));
}
//Return the initial number of susceptible for the type
int poultry_type::getInitialS()
{
	return this->initial_S;
}
//Return the initial number of exposed for the type
int poultry_type::getInitialE()
{
	return this->initial_E;
}
//Return the initial number of infected for the type
int poultry_type::getInitialI()
{
	return this->initial_I;
}
//Return the initial number of recovered for the type
int poultry_type::getInitialR()
{
	return this->initial_R;
}
//Return the initial number of chicken for the type
int poultry_type::getInitialN()
{
	return this->initial_N;
}
//Return the life expectancy of a band for the type
float poultry_type::getLifeExpectancy()
{
	return this->life_expectancy;
}
//Return the min phi parameter for the type
float poultry_type::getMinPhi()
{
	return this->min_phi;
}
//Return the max phi parameter for the type
float poultry_type::getMaxPhi()
{
	return this->max_phi;
}
//Return the number of bands for the type
int poultry_type::getNbBands()
{
	return this->nb_bands;
}
//Return the renewal parameter for the type
bool poultry_type::isRenewal()
{
	return this->renewal;
}
//Return the time between the deactivation and activation of a band for the type
float poultry_type::getTimeRenewal()
{
	return this->time_renewal;
}
//Return the sigma (E->I) for the type
float poultry_type::getSigma()
{
	return this->sigma;
}
//Return the gamma (I->R) for the type
float poultry_type::getGamma()
{
	return this->gamma;
}
//Return the ksi (R->S) for the type
float poultry_type::getKsi()
{
	return this->ksi;
}
//Add a band of poultry to the type
void poultry_type::addBand(poultry_band* aBand)
{
	this->bands.insert(aBand);
	this->nb_bands = this->nb_bands +1;
}
//Return the bands of the type
set<poultry_band*> poultry_type::getBands()
{
	return this->bands;
}
//Add a beta from this type of band to another give type
void poultry_type::addBetaToType (poultry_type* aType, float aBeta)
{
	this->beta_to_type.insert(pair<int,float>(aType->getId(),aBeta));
}
//Add a coupling from this type of band to another give type
void poultry_type::addCouplingToType (poultry_type* aType, float aCoupling)
{
	this->coupling_to_type.insert(pair<int,float>(aType->getId(),aCoupling));
}
//Return all the bands that are currently activated
set<poultry_band*> poultry_type::getActivatedBands()
{
	return this->activated_bands;
}
string poultry_type::toString()
{
	string result = to_string(this->id)+"-"+this->label+" ("+to_string(this->getNbBands())+" bands)\n";
	for(map<int,float>::iterator it = this->beta_to_type.begin(); it!=this->beta_to_type.end();++it)
	{
		result = result + "\t "+to_string(it->first)+" "+to_string(it->second)+"\n";
	}
	return result;
}

void poultry_type::addActivatedBand(poultry_band* aBand)
{
	this->activated_bands.insert(aBand);
	this->total_S = this->total_S + aBand->getS();
	this->total_E = this->total_E + aBand->getE();
	this->total_I = this->total_I + aBand->getI();
	this->total_R = this->total_R + aBand->getR();
	this->total_N = this->total_N + aBand->getN();
}
void poultry_type::removeActivatedBand(poultry_band* aBand)
{
	this->activated_bands.erase(aBand);
	this->total_S = this->total_S - aBand->getS();
	this->total_E = this->total_E - aBand->getE();
	this->total_I = this->total_I - aBand->getI();
	this->total_R = this->total_R - aBand->getR();
	this->total_N = this->total_N - aBand->getN();
}
void poultry_type::addS()
{
	this->total_S=this->total_S+1;
}
void poultry_type::addE()
{
	this->total_E=this->total_E+1;
}
void poultry_type::addI()
{
	this->total_I=this->total_I+1;
}
void poultry_type::addR()
{
	this->total_R=this->total_R+1;
}
void poultry_type::removeS()
{
	this->total_S=this->total_S-1;
}
void poultry_type::removeE()
{
	this->total_E=this->total_E-1;
}
void poultry_type::removeI()
{
	this->total_I=this->total_I-1;
}
void poultry_type::removeR()
{
	this->total_R=this->total_R-1;
}
int poultry_type::getTotalS()
{
	return this->total_S;
}
int poultry_type::getTotalE()
{
	return this->total_E;
}
int poultry_type::getTotalI()
{
	return this->total_I;
}
int poultry_type::getTotalR()
{
	return this->total_R;
}
int poultry_type::getTotalN()
{
	return this->total_N;
}
float poultry_type::getRho()
{
	return this->rho;
}
