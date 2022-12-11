#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

const byte BTN = 1; // pin3 PB1

int previousPattern = 1; int previousPattern2 = 1;
int currentPattern = 1; int currentPattern2 = 1;
int ledMode = 1;
int mode_2 = 1; int mode_3 = 1; int mode_22 = 1; int mode_32 = 1;


void setup() {
  pinMode(BTN, INPUT_PULLUP);
  pinMode(2, OUTPUT); pinMode(0, OUTPUT); pinMode(3, OUTPUT); pinMode(4, OUTPUT);

  while(digitalRead(BTN) == LOW) {
    if(ledMode == 1) { ledMode = 2; Set_Pattern(2, false, false); delay(3000); }
    else if(ledMode == 2) { ledMode = 3; Set_Pattern(3, false, false); delay(3000); }
    else if(ledMode == 3) { ledMode = 4; Set_Pattern(4, false, false); delay(3000); }
    else if(ledMode == 4) { ledMode = 1; Set_Pattern(1, false, false); delay(3000); }
  }

  PCMSK  = bit (PCINT1); // want PB1 / pin3
  GIFR  |= bit (PCIF);   // clear any outstanding interrupts
  GIMSK |= bit (PCIE);   // enable pin change interrupts

  randomSeed(millis());

  Set_Pattern(7, false, false);
  delay(100);
  
  power_off();
}


void loop() {
  delay(100);
}


ISR(PCINT0_vect) // there are pins and vectors, it is not the same
{

}


void power_off() {
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  wdt_disable();
  ADCSRA = 0;           // turn off ADC
  power_all_disable();  // power off ADC, Timer 0 and 1, serial interface
  noInterrupts();       // timed sequence coming up
  sleep_enable();       // ready to sleep
  sleep_bod_disable();  // Disable Brown out detector
  interrupts();         // interrupts are required now
  sleep_cpu();          // sleep                
  sleep_disable();      // precaution
  power_all_enable();   // power everything back on

  Button_Pressed();  
}


void Button_Pressed() {
  randomSeed(millis());
  do {
    delay(89);
    AnimateMix();
  } while(digitalRead(BTN) == LOW);
  
  if(ledMode == 1 || ledMode == 2) { SlowDown(false); }
  else if(ledMode == 3 || ledMode == 4) { SlowDown(true); }
  
  while(digitalRead(BTN) == LOW) { delay(100); }
  power_off();
}


void SlowDown(bool twoDices) {
  for(int i=0; i<13; i+=2) {
    do {
     currentPattern = random(1, 7);
    } while (currentPattern == previousPattern);
    previousPattern = currentPattern;
    
    if(twoDices == true) {
      do {
       currentPattern2 = random(1, 7);
      } while (currentPattern2 == previousPattern2);
      previousPattern2 = currentPattern2;
    }
    
    if(ledMode == 2 || ledMode == 4) {
      Set_Pattern(0, true, false);
      delay(89 + 20 * i);
    }

    Set_Pattern(currentPattern, true, false);
    delay(89 + 20 * i);
  }
  
  if(twoDices == true) {
    int showDice = 1;
    int counter = 0;
    
    while(digitalRead(BTN)==HIGH) {
      counter += 1;
      if(showDice == 1) {
        if(counter >= 20) { showDice = 2; counter = 0; Set_Pattern(0, false, false); delay(200); }
        else Set_Pattern(currentPattern, false, false);
        delay(100);
      }
      else {
        if(counter >= 20) { showDice = 1; counter = 0; Set_Pattern(0, false, true); delay(200); }
        else Set_Pattern(currentPattern2, false, true);
        delay(100);
      }
    }
    
    Button_Pressed();
  }
}


void AnimateMix() {
  digitalWrite(2, random(0, 2)); digitalWrite(0, random(0, 2)); digitalWrite(3, random(0, 2));
  digitalWrite(4, random(0, 2));
}


void Set_Pattern(int pattern, bool set_new23, bool second_dice) {
  if (set_new23 == true) {
    mode_2 = random(1, 3); delay(13);
    mode_3 = random(1, 3); delay(13);
    mode_22 = random(1, 3); delay(13);
    mode_32 = random(1, 3); delay(13);
  }
  
  switch(pattern) {
    case 1:
      digitalWrite(2, HIGH); digitalWrite(0, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW);
      break;
    case 2:
      digitalWrite(2, LOW); digitalWrite(4, LOW);
      if(second_dice == true) {
        if(mode_22 == 1) { digitalWrite(0, HIGH); }
        else { digitalWrite(0, LOW); }
        if(mode_22 == 2) { digitalWrite(3, HIGH); }
        else { digitalWrite(3, LOW); }
      }
      else {
        if(mode_2 == 1) { digitalWrite(0, HIGH); }
        else { digitalWrite(0, LOW); }
        if(mode_2 == 2) { digitalWrite(3, HIGH); }
        else { digitalWrite(3, LOW); }
      }
      break;
    case 3:
      digitalWrite(2, HIGH); digitalWrite(4, LOW);
      if(second_dice == true) {
        if(mode_32 == 1) { digitalWrite(0, HIGH); }
        else { digitalWrite(0, LOW); }
        if(mode_32 == 2) { digitalWrite(3, HIGH); }
        else { digitalWrite(3, LOW); }
      }
      else {
        if(mode_3 == 1) { digitalWrite(0, HIGH); }
        else { digitalWrite(0, LOW); }
        if(mode_3 == 2) { digitalWrite(3, HIGH); }
        else { digitalWrite(3, LOW); }
      }
      break;
    case 4:
      digitalWrite(2, LOW); digitalWrite(0, HIGH); digitalWrite(3, HIGH); digitalWrite(4, LOW);
      break;
    case 5:
      digitalWrite(2, HIGH); digitalWrite(0, HIGH); digitalWrite(3, HIGH); digitalWrite(4, LOW);
      break;
    case 6:
      digitalWrite(2, LOW); digitalWrite(0, HIGH); digitalWrite(3, HIGH); digitalWrite(4, HIGH);
      break;
    case 7:
      digitalWrite(2, HIGH); digitalWrite(0, HIGH); digitalWrite(3, HIGH); digitalWrite(4, HIGH);
      break;
    default:
      digitalWrite(2, LOW); digitalWrite(0, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW);
      break;
  }
}
