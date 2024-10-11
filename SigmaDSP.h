#pragma once

#include <Arduino.h>

class DSPCom {

};

class SigmaDSP {
  public:
    SigmaDSP();

    void begin();

    bool saveloadWrite(uint16_t address, uint32_t value);
    bool saveloadWrite(uint16_t address1, uint32_t value1,
                       uint16_t address2, uint32_t value2);

    bool setDataCapture(uint16_t address1, uint16_t address2);
    bool readDataCapture(int32_t &val1, int32_t &val2);

    inline uint8_t getAddr() { return addr; }
    inline uint32_t floatTo523(float val) { return val * (float)0x0800000; }

    inline float from519(int32_t value) { return ( 1.0f/  (float)0x080000 ) * value; }


    // low level access / Wire pass through
    void beginTransmission();
    bool endTransmission(boolean sendStop=true);
    void write16(uint16_t val);

  private:


    
    /// @brief DSP I2C address
    uint8_t addr = 0;

};
