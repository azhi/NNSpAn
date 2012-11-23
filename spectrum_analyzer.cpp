#include <math.h>

#include "spectrum_analyzer.h"

Spectrum_analyzer::Spectrum_analyzer()
{
  spectrum = new Spectrum();
}

Spectrum_analyzer::~Spectrum_analyzer()
{
  delete spectrum;
}

Spectrum* Spectrum_analyzer::get_spectrum(char* path)
{
  WAV_parser wp(path);

  WAV_descriptor wd = wp.get_wav_descriptor();
  ulong count = wd.subChunk2Size / wd.sampleSize / FFT_CHUNK_SIZE;

  spectrum->reserve( FFT_CHUNK_SIZE / 2 );

  double* in;
  fftw_complex *out;
  fftw_plan p;
  in = (double*) fftw_malloc(sizeof(double) * FFT_CHUNK_SIZE);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_CHUNK_SIZE);
  p = fftw_plan_dft_r2c_1d(FFT_CHUNK_SIZE, in, out, FFTW_ESTIMATE);
  for ( long i = 0; i < count; ++i )
  {
    wp.fill_next_fft_chunk(FFT_CHUNK_SIZE, in);
    // apply window
    for ( long j = 0; j < FFT_CHUNK_SIZE; ++j )
      in[j] = in[j] * 0.5 * (1 - cos(2 * M_PI * j / FFT_CHUNK_SIZE));
    fftw_execute(p);
    for ( long j = 1; j < FFT_CHUNK_SIZE / 2; ++j )
      spectrum->ampl[j] += sqrtl( out[j][0] * out[j][0] + out[j][1] * out[j][1] );
  }

  double max = 0;
  for ( long j = 1; j < FFT_CHUNK_SIZE / 2; ++j )
  {
    spectrum->freq[j] = j / ((double) FFT_CHUNK_SIZE / wd.sampleRate );
    if ( spectrum->ampl[j] > max )
      max = spectrum->ampl[j];
  }

  for ( long j = 1; j < FFT_CHUNK_SIZE / 2; ++j )
    spectrum->ampl[j] /= max;

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
  return spectrum;
}
