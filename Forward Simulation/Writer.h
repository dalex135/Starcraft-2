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
        void add_events(int timestep, int minerals_w, int vespene_w, int minerals_r , int vespene_r, int used_supply, int supply, vector<pair<string,string>> event){


            json j;
            j["time"] =  to_string(timestep);
            j["status"]["workers"]["minerals"] =  to_string(minerals_w);
            j["status"]["workers"]["vespene"] =  to_string(vespene_w);
            j["status"]["resources"]["minerals"] =  to_string(minerals_r);
            j["status"]["resources"]["vespene"] =  to_string(vespene_r);
            j["status"]["resources"]["supply_used"] =  to_string(used_supply);
            j["status"]["resources"]["supply"] =  to_string(supply);
            j["events"]["type"] = event[0].first;
            j["events"]["name"] = event[0].second;

            ofstream o("pretty.json");
            o << setw(2) << j << std::endl;
            /*
            Json::Value j;
            Json::Value vec(Json::arrayValue);
            vec.append(Json::Value(1));
            vec.append(Json::Value(2));
            vec.append(Json::Value(3));

            j["time"] =  to_string(timestep);
            j["status"]["workers"]["minerals"] =  to_string(minerals_w);
            j["status"]["workers"]["vespene"] =  to_string(vespene_w);
            j["status"]["resources"]["minerals"] =  to_string(minerals_r);
            j["status"]["resources"]["vespene"] =  to_string(vespene_r);
            j["status"]["resources"]["supply_used"] =  to_string(used_supply);
            j["status"]["resources"]["supply"] =  to_string(supply);
            j["events"]["type"] = event[0].first;
            j["events"]["name"] = event[0].second;
            std::cout << j << std::endl;
            */
        }
};

#endif // WRITER_H
