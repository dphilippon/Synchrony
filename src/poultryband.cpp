/*
 * poultryband.cpp
 *
 *  Created on: Nov 1, 2019
 *      Author: Damien
 */

#include "poultryband.h"
#include "poultrytype.h"
#include <map>
#include <iostream>

using namespace std;

int poultry_band::nb_bands = 0;
map<int,poultry_band*> poultry_band::bands;

poultry_band::poultry_band(int S, int E, int I, int R, int N, int type, float phi) {
	this->id = poultry_band::nb_bands +1;
	poultry_band::nb_bands = poultry_band::nb_bands+1;
	this->activated = false;
	this->phi = phi;
	this->S = S;
	this->E = E;
	this->I = I;
	this->R = R;
	this->N = N;
	this->type = type;
}
poultry_band::~poultry_band() {
	// TODO Auto-generated destructor stub
}

int poultry_band::getId()
{
	return this->id;
}
int poultry_band::getS()
{
	return this->S;
}
int poultry_band::getE()
{
	return this->E;
}
int poultry_band::getI()
{
	return this->I;
}
int poultry_band::getR()
{
	return this->R;
}
int poultry_band::getN()
{
	return this->N;
}
int poultry_band::getType()
{
	return this->type;
}
float poultry_band::getPhi()
{
	return this->phi;
}
bool poultry_band::getActivated()
{
	return this->activated;
}
void poultry_band::addS()
{
	this->S = this->S +1;
	poultry_type::getType(this->type)->addS();
}
void poultry_band::addE()
{
	this->E = this->E +1;
	poultry_type::getType(this->type)->addE();
}
void poultry_band::addI()
{
	this->I = this->I +1;
	poultry_type::getType(this->type)->addI();
}
void poultry_band::addR()
{
	this->R = this->R +1;
	poultry_type::getType(this->type)->addR();
}
void poultry_band::removeS()
{
	this->S = this->S -1;
	poultry_type::getType(this->type)->removeS();
}
void poultry_band::removeE()
{
	this->E = this->E -1;
	poultry_type::getType(this->type)->removeE();
}
void poultry_band::removeI()
{
	this->I = this->I -1;
	poultry_type::getType(this->type)->removeI();
}
void poultry_band::removeR()
{
	this->R = this->R -1;
	poultry_type::getType(this->type)->removeR();
}
void poultry_band::updatePhi(float dt)
{
	this->phi = this->phi-dt;
	if(this->phi==0)
	{
		if(this->activated==false)
		{
			this->activated = true;
			this->phi = poultry_type::getType(this->type)->getLifeExpectancy();
			poultry_type::getType(this->type)->addActivatedBand(this);
		}
		else
		{
			this->activated = false;
			this->phi = poultry_type::getType(this->type)->getTimeRenewal();
			poultry_type* type= poultry_type::getType(this->getType());
			poultry_type::getType(this->type)->removeActivatedBand(this);
			if(type->isRenewal())
			{
				this->S = type->getInitialS();
				this->E = type->getInitialE();
				this->I = type->getInitialI();
				this->R = type->getInitialR();
				this->N = type->getInitialN();
			}
			else
			{
				this->S = type->getInitialN();
				this->E = 0;
				this->I = 0;
				this->R = 0;
				this->N = type->getInitialN();
			}
		}
	}
}
void poultry_band::addBand(poultry_band* band)
{
	poultry_band::bands.insert(pair<int,poultry_band*>(band->getId(),band));
}
float poultry_band::getMinimumPhi()
{
	float minimum = -1.0;
	for(map<int,poultry_band*>::iterator it = poultry_band::bands.begin(); it != poultry_band::bands.end(); ++it)
	{
	  if(minimum == -1.0)
	  {
		  minimum = it->second->getPhi();
	  }
	  else
	  {
		  if(minimum>it->second->getPhi())
		  {
			  minimum = it->second->getPhi();
		  }
	  }
	}
	return minimum;
}
vector<poultry_band*> poultry_band::getBandsForPhi(float phi)
{
	vector<poultry_band*> results;
	for(map<int,poultry_band*>::iterator it = poultry_band::bands.begin(); it != poultry_band::bands.end(); ++it)
	{
	  if(it->second->getPhi()==phi)
	  {
		  results.push_back(it->second);
	  }
	}
	return results;
}
string poultry_band::toString()
{
	return to_string(this->id)+"- S:"+to_string(this->S)
			+" E:"+to_string(this->E)
			+" I:"+to_string(this->I)
			+" R:"+to_string(this->R)
			+" N:"+to_string(this->N);
}

float poultry_band::getLambda()
{
	return this->lambda;
}
float poultry_band::computeLambda()
{
	this->lambda = 0.0;
	float rho = poultry_type::getTypes()[this->getType()]->getRho();
	for(map<int,poultry_type*>::iterator it= poultry_type::types.begin(); it!= poultry_type::types.end();++it)
	{
		float nb_infected = it->second->getTotalI();
		float total = it->second->getTotalN();
		if(it->first==this->getType())
		{
			nb_infected = nb_infected-this->getI();
			total = total - this->getN();
		}
		if(total > 0)
		{
			float coupling = it->second->getCouplingToTypeBand(this->getType());
			float beta = it->second->getBetaToTypeBand(this->getType());
			this->lambda = this->lambda + beta*coupling*rho*nb_infected/total;
		}
	}
	if(this->getN()>0)
	{
		this->lambda = this->lambda+(poultry_type::getTypes()[this->getType()]->getBetaToTypeBand(this->getType())*(1-rho)*this->getI()/this->getN());
	}
	this->lambda = this->lambda*this->getS();
	return this->lambda;
}
