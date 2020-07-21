#include <iostream>
#include "PushBuilder.h"
#include "RushBuilder.h"
#include "BLBuilder.h"
#include "Simulator.h"

using namespace std;

int main(int argc, char *argv[])
{
    //check if the input is right
    if(argc != 4 ){
        cout<< "invalid number of inputs.first the keyword rush or push, "
        <<"then an arbitrary unit name followed by the number of seconds (rush) or the number of units (push)."
        <<"The unit can be anything from the csv file. ./optimize.sh push CommandCenter 3 is a valid input!" << endl;
        exit(EXIT_SUCCESS);
    }
    string order = argv[1];
    string targetUnit = argv[2];
    string amount = argv[3];
   if (order.compare("rush") == 0) {
        RushBuilder rush(targetUnit,stoi(amount));
   }
   else if (order.compare("push") == 0){
        PushBuilder push(targetUnit,stoi(amount));
   }
   else {
        cout<< "no such order!"<<endl;
   }
   return 0 ;

}



/*
int main(){
   // RushBuilder rush("Zealot",360); //works
    // RushBuilder rush("Marine",360);//works
    //RushBuilder rush ("Marauder" ,400);//somtimes work
   // RushBuilder  rush ("Stalker", 360);
   // PushBuilder push ("SiegeTank",6);
    // PushBuilder push("Battlecruiser",1); //not working
     // PushBuilder push ("Colossus", 3);
     //PushBuilder push ("VoidRay", 1);

}
 */
