#include <Servo.h>
 
Servo monServo;  

//BROCHES SERVOMOTEUR & LEDS
const int boutonservo1 = 2;
const int boutonservo2 = 4;
const int boutonservo3 = 7;
 
const int ledservo1 = 10;
const int ledservo2 = 11;
const int ledservo3 = 12;

// BROCHES VANNE 12V 
const int boutonVanne = 5; 
const int pinRelais = 8; 

// BROCHE CAPTEUR DE DÉBIT
const int pinDebit = 3; 
volatile int compteurImpulsions = 0; 
unsigned long tempsPrecedent = 0; 

// Fonction qui s'exécute à chaque tour de roue du capteur
void compter() {
  compteurImpulsions++;
}

void setup() {

  Serial.begin(9600); 

  monServo.attach(9);

  // Initialisation des boutons du servo
  pinMode(boutonservo1, INPUT_PULLUP);
  pinMode(boutonservo2, INPUT_PULLUP);
  pinMode(boutonservo3, INPUT_PULLUP);

  // Initialisation des LEDs
  pinMode(ledservo1, OUTPUT);
  pinMode(ledservo2, OUTPUT);
  pinMode(ledservo3, OUTPUT);
  digitalWrite(ledservo1, LOW);
  digitalWrite(ledservo2, LOW); 
  digitalWrite(ledservo3, LOW);

  // Position d'origine
  monServo.write(90); 

  // Initialisation de la vanne
  pinMode(boutonVanne, INPUT_PULLUP);
  pinMode(pinRelais, OUTPUT);
  digitalWrite(pinRelais, LOW); 

  // Initialisation du capteur de débit
  pinMode(pinDebit, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinDebit), compter, RISING);
}
 
void loop() {

  // CALCUL ET AFFICHAGE DU DÉBIT (une fois par seconde)
  if (millis() - tempsPrecedent > 1000) {
    tempsPrecedent = millis();
    
    // Convertit les impulsions en Litres par minute 
    float debitLmin = compteurImpulsions / 7.5;
    
    // Envoie le texte vers le moniteur série de l'Arduino
    Serial.print("Debit d'eau : ");
    Serial.print(debitLmin);
    Serial.println(" L/min");
    
    compteurImpulsions = 0; // Remet à zéro pour la seconde suivante
  }

  // GESTION DU SERVOMOTEUR
  if (digitalRead(boutonservo1) == LOW) {
    monServo.write(60);
    digitalWrite(ledservo1, HIGH);
    digitalWrite(ledservo2, LOW); 
    digitalWrite(ledservo3, LOW);            
  }
 
  if (digitalRead(boutonservo2) == LOW) {
    monServo.write(30); 
    digitalWrite(ledservo1, HIGH);
    digitalWrite(ledservo2, HIGH); 
    digitalWrite(ledservo3, LOW);   
  }
 
  if (digitalRead(boutonservo3) == LOW) {
    monServo.write(0); 
    digitalWrite(ledservo1, HIGH);
    digitalWrite(ledservo2, HIGH); 
    digitalWrite(ledservo3, HIGH);   
  }

  // GESTION DE LA VANNE 12V
  if (digitalRead(boutonVanne) == LOW) {
    digitalWrite(pinRelais, HIGH); 
  } else {
    digitalWrite(pinRelais, LOW);  
  }
  delay(50);
}