/*
//Arduino PWM Speed Control：
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;
int value = 255;
void setup()
{
    
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(E1, OUTPUT);
    pinMode(E2, OUTPUT);
}

void loop()
{  
  digitalWrite(M1, 0);
  digitalWrite(M2, 0);
  analogWrite(E1, value);   //PWM Speed Control
  analogWrite(E2, value);
  delay(3000);
  digitalWrite(M2, 1);
  delay(1000);
  
}

#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
}

void loop() {
  if (bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(c);
  }
  if (Serial.available()) {
    char c = Serial.read();
    bluetooth.print(c);
  }
}
*/
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11); // RX, TX

// Définition des broches utilisées pour le contrôle des moteurs
const int motorLeftSpeed = 5;
const int motorLeftPin1 = 4;
const int motorRightSpeed = 6;
const int motorRightPin1 = 7;

// Variables pour stocker les commandes reçues par Bluetooth
char command;
int speedLeft = 0;
int speedRight = 0;

void setup() {
  // Configuration des broches en sortie pour le contrôle des moteurs
  pinMode(motorLeftPin1, OUTPUT);
  pinMode(motorLeftSpeed, OUTPUT);
  pinMode(motorRightPin1, OUTPUT);
  pinMode(motorRightSpeed, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(9, 1);

  // Configuration de la communication série à 9600 bauds
  Serial.begin(9600);
  bluetooth.begin(38400);
}

void loop() {
  // Vérification de la présence de données sur la communication série
  
  if ((Serial.available()) | bluetooth.available()) {
    
    // Lecture de la commande envoyée par Bluetooth
    if (Serial.available()) {
      command = Serial.read();
      bluetooth.write(Serial.read());
    
    }   
    else if (bluetooth.available()) {
      command = bluetooth.read();
      Serial.write(bluetooth.read());      
    }     
    Serial.print(command);

    // Traitement de la commande
    switch (command) {
      case 'B': // Reculer
        speedLeft = 255;
        speedRight = 255;
        break;
      case 'F': // Avancer
        speedLeft = -255;
        speedRight = -255;
        break;
      case 'L': // Tourner à gauche
        speedLeft = -150;
        speedRight = 150;
        break;
      case 'R': // Tourner à droite
        speedLeft = 150;
        speedRight = -150;
        break;
      case 'S': // Stop
        speedLeft = 0;
        speedRight = 0;
        break;
    }

    // Contrôle des moteurs en fonction des vitesses calculées
    if (speedLeft > 0) {
      digitalWrite(motorLeftPin1, HIGH);
      analogWrite(motorLeftSpeed, speedLeft);
    } else if (speedLeft < 0) {
      digitalWrite(motorLeftPin1, LOW);
      analogWrite(motorLeftSpeed, abs(speedLeft));
    } else {
      digitalWrite(motorLeftPin1, LOW);
      analogWrite(motorLeftSpeed, 0);
    }

    if (speedRight > 0) {
      digitalWrite(motorRightPin1, HIGH);
      analogWrite(motorRightSpeed, speedRight);
    } else if (speedRight < 0) {
      digitalWrite(motorRightPin1, LOW);
      analogWrite(motorRightSpeed, abs(speedRight));
    } else {
      digitalWrite(motorRightPin1, LOW);
      analogWrite(motorRightSpeed, 0);
    }
  }
}
