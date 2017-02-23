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
  for (i = 0; i < k; i++)
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


  for (i = 0 ; i < k; i++)
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
  if (k == 1)
  {
    return;
  } else {
    k = k -1;
  }
}


void BSplineGenerator::increaseK()
{
  k = k + 1;
}

int BSplineGenerator::delta(double u, int m, int k)
{
    for (int i = 0; i < (m + k - 1); i++)
    {
      //cout << " i is  " << i << "\n";

      if (u >= knotSequence.at(i) || u < knotSequence.at(i + 1))
      {
          //printf("delta is not -1 \n");
          return i;
      }

    }
    //printf("delta is -1 \n");
    return -1;
}

/*
void generateE(double u, int m, int k)
{
  int delta = delta(u, m, k)

  for (delta == -1 || (delta + k) > (controlPoints.size() - 1))
  {
    //fill E with 0's

    return;
  }

  //fill the rest of the array with 0
   // for delta to k -1

   for (int i; i < k; i++)
   {
     E[delta + i] = 1;
   }


  /*
  for (delta; delta < (delta - k + 2); delta--)
  {
    float omega = (u - knotSequence.at(delta))/(knotSequence.at(delta + k - 1)
                                        - knotSequence.at(delta));
    E[delta] = omega;
    E[delta - 1] =
  }

  */



  /*


    for (delta to delta  - k + 2E delta--)
    {
      float omega = (u - knotSequence.at(delta))/(knotSequence.at(delta + k - 1)
                                          - knotSequence.at(delta));

      E[delta] = omega
      E[delta - 1] = (1 - omega);
    }
    init an array to all 0's
    for r == k to 2 r--
    i = delta
    if delta > sizeOfControlPoints || delta == -1
      //fill E with 0's
      for delta to delta - r + 2
        E[i] = omega * E[i] + (1 - omega ) * E[i - 1]
        i--;




}
*/

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
     if ( (d - k) < 0)
    {
     d = k - 1;
    }

    //cout << "About to add key control points \n";
    //printf("d is %i \n", d);
    for (i = 0; i < (k -1); i++){
        //printf("d -i is %i \n", (d - i));
        c.push_back(controlPoints.at((d - i)));
    }


    //cout << "Done adding key control points to c \n";

    for (int r = k; r > 2; r--)
    {
      i = d;
      //cout << "about to fix special control point \n";
        for (int s = 0; s <(r - 2); s++){

            float omega = (u - knotSequence.at(i))/(knotSequence.at(i + r - 1)
                                                  - knotSequence.at(i));


            c.at(s) = omega * c.at(s) + (1 - omega) * c.at(s + 1);
            i = i - 1;
        }
    }

    return c[0];

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

   //printf("graphData has a size of %i \n", graphData.size());
}

void BSplineGenerator::generateGraph()
{
    generateGraph(0.1);
}
