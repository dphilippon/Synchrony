/*
 * poultrytype.h
 *
 *  Created on: Nov 1, 2019
 *      Author: Damien
 */

#ifndef SRC_POULTRYTYPE_H_
#define SRC_POULTRYTYPE_H_

#include<string>
#include <set>
#include<map>
#include "poultryband.h"

using namespace std;

class poultry_type {
private:

	//***********************************
	//		    Instance variables
	//***********************************
	//ID of the type
	int id;
	//Dictionary containing the transmission from this poultry type to another using the ID as key
	map<int,float> beta_to_type;
	//Dictionary containing the coupling from this poultry type to another using the ID as key
	map<int,float> coupling_to_type;
	//initial number of susceptible chicks when the bands of that type are activated
	//if the renewal attribute is set to false, all the chicks of the band will be susceptible
	//at renewal
	int initial_S;
	//initial number of exposed-latent chicks the bands of that type are activated
	int initial_E;
	//initial number of infectious chicks when the bands of that type are activated
	int initial_I;
	//initial number of recovered-protected chicks when the bands of that type are activated
	int initial_R;
	//initial number of chicks when the bands of that type are activated
	int initial_N;
	//life expectancy of the bands of that type (in days)
	float life_expectancy;
	//time between the re-activation of the bands of that type (in days)
	float time_renewal;
	//Renewing the bands of that type will make their S,E,I,R,N as initial value, if set to false,
	//when the bands are renewing, all the chicks will be susceptible
	bool renewal;
	//Rate of recovered-protected chicks of this band type becoming susceptible again (in days)
	float ksi;
	//Rate of infectious chicks of this band type becoming recovered-protected (in days)
	float gamma;
	//Rate of exposed-latent chicks of this band type becoming infectious (in days)
	float sigma;
	//Number of bands belonging to this band type
	int nb_bands;
	//Time before the first possible activation of the bands of that type (in days)
	float min_phi;
	//Time before the last possible activation of the bands of that type (in days)
	float max_phi;
	//Label of the band type
	string label;
	//Bands belonging to that type
	set<poultry_band*> bands;
	//Activated bands
	set<poultry_band*> activated_bands;
	//Number of susceptible activated of that type
	float total_S;
	//Number of exposed activated of that type
	float total_E;
	//Number of infected activated of that type
	float total_I;
	//Number of recovered activated of that type
	float total_R;
	//Number of chicken activated of that type
	float total_N;
	//Interaction from others bands for poultry bands of that type
	float rho;
public:

	//***********************************
	//		    Static variables
	//***********************************
	//Number of types used in a simulation
	static int nb_types;
	//List of the different type objects
	static map<int,poultry_type*> types;

	//***********************************
	//		    Constructors
	//***********************************
	poultry_type(int id,string label, int initial_S, int initial_E,
			int initial_I, int initial_R, int initial_N, float life_expectancty, float min_phi,
			float max_phi, bool renewal, float time_renewal, float sigma, float gamma,
			float ksi, float rho);
	virtual ~poultry_type();

	//***********************************
	//		    Static methods
	//***********************************
	//Static function returning the number of band type
	static int getNbTypes();
	//Static function returning the types of band
	static map<int,poultry_type*> getTypes();
	//Static function to add a type
	static void addType(poultry_type* aType);
	//Return a type of poultry for a given ID of type
	static poultry_type* getType(int id);
	//***********************************
	//		    Instance methods
	//***********************************
	//Return the ID of the band
	int getId();
	//Return the label of the type of band
	string getLabel();
	//Return the beta_to_type of the type of band
	map<int,float> getBetaToType();
	//Return the coupling from type of the type of band
	map<int,float> getCouplingToType();
	//Return the couplingFromType of the type of band for a given type of band
	float getCouplingToTypeBand(int id_band_type);
	//Return the beta_to_type of the type of band for a given type of band
	float getBetaToTypeBand(int id_band_type);
	//Return the initial number of susceptible of the type of band
	int getInitialS();
	//Return the initial number of exposed-latent of the type of band
	int getInitialE();
	//Return the initial number of infectious of the type of band
	int getInitialI();
	//Return the initial number of recovered-protected of the type of band
	int getInitialR();
	//Return the initial number of chicks of the type of band
	int getInitialN();
	//Return the life expectancy of the type of band
	float getLifeExpectancy();
	//Return the minimum number of days possible for the last band activation of the type of band
	float getMinPhi();
	//Return the maximum number of days possible for the last band activation of the type of band
	float getMaxPhi();
	//Return the number of bands of that type
	int getNbBands();
	//Return the renewal parameter of that type
	bool isRenewal();
	//Return the time of reactivation for a band of the type of band
	float getTimeRenewal();
	//Return the sigma of the type of band
	float getSigma();
	//Return the gamma of the type of band
	float getGamma();
	//Return the ksi of the type of band
	float getKsi();
	//Add a band to the bands of that type
	void addBand(poultry_band* aBand);
	//add a beta between two poultry types
	void addBetaToType (poultry_type* aType, float aBeta);
	//add a coupling between two poultry types
	void addCouplingToType (poultry_type* aType, float aCoupling);
	//add a coupling between two poultry types
	float getRho();
	//Get all the bands activated of that type
	set<poultry_band*> getActivatedBands();
	//Get all the bands of the type
	set<poultry_band*> getBands();
	//Return the string to print the type
	string toString();
	void addActivatedBand(poultry_band* aBand);
	void removeActivatedBand(poultry_band* aBand);
	void addS();
	void addE();
	void addI();
	void addR();
	void removeS();
	void removeE();
	void removeI();
	void removeR();
	int getTotalS();
	int getTotalE();
	int getTotalI();
	int getTotalR();
	int getTotalN();
};

#endif /* SRC_POULTRYTYPE_H_ */
