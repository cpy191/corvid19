/* class R0model implementation
 * Subclass of model for determining R0
 */


#include "epimodel.h"
#include "epimodelparameters.h"
#include "R0model.h"
#include "Simulator.h"
#include "Utility.h"
#include "debug.h"


using namespace std;


R0Model::R0Model(EpiModelParameters &params) : EpiModel(params) {
  nNumInfected=0;
  nRunLength=28;  // maximum run is 4 weeks
}

/*
 * infect
 * Set status of "p" to infected
 * Only index case should become infectious
 * Secondary cases are no longer susceptible, but they are not "infected".
 */
void R0Model::infect(Person& p) {
  EpiModel::infect(p);
  if (nNumInfected++==0) {
    nIndexCaseID = p.id; // index case
  } else {
    p.status = 0;        // not infected, not susceptible
  }
}

/*
 * Code to run at the end of init (and before run)
 */
void R0Model::prerun(void) {
  EpiModel::prerun();
  // infect one person
  mt19937 rng;
  long pid = rand_uniform_int(0,1000000000, &rng) % pvec.size();
  infect(pvec[pid]);
}

void R0Model::summary(void) {
  *sumfile << "R0 summary" << endl;
  *sumfile << "Population: " << szBaseName << endl;
  *sumfile << "beta: " << beta << endl;
  *sumfile << "Random number generator seed: " << seeddisp << endl;
  *sumfile << "Secondary infections: " << (nNumInfected-1) << endl;

  int familyages[5];
  familyages[0] = familyages[1] = familyages[2] = familyages[3] = familyages[4] = 0;
  for (vector< Person >::iterator pit = pvec.begin();
       pit != pvec.end();
       pit++) {
    Person &p = *pit;
    if (p.family==pvec[nIndexCaseID].family && p.id!=nIndexCaseID)
      familyages[p.age]++;
  }
  *sumfile << "Family member ages:";
  for (int i=0; i<5; i++)
    *sumfile << " " << familyages[i];
  *sumfile << endl;
    
  // output list of non-susceptible people (have been infected)
  for (vector< Person >::iterator pit = pvec.begin();
       pit != pvec.end();
       pit++) {
    Person &p = *pit;
    if (!isSusceptible(p))
      *sumfile << p << endl;
  }
}

void R0Model::run(void) {
  prerun();
  while(nTimer<nRunLength*2 &&
	(isInfected(pvec[nIndexCaseID]) || isInfectious(pvec[nIndexCaseID]))) {
    day();
    log();
    nTimer++;
    night();
    log();
    //    response();
    nTimer++;
  }
  summary();
  outputIndividuals();
}

int main(int argc, char *argv[]) {
  char *configname=NULL;
  if (argc>=2)
    configname = argv[1];
  EpiModelParameters parms(configname);
  R0Model model(parms);
  model.run();
  return 0;
}
