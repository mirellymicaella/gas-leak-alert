#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define LED_BUILTIN 2  //For node_MCU Lolin esp8266
// Update these with values suitable for your network.

#define DELAY 3000  //Manda mensagem para o servido a cada DELAY/1000 segundos.

int data;
//Configurações de rede WiFi.
//const char* ssid = "pMoto Z3 Play 9547";
//onst char* password = "d869ff80753";
const char* ssid = "Mi";
const char* password = "abcdefgh";
//Sevidor mqtt.
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
//long lastMsg = 0;
char msg[50];

//Configura a rede WiFi.
void setup_wifi(){

  delay(10);
//Tentativa para conectar o WiFi
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Enderço de IP: ");
  Serial.println(WiFi.localIP());
}

//Configuração para receber mensagens do servidor.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);   // Change for nodeMCU Lolin
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);   // Change for nodeMCU Lolin
  }
}

//Caso nao for possível conectar, tenta de novo até conseguir.
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentativa de conexão MQTT...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      // Once connected, publish an announcement...
      client.publish("pic20192", "Sucesso!");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      client.publish("pic20192", "Falhou! Tentando de novo em 5 segundos");
      Serial.println(" tentando de novo em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Just for ESP8266 nodeMCU Lolin
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

//  long now = millis();
   
//  if (now - lastMsg > DELAY) {
//    lastMsg = now;
//  if(Serial.available()){
    data = Serial.read();
    Serial.println(data);
//    delay(1000);
    if(data=='1'){
      snprintf (msg, 50, "GAS DETECTADO!");
      Serial.print("Mensagem publicada: ");
      Serial.println(msg);
      client.publish("pic20192", msg);
    }
}
