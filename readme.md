# Instruction for COVID19

Code for **A multi-layer network model for studying the impact of non-pharmaceutical interventions implemented in response to COVID-19**. 


## Compilation

The Makefile can be used on Linux systems to compile corvid19. The source files are:

  epimodel.cpp - model implementation
  corvid.cpp - contains main()
  params.cpp - simulation constants
  epimodelparameters.cpp - code to parse config files
  Network.cpp - basic function of the commute network.  
  Utility.cpp - commute network generation. 

## Data files

A set of 3 data files are used to specify a population for Corvid. The file names consist of a prefix (i.e., "Beijing", "Guangzhou") followed by a suffix.

*-tracts.dat - Tract populations and locations. 

*-wf.dat - Tract-to-tract workerflow.The columns in this space-delimited file are: home city code, home city code, home tract code, work city code, work city code, work tract code, and number of workers.

*-employment.dat - The number of employed working-age adults and the  total number of working-age adults. The columns in this space-delimited file are:  city code, city code, tract code, number of employed individuals, and the total number of working-age individuals(19-64 year olds).

## Configuration
A text file with one parameter per line is supplied as the command-line argument for Corvid to configure a simulation run (e.g., "`./corvid config-file`"， “`sh run-Beijing.sh`").

Non-pharmaceutical intervention related parameters:

*vaccinationfraction : proportion of citizens vaccinated
*vaccinedata : including the vaccine id, vaccine efficacy and administration policies for each vaccine.
*communitycontactreduction : proportion of communities carrying out close-off management
*communitycontactreductiondays : duration of close-off management
*PTRestriction : degree of public transportation restriction (between 0 and 1)
*PTRestrictiondays : duration of public transportation restriction 
*mask : proportion of citizens wearing mask
*maskdays : duration of wearing mask
*schoolclosurepolicy : types of school closure policy (by tracks/all)
*schoolclosuredays : duration of school closure 
*workfromhome : proportion of workers work from home
*workfromhomedays : duration of work from home

## running corvid19

We provide two methods for running.
1.   `./corvid config-` , where config is the name of configuration file
 2.  `sh run-Beijing.sh`, run multiple times with varying parameters
      
## Output files

Summary- outputs the settings used for the simulation, final attack rates, vaccines used, daily totals of symptomatic individuals.
Log - The number of symptomatics and infected people per tract each day (prevalence and cumulative). 
Tracts - a unique numeric tract id and the 
populations of the tracts by age. Also contains the number of people  who work in this tract (including residents). 
