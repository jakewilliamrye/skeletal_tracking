#ifndef ST_MESSAGE_HEADER_H_
#define ST_MESSAGE_HEADER_H_

class StMessageHeader {
  public:
    uint16_t tag;
    uint16_t payload_size;
};

#endif
