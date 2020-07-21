#ifndef UNIT_H
#define UNIT_H
#include <iostream>
#include <vector>
#include<string>
using std::string;
using std::vector;
class Unit {

    public :
        string id;
        string production_state;
        int minerals = 0;
        int vespene = 0;
        double supply = 0.0;
        double supply_cost = 0.0;
        int supply_provided = 0;
        int build_time = 0;
        int occupy_limit = 0;
        int units_produced = 0;
        vector<string> producer;
        vector<string> dependencies;
        vector<string> produces;
        int start_energy = 0;
        int max_energy = 0;
        bool structure;
        string race;
        Unit(){};


};
#endif // UNIT_H
