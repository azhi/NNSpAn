#include "wav_parser.h"

#include <stdlib.h>

WAV_parser::WAV_parser(char* path)
{
  init(path);
}

WAV_parser::~WAV_parser()
{
  fclose(input);
}

void WAV_parser::init(char* path)
{
  #define clear_buf\
    buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 0;
  input = fopen(path, "rb");
  buf = (byte*) malloc(sizeof(byte) * 4);
  // reading chunk descriptors
  fread(buf, 1, 4, input); // ignoring RIFF
  clear_buf;
  fread(buf, 1, 4, input);
  wd.chunkSize = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
  fread(buf, 1, 4, input); // ignoring WAVE

  fread(buf, 1, 4, input); // ignoring fmt
  clear_buf;
  fread(buf, 1, 4, input);
  wd.subChunk1Size = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
  ulong skipped_bytes = wd.subChunk1Size - 16;
  clear_buf;
  fread(buf, 1, 2, input);
  wd.audioFormat = (buf[1] << 8) | buf[0];
  clear_buf;
  fread(buf, 1, 2, input);
  wd.numChannels = (buf[1] << 8) | buf[0];
  clear_buf;
  fread(buf, 1, 4, input);
  wd.sampleRate = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
  clear_buf;
  fread(buf, 1, 4, input);
  wd.byteRate = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
  clear_buf;
  fread(buf, 1, 2, input);
  wd.blockAlign = (buf[1] << 8) | buf[0];
  clear_buf;
  fread(buf, 1, 2, input);
  wd.bitsPerSample = (buf[1] << 8) | buf[0];

  if ( skipped_bytes != 0 )
    fread(buf, 1, skipped_bytes, input);

  fread(buf, 1, 4, input); // ignoring data
  clear_buf;
  fread(buf, 1, 4, input);
  wd.subChunk2Size = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];

  wd.sampleSize = (wd.bitsPerSample / 8) * wd.numChannels;

  #undef clear_buf

  fprintf(stderr, "chunkSize:     %lu\n", wd.chunkSize);
  fprintf(stderr, "subChunk1Size: %lu\n", wd.subChunk1Size);
  fprintf(stderr, "audioFormat:   %u\n", wd.audioFormat);
  fprintf(stderr, "numChannels:   %u\n", wd.numChannels);
  fprintf(stderr, "sampleRate:    %lu\n", wd.sampleRate);
  fprintf(stderr, "byteRate:      %lu\n", wd.byteRate);
  fprintf(stderr, "blockAlign:    %u\n", wd.blockAlign);
  fprintf(stderr, "bitsPerSample: %u\n", wd.bitsPerSample);
  fprintf(stderr, "subChunk2Size: %lu\n", wd.subChunk2Size);
  fprintf(stderr, "\n");

  delete buf;
  buf = NULL;
  buf_capacity = 0;
}

void WAV_parser::fill_next_fft_chunk(size_t samples, fftw_complex* ar)
{
  if ( buf_capacity < wd.sampleSize * samples )
    buf = (byte*) realloc(buf, wd.sampleSize * samples);
  size_t readed_bytes = fread(buf, 1, wd.sampleSize * samples, input);
  size_t readed_samples = readed_bytes / wd.sampleSize;
  for ( long i = 0; i < readed_samples; ++i )
  {
    int sample_int;
    byte hi = buf[2 * i + 1];
    byte lo = buf[2 * i];
    bool neg = false;
    if ( (hi & 128) == 128 )
      neg = true;
    hi &= 127;
    sample_int = (hi << 8) | lo;
    if ( neg )
      sample_int *= -1;
    ar[i][0] = sample_int;
  }
  for ( long i = readed_samples; i < samples; ++i )
    ar[i][0] = 0;
}
