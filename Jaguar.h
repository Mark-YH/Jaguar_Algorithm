//
// Created by Mark Hsu on 2019/11/5.
//

#ifndef JA_ABSOLUTE_JAGUAR_H
#define JA_ABSOLUTE_JAGUAR_H

#include "Model.h"
#include "Logger.h"

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

    void seeAround(int i);

    void speed_up(Logger *logger, int i);

    void speed_down(Logger *logger, int i);

    void prtStatusAt(Logger *logger, int i);
};

#endif //JA_ABSOLUTE_JAGUAR_H
