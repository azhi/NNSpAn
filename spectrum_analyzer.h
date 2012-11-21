#ifndef Spectrum_Analyzer_H_
#define Spectrum_Analyzer_H_

#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include "wav_parser.h"

#define FFT_CHUNK_SIZE 512

struct Spectrum
{
  double* ampl;
  double* freq;
  long size;

  Spectrum() : ampl(NULL), freq(NULL), size(0) {};
  Spectrum(double* ampl, double* freq, long size) : ampl(ampl), freq(freq), size(size) {};
  ~Spectrum()
  {
    if (ampl != NULL)
      delete[] ampl;
    if (freq != NULL)
      delete[] freq;
  };

  void reserve(long r_size)
  {
    if ( r_size > size )
    {
      ampl = (double*) realloc(ampl, sizeof(double) * r_size);
      freq = (double*) realloc(freq, sizeof(double) * r_size);
      size = r_size;
    }
  };
};

class Spectrum_analyzer
{
public:
  Spectrum_analyzer();
  virtual ~Spectrum_analyzer();

  Spectrum* get_spectrum(char* path);

private:
  Spectrum* spectrum;
};

#endif
