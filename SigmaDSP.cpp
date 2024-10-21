#include <Arduino.h>
#include <Wire.h>
#include <mbLog.h>

#include "SigmaDSP.h"
#include "ADAU1701_REG.h"

// missing decls...
const int16_t DC1 = 2074;
const int16_t DC2 = 2075;


SigmaDSP::SigmaDSP()
{
    // DSP default adr
    addr = 0x68 >> 1;
}

void SigmaDSP::begin()
{
    Wire.begin();
}

bool SigmaDSP::saveloadWrite(uint16_t address, uint32_t value)
{
    bool result = true;
    // 1. write data to 0x0810 (5 bytes for whatever reason - SigmaStudio does not show the 5th !)
    // 2. write target addr to 0x0815 - doc echt
    //    repeat for up to 5 val/addr pairs in to 0x0811,12,13,14 and 16,17,18,19
    // 3 write exec command to CoreRegister 0x081C < 0x0034 ? das default steht in so nem header.. 1C ? + IST bit 

    uint16_t coreregval = REG_COREREGISTER_IC_1_VALUE | R13_SAFELOAD_IC_1_MASK;

    // LOG <<LOG.hex <<"saveload hex: " <<LOG.endl;
    // LOG <<uint8_t(0x08);          // address high byte
    // LOG <<uint8_t(0x10);        // address low byte
    // LOG <<" ";
    // LOG <<uint8_t(value >> 24);   // address high byte
    // LOG <<uint8_t(value >> 16); // address low byte
    // LOG <<uint8_t(value >> 8);   // address high byte
    // LOG <<uint8_t(value); // address low byte
    // LOG <<LOG.endl;
    // LOG <<uint8_t(0x08);          // address high byte
    // LOG <<uint8_t(0x15);        // address low byte
    // LOG <<" ";
    // LOG <<uint8_t(address >> 8);   // address high byte
    // LOG <<uint8_t(address); // address low byte
    // LOG <<LOG.endl;
    // LOG <<uint8_t(REG_COREREGISTER_IC_1_ADDR >> 8);          // address high byte
    // LOG <<uint8_t(REG_COREREGISTER_IC_1_ADDR);        // address low byte
    // LOG <<" ";
    // LOG <<uint8_t(coreregval >> 8);   // address high byte
    // LOG <<uint8_t(coreregval); // address low byte
    // LOG <<LOG.dec <<LOG.endl;

    // return;

    beginTransmission();
    write16(0x0810);        // target data 0
    Wire.write(0);   // bits 39-32 ???
    Wire.write(value >> 24);   // address high byte
    Wire.write(value >> 16); // address low byte
    Wire.write(value >> 8);   // address high byte
    Wire.write(value); // address low byte
    result &= endTransmission();
    if(!result) return false;

    beginTransmission();
    write16(0x0815);        // target address 0
    write16(address);
    result &= endTransmission();
    if(!result) return false;

    beginTransmission();
    write16(REG_COREREGISTER_IC_1_ADDR);
    write16(coreregval);
    result &= endTransmission();

    return result;
}

bool SigmaDSP::saveloadWrite(uint16_t address1, uint32_t value1, uint16_t address2, uint32_t value2)
{
    // 1. write data to 0x0810 (5 bytes for whatever reason - SigmaStudio does not show the 5th !)
    // 2. write target addr to 0x0815 - doc echt
    //    repeat for up to 5 val/addr pairs in to 0x0811,12,13,14 and 16,17,18,19
    // 3 write exec command to CoreRegister 0x081C < 0x0034 ? das default steht in so nem header.. 1C ? + IST bit 

    bool result = true;

    uint16_t coreregval = REG_COREREGISTER_IC_1_VALUE | R13_SAFELOAD_IC_1_MASK;

    beginTransmission();
    write16(0x0810);        // target data 0
    Wire.write(0);   // bits 39-32 ???
    Wire.write(value1 >> 24);   // address high byte
    Wire.write(value1 >> 16); // address low byte
    Wire.write(value1 >> 8);   // address high byte
    Wire.write(value1 & 0xff); // address low byte
    result &= endTransmission();
    if(!result) return false;

    beginTransmission();
    write16(0x0815);        // target address 0
    Wire.write(address1 >> 8);   // address high byte
    Wire.write(address1); // address low byte
    result &= endTransmission();
    if(!result) return false;

    beginTransmission();
    write16(0x0810+1);        // target data 1
    Wire.write(0);   // bits 39-32 ???
    Wire.write(value2 >> 24);   // address high byte
    Wire.write(value2 >> 16); // address low byte
    Wire.write(value2 >> 8);   // address high byte
    Wire.write(value2 & 0xff); // address low byte
    result &= endTransmission();
    if(!result) return false;

    beginTransmission();
    write16(0x0815+1);        // target address 1
    Wire.write(address2 >> 8);   // address high byte
    Wire.write(address2); // address low byte
    result &= endTransmission();
    if(!result) return false;

    beginTransmission();
    write16(REG_COREREGISTER_IC_1_ADDR);
    write16(coreregval);
    result &= endTransmission();

    return result;
}

bool SigmaDSP::saveloadWrite5(const uint16_t address, const float *values)
{
    bool result = true;

    uint16_t coreregval = REG_COREREGISTER_IC_1_VALUE | R13_SAFELOAD_IC_1_MASK;
    uint32_t dspval = 0; 

    // data
    beginTransmission();
    write16(0x0810);        // target data 0
    for(int i = 0; i < 5; i++) 
    {
        dspval = floatTo523(values[i]);
        Wire.write(0);   // bits 39-32 ???
        Wire.write(dspval >> 24);   // address high byte
        Wire.write(dspval >> 16); // address low byte
        Wire.write(dspval >> 8);   // address high byte
        Wire.write(dspval & 0xff); // address low byte
    }
    result &= endTransmission();
    if(!result) return false;

    // addresses
    beginTransmission();
    write16(0x0815);        // target addr 0
    for(int i = 0; i < 5; i++)
    {
        write16(address+i);
    }
    result &= endTransmission();
    if(!result) return false;

    beginTransmission();
    write16(REG_COREREGISTER_IC_1_ADDR);
    write16(coreregval);
    result &= endTransmission();

    return result;
}


bool SigmaDSP::setDataCapture(uint16_t address1, uint16_t address2)
{
    beginTransmission();
    write16(DC1);
    write16(address1);
    write16(address2);
    return endTransmission();
}

bool SigmaDSP::readDataCapture(int32_t &val1, int32_t &val2)
{
    bool result = true;

    // send addr to read, no stop condition... then read...
    beginTransmission();
    write16(DC1);
    result &= endTransmission(false); // no stop before reading !
    // LOG << "r2: " <<result << LOG.endl;

    if(!result) return false;

    // read...

    uint8_t data[6 * 4];
    byte readlen = 6; // 2x3

    Wire.requestFrom(addr, readlen);
    int idx = 0;
    while (Wire.available()) // slave may send less than requested
    {
        data[idx++] = Wire.read(); // receive a byte as character
        if (idx > readlen)
            break;
    }

    if (idx == 6)
    {
        // 24bit 5.19 fixpoint value. The overall shift is needed for negative values.
        val1 = (((int32_t)data[2] << 8) | ((int32_t)data[1] << 16) | ((int32_t)data[0] << 24)) >> 8;
        val2 = (((int32_t)data[5] << 8) | ((int32_t)data[4] << 16) | ((int32_t)data[3] << 24)) >> 8;
    }
    else
    {
        result = false;
    }

    return result;
}

// low level api

/// @brief Starts a transmissing on communication bus
void SigmaDSP::beginTransmission()
{
    Wire.beginTransmission(addr);
}

/// @brief Finish transmission, check result, stops DSP activity on error
bool SigmaDSP::endTransmission(boolean sendStop)
{
    uint8_t result = Wire.endTransmission(sendStop);
    if (result != 0)
    {
        LOG << "saveloadWrite:i2c error " << result << LOG.endl;
        return false;
    }
    return true;
}

void SigmaDSP::write16(uint16_t val)
{
    Wire.write(val >> 8);   // address high byte
    Wire.write(val & 0xff); // address low byte
}
