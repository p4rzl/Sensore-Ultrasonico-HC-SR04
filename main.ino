#include <WiFi.h>
#include <FirebaseESP32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Imposta le credenziali WiFi
const char* ssid = "OnePlus Nord 2 5G";
const char* password = "Luca1503";

// Imposta le credenziali Firebase
#define FIREBASE_HOST "bidone-arduino-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "f876d543-a23c-4987-b954-2e7890456789"

// Crea un'istanza di Firebase
FirebaseData firebaseData;

// Definizione dei pin
const int trigger = 5;
const int echo = 18;
const float c = 0.0343 / 2; // Velocità del suono in cm/us diviso 2

// Variabili per il tempo
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // Aggiorna ogni minuto

unsigned long lastSendTime = 0;
const unsigned long interval = 900000; // 15 minuti in millisecondi

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void setupFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void sendToFirebase(float distance, String timestamp, String position) {
  FirebaseJson json;
  json.set("/timestamp", timestamp);
  json.set("/position", position);
  json.set("/level", distance);

  if (Firebase.pushJSON(firebaseData, "/sensor/data", json)) {
    Serial.println("Data sent to Firebase");
  } else {
    Serial.println("Failed to send data to Firebase");
    Serial.println(firebaseData.errorReason());
  }
}

void setup() {
  // Definizione di input e output
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(4, OUTPUT);

  // Inizializzazione della comunicazione seriale
  Serial.begin(9600);

  // Connessione al WiFi
  connectToWiFi();

  // Configurazione di Firebase
  setupFirebase();

  // Inizializzazione del client NTP
  timeClient.begin();
}

void loop() {
  long tempo;
  float spazio;

  // Prima della misurazione si porta trigger a livello basso
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);

  // Invio di un impulso (porta trigger a livello alto per 10 microsecondi)
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  // Lettura di echo, tramite pulseIn, che restituisce la durata dell'impulso (in microsecondi)
  // Il dato acquisito viene poi diviso per 2 (andata e ritorno)
  tempo = pulseIn(echo, HIGH) / 2;

  // Calcolo della distanza in centimetri
  spazio = tempo * c;

  // Visualizzazione della distanza sul Serial Monitor
  Serial.print("Distanza = " + String(spazio, 1) + " cm\n");

  delay(100);

  // Logica di segnalazione con LED o buzzer
  if (spazio <= 2) {
    digitalWrite(A0, HIGH); // LED acceso
  } else if (spazio > 2 && spazio < 5) {
    digitalWrite(A0, HIGH); // LED lampeggia
    delay(500);
    digitalWrite(A0, LOW);
  } else {
    digitalWrite(A0, LOW); // LED spento
  }

  // Invia i dati a Firebase ogni 15 minuti
  if (millis() - lastSendTime >= interval) {
    lastSendTime = millis();
    timeClient.update();
    String timestamp = timeClient.getFormattedTime();
    String position = "YourPosition"; // Sostituisci con la tua posizione
    sendToFirebase(spazio, timestamp, position);
  }
}
