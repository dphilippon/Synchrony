# Synchrony
## Description
A SEIRS model to study influences of synchronization and coupling of poultry bands on avian influenza.
Currently, the model is stochastic, using Gillespie's algorithm to generate the exact time of one event occuring. The poultry population is represented by different poultry bands, each belonging to a type (Breeder or Broiler), interacting between each other through a SEIRS model. 

## Organization
The project is composed of different subfolders:

* src, which contains the different files in C++ of the model. 
* exploration, which is a folder composed of different subfolders (one per combination of parameters), each of them containing:
  * input, the folder containing two different files (one for the band type, one for the interaction between the band types)
  * output, the folder that will be used to save the generated files (one per band type and one for the total population)

## Inputs
The model uses two different files:
* *band_type.csv*, a CSV file of 16 columns containing one row per type of poultry band. The columns are the following:  
  * *id*, an integer representing the id of the poultry type,
  * *label*, a string representing the label of the poultry type, 
  * *initial_s*, an integer representing the initial number of susceptible chickens per poultry band of the poultry type, 
  * *initial_e*, an integer representing the initial number of exposed-latent chickens per poultry band of the poultry type, 
  * *initial_i*, an integer representing the initial number of infectious chickens per poultry band of the poultry type, 
  * *initial_r*, an integer representing the initial number of protected-recovered chickens per poultry band of the poultry type, 
   * *initial_n*, an integer representing the initial number of chickens per poultry band of the poultry type, 
   * *life_expectancy*, a float representing the life expectancy in days of a poultry band of the poultry type,  
   * *time_renewal*, a float representing the time in days between the deactivation and reactivation of a poultry band of the poultry type, 
   * *ksi*, a float representing the transition rate from protected-recovered to susceptible in days<sub>-1</sub> for the poultry bands of that poultry type, 
   * *gamma*, a float representing the transition rate from infectious to protected-recovered in days<sub>-1</sub> for the poultry bands of that poultry type, 
   * *sigma*, a float representing the transition rate from latent-exposed to infectious in days<sub>-1</sub> for the poultry bands of that poultry type, 
   * *nb_band*, an integer representing the number of poultry bands of that poultry type, 
   * *min_phi*, a float representing the minimum time in days before the earliest possible activation of a poultry band for that poultry type,
   * *max_phi*, a float representing the maximum time in days before the earliest possible activation of a poultry band for that poultry type,
   * *renewal*, a boolean to renew the bands (after the first activation), if true, with the initial values of S, E, I, R, N as given in the row, or with the chicken of the poultry bands of that poultry type being all susceptibles  
* *linked.csv*, a CSV file of 4 columns containing one row per interaction between poultry types. The columns are the following:  
  * *id_from*, an integer representing the id of the poultry type from which the interaction is set,
  * *id_to*, an integer representing the id of the poultry type to which the interaction is set,
  * *beta*, a float representing the sucessful rate for bands from the id_from poultry type to the id_to poultry type, 
  * *coupling*, a float representing the interaction for bands from the id_from poultry type to the id_to poultry type
## Outputs
The model generates different CSV files per simulation. One file is generated per type of poultry, and one file for the total population of chicken (independently from their type). Both files have the same structure, which is a matrix of N-Rows x 6 columns:
* *Event*, the time value for the event occurring,
* *S*, the number of susceptibles chicken from the activated poultry bands for the poultry population of the file, 
* *E*, the number of exposed-latent chicken from the activated poultry bands for the poultry population of the file, 
* *I*, the number of infectious chicken from the activated poultry bands for the poultry population of the file, 
* *R*, the number of protected-recovered chicken from the activated poultry bands for the poultry population of the file, 
* *N*, the number of chicken from the activated poultry bands for the poultry population of the file, 

## Behavior
The model follow the same process for every simulation: 
1. Initialization of the poultry types and the poultry bands
1. Initialization of the beta and interaction between poultry types
1. Execution of the simulation until the maximum time value is reached
    1. Computation of the number of bands activated (if there aren't band activated, go to v)
    1. Computation of the sum of probabilities of event from all the activated bands
    1. Computation of the time between the current time and the next event 
    1. Computation of the event
    1. Update of the band life cycle using the delta t (if there were no band activated, delta t is equals to the time until 
    the earliest band is activated/deactivated
    1. Storage of data
1. Save of data for each type of poultry and the whole population

## Equations


## About
The model is executed using GCC compiler on both Mac OS and Windows. 
The model was developed on the IDE Eclipse for C/C++ Developer, version of September 2019. 
