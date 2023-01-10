// Test code for Ultimate GPS Using Hardware Serial (e.g. GPS Flora or FeatherWing)
//
// This code shows how to listen to the GPS module via polling. Best used with
// Feathers or Flora where you have hardware Serial and no interrupt
//
// Tested and works great with the Adafruit GPS FeatherWing
// ------> https://www.adafruit.com/products/3133
// or Flora GPS
// ------> https://www.adafruit.com/products/1059
// but also works with the shield, breakout
// ------> https://www.adafruit.com/products/1272
// ------> https://www.adafruit.com/products/746
//
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada

#include <Adafruit_GPS.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <DFRobot_VEML6075.h>


// what's the name of the hardware serial port?
//#define GPSSerial Serial1
UART mySerial(digitalPinToPinName(4), digitalPinToPinName(3), NC, NC); // TX RX
#define GPSSerial mySerial


// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO true

uint32_t timer = millis();

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

#define VEML6075_ADDR   0x10

DFRobot_VEML6075_IIC VEML6075(&Wire, VEML6075_ADDR);  // create object

void setup()
{
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial1.begin(9600);
  Serial1.println("CanSat Data transmission Version 0!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

  unsigned status;

      status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial1.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial1.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial1.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial1.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial1.print("        ID of 0x60 represents a BME 280.\n");
        Serial1.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

  while(VEML6075.begin() != true) {
    Serial1.println("VEML6075 begin faild");
    delay(2000);
  }
  Serial1.println("VEML6075 begin successed");

}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial1.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial1.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    printValues();
    valuesUV();
    timer = millis(); // reset the timer
    Serial1.print("\nTime: ");
    if (GPS.hour < 10) { Serial1.print('0'); }
    Serial1.print(GPS.hour, DEC); Serial1.print(':');
    if (GPS.minute < 10) { Serial1.print('0'); }
    Serial1.print(GPS.minute, DEC); Serial1.print(':');
    if (GPS.seconds < 10) { Serial1.print('0'); }
    Serial1.print(GPS.seconds, DEC); Serial1.print('.');
    if (GPS.milliseconds < 10) {
      Serial1.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial1.print("0");
    }
    Serial1.println(GPS.milliseconds);
    Serial1.print("Date: ");
    Serial1.print(GPS.day, DEC); Serial1.print('/');
    Serial1.print(GPS.month, DEC); Serial1.print("/20");
    Serial1.println(GPS.year, DEC);
    Serial1.print("Fix: "); Serial1.print((int)GPS.fix);
    Serial1.print(" quality: "); Serial1.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial1.print("Location: ");
      Serial1.print(GPS.latitude, 4); Serial1.print(GPS.lat);
      Serial1.print(", ");
      Serial1.print(GPS.longitude, 4); Serial1.println(GPS.lon);
      Serial1.print("Speed (knots): "); Serial1.println(GPS.speed);
      Serial1.print("Angle: "); Serial1.println(GPS.angle);
      Serial1.print("Altitude: "); Serial1.println(GPS.altitude);
      Serial1.print("Satellites: "); Serial1.println((int)GPS.satellites);
      Serial1.print("Antenna status: "); Serial1.println((int)GPS.antenna);
    }
  }
}

void printValues() {
    Serial1.print("T:");
    Serial1.print(bme.readTemperature());

    Serial1.print(" P:");
    Serial1.print(bme.readPressure() / 100.0F);

    Serial1.print(" ~A:");
    Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));

    Serial1.print(" H:");
    Serial1.print(bme.readHumidity());

    Serial1.println();
}

void valuesUV(){
  uint16_t    UvaRaw = VEML6075.readUvaRaw();         // read UVA raw
  uint16_t    UvbRaw = VEML6075.readUvbRaw();         // read UVB raw
  uint16_t    comp1Raw = VEML6075.readUvComp1Raw();   // read COMP1 raw
  uint16_t    comp2Raw = VEML6075.readUvComp2Raw();   // read COMP2 raw

  float       Uva = VEML6075.getUva();                // get UVA
  float       Uvb = VEML6075.getUvb();                // get UVB
  float       Uvi = VEML6075.getUvi(Uva, Uvb);        // get UV index

  Serial1.println();
  Serial1.println("======== start print ========");
  Serial1.print("UVA raw:    ");
  Serial1.println(UvaRaw);
  Serial1.print("UVB raw:    ");
  Serial1.println(UvbRaw);
  Serial1.print("COMP1 raw:  ");
  Serial1.println(comp1Raw);
  Serial1.print("COMP2 raw:  ");
  Serial1.println(comp2Raw);
  Serial1.print("UVA:        ");
  Serial1.println(Uva, 2);
  Serial1.print("UVB:        ");
  Serial1.println(Uvb, 2);
  Serial1.print("UVIndex:    ");
  Serial1.print(Uvi, 2);
  if(Uvi < UVI_LOW)
    Serial1.println("  UVI low");
  else if(Uvi < UVI_MODERATE)
    Serial1.println("  UVI moderate");
  else if(Uvi < UVI_HIGH)
    Serial1.println("  UVI high");
  else if(Uvi < UVI_VERY_HIGH)
    Serial1.println("  UVI very high");
  else
    Serial1.println("  UVI extreme");
  Serial1.print("mw/cm^2:    ");
  Serial1.println(Uvi2mwpcm2(Uvi), 2);
  Serial1.println("======== end print ========");
}
