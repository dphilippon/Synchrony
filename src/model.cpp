//============================================================================
// Name        : Test_Gillespie.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include "poultrytype.h"
#include "parameters.h"
#include "poultryband.h"

using namespace std;

void reset_model()
{
	poultry_band::bands.clear();
	poultry_band::nb_bands=0;
	poultry_type::nb_types=0;
	poultry_type::types.clear();
}

//Initialization of the model using the band file path and linked file path
void initialize_model(string band_file_path, string linked_file_path)
{
	ifstream band_type_file(band_file_path);
	ifstream linked_file(linked_file_path);
	string line = "";
	string word;
	vector<string> row;
	int id_line = 0;

	//Go through the content of the band type file (CSV)
	while (getline(band_type_file, line))
	{
		if(id_line>0)
		{
			stringstream s(line);
			while (getline(s, word, ','))
			{
				row.push_back(word);
			}
			//Initialize a poultry type according to the parameters given in the CSV
			if(row.size()==parameters::NB_COLUMNS_FILE_TYPE)
			{
				poultry_type* tmp_pt = new poultry_type(stoi(row[0]), row[1], stoi(row[2]), stoi(row[3]),
						stoi(row[4]), stoi(row[5]), stoi(row[6]), stof(row[7]), stof(row[13]),
						stof(row[14]), row[15]=="1", stof(row[8]), stof(row[11]), stof(row[10]),
						stof(row[9]), stof(row[16]));
				poultry_type::addType(tmp_pt);
				//Initialize the poultry bands for the current poultry type
				for(int i = 0; i<stoi(row[12]); i++)
				{
					poultry_band* tmp_band =new poultry_band(tmp_pt->getInitialS(), tmp_pt->getInitialE(),tmp_pt->getInitialI(), tmp_pt->getInitialR(), tmp_pt->getInitialN(),tmp_pt->getId(), parameters::get_phi(tmp_pt->getMinPhi(),tmp_pt->getMaxPhi()));
					tmp_pt->addBand(tmp_band);
					poultry_band::addBand(tmp_band);
				}
			}
			row.clear();
		}
		id_line = id_line +1;
	}
	band_type_file.close();
	line = "";
	word="";
	id_line = 0;

	//Go through the content of the linked file (CSV)
	while (getline(linked_file, line))
	{
		row.clear();
		if(id_line>0)
		{
			stringstream s(line);
			while (getline(s, word, ','))
			{
				row.push_back(word);
			}
			//Generate the poultry type
			if(row.size()==parameters::NB_COLUMNS_FILE_LINKED)
			{
				poultry_type* from_type = poultry_type::getType(stoi(row[0]));
				poultry_type* to_type = poultry_type::getType(stoi(row[1]));
				from_type->addBetaToType(to_type,stof(row[2]));
				from_type->addCouplingToType(to_type,stof(row[3]));
			}
		}
		id_line = id_line +1;
	}
	// Close the File
	linked_file.close();
	if(parameters::DEBUG)
	{
		map<int,poultry_type*> types = poultry_type::getTypes();
		for(map<int,poultry_type*>::iterator it_type = types.begin(); it_type != types.end(); ++it_type)
		{
			parameters::print_debug(it_type->second->toString());
			set<poultry_band*> tmp_bands = it_type->second->getBands();
			for(auto it_band : tmp_bands)
			{
			   parameters::print_debug(it_band->toString());
			}
		}
	}
}

void model(string path_type_file, string path_linked_file, string header)
{
	float time_computing_W = 0;
	float time_computing_event = 0;
	float time_updating_band = 0;
	float time_saving = 0;
	//t represents the time
	float time = 0;
	//delta_t represents the time to add for a new event to occur
	float delta_t;
	//event represents the number of events
	int event = 0;
	//Set containing all the poultry bands
	set<poultry_band*> bands_poultry;
	//Set containing all the types of poultry
	set<poultry_type*> types_poultry ;
	//Structure to contain the SIR Data(Type, Event (1..n), Value (time,S,E,I,R,N))
	map<int,vector<vector<float > > > SIR_data; //

	//Initialization of the model
	initialize_model(path_type_file,path_linked_file);

	//Initialization of the set of poultry types using the map of poultry types
	for(map<int,poultry_type*>::iterator it = poultry_type::types.begin(); it!=poultry_type::types.end();++it)
	{
		types_poultry.insert(it->second);
	}
	//Initialization of the set of poultry bands using the map of poultry bands
	for(map<int,poultry_band*>::iterator it = poultry_band::bands.begin(); it!=poultry_band::bands.end();++it)
	{
		bands_poultry.insert(it->second);
	}

	auto start = chrono::system_clock::now();
	auto end = chrono::system_clock::now();
	auto elapsed = end-start;
	//Execution of the model
	while(time<parameters::MAX_TIME)
	{
		//Set containing the bands currently activated
		set<poultry_band*> activated_bands;

		start = chrono::system_clock::now();
		//Sum of all the probabilities of the events
		float W = 0;

		//Check that at least one band is activated and begin to compute W
		for(auto type_poultry_elem:types_poultry)
		{
			set<poultry_band*> tmp_band = type_poultry_elem->getActivatedBands();
			if(tmp_band.size()>0)
			{
				activated_bands.insert(tmp_band.begin(),tmp_band.end());
				W = W + type_poultry_elem->getGamma()*type_poultry_elem->getTotalI() +
						type_poultry_elem->getKsi()*type_poultry_elem->getTotalR()+
						type_poultry_elem->getSigma()*type_poultry_elem->getTotalE();
			}
		}

		//Check that at least one band is activated to compute delta_t, else, we would go directly
		//to the time of the earliest activation of a band
		if(activated_bands.size()>0)
		{
			//Compute lambda for each band which is activated and add it to the sum of probabilities
			//of events W
			for(auto band_poultry_elem : activated_bands)
			{
				float lambda = band_poultry_elem->computeLambda();
				W = W + lambda;
			}
			end = chrono::system_clock::now();
			elapsed = end-start;
			time_computing_W = time_computing_W + chrono::duration<double, milli> (elapsed).count();
			start = chrono::system_clock::now();

			//Get the minimum time value for a change in band status
			float minimum_phi = poultry_band::getMinimumPhi();

			//Computation of delta_t as explained in Gillespie
			float rand_number = parameters::distribution(parameters::generator);
			delta_t = -log(rand_number)/W;


			//Checking that delta_t is actually lower that the minimum time value for a change
			//in band status, else we would directly go to that event
			if(delta_t <= minimum_phi)
			{
				//Determining which event is occurring
				float sum_event = 0;
				float rnd = parameters::distribution(parameters::generator);

				//Go through the band to look for the one having the event
				for(auto band_poultry_elem : activated_bands)
				{
					poultry_type* type = poultry_type::getType(band_poultry_elem->getType());
					sum_event = sum_event + band_poultry_elem->getLambda()/W;

					//It is a new infection
					if((rnd<sum_event))
					{
						band_poultry_elem->addE();
						band_poultry_elem->removeS();
						break;
					}
					sum_event = sum_event + type->getSigma()*band_poultry_elem->getE()/W;

					//It is an latent-exposed becoming infectious
					if(rnd<sum_event)
					{
						band_poultry_elem->addI();
						band_poultry_elem->removeE();
						break;
					}
					sum_event = sum_event + type->getGamma()*band_poultry_elem->getI()/W;

					//It is an infectious becoming protected-recovered
					if(rnd<sum_event)
					{
						band_poultry_elem->addR();
						band_poultry_elem->removeI();
						break;
					}
					sum_event = sum_event + type->getKsi()*band_poultry_elem->getR()/W;

					//It is an protected-recovered becoming susceptible again
					if(rnd<sum_event)
					{
						band_poultry_elem->addS();
						band_poultry_elem->removeR();
						break;
					}
				}
			}
			else
			{
				delta_t = minimum_phi;
			}
		}
		else
		{
			//If no band is activated, jump directly to the time of the next activation
			delta_t = poultry_band::getMinimumPhi();
		}
		end = chrono::system_clock::now();
		elapsed = end-start;
		time_computing_event = time_computing_event + chrono::duration<double, milli> (elapsed).count();
		start = chrono::system_clock::now();

		//Update the phi of all the bands given the delta_t computed
		for(auto band_poultry_elem : bands_poultry)
		{
			band_poultry_elem->updatePhi(delta_t);
		}
		time = time + delta_t;
		event= event+1;
		end = chrono::system_clock::now();
		elapsed = end-start;
		time_updating_band = time_updating_band + chrono::duration<double, milli> (elapsed).count();
		start = chrono::system_clock::now();


		//Saving into the 4D structure
		int total_number_susceptible = 0;
		int total_number_exposed = 0;
		int total_number_infected = 0;
		int total_number_recovered = 0;
		int total_number_population = 0;
		vector<float> data_type;
		map<int,vector<vector<float> > >::iterator it;

		//Saving the number of SEIR+N value for each type of poultry for the current t
		for(auto type_poultry_elem: types_poultry)
		{
			//per type
			it = SIR_data.find(type_poultry_elem->getId());
			float number_susceptible = type_poultry_elem->getTotalS();
			float number_exposed = type_poultry_elem->getTotalE();
			float number_infected = type_poultry_elem->getTotalI();
			float number_recovered = type_poultry_elem->getTotalR();
			float number_chicken =  type_poultry_elem->getTotalN();
			total_number_susceptible = total_number_susceptible + number_susceptible;
			total_number_exposed = total_number_exposed + number_exposed;
			total_number_infected = total_number_infected + number_infected;
			total_number_recovered = total_number_recovered + number_recovered;
			total_number_population = total_number_population + number_chicken;

			data_type.push_back( time );
			data_type.push_back((float)number_susceptible);
			data_type.push_back((float)number_exposed);
			data_type.push_back((float)number_infected);
			data_type.push_back((float)number_recovered);
			data_type.push_back((float)number_chicken);

			if(it==SIR_data.end())
			{
				vector<vector<float> > event_data;
				event_data.push_back(data_type);
				SIR_data.insert(pair<int,vector<vector<float> > >(type_poultry_elem->getId(),event_data));
			}
			else
			{
				SIR_data[it->first].push_back(data_type);
			}
			data_type.clear();
		}

		//Adding the SEIR+N for all types of poultry for the current t
		it = SIR_data.find(0);
		data_type.push_back(time);
		data_type.push_back((float)total_number_susceptible);
		data_type.push_back((float)total_number_exposed);
		data_type.push_back((float)total_number_infected);
		data_type.push_back((float)total_number_recovered);
		data_type.push_back((float)total_number_population);
		if(it==SIR_data.end())
		{
			vector<vector<float> > event_data;
			event_data.push_back(data_type);
			SIR_data.insert(pair<int,vector<vector<float> > >(0,event_data));
		}
		else
		{
			SIR_data[it->first].push_back(data_type);
		}

		end = chrono::system_clock::now();
		elapsed = end-start;
		time_saving = time_saving + chrono::duration<double, milli> (elapsed).count();

	}

	cout << "Model finished" << "\n";
	//Save the structure in CSV
	ofstream myfile;
	map<int,vector<vector<float> > >::iterator it_type;
	for(auto type_poultry_elem: types_poultry)
	{
		myfile.open (header+"_"+type_poultry_elem->getLabel()+".csv");
		myfile <<"Event,S,E,I,R,N,\n";
		map<int,vector<vector<float> > >::iterator it_type = SIR_data.find(type_poultry_elem->getId());
		for(int i = 0; i < it_type->second.size(); i++)
		{
			myfile <<it_type->second[i][0]<<","<<it_type->second[i][1]<<","<<it_type->second[i][2]<<","<<it_type->second[i][3]<<","<<it_type->second[i][4]<<","<<it_type->second[i][5]<<",\n";
		}
		myfile.close();
	}
	myfile.open (header+"_Total.csv");
	myfile <<"Event,S,E,I,R,N,\n";
	it_type = SIR_data.find(0);
	for(int i = 0; i < it_type->second.size(); i++)
	{
		myfile <<it_type->second[i][0]<<","<<it_type->second[i][1]<<","<<it_type->second[i][2]<<","<<it_type->second[i][3]<<","<<it_type->second[i][4]<<","<<it_type->second[i][5]<<",\n";
	}
	myfile.close();
	cout << "Data saved\n";
	cout << "Time computing W: " << to_string(time_computing_W) <<"\n";
	cout << "Time computing Event: " << to_string(time_computing_event) <<"\n";
	cout << "Time updating Bands: " << to_string(time_updating_band) <<"\n";
	cout << "Time saving: " << to_string(time_saving) <<"\n";
}
//Execution of the model
int main()
{
	for(int i=0; i <parameters::NB_SIMULATIONS ; i++)
	{
		cout << "Doing simulation "<< to_string(i+1)<<"... \n";
		auto start = chrono::system_clock::now();

		model("./exploration/1.1_beta_min/inputs/band_type.csv","./exploration/1.1_beta_min/inputs/linked.csv","./exploration/1.1_beta_min/outputs/"+to_string(i));
		auto end = chrono::system_clock::now();
		auto elapsed = end-start;
		cout << chrono::duration<double, milli> (elapsed).count() << " ms" << "\n";
		reset_model();
	}
	return 0;
}
