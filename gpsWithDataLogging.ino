#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>
// 20150403 - This code grabs the GPS information and logs it to the SD card as well as writing it to the
//            serial debugging screen.
//            Current fields: Time:Date:Lat:Lon:heading to camp
//

static const int RXPin = 7, TXPin = 6;
static const uint32_t GPSBaud = 4800;

// The TinyGPS++ object
TinyGPSPlus gps;
//TinyGPSTime myTime;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  const int chipSelect = 8;
  {
  // Open serial communications and wait for port to open:
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to output, even if you don't use it:
  pinMode(10, OUTPUT);
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");  
  }
}
// set up scratch space variables to conver floats to strings below
char tempLat[15];
char tempLon[15];
char tempHomeDeg[10];
//char tempManDeg[10];
//char tempSpeed[10];


void loop()
{
//
//   ___  ___| |_  | | __ _| |_  | | ___  _ __   | |__   ___ _ __ ___ 
//  / __|/ _ \ __| | |/ _` | __| | |/ _ \| '_ \  | '_ \ / _ \ '__/ _ \
//  \__ \  __/ |_  | | (_| | |_  | | (_) | | | | | | | |  __/ | |  __/
//  |___/\___|\__| |_|\__,_|\__| |_|\___/|_| |_| |_| |_|\___|_|  \___|
//
  float HOME_LAT = 39.869491, HOME_LON = -75.431720;
  //float MAN_LAT = 40.786962, MAN_LON = -119.204305;
//
//
//
  // Find the course to "Home"
  double courseToHome =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      HOME_LAT, 
      HOME_LON);
  // Find the course to the man
//  double courseToTheMan =
//    TinyGPSPlus::courseTo(
//      gps.location.lat(),
//      gps.location.lng(),
//      MAN_LAT, 
//      MAN_LON);
      
  // Convert floats to strings    
  dtostrf(gps.location.lng(),3,7,tempLon);
  dtostrf(gps.location.lat(),3,7,tempLat);
  dtostrf(courseToHome,3,2,tempHomeDeg);
 // dtostrf(courseToTheMan,3,2,tempManDeg);
 // dtostrf(gps.speed.mph(),3,2,tempSpeed);
  
  // Define output string (since it's used twice)
  String printString = String(gps.time.hour()) + String(gps.time.minute()) + String(gps.time.second()) + "|" + String(gps.date.year()) + String(gps.date.month()) + String(gps.date.day()) + "|" + String(tempLat) + "|" + String(tempLon) + "|" + String(tempHomeDeg);// + "|" + String(tempManDeg);
  // Open data file for write; write to it; close it
  File dataFile = SD.open("gpslog.txt", FILE_WRITE);
  dataFile.println(printString);
  dataFile.close();
  // Print data to terminal as well. 
  Serial.println(printString); 
  // Wait a couple seconds!
  delay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}
