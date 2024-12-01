#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include <iostream>

using namespace std;

class timeout_error : public runtime_error {

};

class device_not_found_error : public runtime_error {

};

class device_unresponsive_error : public runtime_error {

};


class I2C {
  private:
    int fd;

    void error(int error_num) {
        switch(error_num) {
            case ETIMEDOUT : throw timeout_error();
            case ENXIO : throw device_not_found_error();
            case ENODEV : throw device_unresponsive_error();
        }
    }

  public:
    I2C(const int i2c_dev) {
        char i2c_path[16];
        snprintf(i2c_path, sizeof(i2c_path), "/dev/i2c-%d", i2c_dev);
        if((fd = open(i2c_path, O_RDWR)) < 0) {
            
        }
    }

    ~I2C() {
        close(fd);
    }

    inline uint8_t read(uint8_t slv_addr) {
        uint8_t data;
        read(fd, &data, 1);
        return data;
    }

    inline void write(uint8_t slv_addr, const char data) {
        write(fd, &data, 1);
    }


    inline void read(uint8_t slv_addr, char *buffer, size_t size) {
        int error_num = 0;
        struct i2c_msg msgs[1];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = slv_addr;
        msgs[0].flags = 0;
        msgs[0].len = size;
        msgs[0].buf = buffer;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 1;

        if ((error_num = ioctl(fd, I2C_RDWR, &msgset)) < 0)
            error(error_num);
    }

    inline void write(uint8_t slv_addr, const char *buffer, size_t size) {
        int error_num = 0;
        struct i2c_msg msgs[1];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = slv_addr;
        msgs[0].flags = I2C_M_RD;
        msgs[0].len = size;
        msgs[0].buf = buffer;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 1;

        if ((error_num = ioctl(fd, I2C_RDWR, &msgset)) < 0)
            error(error_num);
    }


    inline void transfer(uint8_t slv_addr, char *buffer, size_t wrtSz, size_t rdSz) {
        int error_num = 0;
        struct i2c_msg msgs[1];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = slv_addr;
        msgs[0].flags = 0;
        msgs[0].len = wrtSz;
        msgs[0].buf = buffer;

        msgs[1].addr = slv_addr;
        msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
        msgs[1].len = rdSz;
        msgs[1].buf = buffer;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 2;

        if ((error_num = ioctl(fd, I2C_RDWR, &msgset)) < 0)
            error(error_num);
    }
};
