#include "I2C.h"

class TemperatureSensor {
    public:
        virtual float getTemperatureC() {
            return (getTemperatureF() - 32) * 5 / 9;
        }

        virtual float getTemperatureF() {
            return (getTemperatureC() * 9 / 5) + 32;
        }
};

class HumiditySensor {
  pubic:
    virtual float getHumidity() = 0;
};

class SHT21 : public TemperatureSensor, HumiditySensor {
  private:
    constexpr static int SLV_ADDR = 0x40;
    constexpr static int TRIG_TEMP_MEAS_HOLD = 0b11100011;
    constexpr static int TRIG_HUMD_MEAS_HOLD = 0b11100101;
    constexpr static int TRIG_TEMP_MEAS_NHOLD= 0b11110011;
    constexpr static int TRIG_HUMD_MEAS_NHOLD= 0b11110101;
    constexpr static int WRITE_USER_REG      = 0b11100110;
    constexpr static int READ_USER_REG       = 0b11100111;
    constexpr static int SOFT_RST            = 0b11111110;

    I2C& bus;

    inline uint16_t readTemperature() {
        char buffer[4];
        buffer[0] = TRIG_TEMP_MEAS_NHOLD;
        
        bus.write(SLV_ADDR, buffer, 1);
        usleep(100000UL);
        bus.read(SLV_ADDR, buffer, 2);
        
        return (buffer[1] | buffer[0] << 8);
    }

    inline uint16_t readHumidity() {
        char buffer[4];
        buffer[0] = TRIG_HUMD_MEAS_NHOLD;
        
        bus.write(SLV_ADDR, buffer, 1);
        usleep(50000UL);
        bus.read(SLV_ADDR, buffer, 2);

        return (buffer[1] | buffer[0] << 8);
    }

  public:
    
    SHT21(I2C& _bus) : bus(_bus) { }

    operator bool() {
        
    }

    virtual float getTemperatureC() {
        return 175.72 * ((float) readTemperature() / 65536) - 46.85;
    }

    virtual float getHumidity() {
        return 125 * ((float) readHumidity() / 65536) - 6;
    }
};
