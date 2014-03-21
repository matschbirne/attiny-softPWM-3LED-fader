

int pins[] = {4,3,2};
int but = 1;
int fadeSpeed = 20;
int overlap = 80;

//some help variables
unsigned long lastHit;
int tmp, delay1, delay2, delay3;
int nrPins = sizeof(pins) / sizeof(int);
int curr = 0;
int next = 0;


void setup(){
  for (int i = 0; i < nrPins; i++) {
    pinMode(pins[i], OUTPUT);
  }
  pinMode(but,INPUT);
}

void loop(){
  next = nextPin(curr);
  
    for(int fade=overlap;fade<255;fade++) { //fade on
      if (digitalRead(but) == HIGH) {    //entering menu
        menu();
      }
      softPWM(pins[curr], fade, fadeSpeed);
    }
    for(int fade=254;fade>=1;fade--) { //fade off 
      if (digitalRead(but) == HIGH) {    //entering menu
        menu();
      }
      if (fade < overlap) {
       // softPWM(pins[next], overlap-fade,fadeSpeed);
       // softPWM(pins[curr], fade, fadeSpeed);
        softPWM2(pins[curr], pins[next], fade, overlap-fade, fadeSpeed);
      } else {
        softPWM(pins[curr], fade, fadeSpeed);
      }
    }
   curr = next;
}

void softPWM(int pin, int freq, int sp) { // software PWM function that fakes analog output
  delay1 = 255 - freq;
  while (sp>0) {
    digitalWrite(pin,HIGH); //on
    delayMicroseconds(freq);
    digitalWrite(pin,LOW); //off
    delayMicroseconds(delay1);
    sp--;
  }
}



void softPWM2(int pin1, int pin2, int freq1, int freq2, int sp) { // software PWM function that fakes analog output
  if (freq1 > freq2) {
    tmp = freq1;
    freq1 = freq2;
    freq2 = tmp;
    tmp = pin1;
    pin1 = pin2;
    pin2 = tmp;
  }
  
  delay1 = freq1;
  delay2 = freq2 - freq1 + 1;
  delay3 = 255 - freq2;
  
  while (sp>0) {
    digitalWrite(pin1,HIGH); //on
    digitalWrite(pin2,HIGH);
    delayMicroseconds(delay1);
    digitalWrite(pin1,LOW); //off
    delayMicroseconds(delay2);
    digitalWrite(pin2, LOW);
    delayMicroseconds(delay3);
    sp--;
  }
}

int nextPin(int current) {
  return (current + 1) % nrPins;
}




void menu() {
   digitalWrite(pins[nextPin(curr)],HIGH);
   while (digitalRead(but) == HIGH) {}//active waiting 
   lastHit = millis();
   fadeSpeed = 1;
   while (lastHit + 2000 > millis()) {
     if (digitalRead(but) == HIGH) {
       fadeSpeed += fadeSpeed/2 + 1;
       digitalWrite(pins[nextPin(nextPin(curr))],HIGH);
       delayMicroseconds(70000);
       digitalWrite(pins[nextPin(nextPin(curr))],LOW);
       if (fadeSpeed > 500) {
         fadeSpeed = 500;
         digitalWrite(pins[nextPin(nextPin(curr))],HIGH);
       }
       while (digitalRead(but) == HIGH) {}
       lastHit = millis();
    }
  }
  digitalWrite(pins[nextPin(nextPin(curr))],HIGH);
  lastHit = millis();
  overlap = 1;
  digitalWrite(pins[nextPin(curr)],LOW);
  delay(150);
  digitalWrite(pins[nextPin(curr)],HIGH);
  while (lastHit + 2000 > millis()) {
   if (digitalRead(but) == HIGH) {
     digitalWrite(pins[curr],HIGH);
     delayMicroseconds(70000);
     digitalWrite(pins[curr],LOW);
     overlap += 10;
     if (overlap > 254) {
       overlap = 254;
       digitalWrite(pins[curr],HIGH);
     }
     while (digitalRead(but) == HIGH) {}
     lastHit = millis();
   }
 }
 digitalWrite(pins[curr],HIGH);
 delay(500);
 for (int i = 0; i < nrPins; i++) {
   digitalWrite(pins[i],LOW);
 }
}
