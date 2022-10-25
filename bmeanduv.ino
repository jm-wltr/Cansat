#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <DFRobot_VEML6075.h>

#define VEML6075_ADDR   0x10

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

DFRobot_VEML6075_IIC VEML6075(&Wire, VEML6075_ADDR);  // create object
Adafruit_BME280 bme; // I2C

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    unsigned status;
    
    // default settings
    status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x");
        Serial.println(bme.sensorID(),16);
        Serial.print("Error! ");
        while (1) delay(10);
    }
    
    Serial.println("-- BME STARTED --");
    delayTime = 1000;

    Serial.println();

    Serial.begin(9600);
    delay(2000);
    while(!Serial);

    Serial.println();
    while(VEML6075.begin() != true) {
      Serial.println("VEML6075 begin failed");
      delay(2000);
    }
    Serial.println("VEML6075 begin successed");
}

void printValues() {
    
    Serial.print(bme.readTemperature());
    Serial.print(", ");
    

    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(", ");
    
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print(", ");

    Serial.print(bme.readHumidity());
    
 

    Serial.print(", ");
}

void loop() { 
    printValues();
    delay(10); //used to be delayTime

    uint16_t    UvaRaw = VEML6075.readUvaRaw();         // read UVA raw
  uint16_t    UvbRaw = VEML6075.readUvbRaw();         // read UVB raw
  uint16_t    comp1Raw = VEML6075.readUvComp1Raw();   // read COMP1 raw
  uint16_t    comp2Raw = VEML6075.readUvComp2Raw();   // read COMP2 raw

  float       Uva = VEML6075.getUva();                // get UVA
  float       Uvb = VEML6075.getUvb();                // get UVB
  float       Uvi = VEML6075.getUvi(Uva, Uvb);        // get UV index


  Serial.print(UvaRaw); //UVA raw: radiation FROM THE SUN with fairly long wavelengths. (for UVA from 350 nm to 375 nm)
  Serial.print(", ");
  Serial.print(UvbRaw); //UVB raw: It can come from natural sources, such as sunlight, as well as artificial sourcesrange (for UVB is from 315 nm to 340 nm)
  Serial.print(", ");
  Serial.print(comp1Raw); //COMP1 raw
  Serial.print(", ");  
  Serial.print(comp2Raw);
  Serial.print(", ");  
  Serial.print(Uva, 2); //UVA
  Serial.print(", ");
  Serial.print(Uvb, 2); //UVB
  Serial.print(", ");
  Serial.print(Uvi, 2); //UVI index
  Serial.print(", ");
  //if(Uvi < UVI_LOW)
  //  Serial.println("  UVI low");
  //else if(Uvi < UVI_MODERATE)
  //  Serial.println("  UVI moderate");
  //else if(Uvi < UVI_HIGH)
  //  Serial.println("  UVI high");
  //else if(Uvi < UVI_VERY_HIGH)
  //  Serial.println("  UVI very high");
  //else
  //  Serial.println("  UVI extreme");
  Serial.print(Uvi2mwpcm2(Uvi), 2); // mw/cm^2
  Serial.print(", ");
  Serial.print("\n");
  delay(990); //used to be 1000
}
