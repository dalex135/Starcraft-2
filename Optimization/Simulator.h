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
using std::size_t;
class Simulator
{

    private:
        const int MINERAL_HARVEST_RATE = 70;
        const int VESPENE_HARVEST_RATE = 63;
        const double ENERGY_GEN_RATE = 0.5625;
        const int CHRONOBOOST_ENERGY_COST = 50;
        const int CHRONOBOOST_DURATION = 20;
        const int MULE_ENERGY_COST = 50;
        const int MULE_DURATION = 64;
        const double MULE_SPEEDUP = 3.8;

    public:
        int minerals = 5000;
        int gas = 0;
        int workers = 12;
        int base = 1;
        int rate;
        int minerals_workers = 12;
        int gas_workers = 0;
        int supply_now = 12;
        int supply_max = 15;
        int time_step = 1;
        int vespene_geysers = 0;
        vector<Unit*> toDo;
        vector<Struct> active;
        vector<Struct> finished;
        vector<Unit> techTree;
        Reader reader;
        Writer writer;
        vector<pair<string,string>> events;
        int nr_targetUnit = 0;
        string targetUnit;

        //bool timed_out = false;


        Simulator(/* args */){};
        ~Simulator(){};

        void update_resources(){

            minerals += minerals_workers*MINERAL_HARVEST_RATE;
            gas += gas_workers*VESPENE_HARVEST_RATE;

            for(size_t j=0;j<this->active.size();++j){

                --active[j].remain_time;

            }

            for(size_t j=0;j<this->finished.size();++j){
                if(finished[j].energy<finished[j].unit->max_energy-ENERGY_GEN_RATE)
                    finished[j].energy += ENERGY_GEN_RATE;
            }

        }

        void check_start_events(){
            //cout<<"check_start_events()"<<endl;
            if(!toDo.empty() && check_dependencies(toDo[0]) && check_producer(toDo[0])){

                //Unit* producer;

                if(!(toDo[0]->structure))
                    supply_now += toDo[0]->supply_cost;

                minerals -= toDo[0]->minerals;
                gas -= toDo[0]->vespene;


                Struct start_build(toDo[0]);
                active.push_back(start_build);
                //cout<<"active: "<<active[0].unit->id;
                pair <string,string> start_event("build-start",toDo[0]->id);



               // use_producer(toDo[0]);
                events.push_back(start_event);
                //cout<<events[0].first<<" "<<events[0].second<<endl;
                //if(!toDo.empty())
               //cout<<time_step<<" - build-start"<<" : "<<toDo[0]->id<<" Resources: "<<minerals<<"|"<<gas<<" Workers: "
               //<<minerals_workers<<"|"<<gas_workers<<" Supply: "<<supply_now<<"|"<<supply_max<<endl;
                //write_message();
                toDo.erase(toDo.begin());

                //cout<<toDo.size()<<endl;
            }
        }

        void check_end_events(){
            //cout<<"check_end_events()"<<endl;


            for(size_t i = 0; i<active.size();++i){

                //if((active[i].unit->id).compare("Barracks")==0){
                  //  cout<<time_step<<" :"<<active[i].unit->id<<" "<<active[i].remain_time<<endl;
                //}

                if(active[i].remain_time == 0){


                    finished.push_back(active[i]);
                    pair <string,string> end_event("build-end",active[i].unit->id);
                    //target unit
                    if((active[i].unit->id).compare(targetUnit)==0)
                    nr_targetUnit++;

                    if((active[i].unit->id).compare("Refinery")==0||(active[i].unit->id).compare("Extractor")==0||(active[i].unit->id).compare("Assimilator")==0)
                    ++vespene_geysers;

                    if((active[i].unit->id).compare("Nexus")==0||(active[i].unit->id).compare("CommandCenter")==0||(active[i].unit->id).compare("Hatchery")==0)
                    ++base;

                    if( ((active[i].unit->id).compare("SCV") == 0) || ((active[i].unit->id).compare("Probe") == 0)
                            || ((active[i].unit->id).compare("Drone") == 0)){
                                ++workers;
                                ++minerals_workers;
                    }

                    if(active[i].unit->structure && !(findUnit( (active[i].unit->producer)[0] )-> structure)){  //check whether structure and whether it is a upgrade
                    //cout<<"Supply Provided"<<endl;
                        supply_max += active[i].unit->supply_provided;
                    }

                    deuse_producer(active[i]);
                    //deoccupy_structure(active[i]);
                    events.push_back(end_event);

                    //cout<<time_step<<" - build-end"<<" : "<<active[i].unit->id<<" Resources: "<<minerals<<"|"<<gas<<" Workers: "
                    //    <<minerals_workers<<"|"<<gas_workers<<" Supply: "<<supply_now<<"|"<<supply_max<<endl;
                    //write_message();
                    if(!active.empty()){
                        active.erase(active.begin()+i);
                        --i;
                    }


                }
            }
        }

        bool use_producer(Unit* u){
            //cout<<u->id<<" "<<u->production_state<<endl;
            if((u->production_state).compare("producer_occupied")==0){
                for(size_t i = 0; i<finished.size(); i++){
                    //cout<<"1"<<endl;
                    for(size_t j = 0; j<u ->producer.size(); j++){
                        //cout<<"2"<< u->producer.size()<<endl;
                        if( ((finished[i].unit->id).compare((u->producer)[j]) == 0) && finished[i].occupied == false){
                            //cout<<finished[i].unit->id<<":"<<u->id<<endl;
                            if( ((finished[i].unit->id).compare("SCV") == 0) || ((finished[i].unit->id).compare("Probe") == 0)
                            || ((finished[i].unit->id).compare("Drone") == 0)){
                                --workers;
                                --minerals_workers;
                            }
                            finished[i].occupied = true;
                            return true;
                        }

                    }
                }
            }else if((u->production_state).compare("producer_consumed_at_start")==0){
                for(size_t i = 0; i<finished.size(); i++){
                    //cout<<"1"<<endl;
                    for(size_t j = 0; j<u ->producer.size(); j++){
                        //cout<<"2"<< u->producer.size()<<endl;
                        if( ((finished[i].unit->id).compare((u->producer)[j]) == 0) && finished[i].occupied == false){
                            //cout<<finished[i].unit->id<<" "<<u->id<<endl;
                            if( ((finished[i].unit->id).compare("SCV") == 0) || ((finished[i].unit->id).compare("Probe") == 0)
                            || ((finished[i].unit->id).compare("Drone") == 0)){
                                --workers;
                                --minerals_workers;
                            }
                            finished.erase(finished.begin()+i);
                            --i;

                            return true;
                        }

                    }
                }
            }else if((u->production_state).compare("producer_consumed_at_end")==0){
                for(size_t i = 0; i<finished.size(); i++){
                    for(size_t j = 0; j<u ->producer.size(); j++){
                        if( ((finished[i].unit->id).compare((u->producer)[j]) == 0) && finished[i].occupied == false){
                            finished[i].occupied = true;
                        }
                    }
                }
            }
            return false;

        }

        bool deuse_producer(Struct s){

            if((s.unit->production_state).compare("producer_occupied")==0){
                for(size_t i = 0; i<finished.size(); i++){
                    //cout<<"1"<<endl;
                    for(size_t j = 0; j<s.unit ->producer.size(); j++){
                        //cout<<"2"<< u->producer.size()<<endl;
                        if( ((finished[i].unit->id).compare((s.unit->producer)[j]) == 0) && finished[i].occupied == true){
                            //cout<<(s.unit->producer)[j]<<": deoccupied"<<endl;
                            if( ((finished[i].unit->id).compare("SCV") == 0) || ((finished[i].unit->id).compare("Probe") == 0)
                            || ((finished[i].unit->id).compare("Drone") == 0)){
                                ++workers;
                                ++minerals_workers;
                            }
                            finished[i].occupied = false;
                            return true;
                        }

                    }
                }
            }

            if((s.unit->production_state).compare("producer_consumed_at_end")==0){
                for(size_t i = 0; i<finished.size(); i++){
                    //cout<<"1"<<endl;
                    for(size_t j = 0; j<s.unit ->producer.size(); j++){
                        //cout<<"2"<< u->producer.size()<<endl;
                        if( ((finished[i].unit->id).compare((s.unit->producer)[j]) == 0) && finished[i].occupied == true){
                            //cout<<"3"<<endl;
                            if( ((finished[i].unit->id).compare("SCV") == 0) && ((finished[i].unit->id).compare("Probe") == 0)
                            && ((finished[i].unit->id).compare("Drone") == 0)){
                                --workers;
                                --minerals_workers;
                            }
                            finished.erase(finished.begin()+i);
                            --i;
                            return true;
                        }

                    }
                }
            }
            return false;

        }

        void redistribute_workers(){
            int funtional_vespene_geysers;                                  //Functional geysers for number of bases
            if(base*2<vespene_geysers)                                      //Only 2 functional geysers per base
                funtional_vespene_geysers = base*2;
            else
                funtional_vespene_geysers = vespene_geysers;

            int half_workers = workers/2;
            if(funtional_vespene_geysers*3>half_workers){
                minerals_workers = half_workers;                            //Assigning half of the workers to geysers
                gas_workers = workers - half_workers;                       //Assigning the rest for minerals
            }
            else{
                gas_workers = funtional_vespene_geysers*3;                  //Assigning as many as possible workers for geysers
                minerals_workers = workers - funtional_vespene_geysers*3;   //Assigning the rest for minerals
            }
        }

        bool check_dependencies(Unit* u){
            //cout<<"check_dependencies(Unit* u)"<<endl;

            if(minerals<u->minerals || gas<u->vespene){
                //cout<<"Not enough mineral"<<"minerals: "<<minerals<<" : "<<u->minerals << endl << "gas: "<<gas<<" : "<<u->vespene<<endl<<"supply: "<<(supply_max-supply_now)<<" : "<<u->supply_cost<<endl<<endl;
                return false;
            }

            if(!(u->structure)&&(supply_max-supply_now)<u->supply_cost){                            //Check whether remaining supply is enough if it is a unit
                //cout<<"Supply not enough!"<<"supply: "<<(supply_max-supply_now)<<" : "<<u->supply_cost<<endl<<endl;
                //cout<<"supply: "<<(supply_max-supply_now)<<" : "<<u->supply_cost<<endl<<endl;
                return false;
            }

            if((u-> dependencies).empty()){
                //cout<<"Throw"<<endl;
                return true;
            }else{
                bool depend_flag = false;
                for(size_t i=0;i<(u -> dependencies).size();++i ){
                    //depend_flag = false;
                    for(size_t j=0;j<this->finished.size();++j){
                        //cout<<u->dependencies[i]<< " "<<finished[j].unit->id<<endl;
                        if(  ( (u-> dependencies)[i] ).compare(  finished[j].unit->id )  == 0 ){
                            //cout<<(u->dependencies)[i]<< " "<<finished[j].unit->id<<endl;
                            depend_flag = true;
                        }
                    }
                    if(!depend_flag ){

                        return false;
                    }
                }
                //cout<<"true in dependencies"<<endl;
                return true;
            }
        }

        bool check_producer(Unit* u){
            //cout<<"check_producer(Unit* u)"<<endl;
            for(size_t i=0;i<(u -> producer).size();++i ){
                for(size_t j=0;j<finished.size();++j){

                    if( ( ( (u-> producer)[i] ).compare(  finished[j].unit->id )  == 0 ) && (finished[j].occupied==false) ){
                            //cout<<"Producer available:  "<<u->id<< " "<<finished[j].unit->id<<endl;
                            use_producer(u);
                            return true;
                        }
                }
            }
            //cout<<time_step<<"Producer not available: "<<u->id<<" "<<(u->producer)[0]<<endl;
            return false;
        }

        void run(){

           // cout<<"Game Start"<<endl;


            // build list valid varriable???
            writer.file_writer.buildListValid = 1;

            while(time_step<=1000 && !(toDo.empty()&&active.empty()) )
            {

                update_resources();
                check_end_events();
                check_start_events();
                //cout<<check_dependencies(toDo[0])<<" "<<toDo[0]->id<<endl;
                redistribute_workers();
                if(!events.empty())
                    write_message();
                //write
                events.clear();
                ++time_step;
                //cout<<toDo.size()<<endl;

            }


            if(time_step>10000){
                //cout << "time out";
                writer.timed_out = true;
                writer.file_writer.buildListValid = 1;
            }
        }

        Unit* findUnit(string unitName){

            //cout<<(techTree[1].id);
            for(size_t i = 0; i< techTree.size();i++){
                if((techTree[i].id).compare(unitName)==0){
                        //cout<<(techTree[i].id);
                        return &techTree[i];

                }
            }
            return nullptr;
        }

        void write_message(){

                writer.w.mineral_workers = minerals_workers;
                writer.w.vespene_workers = gas_workers;

                //writer.resources r;
                writer.r.minerals = minerals/100;
                writer.r.vespene = gas/100;
                writer.r.supply_used = supply_now;
                writer.r.supply = supply_max;

                //writer.status s;
                writer.s.w = writer.w;
                writer.s.r = writer.r;

                //writer.message m;
                writer.m.events = events;
                writer.m.s = writer.s;
                writer.m.time_step = time_step;

                writer.file_writer.me.push_back(writer.m);
        }
        int get_elapsed_time(){
            return time_step;
        }
};
#endif // SIMULATOR_H
