#include "RingBuffer.h"

RingBuffer::RingBuffer(void)
{
  _bufferSize = 512;
  _buffer = new uint8_t[_bufferSize];

  return;
}

RingBuffer::RingBuffer(uint32_t size)
{
  _bufferSize = size;
  _buffer = new uint8_t[_bufferSize];

  return;
}

RingBuffer::~RingBuffer() {
  delete[] _buffer;
}

uint32_t RingBuffer::available(void)
{
  return _availableBytes;
}

void RingBuffer::write(uint8_t *data, uint32_t len)
{
  uint32_t i = 0;

  for (i=0; i<len; i++) {
    _buffer[_writeIndex] = data[i];
    _writeIndex = (_writeIndex+1) % _bufferSize;
    _availableBytes++;
    // overwrite
    if (_availableBytes > _bufferSize) {
      _availableBytes = _bufferSize;
      _readIndex = (_readIndex + 1) % _bufferSize;
    }
  }

  return;
}

uint32_t RingBuffer::read(uint8_t *data, uint32_t len)
{
  uint32_t bytesRead = 0;
  uint32_t i = 0;

  for (i=0; i<len; i++) {
    if (_availableBytes > 0) {
      data[i] = _buffer[_readIndex];
      _readIndex = (_readIndex + 1) % _bufferSize;
      _availableBytes--;
      bytesRead++;
    } else {
      break;
    }
  }

  return bytesRead;
}
