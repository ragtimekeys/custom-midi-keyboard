#include <MIDI.h>

#include <DIO2.h>

#define TOTAL_NUM_KEYS 49

//amount of time to detect velocity
#define MIN_TIME_MS   6
#define MAX_TIME_MS   90
#define DIFFERENCE_TIME_MS (MAX_TIME_MS - MIN_TIME_MS)

#define INITIAL_KEY_OFFSET 36

MIDI_CREATE_DEFAULT_INSTANCE();

byte outputPins[] = {
  34,
  36,
  38,
  40,
  42,
  44,
  46,
  48,


  //second half of keyboard
  2, 4,
  6, 8,
  10, 12,
  22, 26
};

byte inputPins[] = {

  35, 37,
  39, 41,
  43, 45,
  47, 49,


  //second half of keyboard
  3,
  5,
  7,
  9,
  11,
  13,
  24,
  28
};

/*
  Data I've collected:

*/

byte keysState[TOTAL_NUM_KEYS][2];
unsigned long keysTime[TOTAL_NUM_KEYS];

byte iteratorValuesForKeys[TOTAL_NUM_KEYS][2][2] = {
  {{4, 2}, {4, 3}}, //0
  {{5, 2}, {5, 3}}, //1
  {{6, 2}, {6, 3}}, //2
  {{7, 2}, {7, 3}}, //3
  {{0, 4}, {0, 5}}, //4
  {{1, 4}, {1, 5}}, //5
  {{2, 4}, {2, 5}}, //6
  {{3, 4}, {3, 5}}, //7
  {{4, 4}, {4, 5}}, //8
  {{5, 4}, {5, 5}}, //9
  {{6, 4}, {6, 5}}, //10
  {{7, 4}, {7, 5}}, //11

  {{0, 6}, {0, 7}}, //12
  {{1, 6}, {1, 7}}, //13
  {{2, 6}, {2, 7}}, //14
  {{3, 6}, {3, 7}}, //15
  {{4, 6}, {4, 7}}, //16
  {{5, 6}, {5, 7}}, //17
  {{6, 6}, {6, 7}}, //18
  {{7, 6}, {7, 7}}, //19
  {{8, 8}, {8, 9}}, //20
  {{9, 8}, {9, 9}}, //21
  {{10, 8}, {10, 9}}, //22
  {{11, 8}, {11, 9}}, //23
  {{12, 8}, {12, 9}}, //24
  {{13, 8}, {13, 9}}, //25
  {{14, 8}, {14, 9}}, //26
  {{15, 8}, {15, 9}}, //27

  {{8, 10}, {8, 11}}, //28
  {{9, 10}, {9, 11}}, //29
  {{10, 10}, {10, 11}}, //30
  {{11, 10}, {11, 11}}, //31
  {{12, 10}, {12, 11}}, //32
  {{13, 10}, {13, 11}}, //33
  {{14, 10}, {14, 11}}, //34
  {{15, 10}, {15, 11}}, //35

  {{8, 12}, {8, 13}}, //36
  {{9, 12}, {9, 13}}, //37
  {{10, 12}, {10, 13}}, //38
  {{11, 12}, {11, 13}}, //39
  {{12, 12}, {12, 13}}, //40
  {{13, 12}, {13, 13}}, //41
  {{14, 12}, {14, 13}}, //42
  {{15, 12}, {15, 13}}, //43

  {{8, 14}, {8, 15}}, //44
  {{9, 14}, {9, 15}}, //45
  {{10, 14}, {10, 15}}, //46
  {{11, 14}, {11, 15}}, //47
  {{12, 14}, {12, 15}} //48
};

byte keyValuesForIterators[sizeof(inputPins)][sizeof(outputPins)];

boolean signals[sizeof(inputPins) * sizeof(outputPins)];
void setup() {
  Serial.begin(115200);
  //MIDI.begin(MIDI_CHANNEL_OFF);
  Serial.println("Listening to MIDI notes...");
  //initialize all keys as off and no time
  for (byte i = 0; i < TOTAL_NUM_KEYS; i++) {
    keysState[i][0] = 0;
    keysState[i][1] = 0;
    keysTime[i] = 0;
  }
  for (byte pin = 0; pin < sizeof(outputPins); pin++) {
    pinMode(outputPins[pin], OUTPUT);
  }
  for (byte pin = 0; pin < sizeof(inputPins); pin++) {
    pinMode(inputPins[pin], INPUT_PULLUP);
  }
  //reverse lookup so we can get the key number from i and j values later on in the loop() function
  for (byte i = 0; i < TOTAL_NUM_KEYS; i++) {
    keyValuesForIterators[iteratorValuesForKeys[i][0][0]][iteratorValuesForKeys[i][0][1]] = i;
    keyValuesForIterators[iteratorValuesForKeys[i][1][0]][iteratorValuesForKeys[i][1][1]] = i + 100;
  }
  keyValuesForIterators[sizeof(outputPins)-1][sizeof(inputPins)-1] = 200;
}


void sendMidiEvent(byte statusByte, byte keyIndex, unsigned long time) {
  unsigned long t = time;
  if (t > MAX_TIME_MS)
    t = MAX_TIME_MS;
  if (t < MIN_TIME_MS)
    t = MIN_TIME_MS;
  t -= MIN_TIME_MS;
  unsigned long velocity = 127 - (t * 127 / DIFFERENCE_TIME_MS);
  byte vel = (((velocity * velocity) >> 7) * velocity) >> 7;
  byte key = 36 + keyIndex;
  char out[32];
  sprintf(out, "%02X %02X %03d %d", statusByte, key, vel, time);
  Serial.println(out);
  if (statusByte == 0x90) {
    //MIDI.sendNoteOn(key, vel, 1);
  }
  if (statusByte == 0x80) {
    //MIDI.sendNoteOff(key, vel, 1);
  }
  /*
    Serial.write(statusByte);
    Serial.write(key);
    Serial.write(vel);
  */
}

//this function gets called EVENT based, in that it only gets called when a pin value changes
//remember there's two pins for each note, I call them 1st and 2nd contacts
void processPinValueChange(byte keyNumberInQuestion, bool onOrOff) {
  byte keyNumber = keyNumberInQuestion;
  if (keyNumberInQuestion > 99) {
    keyNumber = keyNumber - 100;
  }
  /*
  Serial.print("keyNumber:  ");
  Serial.print(keyNumberInQuestion);
  Serial.print(" onOrOff: ");
  Serial.print(onOrOff);
  Serial.println("----");
  */
  unsigned long currentTime = millis();
  if (keyNumberInQuestion > 99) {
    //FIRST CONTACT
    if (onOrOff == 1) {
      //first contact on
      keysTime[keyNumber] = currentTime;
      keysState[keyNumber][0] = 1;
    } else {
      //first contact off
      keysState[keyNumber][0] = 0;
      if (keysState[keyNumber][1] == 0) {
        //make the 2nd contact a fake value of 1
        keysState[keyNumber][1] = 1;
        Serial.print("NOTE OFF: ");
        Serial.println("   ");
        sendMidiEvent(0x80, keyNumber, currentTime-keysTime[keyNumber]);
      }
    }
  } else {
    //SECOND CONTACT
    if (onOrOff == 1) {
      //second contact on
      keysState[keyNumber][1] = 1;
      //it's only truly a note on if the 1st contact value isn't still zero
      if (keysState[keyNumber][0] == 1) {
        //make the 1st contact a fake value of 0
        keysState[keyNumber][0] = 0;
        Serial.print("NOTE ON: ");
        Serial.println("   ");
        sendMidiEvent(0x90, keyNumber, currentTime-keysTime[keyNumber]);
      }
    } else {
      //second contact off
      keysState[keyNumber][1] = 0;
      keysTime[keyNumber] = currentTime;
    }
  }
}

void loop() {
  boolean *s = signals;
  //we wanna go through the input pins
  for (byte i = 0; i < sizeof(inputPins); i++) {
    //and for each input pin, try the output pins
    byte inputPin = inputPins[i];
    for (byte j = 0; j < sizeof(outputPins); j++) {
      byte outputPin = outputPins[j];
      //turn on the circuit for that output pin
      digitalWrite2(outputPin, LOW);
      s++;
      boolean lastValue = *s;
      *s = !digitalRead2(inputPin);
      digitalWrite2(outputPin, HIGH);
      byte keyNumberInQuestion = keyValuesForIterators[i][j];
      if ((lastValue != *s) && keyNumberInQuestion != 200) {
        processPinValueChange(keyNumberInQuestion, *s);
      }
    }
  }
}
