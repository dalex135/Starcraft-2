#ifndef PROTOSSSIMULATOR_H
#define PROTOSSSIMULATOR_H
#include "Struct.h"
#include "Unit.h"
#include "Simulator.h"
#include "Writer.h"

class ProtossSimulator : public Simulator
{
    public:
        ProtossSimulator(vector<Unit*> buildList,string target_unit) {
            //read csv_file and build tech tree
            techTree = reader.read_csv_file("unit_db.csv");
            targetUnit = target_unit;
            //read build list and check its validity
            toDo = buildList;
            // initilize game
            Unit* u = findUnit("Nexus");
            Struct Nexus(u);
            finished.push_back(Nexus);

            for(int i=0; i<12; i++){
                Struct worker(findUnit("Probe"));
                finished.push_back(worker);
            }
            // run and write messages
            run();
            //writer.print_mesages();
        }
       ~ProtossSimulator() {}


    protected:

    private:
};

#endif // PROTOSSSIMULATOR_H
