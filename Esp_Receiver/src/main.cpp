#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <SSD1306.h>
#include <list>

#define BAND 915E6

const char* ssid = "LaboratorioE117";
const char* password = "LAB@117";

char ip_conv[14];
String IP = "172.24.1.2";

const char* BROKER_MQTT;
int BROKER_PORT = 1883;

String mensagem;
String localAddress;

char info_topico[25];

const int LORA_SCK_PIN  = 5;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 27;
const int LORA_SS_PIN   = 18;
const int LORA_RST_PIN  = 15;
const int LORA_DI00_PIN = 26;

// Dados de IP da rede
IPAddress local_IP(172,24,1,52);
IPAddress gateway(172,24,1,1);
IPAddress subnet(255,255,0,0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);

// Variável para controlar o display
const int DISPLAY_ADDRESS_PIN = 60;
const int DISPLAY_SDA_PIN     = 4;
const int DISPLAY_SCL_PIN     = 15;
const int DISPLAY_RST_PIN     = 16;
SSD1306 display(DISPLAY_ADDRESS_PIN, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

// variaveis para indicação de nucleo
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne = 1;

int travaSite = 0;
String mes;

WiFiClient espCliente;
HTTPClient http;


void setup(){
}

void loop() {}

/*
  --- setupDisplay() ---
  O estado do GPIO16 é utilizado para controlar o display OLED
  digitalWrite(16, LOW) Reseta as configurações do display OLED
  Para o OLED permanecer ligado, o GPIO16 deve permanecer HIGH
  Deve estar em HIGH antes de chamar o display.init() e fazer as demais configurações,
*/

void setupDisplay(){
  
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  digitalWrite(16, HIGH);

  // não inverta a ordem
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

bool loraBegin(){

}