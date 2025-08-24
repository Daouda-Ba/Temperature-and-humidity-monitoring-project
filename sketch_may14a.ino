#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

const char* ssid = "YOUR_SSID";        // Remplacez par le SSID de votre réseau WiFi
const char* password = "YOUR_PASSWORD"; // Remplacez par le mot de passe de votre réseau WiFi
const char* serverUrl = "http://192.168.147.70:8000/api/recevoir/";  // IP Django locale

#define DHTPIN 14       // D5 (GPIO14)
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== DÉMARRAGE ESP8266 ===");
  
  dht.begin();
  Serial.println("Capteur DHT11 initialisé");

  WiFi.begin(ssid, password);
  Serial.print("Connexion WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n=== WiFi CONNECTÉ ===");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Adresse IP ESP8266: ");
  Serial.println(WiFi.localIP());
  Serial.print("Passerelle: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Serveur cible: ");
  Serial.println(serverUrl);
  Serial.println("========================");
}

void loop() {
  Serial.println("\n--- Nouvelle lecture ---");
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Vérification des lectures du capteur
  if (isnan(t) || isnan(h)) {
    Serial.println("❌ ERREUR: Échec de lecture du capteur DHT11!");
    Serial.println("Vérifiez les connexions:");
    Serial.println("- VCC -> 3.3V ou 5V");
    Serial.println("- GND -> GND");
    Serial.println("- DATA -> D5 (GPIO14)");
    delay(5000);
    return;
  }

  Serial.print("✅ Température: ");
  Serial.print(t);
  Serial.print(" °C\tHumidité: ");
  Serial.print(h);
  Serial.println(" %");

  // Vérification de la connexion WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ ERREUR: WiFi déconnecté!");
    delay(5000);
    return;
  }

  // Envoi des données
  Serial.println("📡 Envoi des données au serveur...");
  
  WiFiClient client;
  HTTPClient http;
  
  // Configuration HTTP avec timeout
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000); // 10 secondes de timeout
  
  // Création du JSON
  String json = "{\"temperature\": " + String(t, 2) + ", \"humidity\": " + String(h, 2) + "}";
  Serial.print("📄 JSON envoyé: ");
  Serial.println(json);
  
  // Envoi POST
  int httpCode = http.POST(json);
  
  Serial.print("📈 Code de réponse HTTP: ");
  Serial.println(httpCode);
  
  // Analyse de la réponse
  if (httpCode > 0) {
    String response = http.getString();
    Serial.print("📋 Réponse du serveur: ");
    Serial.println(response);
    
    if (httpCode == 200 || httpCode == 201) {
      Serial.println("✅ Données envoyées avec succès!");
    } else {
      Serial.println("⚠️  Réponse inattendue du serveur");
    }
  } else {
    Serial.print("❌ ERREUR HTTP: ");
    Serial.println(http.errorToString(httpCode));
    Serial.println("Vérifiez:");
    Serial.println("- L'adresse IP du serveur Django");
    Serial.println("- Le port 8000 est-il ouvert?");
    Serial.println("- Le serveur Django fonctionne-t-il?");
  }
  
  http.end();
  
  Serial.println("⏳ Attente 15 secondes...");
  delay(15000);
}