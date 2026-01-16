#include <Arduino.h>
#line 1 "C:\\Users\\beroo\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2025716-20824-132bz2r.yaby\\sketch_aug16b\\sketch_aug16b.ino"
#define IRpin_PIN  PINC   // A0 is PC0, so we read from PINC
#define IRpin      0      // Bit 0 of Port C
#define MAXPULSE 1000

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

// What percent we will allow in variation to match the same code \\ was #define FUZZINESS 20

#define FUZZINESS 45

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing
int DUOPOPsignalRed[] = {
// ON, OFF (in 10's of microseconds)
	152, 48,
	94, 54,
	100, 98,
	52, 46,
	92, 56,
	98, 50,
	104, 94,
	54, 46,
	100, 48,
	96, 52,
	94, 52,
	100, 48,
	104, 46,
	92, 54,
	98, 50,
	104, 46,
	96, 0};
  int DUOPOPsignalBlue[] = {
// ON, OFF (in 10's of microseconds)
	158, 46,
	108, 44,
	98, 54,
	104, 100,
	56, 44,
	98, 54,
	106, 48,
	108, 96,
	52, 48,
	108, 44,
	98, 54,
	102, 52,
	106, 46,
	108, 42,
	98, 56,
	108, 46,
	104, 0};
  int DUOPOPsignalYellow[] = {
// ON, OFF (in 10's of microseconds)
	154, 106,
	50, 50,
	106, 102,
	50, 50,
	106, 102,
	52, 50,
	106, 102,
	50, 52,
	104, 48,
	106, 56,
	98, 50,
	106, 48,
	106, 50,
	104, 48,
	106, 48,
	108, 48,
	102, 0};
int DUOPOPsignalGreen[] = {
// ON, OFF (in 10's of microseconds)
	154, 50,
	106, 102,
	52, 48,
	106, 102,
	52, 48,
	106, 102,
	52, 48,
	106, 102,
	52, 48,
	106, 50,
	104, 48,
	108, 46,
	106, 50,
	104, 48,
	104, 50,
	108, 46,
	104, 0};
#line 93 "C:\\Users\\beroo\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2025716-20824-132bz2r.yaby\\sketch_aug16b\\sketch_aug16b.ino"
void setup();
#line 104 "C:\\Users\\beroo\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2025716-20824-132bz2r.yaby\\sketch_aug16b\\sketch_aug16b.ino"
void loop(void);
#line 137 "C:\\Users\\beroo\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2025716-20824-132bz2r.yaby\\sketch_aug16b\\sketch_aug16b.ino"
int listenForIR(void);
#line 179 "C:\\Users\\beroo\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2025716-20824-132bz2r.yaby\\sketch_aug16b\\sketch_aug16b.ino"
boolean IRcompare(int numpulses, int Signal[]);
#line 93 "C:\\Users\\beroo\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2025716-20824-132bz2r.yaby\\sketch_aug16b\\sketch_aug16b.ino"
void setup() {
   Serial.begin(9600);
  Serial.println("Ready to decode IR!");  
  pinMode(IRpin, INPUT); 
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 

}

void loop(void) {
   
  int numberpulses;
 
  numberpulses = listenForIR();
 
  if (numberpulses == 10)
  {  

  if (IRcompare(numberpulses, DUOPOPsignalRed)) {
    Serial.println("RED Buzzed In");
    digitalWrite(5,HIGH);
  }
  if (IRcompare(numberpulses, DUOPOPsignalBlue)) {
    Serial.println("BLUE Buzzed In");
    digitalWrite(4,HIGH);
  }
    if (IRcompare(numberpulses, DUOPOPsignalYellow)) {
    Serial.println("Yellow Buzzed In");
    digitalWrite(3,HIGH);
  }
   if (IRcompare(numberpulses, DUOPOPsignalGreen)) {
    Serial.println("Green Buzzed In");
    digitalWrite(2,HIGH);
  }
  }
  delay(500);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
}

int listenForIR(void) {
  currentpulse = 0;
 
  while (1) {
    uint16_t highpulse, lowpulse;  
    highpulse = lowpulse = 0; 
 

     while (IRpin_PIN & _BV(IRpin)) {
       // pin is still HIGH
 
       // count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);
 
       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if (((highpulse >= MAXPULSE) && (currentpulse != 0)) || (currentpulse == 10)) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;
 
    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
       // pin is still LOW
       lowpulse++;
       delayMicroseconds(RESOLUTION);
       if (((lowpulse >= MAXPULSE)  && (currentpulse != 0)) || (currentpulse == 10)) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;
    currentpulse++;
  }
}


boolean IRcompare(int numpulses, int Signal[]) {
 
  for (int i=0; i< numpulses-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
 
    
 //   Serial.print(oncode); // the ON signal we heard
 //   Serial.print(" - ");
 //   Serial.print(Signal[i*2 + 0]); // the ON signal we want 
    
 
    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
  //    Serial.print(" (ok)");
    
      
    } else {
  //    Serial.print(" (x)");
      // we didn't match perfectly, return a false match
      return false;
    }
 
    
 //   Serial.print("  \t"); // tab
 //   Serial.print(offcode); // the OFF signal we heard
 //   Serial.print(" - ");
 //   Serial.print(Signal[i*2 + 1]); // the OFF signal we want 
    
 
    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
  //    Serial.print(" (ok)");
    } else {
  //    Serial.print(" (x)");
      // we didn't match perfectly, return a false match
      return false;
    }
 
   // Serial.println();
  }
  // Everything matched!
  return true;
}

