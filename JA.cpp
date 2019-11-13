//
// Created by Mark Hsu on 2019-10-24.
//

#include "JA.h"

JA::JA() {
    // set-up model
    this->model.setDimension(1);
    this->model.setDomain(-100.0f, 100.0f);
    this->model.setFunction(Absolute);
}

void JA::run() {
    Jaguar jaguar(&this->model);
    jaguar.hunting();
}
