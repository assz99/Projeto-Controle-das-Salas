#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include "SSD1306Wire.h"

#define BAND 921E6

const char* ssid = "LaboratorioE117";
const char* password = "LAB@E117";

int BROKER_PORT = 1883;
String IP = "172.24.1.2";
char ip_conv[14];
const char* BROKER_MQTT;

const int LORA_SCK_PIN 	= 5;
const int LORA_MISO_PIN 	= 19;
const int LORA_MOSI_PIN 	= 27;
const int LORA_SS_PIN 	= 18;
const int LORA_RST_PIN 	= 15;
const int LORA_DI00_PIN 	= 26;

// Dados IP rede
IPAddress local_IP(172,24,1,51);
IPAddress gateway(172, 24, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

String mensagem;
String localAddress;

char info_topico[25];
int estado = 0;

const int DISPLAY_ADDRESS_PIN = 0x3c;
const int DISPLAY_SDA_PIN 	= 4;
const int DISPLAY_SCL_PIN 	= 15;
const int DISPLAY_RST_PIN 	= 16;

SSD1306Wire display(DISPLAY_ADDRESS_PIN, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

// definicao das variaveis de threads do esp32
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne = 1;

WiFiClient espClient;
PubSubClient MQTT(espClient);		// instancia do MQTT

void initMQTT(){
	MQTT.setServer(BROKER_MQTT, BROKER_PORT);
	MQTT.setCallback(mqtt_callback);
}

void setupDisplay(){
	// O estado do GPIO16 é utilizado para controlar o display OLED
	pinMode(16,OUTPUT);
	// Reseta as configuracoes do display OLED
	digitalWrite(16, LOW);
	// Para o OLED permanecer ligado, o GPIO16 deve permanecer em HIGH
	// Deve estar em HIGH antes de chamar o display.init() e fazer as demais configuracoes
	// A ordem de declaracao deve ser mantida dessa forma:
	// pinMode() -> digitalWrite(LOW) -> digitalWrite(HIGH)
	digitalWrite(16, HIGH);

	// configuracoes do display
	display.init();
	display.flipScreenVertically();
	display.setFont(ArialMT_Plain_10);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
}

bool loraBegin(){
	// Inicializacao da comunicacao SPI
	SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
	// Setando valores dos pinos do lora
	LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI00_PIN); 
	// Inicializacao do lora
	return LoRa.begin(BAND);
}

void setup() {
	IP.toCharArray(ip_conv, 14);
	BROKER_MQTT = ip_conv;
	setupDisplay();
	initMQTT();
	Serial.begin(115200);
	Serial.println(BROKER_MQTT);
	LoRa.setPins(18, 14, 26);

	if( !loraBegin()){
		Serial.println("LoRa nao conseguiu inicializar...");
		while(true);
	}

	// Ativacao do crc
	LoRa.enableCrc();
	// Ativacao do recebimento de pacotes
	LoRa.receive();

	Serial.println("LoRa incializado com sucesso");
	Serial.println("Inicializando WiFi");

	if( !WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)){
		Serial.println("STA nao conseguiu configurar.");
	}

	// Tenta conexao com WiFi
	WiFi.begin(ssid, password);
	while( WiFi.status() != WL_CONNECTED ){
		delay(100);
		Serial.print(".");
	}
	Serial.println("WiFi conectado com sucesso: ");

	String MAC_LOCAL = WiFi.macAddress();
	char characterMacLocal[18];
	String(MAC_LOCAL).toCharArray(characterMacLocal, 18);
	char* macLocalData[6];

	macLocalData[0] = strtok(characterMacLocal, ":");
	macLocalData[1] = strtok(NULL,":");
	macLocalData[2] = strtok(NULL,":");
	macLocalData[3] = strtok(NULL,":");
	macLocalData[4] = strtok(NULL,":");
	macLocalData[5] = strtok(NULL,":");

	String builtMacLocal;
	for(int i=0; i<6; i++){ builtMacLocal += macLocalData[i];}
	localAddress = builtMacLocal;
	
	display.clear();
	display.drawString(0, 0, ""+String(MAC_LOCAL));
	display.drawString(0, 16, "Novo: "+String(builtMacLocal));
	display.drawString(0, 32, "[PRINCIPAL]");
	display.display();
	
	delay(500);
}
void loop() {}