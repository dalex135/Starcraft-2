#ifndef TERRANSIMULATOR_H
#define TERRANSIMULATOR_H
#include "Struct.h"
#include "Unit.h"
#include "Simulator.h"
#include "Writer.h"

class TerranSimulator : public Simulator
{
    public:

        TerranSimulator(vector<Unit*> buildList, string target_unit) {
            //read csv_ file and build techTree
            targetUnit = target_unit;
            techTree = reader.read_csv_file("unit_db.csv");
            //read build list and check its validity
            toDo = buildList;
            //initilize game
            Unit* u = findUnit("CommandCenter");
            Struct Command(u);
            finished.push_back(Command);

            for(int i=0; i<12; i++){
                Struct worker(findUnit("SCV"));
                finished.push_back(worker);
            }
            
            run();
           
        }


      ~TerranSimulator() {}


    protected:

    private:
};

#endif // TERRANSIMULATOR_H
