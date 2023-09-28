#pragma once

#include <vector>

#define FRAME_HEAD 0x55AA

#pragma pack(1)
typedef struct tuyaMcuWifiFrame{
    uint8_t head[2];
    uint8_t version;
    uint8_t cmd;
    uint16_t len;
    uint8_t data[0];
} TUYA_MCU_WIFI_FRAME;
#pragma pack()

// wifi frame structure length
#define FRAME_HEADER_LEN    2
#define FRAME_VERSION_LEN   1
#define FRAME_CMD_LEN       1
#define FRAME_LEN_LEN       2
#define FRAME_CHECKSUM_LEN  1

#define FRAME_HEADER_INDEX  0
#define FRAME_VERSION_INDEX (FRAME_HEADER_INDEX + FRAME_HEADER_LEN)
#define FRAME_CMD_INDEX     (FRAME_VERSION_INDEX + FRAME_VERSION_LEN)
#define FRAME_LEN_INDEX     (FRAME_CMD_INDEX + FRAME_CMD_LEN)
#define FRAME_DATA_INDEX    (FRAME_LEN_INDEX + FRAME_LEN_LEN)

class frameProcess {
public:
  frameProcess(void);
  ~frameProcess(void);

  void dataInput(uint8_t* data, uint8_t len);
  void dataInput(uint8_t data);
private:
  // frame list data
  struct frameList {
    struct tuyaMcuWifiFrame *frameData;
    struct tuyaMcuWifiFrame *next;
  };

  uint8_t parse(void);
  uint32_t findFrameHeader(void);
  uint8_t checkSum(void);

  std::vector<uint8_t> _recvData;

  // frame minimum length
  const uint16_t _miniFrameLen = FRAME_HEADER_LEN + FRAME_VERSION_LEN + FRAME_CMD_LEN + FRAME_LEN_LEN + FRAME_CHECKSUM_LEN;

  // parse frame list
  struct frameList *_frameHead = NULL;
  struct frameList *_frameTail = NULL;
};