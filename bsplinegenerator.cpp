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
  if ( controlPoints.size() <= k) return;
  int i = 0;
  for (i = 0; i < (k); i++)
  {
    knotSequence.push_back(0.0f);
  }

  float step = (1.0f)/(controlPoints.size() - k + 2);
  float current_step = step;
  for (i = 0; i < (controlPoints.size() - k + 1); i++)
  {
    //Add in a value of the uniform knot sequence
    knotSequence.push_back(current_step);
    current_step += step;
  }


  for (i = 0 ; i < (k); i++)
  {
    knotSequence.push_back(1.0f);
  }

  bool debug = false;

  if (k <= controlPoints.size())
  {
    debug = false;
  }
  if (debug)
  {
    for (int r = 0; r < knotSequence.size(); r ++)
    {

      printf("knotSequence at r is %i and the value is %f \n",
     r, knotSequence.at(r));
  }
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

void BSplineGenerator::decraseK()
{
  if (k <= 2)
  {
    return;
  } else {
    k = k -1;
    printf("K is now %i \n", k );
  }
}


void BSplineGenerator::increaseK()
{
  printf("K is now %i \n", k);
  k = k + 1;
}

int BSplineGenerator::delta(double u, int m, int k)
{
    for (int i = 0; i < (m + k - 1); i++)
    {
      //cout << " i is  " << i << "\n";

      if (u >= knotSequence.at(i) && u < knotSequence.at(i + 1))
      {
          //printf("delta is not -1 \n");
          return i;
      }

    }
    //printf("delta is -1 \n");
    return -1;
}


vec3 BSplineGenerator::E_delta_1(double u, int m, int k)
{

    int d = delta(u, m,k);  //determines the delta..
    vector<vec3> c;
    int i;

    //printf("d is %i \n", d);

    if ( d >= controlPoints.size())
    {
      d = d - 1;

    }

     if ( (d - (k -1)) < 0)
    {
      d = k - 2;
    }

    //cout << "About to add key control points \n";
    //printf("d is %i \n", d);
    for (i = 0; i < k; i++){
        //printf("d -i is %i \n", (d - i));

        c.push_back(controlPoints.at((d - i)));
    }




    //cout << "Done adding key control points to c \n";

    for (int r = k; r > 1; r--)
    {
      i = d;
      //cout << "about to fix special control point \n";
      std::vector<vec3> level;
        for (int s = 0; s <= (r - 2); s++){
            float omega = (u - knotSequence.at(i))/(knotSequence.at(i + r - 1)
                                                  - knotSequence.at(i));

            if (s == 0) {
              addPointToGeometry(c.at(s), k - r);
              addPointToGeometry(c.at(s + 1), k -r);
          }

            c.at(s) = omega * c.at(s) + (1 - omega) * c.at(s + 1);
            i = i - 1;
        }

    }

    //printf("geometryData is %i by %i \n", geometryData.size(), geometryData.at(0).size());

    return c[0];

}

void  BSplineGenerator::addPointToGeometry(vec3 aPoint, int level)
{

}

vec3  BSplineGenerator::S(float u, int k, int m)
{
  vec3 s = vec3(0,0,0);

  for (int i = 0; i < m; i++)
  {
    s = s + N(i, k, u) * controlPoints.at(i);
  }
  return s;
}

float  BSplineGenerator::N(int i, int k, float u)
{
  if (k > 1)
  {
    float result = (u - knotSequence.at(i))/(knotSequence.at(i + k -1) - knotSequence.at(i)) *  N(i,k -1, u)
    + (knotSequence.at(i +k) - u)/(knotSequence.at(i + k)) * N(i +1, k -1, u);
    return result;
  } else if (u > knotSequence.at(i) && u < knotSequence.at(i + 1)){
    return 1.0f;
  } else {
    return 0;
  }
}

void BSplineGenerator::generateGraph(double u_step)
{
  graphData.clear();
  geometryData.clear();
  makeKnotSequence();
  if (controlPoints.size() <= k)
    return;
   double u = 0;
   while( u < 1)
   {

        vec3 point = E_delta_1(u, controlPoints.size(), k);
        //vec3 point = S(u, k, controlPoints.size());
        graphData.push_back(point);
        //graphData.push_back(point * 0.5f + graphData.at(graphData.size() - 1) * 0.5f);
        u += u_step;
   }



   for (int l = 0; l < controlPoints.size(); l++)
   {
     geometryData.push_back(controlPoints.at(l));
   }


   



   //printf("graphData has a size of %i \n", graphData.size());
}

void BSplineGenerator::generateGraph()
{
    generateGraph(0.1);
}
