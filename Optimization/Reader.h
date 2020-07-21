#ifndef READER_H
#define READER_H
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <iostream>
#include <string>
#include "Unit.h"

using namespace std;

class Reader
{
    public:
        bool String2Boolean(string var){
            if(var.compare("True") == 0)
                return true;
            else
                return false;

        }

        vector<string> String2Vector(string var){
            istringstream ss(var);
            string token;

            vector<string> playerVector;
            while(std::getline(ss, token, '/')) {
                playerVector.push_back(token);
            }
            //cout<<"This prints"<<"\n";
            return playerVector;

        }

        vector<Unit> read_csv_file(string file_name){
            Unit u;
            vector<Unit> playerVector;
            char comma = ',';
            string line,attr;
            ifstream file_;
            file_.open(file_name);
            getline(file_, line);
            //cout<<line<<"\n";
            // Read the entire line
            while(getline(file_, line)) {
                //cout<<line<<"\n";
                // The following if condition has been written to ignore the
                // lines in units_db.csv which starts with #
                if(line.front() != '#') {
                    // convert the line into stringstream
                    stringstream s1(line);

                    // save the string before comma into string attr
                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.id = attr;

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.production_state = attr;
                    //cout<<attr<<"\n";

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.minerals = stoi(attr)*100;

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.vespene = stoi(attr)*100;

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.supply = stod(attr);
                    //cout<<attr<<"\n";

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.supply_cost = std::stod(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.supply_provided = std::stod(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.build_time = std::stoi(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.occupy_limit = std::stoi(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.units_produced= std::stoi(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.producer= String2Vector(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.dependencies = String2Vector(attr);
                    //if (!u.dependencies.empty())
                        //cout<<u.dependencies[0]<<endl;

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.produces = String2Vector(attr);
                    //cout<<attr<<"\n";

                    getline(s1, attr, comma);
                    //cout<<"This:"<<attr<<"\n";
                    //cout <<"The type" <<typeid(attr).name() << endl;
                    u.start_energy= stod(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.max_energy= stod(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.structure = String2Boolean(attr);

                    getline(s1, attr, comma);
                    //cout<<attr<<"\n";
                    u.race = string(attr);

                    //getline(s1, attr, comma);
                    // insert the unit into playerVector
                    playerVector.push_back(u);
                }

            }

            file_.close();
            return playerVector;
        }
        /*
        vector<Unit*> read_list(string listName, vector<Unit> techTree){

                ifstream inFile;
                inFile.open(listName);
                string str;
                while (std::getline(inFile, str)) {
                    for(int i = 0; i <=techTree.size();i++){
                        if(str.compare(techTree[i].id) != 0)
                        cout<<"Okay";
                    }

                }

                inFile.close();

        }
        */
        /*vector<Unit*> read_list(string listName,vector<Unit>& techTree){

            ifstream inFile;
            inFile.open(listName);
            string str;
            vector<Unit*> toDo;
            while (getline(inFile, str)) {
                //cout<<"Okay"<<"\n";
                for(size_t i = 0; i < techTree.size();i++){
                    //cout<<str<<"\t"<<techTree[i].id<<"||";
                    if(str.compare(techTree[i].id) == 0){

                        Unit *Up;
                        Up= &techTree[i];
                        //cout<<Up->id<<endl;
                        toDo.push_back(Up);
                    }
                }

            }
            inFile.close();
            return toDo;

        }*/



};

#endif // READER
