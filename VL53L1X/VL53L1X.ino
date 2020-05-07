#include <VL53L1X.h> //Download this library from https://github.com/pololu/vl53l1x-arduino
#include <Wire.h>

VL53L1X sensor; //Create the sensor object

int startTime = millis(); //used for our timing loop
int mInterval = 100; //refresh rate of 10hz

void setup() {

    Serial.begin(57600);
    
    Wire.begin(); //Setup your I2C interface
    Wire.setClock(400000); // use 400 kHz I2C

    sensor.setTimeout(500); //Set the sensors timeout
    
    if (!sensor.init())//try to initilise the sensor
    {
        //Sensor does not respond within the timeout time
        Serial.println("VL53L1X is not responding, check your wiring");
    }
    else
    {
        sensor.setDistanceMode(VL53L1X::Long); //Set the sensor to maximum range of 4 meters
        sensor.setMeasurementTimingBudget(40000); //Set its timing budget in microseconds longer timing budgets will give more accurate measurements
        sensor.startContinuous(50); //Sets the interval where a measurement can be requested in milliseconds
    }   
}

void loop() {
    //We have to be careful here. If we request a measurement before the measurement has been taken your
    //code will be blocked until the measurement is complete. In order to stop this from happening we
    //must ensure that time between measurement requests is greater than the timing budget and the argument 
    //given in the startContinuous() function. In our case our measurement time must be greater than 50mS.

    if((millis()- startTime) > mInterval)
    {
        Serial.println(sensor.read()); //Get a reading in millimeters
        startTime = millis();
    }
}
