#ifndef RUSHBUILDER_H
#define RUSHBUILDER_H
#include <stdlib.h>
#include <time.h>
#include "BLBuilder.h"
#include "Unit.h"
#include <chrono>

using namespace std;

class RushBuilder : public BLBuilder
{
    public:


        RushBuilder(string unit, int timeLimit) {

            techTree = reader.read_csv_file("unit_db.csv");
            targetUnit = findUnit(unit);
            race = (targetUnit->race).substr(0, 4);
            tTimeLimit = timeLimit;
            GetElligibleUnits(targetUnit);
             for(unsigned int i = 0; i < elligible_Units.size(); i++){
                if(elligible_Units[i]->vespene>0)
                {
                    if(race.compare("Terr") == 0){
                        string ves = "Refinery";
                        elligible_Units.push_back(findUnit(ves));
                        break;
                    }

                    if(race.compare("Prot") == 0){
                        string ves = "Assimilator";
                        elligible_Units.push_back(findUnit(ves));
                        break;
                    }
                }

            }
            elligible_Units.erase(elligible_Units.begin());
            
            print_buildlist();

        }

        void generate_buildlists(int n, double alp,int rangeNr){

            int iterations = n;
            double alpha = alp;
            int range = rangeNr*elligible_Units.size();


            vector<Unit*> local_buildList;
            // size of buildlist

            srand (time(NULL));

            for(int i = 0 ; i< iterations;++i){
                // random number of units
               
                int nr_Units = rand() % range + (int)range*alpha;
                for(int j = 0; j<nr_Units;j++){
                    //select a random unit

                    int unit_nr = rand() % elligible_Units.size();
                    
                    local_buildList.push_back(elligible_Units[unit_nr]);
                    

                }

                int d = rand()%(tTimeLimit/100) +1;
                
                for(int j = 0;j<d;j++){
                        
                   local_buildList.push_back(targetUnit);
                }
               


                buildLists.push_back(local_buildList);
                local_buildList.clear();

            }
//
        }

        void get_best_buildlist(){
            
            if( race.compare("Terr") == 0){
                    for(unsigned int i = 0; i<buildLists.size();i++){
                        TerranSimulator terr(buildLists[i],targetUnit->id);
                        pair<int,int> p;
                        p.first = terr.time_step;
                        p.second = terr.nr_targetUnit;
                        performance.push_back(p);
                    }
            }

            if(race.compare("Prot") == 0){
                    for(unsigned int i = 0; i<buildLists.size();i++){
                        ProtossSimulator pro(buildLists[i],targetUnit->id);
                        pair<int,int> p;
                        p.first = pro.time_step;
                        p.second = pro.nr_targetUnit;
                        performance.push_back(p);
                    }
            }

            int maxUnits = 0;
            int time =0;

            for(unsigned int i = 0; i<performance.size();++i){
                if(performance[i].first != tTimeLimit){
                    performance.erase(performance.begin()+i);
                    buildLists.erase(buildLists.begin()+i);
                    i--;
                }else if(performance[i].second==0)
                {
                    performance.erase(performance.begin()+i);
                    buildLists.erase(buildLists.begin()+i);
                    i--;
                }
                else if(performance[i].second > maxUnits){
                    time = performance[i].first;
                    maxUnits = performance[i].second;
                    index = i;


                }
            }

        }

        void print_buildlist(){
            std::chrono::time_point<std::chrono::system_clock> start, end;
	        start = std::chrono::system_clock::now( );

            int elapsed;

            while(index == -1){

                int ran = rand() % 8 + 2 ;
                generate_buildlists(1000,0.2,ran);
                check_dry_validity();
                get_best_buildlist();
                end = chrono::system_clock::now( );
                elapsed = chrono::duration_cast<std::chrono::seconds>
                             (end-start).count();

                if (elapsed>=180){
                    cout<<"no list"<<endl;
                    exit(EXIT_SUCCESS);
                }
            }
            if(race.compare("Terr") == 0){
                TerranSimulator terr(buildLists[index],targetUnit->id);
                terr.writer.file_writer.buildListValid = 1;
                terr.writer.file_writer.game = "Terr";
                terr.writer.print_mesages();
            }
            if(race.compare("Prot") == 0){
                ProtossSimulator prot(buildLists[index],targetUnit->id);
                prot.writer.file_writer.buildListValid = 1;
                prot.writer.file_writer.game = "Prot";
                prot.writer.print_mesages();
            }
        }

    protected:

    private:
};

#endif // RUSHBUILDER_H
