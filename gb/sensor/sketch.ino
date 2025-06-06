/*********
  Rui Santos
  Complete project details at
https://RandomNerdTutorials.com/esp32-hc-sr04-ultrasonic-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// importar livrarias de broker
#include <PubSubClient.h>
#include <WiFi.h>

const int trigPin = 5;
const int echoPin = 18;

// Velocidade do som em cm/uS
#define SPEED_TO_SOUND_CM 0.034

// Configuração MQTT
#define BROKER_MQTT = "broker.emqx.io";
#define BROKER_PORT = 1883;  // Porta do Broker MQTT
#define SUB_TOPIC = "sensor_delay"
#define PUB_TOPIC = "water_level"

// id mqtt (para identificação de sessão) - TODO
#define ID_MQTT "sensor"

//  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// Rede Wi-Fi
#define SSID = "Wifi";
#define PASSWORD = "";

// Variáveis e objetos globais

WiFiClient espClient;
PubSubClient MQTT(espClient);

uint32_t globalDelay = 2000;

unsigned long publishUpdate;
long duration;
float distanceCm;

static char strDistanceCm[6] = {0};

// Se já está conectado a rede WI-FI, nada é feito.
// Caso contrário, são efetuadas tentativas de conexão
void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

// Inicializa e conecta-se na rede WI-FI desejada
void initWiFi(void) {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconnectWiFi();
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg;

  // Obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }

  uint32_t newDelay = msg.toInt();

  Serial.printf("Chegou a seguinte string via MQTT: %s do topico: %s\n", msg,
                topic);

  globalDelay = newDelay;
}

void initMQTT(void) {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
  MQTT.subscribe(SUB_TOPIC);
}

void setup() {
  // Inicia o monitor serial
  Serial.begin(115200);

  // Define o trigPin e echoPin como saída e entrada, respectivamente
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  initWiFi();
  initMQTT();
}

// Enquanto não estiver conectado ao broker MQTT, tenta reconectar
void reconnectMQTT(void) {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT + WiFi.macAddress())) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(SUB_TOPIC);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Nova tentativa de conexao em 2 segundos.");
      delay(2000);
    }
  }
}

void checkWiFIAndMQTT(void) {
  if (!MQTT.connected()) reconnectMQTT();

  reconnectWiFi();
}

void wifiLoop(float distance) {
  checkWiFIAndMQTT();
  MQTT.loop();

  // Publica o valor lido no broker MQTT
  sprintf(strDistanceCm, "%.2f", distance);

  String strDistanceCm =
      "{distance:" + strDistanceCm + ",id: " + WiFi.macAddress() + "}";

  MQTT.publish(PUB_TOPIC, strDistanceCm);
}

void loop() {
  // Liga para disparar um pulso:
  // Inicia trigPin em LOW e aguarda 2 micro segundos
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Definne trigPin em HIGH e aguarda 10 micro segundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  // Retorna trigPin para LOW
  digitalWrite(trigPin, LOW);

  // Lê o echoPin, retorna o tempo de viagem da onda sonora em micro segundos
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  distanceCm = duration * SPEED_TO_SOUND_CM / 2;

  // Imprime a distância no Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  // Loop para enviar a distância via MQTT
  wifiLoop(distanceCm);

  // Aguarda o tempo global definido antes de verificar novamente
  delay(globalDelay);
}