//
// Created by Mark Hsu on 2019-10-24.
//

#include "JA.h"

JA::JA() {
    // set-up model
    this->model = new Model();
    this->model->setDimension(1);
    this->model->setDomain(-100.0f, 100.0f);
    this->model->setFunction(Absolute);
}

JA::~JA() {
    delete this->model;
}

void JA::run(float e) {
    Jaguar jaguar(this->model);
    jaguar.hunting(e);
}
