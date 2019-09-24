
//#include "DigiKeyboard.h"

#define IR_1      3225413783
#define IR_2      3225409703
#define IR_3      3225417863
#define IR_4      3225405623
#define IR_5      3225403583
#define IR_6      3225415823
#define IR_7      3225417735
#define IR_8      3225414165
#define IR_9      3225412635
#define IR_STAR   3225410085
#define IR_0      3225413145
#define IR_SHARP  3225414675
#define IR_UP     3225404985
#define IR_LEFT   3225404475
#define IR_OK     3225407025
#define IR_RIGHT  3225409193
#define IR_DOWN   3225408173


#define PIN_EVENT PINB // ATTiny85 had Port B pins
#define IR_PIN 0
#define D1_PIN 1
#define D2_PIN 2
#define D3_PIN 3
#define D4_PIN 4
#define D5_PIN 5


#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    50   // max IR pulse pairs to sample
#define RESOLUTION    2   // time between IR measurements

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse
uint16_t currentpulse = 0;      // index for pulses we're storing
uint32_t irCode = 0;
uint32_t irCodeLast = 0;
unsigned long lastPress = 0;


void setup() {
//  DigiKeyboard.sendKeyStroke(0);
//  DigiKeyboard.println("Starting IR");
//  DigiKeyboard.delay(5000);

  pinMode(D1_PIN, OUTPUT);
  pinMode(D2_PIN, OUTPUT);
  pinMode(D3_PIN, OUTPUT); // comment this when debugging
  pinMode(D4_PIN, OUTPUT); // comment this when debugging
  pinMode(D5_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);
}


void loop() {

  uint16_t numpulse = listenForIR(); // Wait for an IR Code

  // Process the pulses to get a single number representing code
  for (int i = 0; i < 32; i++) {
    irCode = irCode << 1;
    if ((pulses[i][0] * RESOLUTION) > 0 && (pulses[i][0] * RESOLUTION) < 500) {
      irCode |= 0;
    } else {
      irCode |= 1;
    }
  }

  int repeat = (irCode != irCodeLast || (millis() - lastPress) > 200);
  switch (irCode) {
    case IR_1:
//      DigiKeyboard.println("1");
      digitalWrite(D4_PIN, LOW);
      digitalWrite(D3_PIN, LOW);
      digitalWrite(D2_PIN, LOW);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
    case IR_2:
//      DigiKeyboard.println("2");
      digitalWrite(D4_PIN, LOW);
      digitalWrite(D3_PIN, LOW);
      digitalWrite(D2_PIN, HIGH);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
    case IR_3:
//      DigiKeyboard.println("3");
      digitalWrite(D4_PIN, LOW);
      digitalWrite(D3_PIN, HIGH);
      digitalWrite(D2_PIN, LOW);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
    case IR_4:
//      DigiKeyboard.println("4");
      digitalWrite(D4_PIN, LOW);
      digitalWrite(D3_PIN, HIGH);
      digitalWrite(D2_PIN, HIGH);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
    case IR_5:
//      DigiKeyboard.println("5");
      digitalWrite(D4_PIN, HIGH);
      digitalWrite(D3_PIN, LOW);
      digitalWrite(D2_PIN, LOW);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
    case IR_STAR:
//      DigiKeyboard.println("STAR");
      digitalWrite(D4_PIN, HIGH);
      digitalWrite(D3_PIN, LOW);
      digitalWrite(D2_PIN, HIGH);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
    case IR_SHARP:
//      DigiKeyboard.println("SHARP");
      digitalWrite(D4_PIN, HIGH);
      digitalWrite(D3_PIN, HIGH);
      digitalWrite(D2_PIN, LOW);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
    case IR_OK:
//      DigiKeyboard.println("OK");
      digitalWrite(D4_PIN, HIGH);
      digitalWrite(D3_PIN, HIGH);
      digitalWrite(D2_PIN, HIGH);
      digitalWrite(D1_PIN, !digitalRead(D1_PIN));
      delay(500);
      break;
  }
  irCodeLast = irCode;
  lastPress = millis();
}

uint16_t listenForIR() {  
  currentpulse = 0;
  while (1) {
    unsigned int highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length

    while (PIN_EVENT & _BV(IR_PIN)) { // got a high pulse
      highpulse++;
      delayMicroseconds(RESOLUTION);
      if (((highpulse >= MAXPULSE) && (currentpulse != 0)) || currentpulse == NUMPULSES)
        return currentpulse;
    }
    pulses[currentpulse][0] = highpulse;

    while (! (PIN_EVENT & _BV(IR_PIN))) { // got a low pulse
      lowpulse++;
      delayMicroseconds(RESOLUTION);
      if (((lowpulse >= MAXPULSE) && (currentpulse != 0)) || currentpulse == NUMPULSES)
        return currentpulse;
    }
    pulses[currentpulse][1] = lowpulse;
    currentpulse++;
  }
}
