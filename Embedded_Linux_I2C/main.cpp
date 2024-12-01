#include <iostream>

#include "SHT21.h"

using namespace std;

int main(int argc, char *args[]) {

    try {

        I2C i2c_bus(1);
        SHT21 sensor(i2c_bus);

        if(!sensor) {
            exit(1);
        }


        count << "Sensor Connected " << endl;
        for(;;) {
            cout << sensor.getTemperatureC() << " degC" << endl;
            cout << sensor.getHumidity() << "% RH" << endl;
            cout << endl;
            sleep(1);
        }
    }
    catch(Exception e) {
        cout << e.what() << endl;
    }
    return 0;
}
