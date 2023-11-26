/***************************************************************************
  This is for the BME280 humidity, temperature & pressure sensor

  These sensors use I2C to interface. The device's I2C address is 0x77.

 ***************************************************************************/
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <Wire.h> // Needed for RTC
#include <SPI.h> // For 12c
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <RTClib.h>           // include Adafruit RTC library
#include <SdFat.h>
Adafruit_BME280 bme; // I2C

RTC_PCF8523 rtc;


// user defined filename and header
char filename[] = "AData_00.csv";  // filename must be 6 char 2 zeros
int buttonPushCounter = 0;   // counter for the number of button presses
unsigned long lastupdate = 0;  // timer value when last  update was done
uint32_t timer = 0;    
unsigned long delayTime;
//SD Card
SdFat sd;
SdFile logfile;

void setup() {
Serial.begin(9600);
// while (!Serial){; //Wait for user to open terminal
//}
 
 Serial.println();
  Serial.print("");
   Serial.print("                                         Weather Station Data Collection for DLGS" ); 
    Serial.println("\r\nby g3");
  Serial.println();
     Serial.print("\r\nInitalizing Sensors...");
  Serial.print("\r\n""");
  Serial.print("\nInit SD card...");
  Serial.println();

    if (! bme.begin(0x77, &Wire)) {
        Serial.println("Could not find BME280 !");
        while (1);
    }
  
if(!rtc.begin()){
  Serial.println("Couldnt find RTC..");
  while(1);
}

rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };


 // see if the card is present and can be initialized:
  if (!sd.begin(10)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");


  // create a new file name for each reset/start
  
    for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! sd.exists(filename)) {
       break;  // leave the loop!
       }
    } 


 // Open up the file we're going to log to!
 if (!logfile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }

  //print header
  logfile.println(F("Date,Time,Air Temperature,Ground Temperature, Pressure,Humidity,GPS Lattitude, GPS Longditude"));
  
  
#if ECHO_TO_SERIAL 
 Serial.print(F("setup done"));
#endif
  
} // end setup

void loop() {


    DateTime now = rtc.now();
    String dataString="";
    printValues();
    delay(100);
}
    

void printValues() 
{
   DateTime now = rtc.now();
   logfile.open(filename, FILE_WRITE);
    Serial.println();
    Serial.print("Temp = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print(F(" Ground Temperature = "));
    Serial.print(mlx.readObjectTempC()); 
    Serial.println("*C");
 
    Serial.print("Press = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");
  
    Serial.print("Hum = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %RH");
  

 logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(",");
   logfile.print(now.hour(), DEC);
   logfile.print(":");
   logfile.print(now.minute(), DEC);
   logfile.print(":");
     logfile.print(now.second(), DEC);
     logfile.print(",");
  

    
   
   logfile.print(bme.readTemperature());
  logfile.print(F(", ")); 
  logfile.print(mlx.readObjectTempC()); 
  logfile.print(F(", "));
   logfile.print(bme.readPressure());
   logfile.print(F("-"));
   logfile.print(F(", ")); 
   logfile.print(bme.readHumidity());
   logfile.println(F(""));
    delay(1000);
   logfile.close();
  
  }
  
 
