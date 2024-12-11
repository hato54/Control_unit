// Using D16, D17, D25 - D28.



/*'****************************************************
*   Reads value fom the ADconverter MC3201 (SPI)
*   Input: none
*   Return: float ADvalue
*******************************************************/
unsigned int ReadAdMC3201(void){

unsigned int reading = 0;           // "reading" is the raw ADC value

digitalWrite(CS,LOW);
reading = SPI.transfer16(0x0000);   // "reading" captured, but it needs
digitalWrite(CS,HIGH);              // the following 2 line to clean it up:
reading = reading << 3;             // Must shift out the 3 MSB's (trash bits)
reading = reading >> 4;             // Must shift back 4 MSB's (pushes off LSB)
                                    // The above two lines clears the top 3 MSB that are unknown "trash",
                                    // then shifts back 4 bits to not only bring in all "0" for the 4
                                    // MSB's, but ALSO pushes off the LSB which came in from the LBS
                                    // reading that is transmitted in LSB order (another "trash" bit).
//float voltage = reading * (4.096 / 4095);   // Using 4.096V Ref IC
return(reading);                            // If using another reference voltage, replace the 
}                                           // "4.096" with the actual voltage used for reference.


/*'****************************************************
*   Selects the actual ADchannel (3 x 16 ch analog mux)
*   and reads value fom the Arduino ADconverter 
*   Input: none
*   Return: unsigned int ADvalue 10 bits
*******************************************************/
unsigned int ReadAD(int MuxChannel){

// channel 0-47
  
  int ADch = 0;
  int value;
  int TempMuxChannel = 0;

//  Map channel 1 - 48 to harware PCB 0 - 47
 // channel = channel_map[channel]; ; 

// Select  ADconverter 1 - 3 using digital pins D17 - D15
  if(MuxChannel < 16){ 
    ADch = 0;                                            // Use only Arduino adconverter channel 0 
    PORTH &= ~(1 << PORTH0);          // /EN1 = 0 (D17)
    PORTH |= 1 << PORTH1;             // /EN2 = 1 (D16)
  } else if(MuxChannel > 15 && MuxChannel < 32){
    ADch =0;                                            // Use only Arduino ADconverter channel 0 
    PORTH |= 1 << PORTH0;             // /EN1 = 1 (D17)
    PORTH &= ~(1 << PORTH1);          // /EN2 = 0 (D16)
  } else if(MuxChannel > 31 && MuxChannel < 48){        // This is the SPI ADconverter
    PORTH |= 1 << PORTH0;             // /EN1 = 1 (D17)
    PORTH |= 1 << PORTH1;             // /EN2 = 1 (D16)
  }
 

  // Select channel 0 - 15 on selected analog mux using pins D25 - D28
  if(MuxChannel> 31){
    TempMuxChannel = MuxChannel - 32;
  } else if(MuxChannel > 15){
    TempMuxChannel = MuxChannel - 16;
  }
  switch(TempMuxChannel){
    case 0:                               // channel 0
      PORTA &= ~(1 << PORTA3);            //A0 = 0  (D25)
      PORTA &= ~(1 << PORTA4);            //A1 = 0  (D26)  
      PORTA &= ~(1 << PORTA5);            //A2 = 0  (D27)
      PORTA &= ~(1 << PORTA6);            //A3 = 0  (D28)
      break;
    case 1:                               // channel 1
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA &= ~(1 << PORTA4);            //A1 = 0
      PORTA &= ~(1 << PORTA5);            //A2 = 0
      PORTA &= ~(1 << PORTA6);            //A3 = 0
      break;  
    case 2:                               // channel 2
      PORTA &= ~(1 << PORTA3);            //A0 = 0
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA &= ~(1 << PORTA5);            //A2 = 0
      PORTA &= ~(1 << PORTA6);            //A3 = 0
      break; 
    case 3:                               // channel 3
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA &= ~(1 << PORTA5);            //A2 = 0
      PORTA &= ~(1 << PORTA6);            //A3 = 0
      break; 
    case 4:                               // channel 4
      PORTA &= ~(1 << PORTA3);            //A0 = 0
      PORTA &= ~(1 << PORTA4);            //A1 = 0
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA &= ~(1 << PORTA6);            //A3 = 0
      break;
    case 5:                               // channel 5
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA &= ~(1 << PORTA4);            //A1 = 0
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA &= ~(1 << PORTA6);            //A3 = 0
      break;
    case 6:                               // channel 6
      PORTA &= ~(1 << PORTA3);            //A0 = 0
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA &= ~(1 << PORTA6);            //A3 = 0
      break;
    case 7:                               // channel 7
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA &= ~(1 << PORTA6);            //A3 = 0
      break; 
    case 8:                               // channel 8
      PORTA &= ~(1 << PORTA3);            //A0 = 0
      PORTA &= ~(1 << PORTA4);            //A1 = 0
      PORTA &= ~(1 << PORTA5);            //A2 = 0
      PORTA |= 1 << PORTA6;               //A3 = 1
      break;
    case 9:                               // channel 9
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA &= ~(1 << PORTA4);            //A1 = 0
      PORTA &= ~(1 << PORTA5);            //A2 = 0
      PORTA |= 1 << PORTA6;               //A3 = 1
      break;  
    case 10:                              // channel 10
      PORTA &= ~(1 << PORTA3);            //A0 = 0
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA &= ~(1 << PORTA5);            //A2 = 0
      PORTA |= 1 << PORTA6;               //A3 = 1
      break; 
    case 11:                              // channel 11
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA &= ~(1 << PORTA5);            //A2 = 0
      PORTA |= 1 << PORTA6;               //A3 = 1
      break; 
    case 12:                              // channel 12
      PORTA &= ~(1 << PORTA3);            //A0 = 0
      PORTA &= ~(1 << PORTA4);            //A1 = 0
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA |= 1 << PORTA6;               //A3 = 1
      break;
    case 13:                              // channel 13
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA &= ~(1 << PORTA4);            //A1 = 0
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA |= 1 << PORTA6;               //A3 = 1
      break;
    case 14:                              // channel 14
      PORTA &= ~(1 << PORTA3);            //A0 = 0
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA |= 1 << PORTA6;               //A3 = 1
      break;
    case 15:                              // channel 15
      PORTA |= 1 << PORTA3;               //A0 = 1
      PORTA |= 1 << PORTA4;               //A1 = 1
      PORTA |= 1 << PORTA5;               //A2 = 1
      PORTA |= 1 << PORTA6;               //A3 = 1
      break;         

} 
  if(MuxChannel > 31){                    // Use the SPI ADconverter
    value = ReadAdMC3201();
  }else{
    value = analogRead(ADch);            // Arduino AD
  }
 
  // Clean up addresses and enables
 /* PORTA &= ~(1 << PORTA3);                //A0 = 0
  PORTA &= ~(1 << PORTA4);                //A1 = 0
  PORTA &= ~(1 << PORTA5);                //A2 = 0
  PORTA &= ~(1 << PORTA6);                //A3 = 0
  PORTH |= 1 << PORTH0;                   // /EN1 = 1 (D17)
  PORTH |= 1 << PORTH1;                   // /EN2 = 1 (D16)
  */
  
  return(value);
}

