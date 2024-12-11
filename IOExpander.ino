/*'****************************************************
*   Initiates the IO Expander port to Output
*   Input: none
*   Return: none
*******************************************************/


void SetupIOExpander(void){
	
	// Set pinMode to OUTPUT
	pcf8575.pinMode(P0, OUTPUT);
  pcf8575.pinMode(P1, OUTPUT);
  pcf8575.pinMode(P2, OUTPUT);
  pcf8575.pinMode(P3, OUTPUT);
  pcf8575.pinMode(P4, OUTPUT);
  pcf8575.pinMode(P5, OUTPUT);
  pcf8575.pinMode(P6, OUTPUT);
  pcf8575.pinMode(P7, OUTPUT);
  pcf8575.pinMode(P8, OUTPUT);
  pcf8575.pinMode(P9, OUTPUT);
  pcf8575.pinMode(P10, OUTPUT);
  pcf8575.pinMode(P11, OUTPUT);
  pcf8575.pinMode(P12, OUTPUT);
  pcf8575.pinMode(P13, OUTPUT);
  pcf8575.pinMode(P14, OUTPUT);
  pcf8575.pinMode(P15, OUTPUT);

	pcf8575.begin();
}


/*'****************************************************
*   Writes a byte to D0-D7 or D8-D9
*   Input: Item 0 = D0-D7, 1 = D8-D15.  Value 0-0xFF
*   Return: none
*******************************************************/
void OutputIOExpander(bool Item, byte Value){

if(!Item){  
    if(Value & 0x01 > 1){
      pcf8575.digitalWrite(P0, HIGH);
    }else{
      pcf8575.digitalWrite(P0, LOW);
    }
    if(Value & 0x02 > 1){
      pcf8575.digitalWrite(P1, HIGH);
    }else{
      pcf8575.digitalWrite(P1, LOW);
    }
    if(Value & 0x04> 1){
      pcf8575.digitalWrite(P2, HIGH);
    }else{
      pcf8575.digitalWrite(P2, LOW);
    }
    if(Value & 0x08 > 1){
      pcf8575.digitalWrite(P3, HIGH);
    }else{
      pcf8575.digitalWrite(P3, LOW);
    }
    if(Value & 0x10 > 1){
      pcf8575.digitalWrite(P4, HIGH);
    }else{
      pcf8575.digitalWrite(P4, LOW);
    }
    if(Value & 0x20 > 1){
      pcf8575.digitalWrite(P5, HIGH);
    }else{
      pcf8575.digitalWrite(P5, LOW);
    }
    if(Value & 0x40 > 1){
      pcf8575.digitalWrite(P6, HIGH);
    }else{
      pcf8575.digitalWrite(P6, LOW);
    }
    if(Value & 0x80 > 1){
      pcf8575.digitalWrite(P7, HIGH);
    }else{
      pcf8575.digitalWrite(P7, LOW);
    }
  }else{
     if(Value & 0x01 > 1){
      pcf8575.digitalWrite(P8, HIGH);
    }else{
      pcf8575.digitalWrite(P8, LOW);
    }
    if(Value & 0x02 > 1){
      pcf8575.digitalWrite(P9, HIGH);
    }else{
      pcf8575.digitalWrite(P9, LOW);
    }
    if(Value & 0x04> 1){
      pcf8575.digitalWrite(P10, HIGH);
    }else{
      pcf8575.digitalWrite(P10, LOW);
    }
    if(Value & 0x08 > 1){
      pcf8575.digitalWrite(P11, HIGH);
    }else{
      pcf8575.digitalWrite(P11, LOW);
    }
    if(Value & 0x10 > 1){
      pcf8575.digitalWrite(P12, HIGH);
    }else{
      pcf8575.digitalWrite(P12, LOW);
    }
    if(Value & 0x20 > 1){
      pcf8575.digitalWrite(P13, HIGH);
    }else{
      pcf8575.digitalWrite(P13, LOW);
    }
    if(Value & 0x40 > 1){
      pcf8575.digitalWrite(P14, HIGH);
    }else{
      pcf8575.digitalWrite(P14, LOW);
    }
    if(Value & 0x80 > 1){
      pcf8575.digitalWrite(P15, HIGH);
    }else{
      pcf8575.digitalWrite(P15, LOW);
    }
  }

}