#define BUZZER D8
int speakerPin = D8;
void oscillate(int pin, int lowTone, int highTone, int durationLow, int durationHigh, int cycles) {
  for (int i = 0; i < cycles; i++) {
    if (i%2 == 0) {
       tone(pin, lowTone, durationLow);
       delay(durationLow);
    } else {
       tone(pin, highTone, durationHigh);
       delay(durationHigh);
    }
  } 
}

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1 };
int tempo = 100;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void sampleSound1(int pin) {
  pinMode(pin, OUTPUT);
  for (int i = 0; i < 20; i++) {
    tone(pin, (int)(500+(20*i)), 25);
    delay(30);
  }
 delay(40);
 oscillate(pin, 1100, 1200, 15, 30, 40);
}

void sampleSound2(int pin) {

  for (int i = 10; i < 30; i++) {
    oscillate(pin, i*30, i*i, 20,20, 10);
 }
}

void sampleSound3(int pin) {

  for (int i = 30; i < 70; i++) {
    tone(pin, sqrt(i*i*i), 24);
    delay(26);
 }
 for (int i = 10; i < 60; i++) {
    tone(pin, sqrt(i*i*i), 26);
    delay(28);
 }
  for (int i = 60; i >= 10; i--) {
    tone(pin, sqrt(i*i*i), 26);
    delay(28);
 }
}

void fanfare(int pin) {
  tone(pin, 700, 240);
  delay(240);
  tone(pin, 0, 10);
  delay(10);
  
  tone(pin, 700, 240);
  delay(240);
  tone(pin, 0, 10);
  delay(10);

  tone(pin, 700, 240);
  delay(240);
  tone(pin, 0, 10);
  delay(10);

  tone(pin, 700, 740);
  delay(740);
  tone(pin, 0, 10);
  delay(10);
  
  tone(pin, 500, 750);
  delay(750);
  
  tone(pin, 610, 750);
  delay(740);
  tone(pin, 0, 10);
  delay(10);
  
  tone(pin, 700, 500);
  delay(750);

  
  tone(pin, 610, 500);
  delay(250);
  
  tone(pin, 700, 750);
  delay(750);

}

void setup() {
  
  sampleSound1(BUZZER);
  delay(1000);
  sampleSound2(BUZZER);
  delay(1000);
  sampleSound3(BUZZER);
  delay(1000);
  fanfare(BUZZER);

}

void loop() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[random(4)] * 20); // rest
    } else {
      playNote(notes[random(0,length)], beats[random(4)] * 20);
    }

    // pause between notes
    delay(tempo / 2); 
  }

}
