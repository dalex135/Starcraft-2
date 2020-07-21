#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <iostream>
#include <vector>
#include <fstream>
#include "Unit.h"
#include "Struct.h"
#include "Reader.h"
#include "Writer.h"
using std::vector;
class Simulator
{

    private:
        const int MINERAL_HARVEST_RATE = 0.7;
        const int VESPENE_HARVEST_RATE = 0.63;
        const int ENERGY_GEN_RATE = 0.5625;

    public:
        int minerals;
        int gas;
        int rate;
        int minerals_worker;
        int gas_worker;
        int supply_now;
        int supply_max;
        vector<Unit*> toDo;
        vector<Struct> active;
        vector<Struct> finished;
        vector<Unit> techTree;
        Reader reader;
        Writer writer;
        int time_step =0;

        vector<pair<string,string>> events;

        Simulator(/* args */){};
        ~Simulator(){};

        void update_resources(){
            minerals += minerals_worker*MINERAL_HARVEST_RATE;
            gas += gas_worker*VESPENE_HARVEST_RATE;

            for(int j=0;j<this->active.size();j++){
                active[j].remain_time--;
            }

            for(int j=0;j<this->finished.size();j++){
                finished[j].energy += ENERGY_GEN_RATE;
            }
        }

        void check_start_events(){
            if(check_dependencies(toDo[0])){
                Struct start_build(toDo[0]);
                active.push_back(start_build);
                pair <string,string> start_event("build-start",toDo[0]->id);
                events.push_back(start_event);
                toDo.erase(toDo.begin());
            }
        }

        void check_end_events(){
            for(int i = 0; i<active.size();i++){
                if(active[i].remain_time == 0){
                    finished.push_back(active[i]);
                    pair <string,string> end_event("build-end",active[i].unit->id);
                    events.push_back(end_event);
                    active.erase(active.begin()+i);
                }
            }
        }

        void redistribute_workers(){

        }

        bool check_dependencies(Unit* u){
            if(minerals<u->minerals || gas<u->vespene || (supply_max-supply_now)<u->supply_cost)
                return false;
            if((u-> dependencies).empty()){
                return true;
            }else{
                bool depend_flag = false;
                for(int i=0;i<(u -> dependencies).size();i++ ){
                    for(int j=0;j<this->finished.size();j++){
                        if(  ( (u-> dependencies)[i] ).compare(  this->finished[j].unit->id )  == 0 ){
                            depend_flag = true;
                        }
                    }
                    if(!depend_flag )
                        return false;
                }
                return true;
            }
        }

        void run(){
            while(time_step<1000)
            {
                update_resources();
                check_end_events();
                check_start_events();

                redistribute_workers();
                time_step++;
            }
        }
};
#endif // SIMULATOR_H
