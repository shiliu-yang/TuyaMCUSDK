#include "frameProcess.h"

frameProcess::frameProcess(void)
{
  return;
}

frameProcess::~frameProcess(void)
{
  return;
}

void frameProcess::dataInput(uint8_t *data, uint8_t len)
{
  uint8_t i = 0;

  if (data == NULL || len == 0) {
    return;
  }

  for (i = 0; i < len; i++) {
    dataInput(data[i]);
  }

  return;
}

void frameProcess::dataInput(uint8_t data)
{
  _recvData.push_back(data);
  parse();

  return;
}

uint8_t frameProcess::parse(void)
{
  uint32_t headIndex = 0;
  uint32_t frameDataLen = 0, frameSize = 0;
  uint8_t checkSum = 0;

  // not data
  if (_recvData.size() == 0) {
    return 0;
  }

  headIndex = findFrameHeader();

  // delete data before frame header
  if (headIndex > 0) {
    _recvData.erase(_recvData.begin(), _recvData.begin() + headIndex);
  }

  // not enough data
  if (_recvData.size() < _miniFrameLen) {
    return 0;
  }

  frameDataLen = _recvData[FRAME_LEN_INDEX] + (_recvData[FRAME_LEN_INDEX + 1] << 8);

  // a frame of data has not been received
  if (_recvData.size() < (FRAME_HEADER_LEN + FRAME_VERSION_LEN + FRAME_CMD_LEN + FRAME_LEN_LEN + frameDataLen + FRAME_CHECKSUM_LEN)) {
    return 0;
  }

  checkSum = _recvData[FRAME_DATA_INDEX + frameDataLen];

  frameSize = FRAME_DATA_INDEX + frameDataLen + FRAME_CHECKSUM_LEN;

  // check sum error
  if (checkSum != checkSum()) {
    _recvData.erase(_recvData.begin(), _recvData.begin() + frameSize);
    return 0;
  }

  // add frame to frame list
  struct frameList *newFrame = (struct frameList *)malloc(sizeof(struct frameList));
  if (newFrame == NULL) {
    _recvData.erase(_recvData.begin(), _recvData.begin() + frameSize);
    return 0;
  }
  newFrame->frameData = (struct tuyaMcuWifiFrame *)malloc(sizeof(struct tuyaMcuWifiFrame));
  if (newFrame->frameData == NULL) {
    free(newFrame);
    _recvData.erase(_recvData.begin(), _recvData.begin() + frameSize);
    return 0;
  }
  std::copy(_recvData.begin(), _recvData.begin() + frameSize, (uint8_t*)newFrame->frameData);

  if (_frameHead == NULL) {
    _frameHead = newFrame;
    _frameTail = newFrame;
  } else {
    _frameTail->next = newFrame;
    _frameTail = newFrame;
  }

  return 0;
}

uint32_t frameProcess::findFrameHeader(void)
{
  uint32_t i = 0;
  uint32_t len = _recvData.size();

  for (i = 0; i < len; i++) {
    if (_recvData[i] == (uint8_t)(FRAME_HEAD & 0xFF)) {
      if (i + 1 < len) {
        if (_recvData[i + 1] == (uint8_t)(FRAME_HEAD >> 8)) {
          return i;
        }
      }
    }
  }

  return 0;
}

uint8_t frameProcess::checkSum(void)
{
  uint32_t i = 0;
  uint32_t len = _recvData.size();
  uint8_t sum = 0;

  for (i = 0; i < len; i++) {
    sum += _recvData[i];
  }

  return sum;
}
