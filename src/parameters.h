/*
 * parameters.h
 *
 *  Created on: Nov 1, 2019
 *      Author: Damien
 */

#ifndef SRC_PARAMETERS_H_
#define SRC_PARAMETERS_H_

#include<string>
#include <random>

using namespace std;
class parameters {
public:
	const static bool DEBUG = false;
	const static int NB_COLUMNS_FILE_TYPE = 17;
	const static int NB_COLUMNS_FILE_LINKED = 4;
	const static int NB_SIMULATIONS = 1;
	//T represents the maximum time simulated
	const static float MAX_TIME = 10;
	static uniform_real_distribution<double> distribution;
	static default_random_engine generator;
	static void print_debug(string message);
	static float get_phi(float min_phi, float max_phi);
	parameters();
	virtual ~parameters();
};

#endif /* SRC_PARAMETERS_H_ */
