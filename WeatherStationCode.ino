/* Code written by Ayush Khanal in 2017
 * 
 * 
 * 
 */
#include "SPI.h"                               // defining everything and including libraries
#include "TFT_22_ILI9225.h"
#include <dht.h>
#include <../fonts/FreeSans9pt7b.h>
#include <../fonts/Picopixel.h>
#include <../fonts/FreeMono9pt7b.h>
#include <../fonts/TomThumb.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <SFE_BMP180.h>
#if defined (ARDUINO_ARCH_STM32F1)
#define TFT_RST PA1
#define TFT_RS  PA2
#define TFT_CS  PA0
#define TFT_SDI PA7 
#define TFT_CLK PA5
#define TFT_LED 0 
#else
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  
#define TFT_SDI 11 
#define TFT_CLK 13 
#define TFT_LED 3  
#endif
#define TFT_BRIGHTNESS 200 
#define dht_dpin A0 
#define DS1307_ADDRESS 0x68
#define ALTITUDE 1655.0


TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS); // object for screen
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


SFE_BMP180 pressure;  // bmp180 object

dht DHT; //dht object

const int analogPin=A0; // setting analogpin integer to A0 analog pin for humidity sensor
int Astate=0; 



int16_t x=0, y=0, w, h; //constants for screen printing stuff

const int analogPin2=A1; // setting analogpin2 integer to A1 analog pin for rain sensor
int Astaterain=0;

const String Red = "Red";                             //defining color strings for the parameters for the printing function
const String White = "White";
const String Magenta = "Magenta";
const String Green = "Green";
const String Blue = "Blue";
const String Yellow = "Yellow";
const String Cyan = "Cyan";





void drawTextonScreenColor(String thing_to_print, int y_displacement, int x_displacement, String Color) //printing function that takes the string to print its displacements and color

{
 String s1 = thing_to_print;
 int j = y_displacement;
 int i = x_displacement;
 String colour = Color;
 tft.getGFXTextExtent(s1, x, y, &w, &h);
 x += i;
 y = j; // Set y position to string height
 
 if(colour == Red)
  {
    tft.drawGFXText(x, y, s1, COLOR_RED);
  }
  
 if(colour == White)
  {
    tft.drawGFXText(x, y, s1, COLOR_WHITE);
  }
  
 if(colour == Magenta)
  {
    tft.drawGFXText(x, y, s1, COLOR_MAGENTA);
  }
  
 if(colour == Green)
  {
    tft.drawGFXText(x, y, s1, COLOR_GREEN);
  }
  
 if(colour == Blue)
  {
    tft.drawGFXText(x, y, s1, COLOR_BLUE);
  }
  
 if(colour == Yellow)
  {
    tft.drawGFXText(x, y, s1, COLOR_YELLOW);
  }
  
 if(colour == Cyan)
  {
    tft.drawGFXText(x, y, s1, COLOR_CYAN);
  }
  
 x-=i;

} 


void eraseTextonScreen(String thing_to_print, int y_displacement, int x_displacement) // this really was magic I would update the screen by writing over the recent values in black

{
 String s1 = thing_to_print;
 int j = y_displacement;
 int i = x_displacement;
 tft.getGFXTextExtent(s1, x, y, &w, &h); 
 y = j; // Set y position to string height
 tft.drawGFXText(x, y, s1, COLOR_BLACK); 

} 




void setup() //setting up the light sensor and the screen to begin recording
{
  tft.begin();
  tft.clear();
  Serial.begin(9600);
  !tsl.begin();
  configureSensor(); 
  pressure.begin();
}


void configureSensor(void) //more configuring the light sensor
{
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);  
}
byte bcdToDec(byte val) {             //used for the RTC from a library we dont know what it does but its needed
return ( (val/16*10) + (val%16) );
}

void loop() { // main method
char status;
double T,P,p0,a;
status = pressure.startTemperature();
delay(status);
status = pressure.getTemperature(T);
status = pressure.startPressure(3);
delay(status);
status = pressure.getPressure(P,T); 
                                                       



String RainValue;

Astaterain=analogRead(analogPin2);
  if(Astaterain<=800) 
  {

    RainValue = "Raining";
  }
  else //if the value of D0 is LOW      //logic for deciding whether it is raining or not
  {
    RainValue = "Not raining";
  }

 String lightvalue;
  sensors_event_t event;
  tsl.getEvent(&event);
  if(event.light >=0 and event.light <= 200)
  {
    lightvalue = "It's cloudy.";
  }else if( event.light > 200 and event.light <= 700)        // logic for deciding the cloud condition or wheter its dark outside
  {
    lightvalue = "It's overcast.";
  }
  else if(event.light > 700)
  {
    lightvalue = "It's sunny.";           
  }

  DHT.read11(dht_dpin);

  String HumidityValue =  getStringValuefromFloat(DHT.humidity);
  String PressureValue =  getStringValuefromFloat(P);           
  String TemperatureValue = getStringValuefromFloat(T);    
/*
 * Colors available to use are Red White Magenta Green Blue Yellow Cyan
 */

tft.setGFXFont(&FreeMono9pt7b);
drawTextonScreenColor("Humidity %:", 20, 0, Red);        //drawing all the stuff on the screen
drawTextonScreenColor(HumidityValue, 40, 0, Red);
drawTextonScreenColor("Pressure (mB):", 60, 0, Green);
drawTextonScreenColor(PressureValue, 80, 0, Green);
drawTextonScreenColor("Temperature (C):", 100, 0, Cyan);
drawTextonScreenColor(TemperatureValue, 120, 0, Cyan);
drawTextonScreenColor(RainValue, 140, 0, Magenta);
drawTextonScreenColor(lightvalue, 160, 0, White);

delay(1000); 

eraseTextonScreen("Humidity %:", 20, 0);
eraseTextonScreen(HumidityValue, 40, 0);
eraseTextonScreen("Pressure (mB):", 60, 0);                       //erasing all the stuff on the screen
eraseTextonScreen(PressureValue, 80, 0);
eraseTextonScreen("Temperature (C):",100, 0);
eraseTextonScreen(TemperatureValue, 120, 0);
eraseTextonScreen(RainValue, 140, 0);
eraseTextonScreen(lightvalue, 160, 0);   

}

String getStringValuefromFloat(float val)
{
  float floatVal= val;    
  String stringVal = "";    
  stringVal+=String(int(floatVal))+ "."+String(getDecimal(floatVal)); //this is how we convert floating point values to strings to display
  char charVal[stringVal.length()+1];                      
  stringVal.toCharArray(charVal,stringVal.length()+1);    
  return stringVal;
}

long getDecimal(float val)
{
  int intPart = int(val);
  long decPart = 1000*(val-intPart);  //Change to match the number of decimal places you need
  if(decPart>0)return(decPart);           //return the decimal part of float number if it is available //part of the floating point conversion
  else if(decPart<0)return((-1)*decPart); //if negative, multiply by -1
  else if(decPart=0)return(00);           //return 0 if decimal part of float number is not available
}

