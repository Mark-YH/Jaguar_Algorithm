//
// Created by Mark Hsu on 2019/11/11.
//

#include "Model.h"
#include <cmath>

void Model::setFunction(Functions function) {
    this->function = function;
}

void Model::setDimension(int dim) {
    this->dimension = dim;
}

int Model::getDimension() {
    return dimension;
}

void Model::setDomain(float lower, float upper) {
    this->domain.lower = lower;
    this->domain.upper = upper;
}

Domain Model::getDomain() {
    return this->domain;
}

double Model::calcFitness(float pos) {
    switch (this->function) {
        case Absolute:
            return absolute(pos);
        case Sphere:
            return sphere(pos);
        case Rastrigin:
            return rastrigin(pos);
    }
}

double Model::absolute(float pos) {
    return fabs(pos);
}

double Model::sphere(float pos) {
    return pow(2, pos);
}

double Model::rastrigin(float pos) {
    return 0.0;
}
