#ifndef BSPLINEGENERATOR_H
#define BSPLINEGENERATOR_H


#include <glm/glm.hpp>
#include <vector>
#include <stdio.h>
#include <iostream>

using namespace glm;
using namespace std;

class BSplineGenerator
{
public:
    BSplineGenerator();
    BSplineGenerator(int k);
    BSplineGenerator( int k,
                     double u_inc,
                     vector <vec2> controlPoints,
                     vector <float> knotSequence);

    vector<vec3> getControlPoints();
    void decraseK();
    void increaseK();
    int getNumberOfControlPoints();

    void addControlPoint(vec3 aPoint);

    void moveControlPoint(vec3 aPoint, int index);
    void deleteControlPoint(int index);

    int delta(double u, int m, int k);

    vec3 E_delta_1(double u, int m, int k);

    void generateGraph(double u_step);
    void generateGraph();

    vector<vec3> graphData;
    int selected = -1;
    vector<vec3> geometryData;
private:
    int k; //order of the curve
    double u_inc; //User control over incrementation

    void makeKnotSequence();
    vector<vec3> controlPoints;
    vector<float> knotSequence;

    vec3  S(float u, int k, int m);
    float N(int i, int k, float u);

    void addPointToGeometry(vec3 aPoint, int level);






};

#endif // BSPLINEGENERATOR_H
