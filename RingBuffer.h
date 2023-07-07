#pragma once

#include <Arduino.h>

class RingBuffer {
public:

  RingBuffer(void);
  RingBuffer(uint32_t size);
  ~RingBuffer();

  uint32_t available(void);
  void write(uint8_t *data, uint32_t len);
  uint32_t read(uint8_t *data, uint32_t len);

private:
  uint8_t *_buffer = NULL;

  uint32_t _bufferSize = 0;
  uint32_t _writeIndex = 0;
  uint32_t _readIndex = 0;
  uint32_t _availableBytes = 0;
};

