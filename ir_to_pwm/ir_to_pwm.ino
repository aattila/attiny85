
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
#define IR_PIN   2
#define SYN_PIN  1
#define DATA_PIN 0


#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    50   // max IR pulse pairs to sample
#define RESOLUTION    2   // time between IR measurements

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse
uint16_t currentpulse = 0;      // index for pulses we're storing
uint32_t irCode = 0;
uint32_t irCodeLast = 0;
unsigned long lastPress = 0;


void sendIR(uint16_t val) {
//  DigiKeyboard.println(val);
  analogWrite(DATA_PIN, val);
  digitalWrite(SYN_PIN, !digitalRead(SYN_PIN));
  delay(1000); 
  analogWrite(DATA_PIN, 0);
  irCodeLast = irCode;
  lastPress = millis();
//  DigiKeyboard.println("Button pushed");
}

void processPulses() {
  irCode = 0;

  // Process the pulses to get a single number representing code
  for (int i = 0; i < 32; i++) {
    irCode = irCode << 1;
    if ((pulses[i][0] * RESOLUTION) > 0 && (pulses[i][0] * RESOLUTION) < 500) {
      irCode |= 0;
    } else {
      irCode |= 1;
    }
  }

  if (irCode != irCodeLast || (millis() - lastPress) > 2000) {
    switch (irCode) {
      case IR_1:
        sendIR(10);
        break;
      case IR_2:
        sendIR(20);
        break;
      case IR_3:
        sendIR(30);
        break;
      case IR_4:
        sendIR(40);
        break;
      case IR_5:
        sendIR(50);
        break;
      case IR_6:
        sendIR(60);
        break;
      case IR_7:
        sendIR(70);
        break;
      case IR_8:
        sendIR(80);
        break;
      case IR_9:
        sendIR(90);
        break;
      case IR_0:
        sendIR(100);
        break;
      case IR_STAR:
        sendIR(110);
        break;
      case IR_SHARP:
        sendIR(120);
        break;
      case IR_UP:
        sendIR(130);
        break;
      case IR_DOWN:
        sendIR(140);
        break;
      case IR_LEFT:
        sendIR(150);
        break;
      case IR_RIGHT:
        sendIR(160);
        break;
      case IR_OK:
        sendIR(170);
        break;
    }
  }
}

void setup() {
//  DigiKeyboard.sendKeyStroke(0);
//  DigiKeyboard.println("Starting IR");
//  DigiKeyboard.delay(1000);

  pinMode(SYN_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT_PULLUP); // some are saying this will not work
  digitalWrite(IR_PIN, HIGH);    // force pullup workaround
}


void loop() {
  uint16_t numpulse = listenForIR(); // Wait for an IR Code
  //34 is the specific pulse number for my IR transmitter
  if(numpulse == 34) { 
//    DigiKeyboard.println(numpulse);
    processPulses();
  }
}

uint16_t listenForIR() {
  currentpulse = 0;
  while (1) {
    unsigned int highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length

    while (PIN_EVENT & _BV(IR_PIN)) { // got a high pulse
      highpulse++;
      delayMicroseconds(RESOLUTION);
      if (((highpulse >= MAXPULSE) && (currentpulse != 0)) || currentpulse == NUMPULSES) {
        return currentpulse;
      }
    }
    pulses[currentpulse][0] = highpulse;

    while (! (PIN_EVENT & _BV(IR_PIN))) { // got a low pulse
      lowpulse++;
      delayMicroseconds(RESOLUTION);
      if (((lowpulse >= MAXPULSE) && (currentpulse != 0)) || currentpulse == NUMPULSES) {
        return currentpulse;
      }
    }
    pulses[currentpulse][1] = lowpulse;
    currentpulse++;
  }
}
