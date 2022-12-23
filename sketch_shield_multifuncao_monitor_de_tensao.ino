/*
INTRODUCAO A ARQUITETURA DE COMPUTADORES - TRABALHO 5
Pedro Machado Peçanha - 2110535
Eric Ruas Leão - 2110694
*/

#define HIGH_VOLTAGE_LIMIT 5.0
#define LOW_VOLTAGE_LIMIT 0.1
#define VOLTAGE_UNIT 0.0049
#define BUFFERSIZE 7
#define TRUE 1
#define FALSE 0

#include <TimerOne.h>                     // Bibliotec TimerOne 
#include <Wire.h>                         // Biblioteca Wire 
#include <MultiFuncShield.h>              // Biblioteca Multifunction shield
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
enum MonitorModeValues
{
MONITORING_STOPPED,
MONITORING_STARTED,
SETTING_HIGH_TH_STARTED,
SETTING_LOW_TH_STARTED
};
enum ThresholdStatusValues
{
 THRESHOLD_NOT_SET,
 THRESHOLD_SET
};
enum ledModeValues
{
LED_ALL_OFF,
LED_1_ON,
LED_2_ON,
LED_3_ON,
LED_4_ON
};

float highValue = 5.0;
float lowValue =  0.1;
char floatBuffer [BUFFERSIZE];
char printBuffer[6];
char setHigh = FALSE;
char setLow = FALSE;
float currentTension;

ledModeValues currentLedState = LED_ALL_OFF;
MonitorModeValues currentMonitorState = MONITORING_STOPPED;
ThresholdStatusValues currentThresholdState = THRESHOLD_NOT_SET;
void (*button1Quick)(void);
void (*button1Long)(void);
void (*button2Quick)(void);
void (*button2Long)(void);
void (*button3Quick)(void);
void (*button3Long)(void);


void setup() {
 

 Timer1.initialize();                    // inicializa o Timer 1
  MFS.initialize(&Timer1);                // initializa a biblioteca Multi função
   Serial.begin(9600); 
  
}

void loop() {

  byte btn = MFS.getButton();  
                 // AND bit a bit para identificar o evento
 switch (currentMonitorState){
    case MONITORING_STOPPED:

      MFS.write(" off");
    
      button1Quick = button1StoppedQ;
      button2Quick = button2StoppedQ;
      button3Quick = button3StoppedQ;
      button2Long = button2StoppedL;
      button3Long = button3StoppedL;

      break;

    case MONITORING_STARTED:
     ledAnimation();
      currentTension = voltValue(analogRead(POT_PIN));
     dtostrf(currentTension,2,1,floatBuffer);
     if(currentTension > highValue){
      sprintf(printBuffer, "hi%s", floatBuffer);
      MFS.write(printBuffer);
      MFS.beep(5,5,4,1,1);
      delay(400);

      }

     else if(currentTension<lowValue){
        sprintf(printBuffer, "lo%s", floatBuffer);
        MFS.write(printBuffer);
       MFS.beep(5,5,4,1,1);
       delay(400);
          
      
       
      }
      
     else{
     
     sprintf(printBuffer, "on%s", floatBuffer);
     MFS.write(printBuffer);
     
     }
     MFS.blinkDisplay(DIGIT_1 | DIGIT_2, ON);
     button1Quick = button1StartedQ;
     button2Quick = button2StoppedQ;
     button3Quick = button3StoppedQ;

      break;

    case SETTING_HIGH_TH_STARTED:
       dtostrf(highValue,2,1,floatBuffer);
       sprintf(printBuffer, "ht%s", floatBuffer);
        MFS.write(printBuffer);
      button1Quick = button1HighQ;
      button2Quick = button2HighQ;
      button3Quick = button3HighQ;
//      button1Long = button1HighL;
      button2Long = button2HighQ;
      button3Long = button3HighQ;

      break;

    case SETTING_LOW_TH_STARTED:
       dtostrf(lowValue,2,1,floatBuffer);
       sprintf(printBuffer, "lt%s", floatBuffer);
        MFS.write(printBuffer);

      button1Quick = button1LowQ;
      button2Quick = button2LowQ;
      button3Quick = button3LowQ;
//      button1Long = button1HighL;
      button2Long = button2LowQ;
      button3Long = button3LowQ;
      
      break;
    }
    
if(btn){  
    byte buttonNumber = btn & B00111111;                 // AND bit a bit para identificar o botão
    byte buttonAction = btn & B11000000;

    switch (btn){

      case BUTTON_1_PRESSED:
        button1Quick();
        if (currentMonitorState == MONITORING_STOPPED)
          delay(800);
        break;
      case BUTTON_1_LONG_PRESSED:
        button1Long();
        break;
      case BUTTON_2_PRESSED:
        button2Quick();
        if (currentMonitorState == MONITORING_STOPPED)
         delay(1300);
          
        break;
      case BUTTON_2_LONG_PRESSED:
        button2Long();
        break;
      case BUTTON_3_PRESSED:
        button3Quick();
         if (currentMonitorState == MONITORING_STOPPED)
          delay(1300);
        break;
      case BUTTON_3_LONG_PRESSED:
        button3Long();
         if (currentMonitorState == MONITORING_STOPPED)
          delay(1300);
        break;
         
      }
  }
}

float voltValue(float pot_pin){

  return pot_pin * 5 /1023;
  
  }

void button1StoppedQ(){
  if (setHigh == TRUE && setLow == TRUE){
    currentMonitorState = MONITORING_STARTED;
  }
  return;
  
  }

  void button2StoppedQ(){
    MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);
    dtostrf(highValue,2,1,floatBuffer);
  sprintf(printBuffer, "ht%s", floatBuffer);
  MFS.write(printBuffer);
 
  }

  void button3StoppedQ(){
    MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);
  dtostrf(lowValue,2,1,floatBuffer);
  sprintf(printBuffer, "lt%s", floatBuffer);
  MFS.write(printBuffer);
  
  }


 

  void button2StoppedL(){

    currentMonitorState = SETTING_HIGH_TH_STARTED;
  
  }

  void button3StoppedL(){
  
    currentMonitorState = SETTING_LOW_TH_STARTED;
  
  }

  void button1StartedQ(){

    currentMonitorState = MONITORING_STOPPED;
      MFS.writeLeds(LED_ALL, OFF);
    setHigh = FALSE;
    setLow = FALSE;
        MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);
    
    
  
  }

   void button1HighQ(){
  currentMonitorState = MONITORING_STOPPED;
  MFS.writeLeds(LED_1, ON);
    MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);
    setHigh = TRUE;
    }

      void button1LowQ(){
  currentMonitorState = MONITORING_STOPPED;
  MFS.writeLeds(LED_2, ON);
    MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);
    setLow = TRUE;
   }


  
  void button2HighQ(){
  if (highValue<HIGH_VOLTAGE_LIMIT){
      highValue+=0.1;
    }
  MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);

  dtostrf(highValue,2,1,floatBuffer);
  sprintf(printBuffer, "ht%s", floatBuffer);
  MFS.blinkDisplay(DIGIT_1 | DIGIT_2, ON);
  MFS.write(printBuffer);
    
    }
 void button2LowQ(){
  if ((lowValue+0.1)<highValue){
      lowValue+=0.1;
    }
  MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);

  dtostrf(lowValue,2,1,floatBuffer);
  sprintf(printBuffer, "lt%s", floatBuffer);
  MFS.blinkDisplay(DIGIT_1 | DIGIT_2, ON);
  MFS.write(printBuffer);
    
    }
    void button3HighQ(){
  if((highValue-0.1)>lowValue){
  highValue-=0.1;
  }
  dtostrf(highValue,2,1,floatBuffer);
  sprintf(printBuffer, "ht%s", floatBuffer);
  MFS.write(printBuffer);
    
    }

    void button3LowQ(){
  if ((lowValue-0.1)>LOW_VOLTAGE_LIMIT){
      lowValue-=0.1;
    }
  MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);

  dtostrf(lowValue,2,1,floatBuffer);
  sprintf(printBuffer, "lt%s", floatBuffer);
  MFS.blinkDisplay(DIGIT_1 | DIGIT_2, ON);
  MFS.write(printBuffer);
    
    }

void ledAnimation(){
  MFS.writeLeds(LED_ALL, OFF);
  delay(100);
  MFS.writeLeds(LED_1, ON);
  delay(100);
  MFS.writeLeds(LED_2, ON);
  delay(100);
  MFS.writeLeds(LED_3, ON);
  delay(100);
  MFS.writeLeds(LED_4, ON);
  delay(100);
  MFS.writeLeds(LED_ALL, OFF);
  
  }

 
