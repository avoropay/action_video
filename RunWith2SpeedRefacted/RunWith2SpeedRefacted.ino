#include <AccelStepper.h>

// For arduino speed up

#define FASTADC 1
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
// For arduino speed up end

// Define some steppers and the pins the will use
AccelStepper stepper1(1, 4, 5); // stepP, dirP
AccelStepper stepper2(1, 6, 7);

float round_speed1 = 0.0;
float correction1 = 3.0;

float round_speed2 = 0.0;
float correction2 = 3.0;

float djoistik1 = 0.0;
float djoistik2 = 0.0;

#define speedPinRead1 A0
#define speedPinRead2 A2

void setup()
{  
  // For arduino speed up
  #if FASTADC
  // set prescale to 16
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  #endif
  // For arduino speed up end

  pinMode(speedPinRead1, INPUT);
  pinMode(speedPinRead2, INPUT);
  
  stepper1.setMaxSpeed(1000.0);
  stepper2.setMaxSpeed(1000.0);  

  stepper1.setSpeed(0.0);
  stepper2.setSpeed(0.0);

}

void readDjoistiks(){
  djoistik1 = analogRead(speedPinRead1);
  djoistik2 = analogRead(speedPinRead2);
}

void setMotorSpeed(float djoistik, AccelStepper stepper, float correction){
  
  float round_speed = 0.0;
  
  if(djoistik > 522.0){
      round_speed = (djoistik - 512.0) * correction; 
      
      stepper.setSpeed(round_speed);
      stepper.setDirection(false);
      
    }else if(djoistik1 < 500){
      round_speed = (512.0 - djoistik) * correction;
      
      stepper.setSpeed(round_speed);
      stepper.setDirection(true);
      
    }else{
      
      round_speed = 0.0;
      stepper.setSpeed(round_speed);
    }
}
void loop()
{
    readDjoistiks();
    setMotorSpeed(djoistik1, stepper1, correction1);
    setMotorSpeed(djoistik2, stepper2, correction2);
    
    stepper1.runSpeed();
    stepper2.runSpeed();
}
