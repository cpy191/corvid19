#ifndef PERCOL_SIMULATOR_H
#define PERCOL_SIMULATOR_H

#include "Simulator.h"

class Percolation_Sim: public Simulator
{
    public:
        vector<Node*> exposed;
        vector<Node*> infected;
        vector<Node*> otherinfected;
        vector < int > nExposedPerDay;
        int counter = 0;

    
        typedef enum {           //Whatever is equal to zero is the default state
            S=0, E=1, I=-1
        } stateType;
        float T;                 // transmissibiltiy, e.g. P{infection spreading along a given edge that connects an infected and a susceptible}

        int nInfected; 
        Percolation_Sim():Simulator() {};
        Percolation_Sim(Network* net):Simulator(net) {};
        ~Percolation_Sim() { };

        void set_transmissibility(double t) { this->T = t; }

        double expected_R0 () {
            //assert(T != NULL);
            return T / calc_critical_transmissibility();
        }

        vector<Node*> rand_infect (int n) {
            assert(n > 0);
            vector<Node*> patients_zero = rand_set_nodes_to_state(n, E);
            for (int i = 0; i < n; i++) {
                exposed.push_back(patients_zero[i]);
            };
            cout << "size (rand infect)" << exposed.size() << endl;
            return patients_zero;
        }


        void step_simulation () {
            if (exposed.size() == 0) {
                time++;
                nExposedPerDay.push_back(0);
                return;}
            //assert(exposed.size() > 0);
            time++;
            counter = exposed.size() - counter;
            //cerr << "\t" << infected.size() << endl;
            //cout << "counter" << counter << endl;
            vector<Node*> new_exposed;
            for (unsigned int i = 0; i < exposed.size(); i++) {
                Node* inode = exposed[i];
                vector<Node*> neighbors = inode->get_neighbors();
                for (unsigned int j = 0; j < neighbors.size(); j++) {
                    Node* test = neighbors[j];
                    if ( test->get_state() == S && rand_uniform(0, 1, rng) < T ) {
                        test->set_state( E );
                        new_exposed.push_back( test );
                        exposed.push_back(test);
                    }
                }
                //inode->set_state( I );
                //infected.push_back( inode );
            }
            //exposed = new_exposed;
            //cout << "new commute exposed" << new_exposed.size() << endl;
            nExposedPerDay.push_back(new_exposed.size());
            if(nExposedPerDay.size()==1){nExposedPerDay[0] = nExposedPerDay[0] + 5;}
            if(time > 3){//if(nExposedPerDay.size() > 3){
                //cout << "nExposedPerDay[time-4]" << nExposedPerDay[time-4] << endl;
                if (nExposedPerDay[time-4] + counter > 0) {
                    for (int i = 0; i < nExposedPerDay[time-4]+counter; i++) {
                    Node* inode = exposed[i];
                    inode->set_state( I );
                    infected.push_back( inode );
                }
                    exposed.erase(exposed.begin(),exposed.begin()+nExposedPerDay[time-4]+counter);
                }
            }
            counter = exposed.size();

        }

        void set_suscept(int n){
            for (int i = 0; i < n; i++) {
                Node* inode = otherinfected[i];
                inode->set_state( S );
            };
            otherinfected.erase(otherinfected.begin(),otherinfected.begin()+n);   
        }

        vector<Node*> set_infected(int n){
            vector<Node*> patients_zero = rand_set_nodes_to_state(n, I);
            for (int i = 0; i < n; i++) {
                otherinfected.push_back(patients_zero[i]);
            };
            return patients_zero;
        }

        void run_simulation() {
            while (exposed.size() > 0) {
                step_simulation();
            }
        }

        int count_exposed() {
            return exposed.size();
        }

        const vector<Node*> get_exposed_nodes() {
            return exposed;
        }

        int epidemic_size() {
            return infected.size();
        }

        void reset() {
            reset_time();

            set_these_nodes_to_state(exposed, S);
            exposed.clear();

            set_these_nodes_to_state(infected, S);
            infected.clear();
        }

        void summary() {
            cerr << "Network size: " << net->size();
            cerr << "\tTransmissibility: " << T;
            cerr << "\tEpidemic size: " << infected.size() << "\n\n";

        }
};
#endif
