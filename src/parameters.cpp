/*
 * parameters.cpp
 *
 *  Created on: Nov 1, 2019
 *      Author: Damien
 */

#include "parameters.h"
#include <iostream>

//Variables for uniform random numbers
uniform_real_distribution<double> parameters::distribution(0.0,1.0);
default_random_engine parameters::generator;

parameters::parameters() {
	// TODO Auto-generated constructor stub

}
parameters::~parameters() {
	// TODO Auto-generated destructor stub
}
void parameters::print_debug(string message)
{
	if(parameters::DEBUG==true)
	{
		cout<<message<<"\n";
	}
}
float parameters::get_phi(float min_phi, float max_phi)
{
	return (parameters::distribution(parameters::generator)*(max_phi-min_phi))+min_phi;
}
