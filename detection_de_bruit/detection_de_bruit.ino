#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = ".........";
const char* password = "........";

// ThingSpeak
const char* server = "http://api.thingspeak.com/update";
String apiKey = "RVLB8LR1EE38CA70";

// Broches
const int soundPin = 34;     // Broche analogique pour capteur sonore
const int ledPin = 15;       // LED normale 
const int buzzerPin = 13;    // Buzzer

unsigned long lastTime = 0;
unsigned long timerDelay = 15000; // 15 secondes

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Connexion Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connexion Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté à Wi-Fi !");
}

void loop() {
  int soundValue = analogRead(soundPin);
  Serial.print("Valeur bruit : ");
  Serial.println(soundValue);

  // LED + buzzer si bruit > 20
  if (soundValue > 20) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  // Envoi vers ThingSpeak toutes les 15 secondes
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = server + String("?api_key=") + apiKey + "&field1=" + String(soundValue);
      http.begin(url);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("Données envoyées ! Code : ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Erreur HTTP : ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
    lastTime = millis();
  }

  delay(1000);
}
