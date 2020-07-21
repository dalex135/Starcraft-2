#ifndef BLBUILDER_H
#define BLBUILDER_H
#include "Simulator.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "Unit.h"
#include "Struct.h"
#include "Reader.h"
#include "Writer.h"
#include "TerranSimulator.h"
#include "ProtossSimulator.h"
#include <chrono>

using std::vector;
using std::size_t;

class BLBuilder
{
    public:

        int nrTUnits = 1;
        int tTimeLimit;
        int targetUnitCreated = 0;
        vector<Unit> techTree;
        Unit* targetUnit;
        Reader reader;
        vector<Unit*> elligible_Units;
        vector<vector<Unit*>> buildLists;
        int supply_now = 12;
        int supply_max = 15;
        vector<pair<int,int>> performance;
        string race;
        int index = -1;

        BLBuilder() {
        //read tech tree

        }

        ~BLBuilder() {}
        void GetElligibleUnits(Unit* u){
            //cout<<"Runs";
            if(!elligible_Units.empty()){
                for(unsigned int i = 0; i < elligible_Units.size();i++){
                    if((elligible_Units[i]->id).compare(u->id) == 0)
                        return;
                }
            }
            elligible_Units.push_back(u);
            //cout<<" "<<u->producer[0];


            if(!(u->producer.empty()))
               GetElligibleUnits(findUnit(u->producer[0]));
            if(!(u->dependencies.empty()))
               GetElligibleUnits(findUnit(u->dependencies[0]));

        }


        void check_dry_validity(){
            // check th race of the unit

            //cout<<"This"<<(targetUnit->race).substr(0, 3)<<"::"<<endl;

            if(race.compare("Terr") == 0){
                for(unsigned int i = 0; i< buildLists.size();++i){
                    if(!check_terran_Validity(buildLists[i])){
                        buildLists.erase(buildLists.begin()+i);
                        //cout<<"Runs"<<endl;
                        i--;
                    }
                }
                //cout<<buildLists.size()<<endl;
            }
            else if(race.compare("Prot") == 0){
                for(unsigned int i = 0; i< buildLists.size();++i){
                    if(!check_protoss_Validity(buildLists[i])){
                        buildLists.erase(buildLists.begin()+i);
                        i--;
                    }
                }
            }
            else {
                cout<< "invalid race !"<< endl;
                exit(EXIT_SUCCESS);
            }

        }

        Unit* findUnit(string &unitName){

            //cout<<(techTree[1].id);
            for(size_t i = 0; i< techTree.size();i++){
                if((techTree[i].id).compare(unitName)==0){
                        //cout<<(techTree[i]);
                        return &techTree[i];

                }
            }
            return nullptr;
        }

        bool check_terran_Validity(vector<Unit*> buildlist){
            vector<string> virtual_finished_list ;
            //virtually initializing the finished list with Nexus and Probes
            virtual_finished_list.push_back("CommandCenter");
            for(int i=0; i<12; ++i){
                virtual_finished_list.push_back("SCV");
            }
            //virtually initializing the supply-variables
            int virtual_supply_now = supply_now;
            int virtual_supply_max = supply_max;
            int virtual_nr_refineries = 0;
            int virtual_nr_commandCenter = 1;
            //checking validity of todo list
            for (auto & todo : buildlist){
                bool check_dependencies = false;
                bool check_producer = false;
                bool check_vespene = false;
                //checking supply
                bool check_supply = (*todo).supply_cost + virtual_supply_now <= virtual_supply_max;
                if(!check_supply){

                    return false;

                }
                //checking vespene: checking if a Refinery is available
                for(auto &iterator : virtual_finished_list){
                    if((*todo).vespene == 0 || iterator == "Refinery"){
                        check_vespene = true;
                        break;
                    }
                }
                if(!check_vespene){

                    return false;
                }

                //checking the producer and dependency: wheather they are available in the virtual finished list
                for(auto &iterator :virtual_finished_list){
                    if((*todo).dependencies.size() == 0 || (*todo).dependencies[0] == iterator ){
                        check_dependencies = true;
                    }
                    if((*todo).producer.size()== 0 || (*todo).producer[0] == iterator){
                        check_producer = true;
                    }
                    if(check_dependencies && check_producer){
                        break;
                    }
                }
                if(!check_dependencies){

                    return false;
                }
                if(!check_producer){

                    return false;
                }
                //2 refineries pro nexus allowed
                if((*todo).id == "CommandCenter"){
                    virtual_nr_commandCenter ++;
                }
                if((*todo).id == "Refinery"){
                    //compare the number of refineries and comman centers
                    if(virtual_nr_refineries >= 2 ){ //* virtual_nr_commandCenter

                        return false;
                    }else{
                        virtual_nr_refineries ++;
                    }
                }

                //virtual build
                virtual_finished_list.push_back((*todo).id);
                virtual_supply_now += (*todo).supply_cost;
                virtual_supply_max += (*todo).supply_provided;
            }

            return true;
    }

        bool check_protoss_Validity(vector<Unit*> buildList){
            vector<string> virtual_finished_list ;
            //virtually initializing the finished list with Nexus and Probes
            virtual_finished_list.push_back("Nexus");
            for(int i=0; i<12; ++i){
                virtual_finished_list.push_back("Probe");
            }
            //virtually initializing the supply-variables
            int virtual_supply_now = supply_now;
            int virtual_supply_max = supply_max;
            int virtual_nr_assimilators = 0;
            int virtual_nr_nexus = 1;
            //checking validity of todo list
            for (auto & todo : buildList){
                bool check_dependencies = false;
                bool check_producer = false;
                bool check_vespene = false;
                //checking supply
                bool check_supply = ((*todo).supply_cost + virtual_supply_now <= virtual_supply_max);
                if(!check_supply){
                   return false;
                }
                //checking vespene: checking if an Assimilator is available
                for(auto &iterator : virtual_finished_list){
                    if((*todo).vespene == 0 || iterator == "Assimilator"){
                        check_vespene = true;
                        break;
                    }
                }

                if(!check_vespene){
                    return false;
                }

                //checking the producer and dependency: wheather they are available in the virtual finished list
                for(auto &iterator :virtual_finished_list){
                    if((*todo).dependencies.size() == 0 || (*todo).dependencies[0] == iterator ){
                        check_dependencies = true;
                    }
                    if((*todo).producer.size()== 0 || (*todo).producer[0] == iterator){
                        check_producer = true;
                    }
                    if(check_dependencies && check_producer){
                        break;
                    }
                }
                if(!check_dependencies){
                    //cout<<"check dependencies"<<endl;
                    return false;
                }
                if(!check_producer){
                    //cout<<"check producer"<<endl;
                   return false;
                }
                //2 assimilators pro nexus allowed
                if((*todo).id == "Nexus"){
                    virtual_nr_nexus ++;
                }
                if((*todo).id == "Assimilator"){

                    if(virtual_nr_assimilators >= 2 ){
                        return false;
                    }else{
                            virtual_nr_assimilators ++;
                    }
                }

                //virtual build
                virtual_finished_list.push_back((*todo).id);
                virtual_supply_now += (*todo).supply_cost;
                virtual_supply_max += (*todo).supply_provided;
            }
         return true;
    }

    protected:

    private:
};

#endif // BLBUILDER_H
