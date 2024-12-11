/*****************************************************************
 *  This software is used to control the hardware system in
 *  CONTOL UNIT
 *  It consist of following hardware:
 *  1 pcs Arduino MEGA
 *  16 pcs PWM Modules (pwm to analoge) 
 *  16 Phase detectors
 *  1 pcs Power divider (1-16)
 *  2 pcs Phase skifter
 *  1 pcs Main switch including LED
 *  1 pcs Relay 12V (power contol)
 *  1 pcs Power supply 12V 3A
 *  2 pcs Power supply +5v, -5V
 *  1 pcs A/D converter (MCP3201 SPI, 12 bit)
 *  3 pcs Analoge Mux board (Analog/digital mux lab board 16 ch)
 *  1 pcs PWM board, (Adafruit PCA9685 16-Channel Servo driver I2C)
 *  1 pcs I/O expander (I2C, 16 I/O)
 * 
 *  2023-02-27  Control_unit_03:  SPI init, File AnalogeIO:TempMyxChannel (ReadAD())
 *  
 *  https://github.com/hato54/xxxx
 ***************************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include "PCF8575.h"

#define  DAT (const byte)  50 // SPI MISO Pin
#define  CLK (const byte)  52 // SPI Clock Pin
#define  CS  (const byte)  53  // SPI SS Pin (Chip Select)


#define TRUE HIGH
#define FALSE LOW

#define LED_PIN 6
#define POWER_RELAY_CONTOLER 3
#define POWER_RELAY1_PREAMP 4
#define POWER_RELAY2_PREAMP 5

bool LEDToggle = 0;





//void Exequte_Command(byte, byte, unsigned int);




// Set i2c address
PCF8575 pcf8575(0x20);

void setup() {
  Serial.begin(19200);
  Serial1.begin(19200);
  
  // Analog Mux
  // *** Do not change!!!! ***
  // this is hard coded in AnaalogIO.ino (for speed)
  pinMode(16, OUTPUT); // EN2
  pinMode(17, OUTPUT); // EN1
  pinMode(25, OUTPUT); // A0
  pinMode(26, OUTPUT); // A1
  pinMode(27, OUTPUT); // A2
  pinMode(28, OUTPUT); // A3

  // Digital control phase shifter
  pinMode(POWER_RELAY_CONTOLER, OUTPUT);  // PowerOnRelayController
  pinMode(POWER_RELAY1_PREAMP,OUTPUT);  // PowerOnRelay1Preamp
  pinMode(POWER_RELAY2_PREAMP,OUTPUT);  // PowerOnRelay2Preamp
  pinMode(LED_PIN,OUTPUT); // Power switch LED
/* Free
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);  
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(46,OUTPUT);
  pinMode(47,OUTPUT);
  pinMode(48,OUTPUT);
  pinMode(49,OUTPUT);
*/

  // SPI
  pinMode(DAT,INPUT);
  pinMode(CS,OUTPUT);
  SPI.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(CS,LOW);   // CS pin is cycled low then high because
  digitalWrite(CS,HIGH);  // the PWR ON state of the pin is unknown.
  digitalWrite(CLK,LOW);


  SetupIOExpander();  // PCF8575
  SetupPWM();         // Config PWM Module PCA9685
  OutputIOExpander(0, 0xAA);    // IOExpander address

  digitalWrite(POWER_RELAY_CONTOLER, true);         // PowerOnRelay Controller

}

void loop() {

  CheckInputData();

}

/*'****************************************************
*   Power On/Off the sytem power
*   Input: none
*   Return: none
*******************************************************/
void PowerOff(void){
  BlinkLED();
  delay(5000);  // Wait for some flashes
  CleanUp();
  digitalWrite(3, false);         // Shut off

}

/*'****************************************************
*   Setup the Timer1 for 0.5 Hz interrupt
*   Input: none
*   Return: none
*******************************************************/
void BlinkLED(void){
  
  cli();                                    //Stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;                               // Set entire TCCR1A register to 0
  TCCR1B = 0;                               // Same for TCCR1B
  TCNT1  = 0;                               //Initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;                            // = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();                                    //Allow interrupts
}

/*'****************************************************
*   Set all outputs and equipments for safe power off
*   Input: none
*   Return: none
*******************************************************/
void CleanUp(void){

}

/*'****************************************************
*   Power On/Off both 28V power supply used by Preamps
*   Input: tru = On, fale = Off
*   Return: none
*******************************************************/
void PowerPreamp(bool OnOff){

  if(OnOff == true){        // Limit current rush
    digitalWrite(POWER_RELAY1_PREAMP, true);
    delay(100);
    digitalWrite(POWER_RELAY2_PREAMP, true);
  }else{
    digitalWrite(POWER_RELAY1_PREAMP, false);
    digitalWrite(POWER_RELAY2_PREAMP, false);
  }
}


/**********************************
*
* ISR function
* Used for flashing the Power LED
*
***********************************/
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (LEDToggle){
    digitalWrite(13,HIGH);
    LEDToggle = 0;
  }
  else{
    digitalWrite(13,LOW);
    LEDToggle = 1;
  }
}


