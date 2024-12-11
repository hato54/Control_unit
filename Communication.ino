
#define Power_Off_Control_Unit (byte) 0x10
#define Power_ON_Preamp (byte) 0x11
#define Power_Off_Preamp (byte) 0x12
#define Set_PWM_Single (byte) 0x13
#define Set_PWM_All (byte) 0x14
#define Set_Phase_Shifter (byte) 0x15
#define Read_Phase_Detector_Single (byte) 0x16
#define Read_Phase_Detector_All (byte) 0x17
#define Power_Feedback_Single (byte) 0x18
#define Power_Feedback_All (byte) 0x19

#define SOH (byte) 0x01
#define ACK (byte) 0x06
#define NAK (byte) 0x15
#define EOT (byte) 0x04

#define _SOH 0
#define _SIZE 1
#define _COMMAND 2
#define _CHKSUM_LSB 3
#define _CHKSUM_MSB 4
#define _CHANNEL 5
#define _PWM_VALUE_LSB 6
#define _PWM_VALUE_MSB 7
#define _DATA 8
#define _EOT 9
#define _CMD_ERROR 10

#define INPUT_DATA_TIMOUT 10L // 10 ms

struct Frame{
  byte Command;
  byte Size;
  byte Channel;
  byte DataValues[80];
  unsigned int ChkSum;
};


bool TimerEnable = false;
bool MessageReady = false;
bool Error = false;
bool NewFrame = true;
bool ReadingFrame = false;
byte Command;
byte InputBuffer[80];
byte Temp;
byte Next = _SOH;
int SizeCounter = 0;
//unsigned int SizeCounter = (unsigned int)0;
unsigned int CChecksum = (unsigned int) 0;  // Calculated checksum
//unsigned int DataBuffer[33];

int DataIndex = 0;
int BufferIndex = 0;
int BufferLoad = 0;
int DataCounter = 0;
unsigned long TimerFirstCharacter;
unsigned long TimerDecoderTimeout;

Frame Message;

union {
  unsigned int WordSize;
  byte ByteSize[2];           // _bytes[1] = MSB of _word, _bytes[0] = LSB of _word
}Convert;




 void CheckInputData(void){ 

  if(ReadingFrame == true){
    if(millis() - TimerFirstCharacter > INPUT_DATA_TIMOUT){ Serial.println("Time");
      ReadingFrame = false;
      BufferIndex = 0;
      MessageReady = true;
      TimerDecoderTimeout = millis();
    }
  }
  
  if(Serial1.available()){
    if(NewFrame){
      Temp = (byte)Serial1.read();  
      if(Temp == SOH){
        NewFrame = false; 
        BufferIndex = 0;
        ReadingFrame = true;
        TimerFirstCharacter = millis();
      }else{
        ReadingFrame = false;
        NewFrame = true;
        Serial1.print(NAK, HEX); Serial.println("NAK");
      }
    }else{
       Temp = (byte)Serial1.read();
    }
    
    TimerEnable = true;
      
    InputBuffer[BufferIndex++] = Temp;  //Serial.println(InputBuffer[BufferIndex-1], HEX);
    BufferLoad = BufferIndex -1;      //Serial.println(MessageReady);
    
    if(BufferIndex == 80){
      Serial1.print(NAK, HEX);     Serial.println("Buffer full");     // Max buffer
      while(Serial1.available()){
        Serial1.read();                 // Emty the Arduino buffer
      }
    }
  }

  if(MessageReady){ 
    byte DataByte;
    
    if(millis() - TimerDecoderTimeout > 100L){
      Serial1.print(NAK, HEX);  // No valid package!
      Serial.println("Error package");
      // Prepair for next frame
      SizeCounter = 0;
      Next = _SOH;
      CChecksum = 0;
      MessageReady = false;
      NewFrame = true;
      Error = true;   // Not used
    }
    while(BufferIndex <= BufferLoad){
      DataByte = InputBuffer[BufferIndex++]; //Serial.print("Char: "); Serial.println(DataByte);
      switch(Next){
        case _SOH:
          if(DataByte == SOH){ Serial.print("SOH: ");Serial.println(DataByte,HEX); 
              Next = _SIZE;
              DataCounter = 0;    // Reset the counter
          }else{                // First byte is illegal!
              Next = _SOH;        // Clean up
              SizeCounter = (unsigned int)0;
              MessageReady = false;
              Serial1.print(NAK, HEX); Serial.println("Error SOH");
          }
          break;
        case _SIZE:
          Next = _COMMAND;
          //SizeCounter = (unsigned int)DataByte;Serial.print("Size: ");Serial.println(DataByte,HEX);
          SizeCounter = (int)DataByte;Serial.print("Size: ");Serial.println(DataByte,HEX);
          CChecksum = (unsigned int)DataByte;
          break;
        case _COMMAND:
          Message.Command = DataByte; Serial.print("Command: ");Serial.print(DataByte, HEX);
            switch(DataByte){
              case Power_Off_Control_Unit:Serial.println(" Power_Off_Control_Unit");
                Next = _CHKSUM_LSB;
                CChecksum += (unsigned int)DataByte;
                break;
              case Power_ON_Preamp: Serial.println(" Power_ON_Preamp");
                Next = _CHKSUM_LSB;
                CChecksum += (unsigned int)DataByte;
                break;
              case Power_Off_Preamp:Serial.println(" Power_Off_Preamp");
                Next = _CHKSUM_LSB;
                CChecksum += (unsigned int)DataByte;
                break;
              case Set_PWM_Single:Serial.println(" Set_PWM_Single");
                Next = _CHANNEL;
                CChecksum += (unsigned int)DataByte; 
                break;
              case Set_PWM_All:Serial.println(" Set_PWM_All");
                Next = _DATA;
                DataIndex = 0;
                CChecksum += (unsigned int)DataByte;
                break;
              case Set_Phase_Shifter:Serial.println(" Set_Phase_Shifter");
                Next = _CHANNEL;
                CChecksum += (unsigned int)DataByte;
                break;
              case Read_Phase_Detector_Single:Serial.println(" Read_Phase_Detector_Single");
                Next = _CHANNEL;
                CChecksum += (unsigned int)DataByte;
                break; 
              case Read_Phase_Detector_All:Serial.println(" Read_Phase_Detector_All");
                Next = _CHKSUM_LSB;
                CChecksum += (unsigned int)DataByte;
                break; 
              default:
                Next = _CMD_ERROR;
                Message.Command = 0x00;
                break;              
            }
            break;
        case _CHANNEL:
          CChecksum += (unsigned int)DataByte;
          if(Message.Command == Read_Phase_Detector_Single){
            Next = _CHKSUM_LSB;
          }else{
            Next = _DATA;
          }
          DataIndex = 0;
          Message.Channel = DataByte; Serial.print("Channel: ");Serial.println(DataByte, HEX);
          break;    
        case _DATA: 
          if(Message.Command == Set_Phase_Shifter){
            Next = _CHKSUM_LSB;
          }else{  
            if((SizeCounter - 2) > 0){ // Substract Size and Command and Channel
              if((SizeCounter -2) > 1){
                Next = _DATA;
              }else{
                Next = _CHKSUM_LSB;
              }
            }    
          }  
          SizeCounter--; //Serial.print("Data: ");Serial.println(DataByte, HEX);
          CChecksum += (unsigned int)DataByte; //Serial.println(CChecksum, HEX);
          Message.DataValues[DataIndex++] = DataByte;   
          break;
        case _CHKSUM_LSB:
          Next = _CHKSUM_MSB;  
          Convert.ByteSize[0] = DataByte; //Serial.println(DataByte, HEX);
          break;
        case _CHKSUM_MSB:
          Next = _EOT;
          Convert.ByteSize[1] = DataByte; //Serial.println(DataByte, HEX);
          Message.ChkSum = Convert.WordSize; //Serial.print("PChecksum: ");Serial.println(Message.ChkSum, HEX); 
          break; 
        case _EOT:
          Serial.print("CChecksum: ");Serial.println(CChecksum, HEX); Serial.print("Message.ChkSum: "); Serial.println(Message.ChkSum, HEX); 
          if(Message.ChkSum != CChecksum){
            Serial1.print(NAK, HEX); Serial.println("NAK");        // Checksum error!
          }else{
            Serial1.print(ACK, HEX); Serial.println("ACK");
            ExecuteCommand();
          }
          // Prepair for next frame
          SizeCounter = 0;
          Next = _SOH;
          CChecksum = 0;
          MessageReady = false;
          NewFrame = true;
          break;  
        case _CMD_ERROR:
          SizeCounter = 0;
          Next = _SOH;
          CChecksum = 0;
          MessageReady = false;
          NewFrame = true;
          Serial1.print(NAK, HEX);  // No valid command!
          Error = true; Serial.println("Error no valid command");
          break;  
        default:
          Serial1.print(NAK, HEX);  // No valid package!
          Serial.println("Error package");
          // Prepair for next frame
          SizeCounter = 0;
          Next = _SOH;
          CChecksum = 0;
          MessageReady = false;
          NewFrame = true;
          Error = true;   // Not used
          break; // Outer   
      } // END switch Next
      
    }  // END while(BufferIndex <= BufferLoad)

  } // END if(MessageReady)


}

void ExecuteCommand(){

  unsigned int CheckSum = 0;

  switch(Message.Command){
    case Power_Off_Control_Unit:
      PowerOff();
      break;
    case Power_ON_Preamp:
      PowerPreamp(true); 
      break;
    case Power_Off_Preamp:
      PowerPreamp(false);
      break;
    case Set_PWM_Single:
      Convert.ByteSize[0] = Message.DataValues[0];
      Convert.ByteSize[1] = Message.DataValues[1];
      SetPWM((int)Message.Channel, Convert.WordSize);
      break;
    case Set_PWM_All:
      for(int i = 0; i < 16; i++){
        Convert.ByteSize[0] = Message.DataValues[i];
        Convert.ByteSize[1] = Message.DataValues[i + 1];
        SetPWM(i, Convert.WordSize);  
      }
    break;
    case Set_Phase_Shifter:
      OutputIOExpander((bool)Message.Channel, Message.DataValues[0]);
      break;
    case Read_Phase_Detector_Single:
      InputBuffer[0] = SOH;                       // Start build frame
      InputBuffer[1] = 0x05;                      // Size
      CheckSum = InputBuffer[1];                  // Start counting checksum
      InputBuffer[2] = Read_Phase_Detector_Single;
      CheckSum += InputBuffer[2];
      Convert.WordSize = (unsigned int)ReadAD((int)2 * Message.Command);        // Read VINA 0
      InputBuffer[3] = Convert.ByteSize[0];       // VINA 0 LSB
      CheckSum += InputBuffer[3];
      InputBuffer[4] = Convert.ByteSize[1];       // VINA 0 MSB
      CheckSum += InputBuffer[4]; 
      Convert.WordSize = (unsigned int)ReadAD((int)(2 * Message.Command) + 1);  // Read VINB 0
      InputBuffer[5] = Convert.ByteSize[0];       // VINB 0 LSB
      CheckSum += InputBuffer[3];               
      InputBuffer[6] = Convert.ByteSize[1];       // VINB 0 MSB
      CheckSum += InputBuffer[5];
      Convert.WordSize = CheckSum;
      InputBuffer[7] = Convert.ByteSize[0];
      InputBuffer[8] = Convert.ByteSize[1];
      InputBuffer[9] = _EOT;
      for(int i = 0; i < 10; i++){
        Serial1.print(InputBuffer[i]);
      }
      // WAIT FOR ACK !!!! 
      break; 
    case Read_Phase_Detector_All:
      InputBuffer[0] = SOH;                       // Start build frame
      InputBuffer[1] = 0x41;                      // Size
      CheckSum = InputBuffer[1];                  // Start counting checksum
      InputBuffer[2] = Read_Phase_Detector_All;
      CheckSum += InputBuffer[2];
      for(int i = 0; i < 32; i++){
        Convert.WordSize = (unsigned int)ReadAD(i);
        InputBuffer[i + 3] = Convert.ByteSize[0];
        CheckSum += InputBuffer[i + 3];
        InputBuffer[i + 4] = Convert.ByteSize[1];
        CheckSum += InputBuffer[i + 4];
        Convert.WordSize = CheckSum;
        InputBuffer[i + 4] = Convert.ByteSize[0];
        InputBuffer[i + 5] = Convert.ByteSize[1];
        InputBuffer[i + 6] = _EOT;
        for(int j = 0; j < i + 7; j++){
          Serial1.print(InputBuffer[j]);
        }
      // WAIT FOR ACK !!!! 
      }
      break; 
    case Power_Feedback_Single:
      InputBuffer[0] = SOH;                       // Start build frame
      InputBuffer[1] = 0x03;                      // Size
      CheckSum = InputBuffer[1];                  // Start counting checksum
      InputBuffer[2] = Power_Feedback_Single;
      CheckSum += InputBuffer[2];
      Convert.WordSize = (unsigned int)ReadAD((int)31 + Message.Command);        // Read extern 12 bits A/D (ch 32 -> 47)
      InputBuffer[3] = Convert.ByteSize[0];       // VINA 0 LSB
      CheckSum += InputBuffer[3];
      InputBuffer[4] = Convert.ByteSize[1];       // VINA 0 MSB
      CheckSum += InputBuffer[4]; 
      Convert.WordSize = CheckSum;
      InputBuffer[5] = Convert.ByteSize[0];
      InputBuffer[6] = Convert.ByteSize[1];
      InputBuffer[7] = _EOT;
      for(int i = 0; i < 8; i++){
        Serial1.print(InputBuffer[i]);
      }
      // WAIT FOR ACK !!!! 
      break;
    case Power_Feedback_All:
      InputBuffer[0] = SOH;                       // Start build frame
      InputBuffer[1] = 0x21;                      // Size
      CheckSum = InputBuffer[1];                  // Start counting checksum
      InputBuffer[2] = Power_Feedback_All;
      CheckSum += InputBuffer[2];
      for(int i = 0; i < 16; i++){
        Convert.WordSize = (unsigned int)ReadAD((int)31 + i);   // Read extern 12 bits A/D (ch 32 -> 47)
        InputBuffer[i + 3] = Convert.ByteSize[0];
        CheckSum += InputBuffer[i + 3];
        InputBuffer[i + 4] = Convert.ByteSize[1];
        CheckSum += InputBuffer[i + 4];
        Convert.WordSize = CheckSum;
        InputBuffer[i + 4] = Convert.ByteSize[0];
        InputBuffer[i + 5] = Convert.ByteSize[1];
        InputBuffer[i + 6] = _EOT;
        for(int j = 0; j < i + 7; j++){
          Serial1.print(InputBuffer[j]);
        }
      // WAIT FOR ACK !!!! 
      }
      break;    
    default:
      break;              
  }
}
