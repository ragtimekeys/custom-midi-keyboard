#include <DIO2.h>

#define TOTAL_NUM_KEYS

//amount of time to detect velocity
#define MIN_TIME_MS   3
#define MAX_TIME_MS   50
#define MAX_TIME_MS_N (MAX_TIME_MS - MIN_TIME_MS)

byte outputPins[] = {
  30,
  32,
  34,
  36,
  38,
  40,
  42,
  44
};
byte inputPins[] = {
  31, 33,
  35, 37,
  39, 41,
  43, 45
};

byte keysState[TOTAL_NUM_KEYS];
unsigned long keysTime[TOTAL_NUM_KEYS];

void setup() {
  Serial.begin(115200);
  Serial.println("Listening to MIDI notes...");
  int i;
  //initialize all keys as off and no time
  for (i = 0; i < TOTAL_NUM_KEYS; i++) {
    keysState[i] = KEY_OFF;
    keysTime[i] = 0;
  }
  for (byte pin = 0; pin < sizeof(outputPins); pin++) {
    pinMode(outputPins[pin], OUTPUT);
  }
  for (byte pin = 0; pin < sizeof(inputPins); pin++) {
    pinMode(inputPins[pin], INPUT_PULLUP);
  }

}


void sendMidiEvent(byte statusByte, byte keyIndex, unsigned long time) {
  unsigned long t = time;
  if (t > MAX_TIME_MS)
    t = MAX_TIME_MS;
  if (t < MIN_TIME_MS)
    t = MIN_TIME_MS;
  t -= MIN_TIME_MS;
  unsigned long velocity = 127 - (t * 127 / MAX_TIME_MS_N);
  byte vel = (((velocity * velocity) >> 7) * velocity) >> 7;
  byte key = 36 + keyIndex;
  char out[32];
  sprintf(out, "%02X %02X %03d %d", statusByte, key, vel, time);
  Serial.println(out);
  /*
    Serial.write(statusByte);
    Serial.write(key);
    Serial.write(vel);
  */
}

void loop() {
  boolean *s = signals;
  //we wanna go through the input pins
  for (byte i = 0; i < sizeof(inputPins); i++) {
    //and for each input pin, try the output pins
    
    for (byte j = 0; j < sizeof(outputPins); j++) {
      //turn on the circuit for that output pin
      digitalWrite2(outputPin, LOW);
      *(s++) = !digitalRead2(input_pin);
      digitalWrite2(outputPin, HIGH);
    }
  }
  println();
}
