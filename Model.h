//
// Created by Mark Hsu on 2019/11/11.
//

#ifndef JA_ABSOLUTE_MODEL_H
#define JA_ABSOLUTE_MODEL_H

//// Set-up test function
enum Functions {
    Absolute,
    Sphere,
    Rastrigin,
};

class Domain {
public:
    float upper;
    float lower;
};

class Model {
public:
    Model() = default;

    void setFunction(Functions function);

    void setDimension(int dim);

    int getDimension();

    void setDomain(float lower, float upper);

    Domain getDomain();

    double calcFitness(float pos);

private:
    Functions function;
    int dimension;
    Domain domain;

    double absolute(float pos);

    double sphere(float pos);

    double rastrigin(float pos);
};

#endif //JA_ABSOLUTE_MODEL_H
