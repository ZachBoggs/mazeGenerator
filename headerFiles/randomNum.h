#ifndef RANDOMNUM_H
#define RANDOMNUM_H

#include <math.h>

// generates a random number based on a given range
int randomNumber(int low, int high)
{
  int numberCalculated = ((rand() % (high - low )) + low);
  return numberCalculated;
}

#endif
