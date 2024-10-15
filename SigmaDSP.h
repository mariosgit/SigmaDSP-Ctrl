#pragma once

#include <Arduino.h>

class DSPCom {

};

class SigmaDSP {
  public:
    SigmaDSP();

    void begin();

    /// @brief Wite a single value using saveload procedure
    /// @param address - register address
    /// @param value - DSP native binary
    /// @return 
    bool saveloadWrite(uint16_t address, uint32_t value);
    /// @brief Writes 2 values using saveload procedure
    /// @param address1 - register address
    /// @param value1 - DSP native binary
    /// @param address2 
    /// @param value2 
    /// @return 
    bool saveloadWrite(uint16_t address1, uint32_t value1,
                       uint16_t address2, uint32_t value2);
    /// @brief Converts and writes 5 values, e.g. biquad using saveload procedure
    /// @param address - start address, e.g. b0
    /// @param values - array of 5 floats
    /// @return 
    bool saveloadWrite5(const uint16_t address, const float *values);

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
