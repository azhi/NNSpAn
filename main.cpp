#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fftw3.h>

#include "spectrum_analyzer.h"


int main(int argc, char *argv[])
{
  Spectrum_analyzer* sa = new Spectrum_analyzer();
  Spectrum* sp = sa->get_spectrum(argv[1]);
  for ( long i = 1; i < sp->size; ++i )
    printf("%.1f %.8f\n", sp->freq[i], sp->ampl[i]);
  delete sa;
  return 0;
}
