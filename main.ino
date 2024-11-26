/*

    CONTROLLO DEL SENSORE DI PROSSIMITA' HR-SC04

    Questo codice riceve i dati provenienti dal sensore di

    prossimita' HR-SC04, li analizza e li mostra a video nel

    monitor seriale.

    3A/EN, ITIS "Enrico Mattei", Urbino, 16 Febbraio 2021

*/

// Definizione dei pin trigger ed echo

#define trigger 2

#define echo 3

#define BUZZER_PIN 5

#define NOTE_C5 523

// Definizione della velocita' del suono (centimetri / microsecondo)

#define c 0.0343

// Definizione delle variabili

long tempo;

float spazio;

void setup() {

  // Definizione di input e output

  pinMode(trigger, OUTPUT);

  pinMode(echo, INPUT);

  pinMode(A0, OUTPUT);

  pinMode(4, OUTPUT);

  // Inizializzazione della comunicazione seriale

  Serial.begin(9600);

}

void loop() {

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

}
