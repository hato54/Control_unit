// Digital Control Phase Shifters 1, 2 DST-11-480/1S
#define DCPS0_1_4 (const byte) 4    // Phase shifter 0, 1.4
#define DCPS0_2_8 (const byte) 5    // Phase shifter 0, 2.8
#define DCPS0_5_6 (const byte) 6    // Phase shifter 0, 5.6  
#define DCPS0_11_25 (const byte) 7  // Phase shifter 0, 11.25
#define DCPS0_22_5 (const byte) 8   // Phase shifter 0, 22.5
#define DCPS0_45 (const byte) 9     // Phase shifter 0, 45
#define DCPS0_90 (const byte) 10    // Phase shifter 0, 90
#define DCPS0_180 (const byte) 11   // Phase shifter 0, 180

#define DCPS1_1_4 (const byte) 12   // Phase shifter 1, 1.4
#define DCPS0_2_8 (const byte) 13    // Phase shifter 1, 2.8
#define DCPS1_5_6 (const byte) 14   // Phase shifter 1, 5.6
#define DCPS1_11_25 (const byte) 15 // Phase shifter 1, 11.25
#define DCPS1_22_5 (const byte) 46  // Phase shifter 1, 22.5
#define DCPS1_45 (const byte) 47    // Phase shifter 1, 45
#define DCPS1_90 (const byte) 48    // Phase shifter 1, 90
#define DCPS1_180 (const byte) 49   // Phase shifter 1, 180


/*'****************************************************
*   Write phase values to the target phase skifter
*   Input: Unit number (0 or 1), 8 bit phase value
*   Return: none
*******************************************************/
void SetFSDeg(int Unit, byte Phase){

  byte Mask = 0x01;
  byte PhaseByte = 0x00;
  byte TempPhase = ~Phase;

    // Check if bits are Zero
  for(int i = 0; i < 8; i++){
    PhaseByte = Mask & TempPhase;
    if(Unit == 0){
      switch(PhaseByte){
        case  0x01:
          PORTG &= ~(1 << PORTG5);  // DCPS0_1_4 = FALSE            
          break; 
        case  0x02:
          PORTE &= ~(1 << PORTE3);  // DCPS0_2_8 = FALSE     
          break; 
        case  0x04:
          PORTH &= ~(1 << PORTH3);  // DCPS0_5_6 = FALSE    
          break; 
        case  0x08:
          PORTH &= ~(1 << PORTH4);  // DCPS0_11_25 = FALSE     
          break; 
        case  0x10:
          PORTH &= ~(1 << PORTH5);  // DCPS0_22_5 = FALSE   
          break; 
        case  0x20:
          PORTH &= ~(1 << PORTH6);  // DCPS0_45 = FALSE   
          break; 
        case  0x40:
          PORTB &= ~(1 << PORTB4);  // DCPS0_90 = FALSE  
          break; 
        case  0x80:
          PORTB &= ~(1 << PORTB5);  // DCPS0_180 = FALSE 
          break; 
      }
    }else{  // Unit == 1
      switch(PhaseByte){
        case  0x01:
          PORTB &= ~(1 << PORTB6);  // DCPS1_1_4 = FALSE          
          break; 
        case  0x02:
          PORTB &= ~(1 << PORTB7);  // DCPS1_2_8 = FALSE     
          break; 
        case  0x04:
          PORTJ &= ~(1 << PORTJ1);  // DCPS1_5_6 = FALSE    
          break; 
        case  0x08:
          PORTJ &= ~(1 << PORTJ0);  // DCPS1_11_25 = FALSE    
          break; 
        case  0x10:
          PORTL &= ~(1 << PORTL3);  // DCPS1_22_5 = FALSE   
          break; 
        case  0x20:
          PORTL &= ~(1 << PORTL2);  // DCPS1_45 = FALSE   
          break; 
        case  0x40:
          PORTL &= ~(1 << PORTL1);  // DCPS1_90 = FALSE   
          break; 
        case  0x80:
          PORTL &= ~(1 << PORTL0);  // DCPS1_180 = FALSE 
          break; 
      }
    }
    Mask <<=1;
  }
  // Check if bits are set
  Mask = 0x01;
  for(int i = 0; i < 8; i++){
    PhaseByte = Mask & Phase;
    if(Unit == 0){
      switch(PhaseByte){
        case  0x01:
          PORTG |= 1 << PORTG5;     // DCPS1_1_4 = TRUE          
          break; 
        case  0x02:
          PORTE |= 1 << PORTG3;     // DCPS1_2_8 = TRUE     
          break; 
        case  0x04:
          PORTH |= 1 << PORTH3;     // DCPS1_5_6 = TRUE    
          break; 
        case  0x08:
          PORTH |= 1 << PORTH4;     // DCPS1_11_25= TRUE    
          break; 
        case  0x10:
          PORTH |= 1 << PORTH5;     // DCPS1_22_5= TRUE   
          break; 
        case  0x20:
          PORTH |= 1 << PORTH6;     // DCPS1_45 = TRUE   
          break; 
        case  0x40:
          PORTB |= 1 << PORTB4;     // DCPS1_90 = TRUE  
          break; 
        case  0x80:
          PORTB |= 1 << PORTB5;     // DCPS1_180 = TRUE 
          break; 
      }
    }else{  // Unit == 1
      switch(PhaseByte){
        case  0x01:
          PORTB |= 1 << PORTB6;     // DCPS1_1_4 = TRUE          
          break; 
        case  0x02:
          PORTB |= 1 << PORTB7;     // DCPS1_2_8 = TRUE     
          break; 
        case  0x04:
          PORTJ |= 1 << PORTJ1;     // DCPS1_5_6 = TRUE    
          break; 
        case  0x08:
          PORTJ |= 1 << PORTJ0;     // DCPS1_11_25= TRUE    
          break; 
        case  0x10:
          PORTL |= 1 << PORTL3;     // DCPS1_22_5= TRUE   
          break; 
        case  0x20:
          PORTL |= 1 << PORTL2;     // DCPS1_45 = TRUE   
          break; 
        case  0x40:
          PORTL |= 1 << PORTL1;     // DCPS1_90 = TRUE  
          break; 
        case  0x80:
          PORTL |= 1 << PORTL0;     // DCPS1_180 = TRUE 
          break; 
      }
    }
    Mask <<=1;
  }
  
}