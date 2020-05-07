#include <VL53L1X.h> //Download this library from https://github.com/pololu/vl53l1x-arduino
#include <Wire.h>

VL53L1X sensor_A; //Create the sensor object
VL53L1X sensor_B; //Create the sensor object

int startTime = millis(); //used for our timing loop
int mInterval = 100; //refresh rate of 10hz

#define XSHUT_A 9
#define XSHUT_B 10

void setup() {

    Serial.begin(57600);
    
    Wire.begin(); //Setup your I2C interface
    Wire.setClock(400000); // use 400 kHz I2C

    //Set the pin mode to output
    pinMode(XSHUT_A ,OUTPUT);
    pinMode(XSHUT_B ,OUTPUT);
    
    //Turn all TOF's off
    digitalWrite(XSHUT_A, LOW);
    digitalWrite(XSHUT_B, LOW);

    //-----------------------------------------------------------------
    //FIRST WE WILL CONFIGURE AND SETUP SENSOR_A
    //-----------------------------------------------------------------
    delay(50);
    digitalWrite(XSHUT_A, HIGH); //Turn sensor_A on
    delay(50);
    
    sensor_A.setTimeout(500); //Set the sensors timeout
    
    if (!sensor_A.init())//try to initilise the sensor
    {
        //Sensor does not respond within the timeout time
        Serial.println("Sensor_A is not responding, check your wiring");
    }
    else
    {
        sensor_A.setAddress(42); //Set the sensors I2C address
        sensor_A.setDistanceMode(VL53L1X::Long); //Set the sensor to maximum range of 4 meters
        sensor_A.setMeasurementTimingBudget(40000); //Set its timing budget in microseconds longer timing budgets will give more accurate measurements
        sensor_A.startContinuous(45); //Sets the interval where a measurement can be requested in milliseconds
    }   

    //-----------------------------------------------------------------
    //NOW CONFIGURE AND SETUP SENSOR_B
    //-----------------------------------------------------------------
    delay(50);
    digitalWrite(XSHUT_B, HIGH); //Turn sensor_A on
    delay(50);
    
    sensor_B.setTimeout(500); //Set the sensors timeout
    
    if (!sensor_B.init())//try to initilise the sensor
    {
        //Sensor does not respond within the timeout time
        Serial.println("Sensor_A is not responding, check your wiring");
    }
    else
    {
        sensor_B.setAddress(43); //Set the sensors I2C address
        sensor_B.setDistanceMode(VL53L1X::Long); //Set the sensor to maximum range of 4 meters
        sensor_B.setMeasurementTimingBudget(40000); //Set its timing budget in microseconds longer timing budgets will give more accurate measurements
        sensor_B.startContinuous(45); //Sets the interval where a measurement can be requested in milliseconds
    }  
}

void loop() {
    //We have to be careful here. If we request a measurement before the measurement has been taken your
    //code will be blovked until the measurement is complete. In order to stop this from happening we
    //must ensure that time between measurement requests is greater than the timing budget and the argument 
    //given in the startContinuous() function. In our case our measurement time must be greater than 50mS.

    if((millis()- startTime) > mInterval)
    {
        Serial.print("Sensor_A Reading: ");
        Serial.println(sensor_A.read()); //Get a reading in millimeters
        Serial.print("Sensor_B Reading: ");
        Serial.println(sensor_B.read()); //Get a reading in millimeters
        startTime = millis();
    }
}
