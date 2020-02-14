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
#define A 0
#define B 0
#define C 0
/**
 * @param A: the method of calculating initial step
 *      0: by domain
 *      1: by current position
 * @param B: the method of calculating reduced step
 *      0: divided by 2
 *      1: by current position
 * @param C: the method of squeeze
 *      0: divided by 2
 *      1: bisection
 */

enum Status {
    Left_right_check = 0,
    Speed_up,
    Speed_down,
};

class Jaguar {
public:
    Jaguar() = delete;

    explicit
    Jaguar(Model *model);

    ~Jaguar();

    void hunting(float e);

    void jump();

    void createTerritory();

    void taboo();

private:
    Status status;
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

    void updateStep(int i, float e);

    void prtStatusAt(Logger *logger, int i);

    static string enum2str(int e);

    static string direction2str(float direction);
};

#endif //JA_ABSOLUTE_JAGUAR_H
