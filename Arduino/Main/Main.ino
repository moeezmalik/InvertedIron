#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "max6675.h"
#include <avr/wdt.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int lastPos = 0;

///////////////////////////////////////
//////////MENU BOOL VARIABLES//////////
///////////////////////////////////////

bool mainMenu = false;
bool mainMenuSet = false;
bool mainMenuStart = false;

bool setMenu = false;
bool startMenu = false;

///////////////////////////////////////
/////////ROTARY ENCODER THINGS/////////
///////////////////////////////////////

int outputA = 5;
int outputB = 6;

int interruptPin = 2;

int counterState = 0;
int counter = 0; 
int aState;
int aLastState;

bool clockWise = false;
bool counterClockWise = false;
bool change = false;

///////////////////////////////////////
//////////THERMOCOUPLE THINGS//////////
///////////////////////////////////////

int thermoDO = 11;
int thermoCS = 10;
int thermoCLK = 9;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

int tempSetPoint = 120;

///////////////////////////////////////
//////////////SSR  THINGS//////////////
///////////////////////////////////////

int ssrPin = 13;



//Resetting the MCU to get out of Last Menu
void getOutOfStartMenu(){
  
  Serial.println("RESETING MCU");
  
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}
//Setting Up Menu for the first time
void firstStartMenu(){
  mainMenu = true;
  mainMenuSet = true;
  mainMenuStart = false;

  setMenu = false;
  startMenu = false;

  updateMenu();
}

//Draw the Arrow Indicator according to the position
void drawArrow(int pos){

  int x;
  int y;

  if(lastPos == pos){
    return;
  }
  
  if(lastPos == 1){

    Serial.println("Last Pos == 1");
    for (int i = 2; i < 9; i++){
      for(int j = 16; j < 32; j++){
        display.drawPixel(i, j, BLACK);
      }
    }
  }

  if(lastPos == 2){
    Serial.println("Last Pos == 2");
    for (int i = 2; i < 9; i++){
      for(int j = 32; j < 48; j++){
        display.drawPixel(i, j, BLACK);
      }
    }
  }

  if(lastPos == 3){
    Serial.println("Last Pos == 3");
    for (int i = 2; i < 9; i++){
      for(int j = 48; j < 64; j++){
        display.drawPixel(i, j, BLACK);
      }
    }
  }

  display.display();

  if(pos == 1){
    for(int i = 32; i < 64; i++){
    display.drawPixel(0,i,WHITE);
    display.drawPixel(1,i,WHITE);
    }
    x = 0;
    y = 16;

    for (int i = 0; i < 2; i++){
      for(int j = 16; j < 32; j++){
        display.drawPixel(i, j, BLACK);
      }
    }
  }

  if(pos == 2){
    for(int i = 16; i < 32; i++){
    display.drawPixel(0,i,WHITE);
    display.drawPixel(1,i,WHITE);
    }

    for(int i = 47; i < 64; i++){
    display.drawPixel(0,i,WHITE);
    display.drawPixel(1,i,WHITE);
    }
    
    x = 0;
    y = 31;

    for (int i = 0; i < 2; i++){
      for(int j = 32; j < 47; j++){
        display.drawPixel(i, j, BLACK);
      }
    }
  }

  if(pos == 3){
    for(int i = 16; i < 48; i++){
    display.drawPixel(0,i,WHITE);
    display.drawPixel(1,i,WHITE);
    }
    
    x = 0;
    y = 47;

    for (int i = 0; i < 2; i++){
      for(int j = 48; j < 64; j++){
        display.drawPixel(i, j, BLACK);
      }
    }
  }
  
  display.drawPixel(x, y, WHITE);
  display.drawPixel(x+1, y, WHITE);
  display.drawPixel(x, y+1, WHITE);
  
  display.drawPixel(x+1, y+1, WHITE);
  display.drawPixel(x+2, y+1, WHITE);
  display.drawPixel(x+1, y+2, WHITE);

  display.drawPixel(x+2, y+2, WHITE);
  display.drawPixel(x+3, y+2, WHITE);
  display.drawPixel(x+2, y+3, WHITE);
  
  display.drawPixel(x+3, y+3, WHITE);
  display.drawPixel(x+4, y+3, WHITE);
  display.drawPixel(x+3, y+4, WHITE);

  display.drawPixel(x+4, y+4, WHITE);
  display.drawPixel(x+5, y+4, WHITE);
  display.drawPixel(x+4, y+5, WHITE);

  display.drawPixel(x+5, y+5, WHITE);
  display.drawPixel(x+6, y+5, WHITE);
  display.drawPixel(x+5, y+6, WHITE);
  display.drawPixel(x+6, y+6, WHITE);

  display.drawPixel(x+7, y+6, WHITE);
  display.drawPixel(x+6, y+7, WHITE);
  display.drawPixel(x+7, y+7, WHITE);

  display.drawPixel(x+8, y+7, WHITE);
  display.drawPixel(x+7, y+8, WHITE);
  display.drawPixel(x+8, y+8, WHITE);

  display.drawPixel(x+6, y+8, WHITE);
  display.drawPixel(x+7, y+8, WHITE);
  display.drawPixel(x+7, y+9, WHITE);

  display.drawPixel(x+5, y+9, WHITE);
  display.drawPixel(x+6, y+9, WHITE);
  display.drawPixel(x+6, y+10, WHITE);

  display.drawPixel(x+4, y+10, WHITE);
  display.drawPixel(x+5, y+10, WHITE);
  display.drawPixel(x+5, y+11, WHITE);

  display.drawPixel(x+3, y+11, WHITE);
  display.drawPixel(x+4, y+11, WHITE);
  display.drawPixel(x+4, y+12, WHITE);

  display.drawPixel(x+2, y+12, WHITE);
  display.drawPixel(x+3, y+12, WHITE);
  display.drawPixel(x+3, y+13, WHITE);

  display.drawPixel(x+1, y+13, WHITE);
  display.drawPixel(x+2, y+13, WHITE);
  display.drawPixel(x+2, y+14, WHITE);

  display.drawPixel(x, y+14, WHITE);
  display.drawPixel(x+1, y+14, WHITE);
  display.drawPixel(x, y+15, WHITE);
  display.drawPixel(x+1, y+15, WHITE);

  display.display();

  lastPos = pos;
}

//Updates menu after every change
void updateMenu(){

  if(mainMenu){
    if(mainMenuSet){

      display.setCursor(0,0);
      display.println(" Main");
      display.println(" Set");
      display.println(" Start");

      drawArrow(1);

    }else if(mainMenuStart){

      display.setCursor(0,0);
      display.println(" Main");
      display.println(" Set");
      display.println(" Start");

      drawArrow(2);

    }
  }else if(setMenu){

    for(int i = 9; i < 128; i++){
      for(int j = 16; j < 32; j++){
        display.drawPixel(i, j, BLACK);
      }
    }

    display.setCursor(0,0);
    display.println(" Set Point");
    display.print(" ");
    display.println(tempSetPoint);
    display.println(" Done");

    drawArrow(2);

  }else if(startMenu){

    display.setCursor(0,0);
    display.println(" Heating");
    display.print(" SP: ");
    display.println(tempSetPoint);
    display.print(" CT: ");
    display.println(10);
    display.println(" Stop");

    drawArrow(3);
    display.display();
    
    while(1){
      int tempReading = thermocouple.readCelsius();
      Serial.print("C = "); 
      Serial.println(tempReading);

      for (int i = 60; i < 128; i++){
        for(int j = 32; j < 48; j++){
          display.drawPixel(i, j, BLACK);
        }
      }

      display.setCursor(62, 32);
      display.print(tempReading);

      display.display();

      delay(1000);

      int err = 0;

      int avgTemperatureReadingA = avgTemperatureReadings(4);
      delay(5000);
      int avgTemperatureReadingB = avgTemperatureReadings(4);

      if (avgTemperatureReadingB > avgTemperatureReadingA){
      Serial.println("Do Nothing");
      } else {
        err = tempSetPoint - avgTemperatureReadingB;
        if (err <= 0){
          Serial.println("Do Nothing");
        } else {
          if (err >= 65){
            turnOnIronLong();
          } else {
            turnOnIronShort();
          }
        }
      }
    }

  }

  display.display();

  return;
}

void updateState(){

  if(mainMenu){
    if(mainMenuSet){

      if(clockWise){

        mainMenuSet = false;
        mainMenuStart = true;

      }


    }else if(mainMenuStart){
      if(counterClockWise){

        mainMenuSet = true;
        mainMenuStart = false;

      }

    }
  }else if(setMenu){

    if(clockWise){
      tempSetPoint = tempSetPoint + 2;
    } else if(counterClockWise){
      tempSetPoint = tempSetPoint - 2;
    }

  }else if(startMenu){

  }

  updateMenu();
  return;
}

void changeMenus(){
  if(mainMenu){
    if(mainMenuSet){
      mainMenu = false;
      setMenu = true;
    }else if(mainMenuStart){      
      mainMenu = false;
      startMenu = true;
    }
  }else if(setMenu){
    mainMenu = true;
    setMenu = false;

  }else if(startMenu){
    getOutOfStartMenu();
  }

  display.clearDisplay();
  lastPos = 0;
  change = true;
  return;
}

int avgTemperatureReadings(int times){
  
  int tempReading = 0;
  
  for (int i = 0; i<times; i++){
    tempReading = tempReading + thermocouple.readCelsius();
    delay(250);
  }

  return tempReading/times;
}

void turnOnIronLong(){
  
  Serial.println("Turning On Iron for Long");
  int tempReading = 0;

  while(1){
    digitalWrite(ssrPin, HIGH);

    tempReading = thermocouple.readCelsius();
    Serial.println(tempReading);

    for (int i = 60; i < 128; i++){
        for(int j = 32; j < 48; j++){
          display.drawPixel(i, j, BLACK);
        }
      }

      display.setCursor(62, 32);
      display.print(tempReading);

      display.display();
    
    if (tempReading > tempSetPoint - 65){
      Serial.println("Turning Off Iron");
      digitalWrite(ssrPin, LOW);
      break;
    }

    delay(250);
    
  } 
}

void turnOnIronShort(){

  Serial.println("Turning On Iron Short");
  digitalWrite(ssrPin, HIGH);
  delay(8000); //8sec
  digitalWrite(ssrPin, LOW);
  delay(30000);  
}

//Runs for the first time
void setup() { 

  //Setting up the Solid State Relay pin as output
  pinMode(ssrPin, OUTPUT);

  //Disabling the WatchDog Timer in order to prevent a reset loop
  wdt_disable();

  //Setup Interrupt for the Rotary Encoder Button
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeMenus, FALLING);

  //Setup Rotary Encoder Inputs
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  
  //Setting up Serial Communication for debugging
  Serial.begin (9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(500);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);
  // Display static text

  firstStartMenu();
  
  // Reads the initial state of the outputA
  aLastState = digitalRead(outputA);
  counterState = counter;   
} 



void loop() { 
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState){    
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) { 
      counter ++;
    } else {
      counter --;
    }
    
    if(counterState - counter >= 2){
      counterState = counter;
      change = true;
      clockWise = true;
    }

    else if(counter - counterState >= 2){
      counterState = counter;
      change = true;
      counterClockWise = true;
    }

    
  
  }

  if(change){
    if(clockWise){
      Serial.println("ClockWise");
    }else{
      Serial.println("Counter ClockWise");
    }

    updateState();

    clockWise = false;
    counterClockWise = false;
    change = false;
  }
  
  
  aLastState = aState; // Updates the previous state of the outputA with the current state
}
