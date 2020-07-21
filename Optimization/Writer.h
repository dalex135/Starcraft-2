#ifndef WRITER_H
#define WRITER_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <iomanip>

//#include "JsonWriter.h"
#include "Unit.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Writer
{

        public:

        bool timed_out = false;

        struct workers{
            int mineral_workers;
            int vespene_workers;
        }w;

        struct resources{
            int minerals;
            int vespene;
            int supply_used;
            int supply;
        }r;

        struct status{
            workers w;
            resources r;
        }s;


        struct message{
            int time_step;
            status s;
            vector<pair<string,string>> events;
        }m;

        struct file{
            int buildListValid;
            string game;
            vector<message> me;
        }file_writer;

         json add_messages(message m){
            json j;
            json ev; // for events

                j["time"] =  to_string(m.time_step);
                j["status"]["workers"]["minerals"] =  to_string(m.s.w.mineral_workers);
                j["status"]["workers"]["vespene"] =  to_string(m.s.w.vespene_workers);
                j["status"]["resources"]["minerals"] =  to_string(m.s.r.minerals);
                j["status"]["resources"]["vespene"] =  to_string(m.s.r.vespene);
                j["status"]["resources"]["supply-used"] =  to_string(m.s.r.supply_used);
                j["status"]["resources"]["supply"] =  to_string(m.s.r.supply);
                for(unsigned int i = 0; i<m.events.size();++i){
                    ev["type"] = m.events[i].first;
                    ev["name"] = m.events[i].second;
                    j["events"].push_back(ev);
                }


                return j;



        }
        void print_mesages(){
            json j;

            ofstream o("pretty.json");

            j["buildlistValid"] = to_string(file_writer.buildListValid);
            j["game"] = file_writer.game;
            //j["messages"] = "";
            json s;
            // only write messages if the buildlist is valid
            if( file_writer.buildListValid == 1 ){
            //&& timed_out == false
                for(unsigned int i = 0; i< file_writer.me.size();++i){
                    s = add_messages(file_writer.me[i]);
                    j["messages"].push_back(s);
                }
            }
            o <<setw(4)<<j<<endl;
            cout<< setw(4) << j << endl;

        }



};
#endif // READER
