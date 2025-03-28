#include <Arduino.h>

class DataEncoder {
private:
  static const uint8_t START_STOP_BYTE = 0x6E;
  static const uint8_t MAX_BUFFER_SIZE = 64;
  
  uint8_t buffer[MAX_BUFFER_SIZE];
  uint8_t bufferIndex = 0;

public:
  // Direct encoding of a string to a byte array
  size_t encodeString(const char* input, uint8_t* output) {
    size_t inputLen = strlen(input);
    
    if (inputLen > (MAX_BUFFER_SIZE - 2)) {
      return 0; // Encoding failed
    }
    
    output[0] = START_STOP_BYTE;
    output[1] = inputLen;
    
    memcpy(output + 2, input, inputLen);
    
    output[inputLen + 2] = START_STOP_BYTE;
    return inputLen + 3; // Total length of encoded message
  }
  
  // Buffered encoding methods
  bool addToBuffer(uint8_t data) {
    if (bufferIndex >= MAX_BUFFER_SIZE) {
      return false; // Buffer full
    }
    
    buffer[bufferIndex++] = data;
    return true;
  }
  
  // Encode the current buffer contents
  size_t encodeBuffer(uint8_t* output) {
    if (bufferIndex == 0) {
      return 0; // No data in buffer
    }
    
    output[0] = START_STOP_BYTE;
    output[1] = bufferIndex;

    memcpy(output + 2, buffer, bufferIndex);
    
    output[bufferIndex + 2] = START_STOP_BYTE;
    
    size_t encodedLength = bufferIndex + 3;
    bufferIndex = 0;
    
    return encodedLength;
  }
  
  // Clear the buffer without encoding
  void clearBuffer() {
    bufferIndex = 0;
  }
  
  // Get current buffer usage
  uint8_t getBufferSize() {
    return bufferIndex;
  }
};