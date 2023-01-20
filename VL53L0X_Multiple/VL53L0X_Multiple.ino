#include <VL53L0X.h> //Download this library from https://github.com/pololu/vl53l0x-arduino
#include <Wire.h>

VL53L0X sensor_A; //Create the sensor object
VL53L0X sensor_B; //Create the sensor object

int startTime = millis(); //used for our timing loop
int mInterval = 100; //refresh rate of 10hz

#define XSHUT_A 10
#define XSHUT_B 8

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
        //SET THE SENSOR TO LONG RANGE MODE
        // lower the return signal rate limit (default is 0.25 MCPS)
        sensor_A.setSignalRateLimit(0.1);
        // increase laser pulse periods (defaults are 14 and 10 PCLKs)
        sensor_A.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        sensor_A.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
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
        //SET THE SENSOR TO LONG RANGE MODE
        // lower the return signal rate limit (default is 0.25 MCPS)
        sensor_B.setSignalRateLimit(0.1);
        // increase laser pulse periods (defaults are 14 and 10 PCLKs)
        sensor_B.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        sensor_B.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
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
        Serial.println(sensor_A.readRangeContinuousMillimeters()); //Get a reading in millimeters
        Serial.print("Sensor_B Reading: ");
        Serial.println(sensor_B.readRangeContinuousMillimeters()); //Get a reading in millimeters
        startTime = millis();
    }
}
