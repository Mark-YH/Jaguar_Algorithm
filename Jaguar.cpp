//
// Created by Mark Hsu on 2019/11/5.
//

#include "Jaguar.h"
#include <cmath>

using std::ios;

Jaguar::Jaguar(Model *model) {
    this->model = model;
    int dim = model->getDimension();
    this->position = new float[dim];
    Domain domain = this->model->getDomain();

    this->bestFitness = INT_MAX;
    this->bestPosition = new float[dim];
    this->direction = 1.0f;
    this->rate = 1.0f;
    this->foundBestAt = 0;

    //TODO function should write in one line
    double tmp = 0; // for fitness

    for (int i = 0; i < dim; i++) {
        this->position[i] = rand() / (float) RAND_MAX * (domain.upper - domain.lower) + domain.lower;
        this->bestPosition[i] = this->position[i];

        //TODO function should write in one line
        tmp += this->model->calcFitness(this->position[i]);
    }
    this->fitness = tmp;
    cntCalculation = 1; // Initialization calculated once of fitness.
}

void Jaguar::seeAround(int i) {
    Logger logger("../log/see_around.csv");
    float rPos, lPos;
    double rFitness, lFitness;

    rPos = this->position[i] + this->step * this->rate;
    lPos = this->position[i] - this->step * this->rate;

    rFitness = this->model->calcFitness(rPos);
    lFitness = this->model->calcFitness(lPos);
    this->cntCalculation += 2;

    // Right position & fitness
    logger.writeComma(this->cntCalculation - 1);
    logger.writeComma(rPos);
    logger.writeComma(rFitness);
    logger.writeComma(this->step * this->rate);
    logger.writeLine('R');

    // Left position & fitness
    logger.writeComma(this->cntCalculation);
    logger.writeComma(lPos);
    logger.writeComma(lFitness);
    logger.writeComma(this->step * this->rate);
    logger.writeLine('L');

    if (rFitness <= lFitness && rFitness < this->fitness) { // Turn right is a better way.
        // Turn right
        this->position[i] = rPos;
        this->fitness = rFitness;
        this->direction = 1;
    } else if (rFitness > lFitness && lFitness < this->fitness) { // Turn left is a better way.
        // Turn left
        this->position[i] = lPos;
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
    logger->writeLine("Speed-up:");

    double nextFitness;
    float nextPosition = this->position[i];

    while (true) {
        nextPosition = this->direction * this->step * this->rate + nextPosition;

        // Check if next position is outside of the domain.
        if (nextPosition > model->getDomain().upper || nextPosition < model->getDomain().lower) {
            logger->writeComma("Out of range at: ");
            logger->writeLine(nextPosition);
            break;
        }

        nextFitness = this->model->calcFitness(nextPosition);
        cntCalculation++;

        // Occur a worse fitness which means that position is worse than the last one so that stopping speed-up.
        if (nextFitness > this->fitness) {
            logger->writeComma("Next position has worse fitness: ");
            logger->writeLine(nextFitness);

            prtStatusAt(logger, i);
            break;
        }

        // Move
        this->position[i] = nextPosition;
        this->fitness = nextFitness;

        // Check if it needs update best fitness
        if (this->fitness < this->bestFitness) {
            this->bestPosition[i] = this->position[i];
            this->bestFitness = this->fitness;
        }

        prtStatusAt(logger, i);

        this->rate *= 2.0;
    } // end of speed-up
}

void Jaguar::speed_down(Logger *logger, int i) {
    logger->writeLine("Speed-down:");
    this->rate /= 2.0;

    float nextPosition;
    double nextFitness;
    // First speed-down
    nextPosition = this->direction * step * this->rate + this->position[0];

    // Check if next position is outside of the domain.
    if (nextPosition > model->getDomain().upper || nextPosition < model->getDomain().lower) {
        logger->writeComma("Out of range at: ");
        logger->writeLine(nextPosition);
    } else {
        nextFitness = this->model->calcFitness(nextPosition);
        cntCalculation++;

        logger->writeLine("First step of speed-down:");
        logger->writeComma(cntCalculation);
        logger->writeComma(nextPosition);
        logger->writeComma(nextFitness);
        logger->writeLine(this->step * this->rate);

        if (this->fitness > nextFitness) {
            // Move
            this->position[i] = nextPosition;
            this->fitness = nextFitness;
            //}
            // Check if it needs update best fitness
            if (this->fitness < this->bestFitness) {
                this->bestPosition[i] = this->position[i];
                this->bestFitness = this->fitness;
            }
        }
    }

    // The others speed-down
    logger->writeLine("The others speed-down");
    while (this->rate != 1) {
        this->rate /= 2;
        seeAround(i);
    }
}

void Jaguar::hunting() {
    Logger logger("../log/hunting.csv");
    this->step = powf(2.0, (floor)(log(this->model->getDomain().upper) / log(2)) - 11);

    for (int i = 0; i < this->model->getDimension(); i++) {
        while (this->position[i] + this->step * this->rate != this->position[i]) {
            double tmpFitness = this->fitness;
            seeAround(i);
            if (tmpFitness == fitness) {
                step /= 2.0;
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
        logger.write("Found 0: ");
        logger.writeLine(this->foundBestAt);
        logger.write("Best: ");
        logger.writeLine(this->bestFitness);
    }
}

void Jaguar::jump() {

}

void Jaguar::createTerritory() {

}

void Jaguar::taboo() {

}

void Jaguar::prtStatusAt(Logger *logger, int i) {
    logger->writeComma(cntCalculation);
    logger->writeComma(this->position[i]);
    logger->writeComma(this->fitness);
    logger->writeLine(this->step * this->rate);
}
