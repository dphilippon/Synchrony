/*
 * poultryband.h
 *
 *  Created on: Nov 1, 2019
 *      Author: Damien
 */

#ifndef SRC_POULTRYBAND_H_
#define SRC_POULTRYBAND_H_

#include <map>
#include <vector>

using namespace std;

class poultry_band {
private:
	int id;
	int S;
	int E;
	int I;
	int R;
	int N;
	float phi;
	int type;
	bool activated;
	float lambda;
public:
	static int nb_bands;
	static map<int,poultry_band*> bands;
	static map<int,poultry_band> getBands();
	static void addBand(poultry_band* band);
	static float getMinimumPhi();
	static vector<poultry_band*> getBandsForPhi(float phi);
	poultry_band(int S, int E, int I, int R, int N, int type, float phi);
	virtual ~poultry_band();
	int getId();
	int getS();
	int getE();
	int getI();
	int getR();
	int getN();
	int getType();
	float getPhi();
	float getLambda();
	float computeLambda();
	bool getActivated();
	void addS();
	void addE();
	void addI();
	void addR();
	void removeS();
	void removeE();
	void removeI();
	void removeR();
	void updatePhi(float dt);
	string toString();
};

#endif /* SRC_POULTRYBAND_H_ */
