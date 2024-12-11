#define OscFreq (const unsigned int) 27000000
#define WireClock (const unsigned int) 400000
#define PWM_FREQ (const unsigned int)3

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

/*'****************************************************
*   Using PWM board (Adafruit PCA9685)
*   Board address = 0x40 Offset = binary 00000 (no jumpers required)
*   Mega2560, SCL = D21, SDA = D20
*   Initiates the PWM bord prameter
*   Input: none
*   Return: none
*******************************************************/
void SetupPWM(void){
  pwm.begin();
  pwm.setOscillatorFrequency(OscFreq);
  pwm.setPWMFreq(PWM_FREQ);  // This is the maximum PWM frequency    
  Wire.setClock(WireClock);
}

/*'****************************************************
*   Set PWM width 0 - 100% 
*   Input: Channel (0 -15), Pulse width (0 - 4095)
*   Return: none
*******************************************************/
void SetPWM(int Channel, int PulsWidth){
   pwm.setPWM(Channel, PulsWidth, 0);
}
