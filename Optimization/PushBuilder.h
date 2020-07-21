#ifndef PUSHBUILDER_H
#define PUSHBUILDER_H
#include "BLBuilder.h"
#include <chrono>
using namespace std;

class PushBuilder : public BLBuilder
{
    public:

        PushBuilder(string unit,int nr_units) {
            techTree = reader.read_csv_file("unit_db.csv");
            targetUnit = findUnit(unit);

            race = (targetUnit->race).substr(0, 4);
            //cout<<targetUnit->id;
            //cout<<"This"<<r<<"::"<<endl;
            nrTUnits = nr_units;
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

            //cout<<elligible_Units.size()<<endl;

           // for(unsigned int i = 0;i<elligible_Units.size();i++){
             //  cout<<"Ell: "<<elligible_Units[i]->id<<endl;}
            //generate_buildlists(5000,0.5,5);

            /*for(int j = 0; j< buildLists.size();j++){
                cout<<"***********************************"<<j<<endl;
                for(int i = 0; i< buildLists[j].size(); i++)
                {
                    cout<<buildLists[j][i]->id<<endl;
                }
            }*/
            // check_dry_validity();
            //cout<<buildLists.size()<<endl;
            //cout<<buildLists.size()<<endl;
            // get_best_buildlist();
            print_buildlist();
        }

        /*void generate_buildlists(){
            vector<vector<Unit*>> buildList_Stack;
            //vector<vector<Unit*>> buildList_TargetUnit_Stack;
            if( race.compare("Terr") == 0){
                for(unsigned int i = 0; i < elligible_Units.size(); i++){
                    if((elligible_Units[i]->producer[0]).compare("CommandCenter")==0 || (elligible_Units[i]->producer[0]).compare("SCV")==0){
                        vector<Unit*> initial_List;
                        initial_List.push_back(elligible_Units[i]);
                        buildList_Stack.push_back(initial_List);
                    }
                }
                for(unsigned int i=0; i< elligible_Units.size();i++){
                    if((elligible_Units[i]->id).compare("CommandCenter")==0){
                        elligible_Units.erase(elligible_Units.begin()+i);
                    }
                }
            }
            if( race.compare("Prot") == 0){
                for(unsigned int i = 0; i < elligible_Units.size(); i++){
                    if((elligible_Units[i]->producer[0]).compare("Nexus")==0 || (elligible_Units[i]->producer[0]).compare("Probe")==0){
                        vector<Unit*> initial_List;
                        //cout<<"Initial Unit: "<<elligible_Units[i]->id<<endl;
                        initial_List.push_back(elligible_Units[i]);
                        buildList_Stack.push_back(initial_List);
                    }
                }
                for(unsigned int j=0; j< elligible_Units.size();j++){
                    if((elligible_Units[j]->id).compare("Nexus")==0){
                        elligible_Units.erase(elligible_Units.begin()+j);
                    }
                }
            }
            //cout<<buildList_Stack.size()<<endl;
            //while(!buildList_Stack.empty())
            int j = 0;
            while(!buildList_Stack.empty()&&buildLists.size()<10000)
            //while(j<10000)
            {
                //cout<<"Build List Stack: "<<buildList_Stack.size()<<endl;
                vector<Unit*> firstList = buildList_Stack[0];

                buildList_Stack.erase(buildList_Stack.begin());
                //cout<<"KKKKKKKKKKK"<<firstList[firstList.size()-1]->id<<endl;
                if((firstList[firstList.size()-1]->id).compare(targetUnit->id)==0){
                    for(unsigned int i = 0; i < nrTUnits-1;i++){
                        firstList.push_back(targetUnit);
                        //cout<<"BuildLists: "<<buildLists.size()<<endl;
                    }

                    buildLists.push_back(firstList);
                    cout<<"BuildLists: "<<buildLists.size()<<endl;

                }
                else{
                    for(unsigned int i = 0; i < elligible_Units.size(); i++){
                        if((elligible_Units[i]->producer[0]).compare(firstList[firstList.size()-1]->id)==0 ||(!(elligible_Units[i]->dependencies).empty()&&(elligible_Units[i]->dependencies[0]).compare(firstList[firstList.size()-1]->id)==0 )||
                        (firstList[firstList.size()-1]->id).compare("Assimilator")==0 ||(firstList[firstList.size()-1]->id).compare("Refinery")==0 ){

                            vector<Unit*> secondList = firstList;
                            secondList.push_back(elligible_Units[i]);
                            buildList_Stack.push_back(secondList);
                            //cout<<firstList[firstList.size()-1]->id<<" : "<<secondList[secondList.size()-1]->id<<endl;
                        }
                    }
                }
                j++;
            }

            //cout<<"BuildLists: "<<buildLists.size()<<endl;


        }*/
        void generate_buildlists(int n, double alp,int rangeNr){

            int iterations = n;
            double alpha = alp;
            int range = rangeNr*elligible_Units.size();


            vector<Unit*> local_buildList;
            // size of buildlist

            srand (time(NULL));

            //for(int j = 0;j<nrTUnits;j++){

            //        elligible_Units.push_back(targetUnit);
            //}



            for(int i = 0 ; i< iterations;++i){
                // random number of units
                //cout<<"BuildList Nr : "<<i<<endl;
               // targetUnitCreated = 0;
                int nr_Units = rand() % range + (int)range*alpha;
                for(int j = 0; j<nr_Units;j++){
                    //select a random unit

                    int unit_nr = rand() % elligible_Units.size();
                    //cout<<elligible_Units[unit_nr]->id<<endl;
                    local_buildList.push_back(elligible_Units[unit_nr]);
                   /* if((elligible_Units[unit_nr]->id).compare(targetUnit->id)==0){
                        targetUnitCreated++;
                    }*/

                }

               // int d = nrTUnits-targetUnitCreated;
                //cout<<"Runs"<<d<<endl;
               // if(d>0){
                    for(int j = 0;j<nrTUnits;j++){
                            //cout<<"Runs"<<targetUnitCreated<<endl;
                        local_buildList.push_back(targetUnit);
                    }
               // }

                buildLists.push_back(local_buildList);
                local_buildList.clear();

            }

        }
        void get_best_buildlist(){


            //cout<<"Runs"<<endl;
            //string r = (targetUnit->race).substr(0, 3);
            //for(int)
            if( race.compare("Terr") == 0){
                    //cout<<"Runs";

                    for(unsigned int i = 0; i<buildLists.size();i++){
                        TerranSimulator terr(buildLists[i],targetUnit->id);
                        pair<int,int> p;
                        p.first = terr.time_step;
                        p.second = terr.nr_targetUnit;
                        //cout<<p.first <<":"<<p.second <<endl;
                        performance.push_back(p);
                    }
            }


            if(race.compare("Prot") == 0){
                    for(unsigned int i = 0; i<buildLists.size();i++){
                        ProtossSimulator pro(buildLists[i],targetUnit->id);
                        pair<int,int> p;
                        p.first = pro.time_step;
                        p.second = pro.nr_targetUnit;
                        //cout<<p.first <<":"<<p.second <<endl;
                        performance.push_back(p);
                    }
            }
            int unit =0;
            int minTime = 10000;
              for(unsigned int i = 0; i<performance.size();i++){
                if(performance[i].second != nrTUnits){
                    performance.erase(performance.begin()+i);
                    buildLists.erase(buildLists.begin()+i);
                    i--;
                }
                else if(performance[i].first<=minTime){
                    unit = performance[i].second;
                    minTime = performance[i].first;
                    index = i;
                }

            }
            //cout<<buildLists.size()<<endl;
            //cout<< minTime<< " : "<< index<<" units: "<<unit << endl;

        }
        void print_buildlist(){
            std::chrono::time_point<std::chrono::system_clock> start, end;
	        start = std::chrono::system_clock::now( );


	       int elapsed ;


            while(index == -1){
                //cout<<"run"<<endl;
                int ran = rand() % 8 + 2 ;
                generate_buildlists(1000,0.2,ran);
                check_dry_validity();
                get_best_buildlist();
                end = std::chrono::system_clock::now( );
                elapsed = std::chrono::duration_cast<std::chrono::seconds>
                             (end-start).count();

                if (elapsed==180){
                    cout<<"no list"<<endl;
                    exit(EXIT_SUCCESS);
                }
            }

            //simulate the best build list
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

#endif // PUSHBUILDER_H
