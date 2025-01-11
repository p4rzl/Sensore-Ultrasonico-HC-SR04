// Definizione dei pin
const int trigger = 5;
const int echo = 7;
const float c = 0.0343 / 2; // Velocità del suono in cm/us diviso 2

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
  if (spazio <= 4) {
    digitalWrite(A0, HIGH); // LED acceso
  } else if (spazio > 4 && spazio < 8) {
    digitalWrite(A0, HIGH); // LED lampeggia
    delay(500);
    digitalWrite(A0, LOW);
  } else {
    digitalWrite(A0, LOW); // LED spento
  }

}
