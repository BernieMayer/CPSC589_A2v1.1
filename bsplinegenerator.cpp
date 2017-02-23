#include "bsplinegenerator.h"


BSplineGenerator::BSplineGenerator()
{

}

/*
BSplineGenerator(int k, double u_inc,
                 vector<vec2> controlPoints,
                 vector<float> knotSequence)
{
    this->k = k;
    this->u_inc = u_inc;
    this->controlPoints = controlPoints;
    this->knotSequence = knotSequence;
}*/

BSplineGenerator::BSplineGenerator( int k,
                 double u_inc,
                 vector <vec2> controlPoints,
                 vector <float> knotSequence)
{

}

BSplineGenerator::BSplineGenerator(int k)
{
  this->k = k;
}

vector<vec3> BSplineGenerator::getControlPoints()
{
    return controlPoints;
}

//this function is used to update the knot sequence
void  BSplineGenerator::makeKnotSequence()
{
  knotSequence.clear();
  int i = 0;
  for (i = 0; i < k; i++)
  {
    knotSequence.push_back(0.0f);

  }

  float step = (1.0f)/(controlPoints.size() - k + 2);
  float current_step = step;
  for (i = 0; i < controlPoints.size(); i++)
  {
    //Add in a value of the uniform knot sequence
    knotSequence.push_back(current_step);
    current_step += step;
  }






  for (i = 0 ; i < k; i++)
  {
    knotSequence.push_back(1.0f);
  }
}

int BSplineGenerator::getNumberOfControlPoints()
{
    return controlPoints.size();
}

void BSplineGenerator::addControlPoint(vec3 aPoint)
{
    controlPoints.push_back(aPoint);

    makeKnotSequence();
    //update the knot sequence
}

void BSplineGenerator::deleteControlPoint(int index)
{
  controlPoints.erase(controlPoints.begin() + index);

  makeKnotSequence();
}

void BSplineGenerator::moveControlPoint(vec3 aPoint, int index)
{
  controlPoints.at(index) = aPoint;
}

int BSplineGenerator::delta(double u, int m, int k)
{
    for (int i = 0; i < (m + k - 1); i++)
    {
      //cout << " i is  " << i << "\n";

      if (u >= knotSequence.at(i) && u < knotSequence.at(i + 1))
        return i;
    }
}

vec3 BSplineGenerator::E_delta_1(double u, int m, int k)
{
    int d = delta(u, m,k);  //determines the delta..
    vector<vec3> c;
    int i;

    //cout << "About to add key control points \n";
    //printf("d is %i \n", d);
    for (i = 0; i < (k - 1); i++){
        //printf("d -i is %i \n", (d - i));
        c.push_back(controlPoints.at((d - i) % controlPoints.size()));
    }

    //cout << "Done adding key control points to c \n";

    for (int r = k; r < 2; r--)
    {
      i = d;
      //cout << "about to fix special control point \n";
        for (int s = 0; s <(r - 2); s++){
            float omega = (u - knotSequence.at(i))/(knotSequence.at(i + r -1)
                                                  - knotSequence.at(i));
            c.at(s) = omega * c.at(s) + (1 - omega) * c.at(s + 1);
            i = i - 1;
        }
    }

    return c[0];

}

void BSplineGenerator::generateGraph(double u_step)
{
  graphData.clear();
  makeKnotSequence();
   double u = 0;
   while( u < k)
   {

        vec3 point = E_delta_1(u, controlPoints.size(), k);
        graphData.push_back(point);
        u += u_step;
   }
}

void BSplineGenerator::generateGraph()
{
    generateGraph(0.1);
}
