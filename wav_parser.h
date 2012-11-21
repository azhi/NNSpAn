#ifndef WAV_Parser_H_
#define WAV_Parser_H_

#include <stdio.h>
#include <fftw3.h>

typedef unsigned long ulong;
typedef unsigned char byte;
typedef unsigned int uint;

struct WAV_descriptor
{
  ulong chunkSize;
  ulong subChunk1Size;
  uint audioFormat;
  uint numChannels;
  ulong sampleSize;
  ulong sampleRate;
  ulong byteRate;
  uint blockAlign;
  uint bitsPerSample;
  ulong subChunk2Size;
};

class WAV_parser
{
public:
  WAV_parser(char* path);
  virtual ~WAV_parser();

  void init(char* path);
  void fill_next_fft_chunk(size_t samples, fftw_complex* ar);

  inline WAV_descriptor get_wav_descriptor() { return wd; };

private:
  WAV_descriptor wd;
  FILE* input;

  byte* buf;
  size_t buf_capacity;
};

#endif
