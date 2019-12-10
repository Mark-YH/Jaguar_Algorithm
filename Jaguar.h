//
// Created by Mark Hsu on 2019/11/5.
//

#ifndef JA_ABSOLUTE_JAGUAR_H
#define JA_ABSOLUTE_JAGUAR_H

#include "Model.h"
#include "Logger.h"

/*! @EPANEL:
 * 0: print debug information in files
 * 1: print e-panel format in files
 * others number: No data printed
 */
#define EPANEL -1

class Jaguar {
public:
    Jaguar() = delete;

    explicit
    Jaguar(Model *model);

    void hunting();

    void jump();

    void createTerritory();

    void taboo();

private:
    Model *model;
    float *position;
    double fitness;
    float step;
    float rate;
    float direction; // right: 1 , left: -1
    int cntCalculation;
    int foundBestAt;
    double bestFitness;
    float *bestPosition;

    void seeAround(Logger *logger, int i);

    void speed_up(Logger *logger, int i);

    void speed_down(Logger *logger, int i);

    void updateStep(int i);

    void prtStatusAt(Logger *logger, int i);
};

#endif //JA_ABSOLUTE_JAGUAR_H
