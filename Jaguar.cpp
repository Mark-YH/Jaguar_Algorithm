//
// Created by Mark Hsu on 2019/11/5.
//

#include "Jaguar.h"
#include <cmath>
#include <string>

using std::ios;

Jaguar::Jaguar(Model *model) {
    this->model = model;
    int dim = model->getDimension();
    this->position = new float[dim];
    Domain domain = this->model->getDomain();

    this->bestPosition = new float[dim];
    this->direction = 1.0f;
    this->rate = 1.0f;
    this->foundBestAt = 0;

    for (int i = 0; i < dim; i++) {
        this->position[i] = rand() / (float) RAND_MAX * (domain.upper - domain.lower) + domain.lower;
        this->bestPosition[i] = this->position[i];
    }

    this->fitness = this->model->calcFitness(this->position);
    this->bestFitness = this->fitness;
    cntCalculation = 1; // Initialization calculated once of fitness.

#if EPANEL
    Logger ePanel("../log/ja.epin");
    ePanel.writeSpace("Dimension :");
    ePanel.writeLine(dim);
    ePanel.writeLine("Formula : sum(|Xi|)");
    ePanel.writeSpace("Range :");
    ePanel.writeSpace(domain.lower);
    ePanel.writeSpace("~");
    ePanel.writeLine(domain.upper);
    ePanel.writeLine("Position :");

    ePanel.writeSpace("*0");
    ePanel.write(this->bestFitness);
    ePanel.writeSpace(":");
    for (int i = 0; i < this->model->getDimension(); i++) {
        ePanel.writeComma(this->position[i]);
    }
    ePanel.writeLine("10,0,12");
#endif
}

void Jaguar::seeAround(Logger *logger, int i) {
    float *rPos, *lPos;
    double rFitness, lFitness;
    int rCount, lCount;
    lPos = new float[this->model->getDimension()];
    rPos = new float[this->model->getDimension()];

    for (int j = 0; j < this->model->getDimension(); j++) {
        if (j == i) {
            rPos[j] = this->position[j] + this->step * this->rate;
            lPos[j] = this->position[j] - this->step * this->rate;
        } else {
            rPos[j] = this->position[j];
            lPos[j] = this->position[j];
        }
    }

    if (this->model->isOutOfRange(rPos[i])) {
        rFitness = INT_MAX;
    } else {
        rFitness = this->model->calcFitness(rPos);
        this->cntCalculation++;
    }

    rCount = cntCalculation;

    if (this->model->isOutOfRange(lPos[i])) {
        lFitness = INT_MAX;
    } else {
        lFitness = this->model->calcFitness(lPos);
        this->cntCalculation++;
    }

    lCount = cntCalculation;

#if EPANEL
    logger->write("*");
    logger->writeSpace(rCount);
    logger->write(this->bestFitness);
    logger->writeSpace(":");
    for (int k = 0; k < this->model->getDimension(); k++) {
        logger->writeComma(rPos[k]);
    }
    logger->writeLine("10,0,12");

    logger->write("*");
    logger->writeSpace(lCount);
    logger->write(this->bestFitness);
    logger->writeSpace(":");
    for (int k = 0; k < this->model->getDimension(); k++) {
        logger->writeComma(lPos[k]);
    }
    logger->writeLine("10,0,12");
#else
    // Right position & fitness
    logger->writeComma(rCount);
    logger->writeComma(rPos[i]);
    logger->writeComma(rFitness);
    logger->writeComma(this->step * this->rate);
    logger->writeLine('R');

    // Left position & fitness
    logger->writeComma(lCount);
    logger->writeComma(lPos[i]);
    logger->writeComma(lFitness);
    logger->writeComma(this->step * this->rate);
    logger->writeLine('L');
#endif
    if (rFitness <= lFitness && rFitness < this->fitness) { // Turn right is a better way.
        // Turn right
        this->position[i] = rPos[i];
        this->fitness = rFitness;
        this->direction = 1;
    } else if (rFitness > lFitness && lFitness < this->fitness) { // Turn left is a better way.
        // Turn left
        this->position[i] = lPos[i];
        this->fitness = lFitness;
        this->direction = -1;
    }

    // Check if it needs update best fitness
    if (this->fitness < this->bestFitness) {
        this->bestPosition[i] = this->position[i];
        this->bestFitness = this->fitness;

        if (this->bestFitness == 0 && this->foundBestAt == 0 && rFitness == 0) {
            this->foundBestAt = cntCalculation - 1;
        } else if (this->bestFitness == 0 && this->foundBestAt == 0 && lFitness == 0) {
            this->foundBestAt = cntCalculation;
        }
    }
}

void Jaguar::speed_up(Logger *logger, int i) {
#if !EPANEL
    logger->writeLine("Speed-up:");
#endif

    double nextFitness;
    float *nextPosition;
    nextPosition = new float[this->model->getDimension()];

    memcpy(nextPosition, this->position, sizeof(float) * this->model->getDimension());

    while (true) {
        nextPosition[i] = this->direction * this->step * this->rate + nextPosition[i];

        // Check if next position is outside of the domain.
        if (this->model->isOutOfRange(nextPosition[i])) {
#if !EPANEL
            logger->writeComma("Out of range at: ");
            logger->writeLine(nextPosition[i]);
#endif
            break;
        }

        nextFitness = this->model->calcFitness(nextPosition);
        cntCalculation++;

        // Occur a worse fitness which means that position is worse than the last one so that stopping speed-up.
        if (nextFitness > this->fitness) {
#if !EPANEL
            logger->writeComma("Next position has worse fitness: ");
            logger->writeLine(nextFitness);
#endif
            prtStatusAt(logger, i);
            break;
        }

        // Move
        this->position[i] = nextPosition[i];
        this->fitness = nextFitness;

        // Check if it needs update best fitness
        if (this->fitness < this->bestFitness) {
            this->bestPosition[i] = this->position[i];
            this->bestFitness = this->fitness;

            // Check if it found best
            if (this->bestFitness == 0 && this->foundBestAt == 0) {
                this->foundBestAt = cntCalculation;
            }
        }

        prtStatusAt(logger, i);

        this->rate *= 2.0;
    } // end of speed-up
}

void Jaguar::speed_down(Logger *logger, int i) {
#if !EPANEL
    logger->writeLine("Speed-down:");
#endif
    this->rate /= 2.0;

    float *nextPosition;
    double nextFitness;

    nextPosition = new float[this->model->getDimension()];
    memcpy(nextPosition, this->position, sizeof(float) * this->model->getDimension());

    // First speed-down
    nextPosition[i] = this->direction * step * this->rate + this->position[0];

    // Check if next position is outside of the domain.
    if (this->model->isOutOfRange(nextPosition[i])) {
#if !EPANEL
        logger->writeComma("Out of range at: ");
        logger->writeLine(nextPosition[i]);
#endif
    } else {
        nextFitness = this->model->calcFitness(nextPosition);
        cntCalculation++;

#if EPANEL
        logger->write("*");
        logger->writeSpace(this->cntCalculation);
        logger->write(this->bestFitness);
        logger->writeSpace(":");
        for (int k = 0; k < this->model->getDimension(); k++) {
            logger->writeComma(nextPosition[k]);
        }
        logger->writeLine("10,0,12");
#else
        logger->writeLine("First step of speed-down:");
        logger->writeComma(cntCalculation);
        logger->writeComma(nextPosition[i]);
        logger->writeComma(nextFitness);
        logger->writeLine(this->step * this->rate);
#endif
        if (this->fitness > nextFitness) {
            // Move
            this->position[i] = nextPosition[i];
            this->fitness = nextFitness;

            // Check if it needs update best fitness
            if (this->fitness < this->bestFitness) {
                this->bestPosition[i] = this->position[i];
                this->bestFitness = this->fitness;

                // Check if it found best
                if (this->bestFitness == 0 && this->foundBestAt == 0) {
                    this->foundBestAt = cntCalculation;
                }
            }
        }
    }
#if !EPANEL
    // The others speed-down
    logger->writeLine("The others speed-down");
#endif
    while (this->rate != 1) {
        this->rate /= 2;
        seeAround(logger, i);
    }
}

void Jaguar::hunting() {
    for (int i = 0; i < this->model->getDimension(); i++) {
#if EPANEL
        Logger logger("../log/ja.epin");
#else
        Logger logger("../log/hunting" + std::to_string(i + 1) + ".csv");
#endif
        this->fitness = INT_MAX;
        this->bestFitness = INT_MAX;
        this->foundBestAt = 0;
        this->rate = 1.0f;
        this->step = powf(2.0, (floor)(log(this->model->getDomain().upper) / log(2)) - 11);

        while (this->fitness != 0) {
//        while (this->position[i] + this->step * this->rate != this->position[i]) {
            double tmpFitness = this->fitness;
#if EPANEL
            seeAround(&logger, i);
#else
            Logger logSeeAround("../log/see_around.csv");
            seeAround(&logSeeAround, i);
#endif
            if (tmpFitness == fitness) {
                float exp;

                exp = floor((log2(abs(this->position[i])) - 23) / 2);

                if (exp < -149)
                    exp = -149;

                this->step = powf(2, exp);
                std::cout << setprecision(50) << this->position[i] << '\t';
                std::cout << setprecision(50) << this->step << '\t';
                std::cout << setprecision(50) << this->rate * this->step << std::endl;

                continue;
            }
            this->rate *= 2.0;

            speed_up(&logger, i);

            speed_down(&logger, i);

            // next speed cycle
            this->step /= 2.0;

            if (this->bestFitness == 0 && this->foundBestAt == 0) {
                this->foundBestAt = cntCalculation;
            }
        }
#if !EPANEL
        logger.write("Found 0: ");
        logger.writeLine(this->foundBestAt);
        logger.write("Best: ");
        logger.writeLine(this->bestFitness);
#endif
    }
}

void Jaguar::jump() {

}

void Jaguar::createTerritory() {

}

void Jaguar::taboo() {

}

void Jaguar::prtStatusAt(Logger *logger, int i) {
#if EPANEL
    logger->write("*");
    logger->writeSpace(this->cntCalculation);
    logger->write(this->bestFitness);
    logger->writeSpace(":");
    for (int k = 0; k < this->model->getDimension(); k++) {
        logger->writeComma(this->position[k]);
    }
    logger->writeLine("10,0,12");
#else
    logger->writeComma(cntCalculation);
    logger->writeComma(this->position[i]);
    logger->writeComma(this->fitness);
    logger->writeLine(this->step * this->rate);
#endif
}
