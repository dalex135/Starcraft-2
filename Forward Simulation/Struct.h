#ifndef STRUCT_H
#define STRUCT_H
#include <iostream>
#include <vector>
#include "Unit.h"
class Struct
{
    public :
        Struct(Unit* u):unit(u),remain_time(u->build_time){

        }

        Unit* unit;
        int energy = 50;
        int personam_id = 0;
        int remain_time;
        bool occupied = 1;

};
#endif // STRUCT_H
