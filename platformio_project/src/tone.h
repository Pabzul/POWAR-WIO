#pragma once

/* Macro Define */
#define BUZZER_PIN WIO_BUZZER /* sig pin of the buzzer */

int length = 2; /* the number of notes */
char notes[] = "dfdb";
int beats[] = {1, 1, 1, 2};
int tempo = 200;

// Play tone
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
  int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void looptone() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo);
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    delay(tempo / 2); /* delay between notes */
  }
}
