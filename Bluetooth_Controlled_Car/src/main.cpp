#include <Arduino.h>
#include <TM1637Display.h>

//Pinovi za displej
#define CLK 2
#define DIO 4
//Pinovi za motore
#define motorNaprijedA 11
#define motorNaprijedB 6
#define motorNazadA 10
#define motorNazadB 9
//Pinovi za ultrasonicni senzor
#define trigPin 3
#define echoPin 5


TM1637Display display = TM1637Display(CLK, DIO);
char t;
int udaljenost;
long trajanje;
const uint8_t upali[] = {0xff, 0xff, 0xff, 0xff}; //Za paljenje svih segmenata na displeju
const uint8_t ugasi[] = {0x00, 0x00, 0x00, 0x00}; //Za gasenje svih segmenata na displeju
const uint8_t velikaUdaljenost[] = { SEG_G, SEG_G, SEG_G, SEG_G }; //Custom karakter
unsigned long vrijeme;
void izracunajUdaljenost();
void naprijed();
void nazad();
void lijevo();
void desno();
void stop();

void setup() {
  //Motori
  pinMode(motorNaprijedB, OUTPUT); 
  pinMode(motorNazadB, OUTPUT); 
  pinMode(motorNaprijedA, OUTPUT);  
  pinMode(motorNazadA, OUTPUT); 
  //Ultrasocnicni senzor
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  
  //Display
  display.clear();
  display.setBrightness(7);
  display.setSegments(upali);
  delay(1500);
  display.setSegments(velikaUdaljenost);
  delay(1500);
  display.clear();
  display.setSegments(ugasi);
  display.clear();
  //Otvaranje komunikacije
  Serial.begin(9600);
}

void loop() {
  if(millis() - vrijeme > 400){
    vrijeme = millis();
    izracunajUdaljenost();
    if(udaljenost < 3000)
      display.showNumberDec(udaljenost);
    else
      display.setSegments(velikaUdaljenost);
  }

  if (Serial.available()) {
    t = Serial.read();
  }

  if (t == 'F' && udaljenost > 10)  
    naprijed();
    else if (t == 'B')  
      nazad();
    else if (t == 'L')    
      lijevo();
    else if (t == 'R') 
      desno();
    else if (t == 'S' || udaljenost < 10)
      stop();
}

void naprijed() {
  digitalWrite(motorNaprijedA, HIGH);
  digitalWrite(motorNaprijedB, HIGH);
  digitalWrite(motorNazadA, LOW);
  digitalWrite(motorNazadB, LOW);
}

void nazad() {
  digitalWrite(motorNazadA, HIGH);
  digitalWrite(motorNazadB, HIGH);
  digitalWrite(motorNaprijedA, LOW);
  digitalWrite(motorNaprijedB, LOW);
}

void lijevo() {
  digitalWrite(motorNaprijedA, HIGH);
  digitalWrite(motorNazadA, LOW);
  digitalWrite(motorNaprijedB, LOW);
  digitalWrite(motorNazadB, LOW);
}

void desno() {
  digitalWrite(motorNaprijedA, LOW);
  digitalWrite(motorNazadA, LOW);
  digitalWrite(motorNaprijedB, HIGH);
  digitalWrite(motorNazadB, LOW);
}

void stop() {
  digitalWrite(motorNaprijedA, LOW);
  digitalWrite(motorNaprijedB, LOW);
  digitalWrite(motorNazadA, LOW);
  digitalWrite(motorNazadB, LOW);
}

void izracunajUdaljenost(){
  digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    trajanje = pulseIn(echoPin, HIGH);
    udaljenost = trajanje * 0.034 / 2; 
}