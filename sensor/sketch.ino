
#include <PubSubClient.h>
#include <WiFi.h>

#define PIN_LED 2   // LED
#define PIN_LDR 34  // pino do sensor

/* Configura os tópicos do MQTT */
#define TOPIC_SUBSCRIBE_INTENSITY_LEVEL "praga-on_off_int_level"
#define TOPIC_PUBLISH_LUM_CHECKER "praga-lum-checker"

#define PUBLISH_DELAY 2000  // Atraso da publicação (2 segundos)

#define ID_MQTT \
  "praga-ldr_esp32_mqtt_lum_checker"  // id mqtt (para identificação de sessão)

#define LUMINOSITY_THRESHOLD 500

/* Variaveis, constantes e objetos globais */
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";

// URL do broker MQTT que se deseja utilizar
const char *BROKER_MQTT = "broker.emqx.io";

int BROKER_PORT = 1883;  // Porta do Broker MQTT

unsigned long publishUpdate;
static char strLuminosity[10] = {0};

// Variáveis e objetos globais
WiFiClient espClient;
PubSubClient MQTT(espClient);

/* Prototypes */
int getLuminosity(void);

void initWiFi(void);
void initMQTT(void);
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void checkWiFIAndMQTT(void);

/*Luminosity*/
const float GAMMA = 0.7;
const float RL10 = 50;
int getLuminosity(void) {
  int analogValue = analogRead(PIN_LDR);
  float voltage = analogValue / 4096. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  Serial.println("Lux Value:");
  Serial.println(lux);
  return lux;
}

/* Inicializa e conecta-se na rede WI-FI desejada */
void initWiFi(void) {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconnectWiFi();
}

/* Inicializa os parâmetros de conexão MQTT(endereço do broker, porta e seta
  função de callback) */
void initMQTT(void) {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

/* Função de callback  esta função é chamada toda vez que uma informação
   de um dos tópicos subescritos chega */
void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg;

  // Obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }

  Serial.printf("Chegou a seguinte string via MQTT: %s do topico: %s\n", msg,
                topic);

  /* Toma ação dependendo da string recebida pelo mqtt sub*/
  if (msg.equals("on")) {
    digitalWrite(PIN_LED, HIGH);
    Serial.print("LED ligado por comando MQTT");
  }

  if (msg.equals("off")) {
    digitalWrite(PIN_LED, LOW);
    Serial.print("LED desligado por comando MQTT");
  }
}

/* Reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a
   conexão cair) em caso de sucesso na conexão ou reconexão, o subscribe dos
   tópicos é refeito. */
void reconnectMQTT(void) {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_INTENSITY_LEVEL);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Nova tentativa de conexao em 2 segundos.");
      delay(2000);
    }
  }
}

/* Verifica o estado das conexões WiFI e ao broker MQTT.
  Em caso de desconexão (qualquer uma das duas), a conexão é refeita. */
void checkWiFIAndMQTT(void) {
  if (!MQTT.connected())
    reconnectMQTT();  // se não há conexão com o Broker, a conexão é refeita

  reconnectWiFi();  // se não há conexão com o WiFI, a conexão é refeita
}

void reconnectWiFi(void) {
  // se já está conectado a rede WI-FI, nada é feito.
  // Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(SSID, PASSWORD);  // Conecta na rede WI-FI

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

void setup() {
  Serial.begin(115200);

  // Configura o pino do LED como output e inicializa em nível baixo
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  // Inicializa a conexao Wi-Fi
  initWiFi();

  // Inicializa a conexao ao broker MQTT
  initMQTT();
}

void loop() {
  // Processa as conexões e mensagens MQTT continuamente
  checkWiFIAndMQTT();
  MQTT.loop();

  // Verifica se passou o tempo para publicar a atualização
  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();

    int luminosity = getLuminosity();
    sprintf(strLuminosity, "%d", luminosity);

    // Publica o valor lido no broker MQTT
    MQTT.publish(TOPIC_PUBLISH_LUM_CHECKER, strLuminosity);

    // Print no Serial para depuração
    Serial.print("Valor do sensor: ");
    Serial.print(luminosity);
    Serial.print(" | Valor enviado MQTT: ");
    Serial.println(strLuminosity);
  }
}
