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
String msg;

WiFiClient espCliente;
HTTPClient http;



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
  // Inicializacao da comunicacao SPI
  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
  // Pinos do LoRa setados
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI00_PIN);
  
  return LoRa.begin(BAND);
}

void onReceive(int packetSize){
  /*
    O recebimento funciona com o envio de um pacote de duas linhas (mensagem)
    A primeira linha é sempre o tamanho da mensagem
    A segunda é o conteudo da mensagem, como método de seguranca
    para garantir que todo o pacote chegou.  
  */
  
  if( packetSize == 0 ) return;  // se nenhuma mensagem foi recebida sai da funcao
  byte incomingLength = LoRa.read(); // tamanho da mensagem
  String incoming = "";
  while( LoRa.available() ){
    incoming += (char)LoRa.read();
  }
  if( incomingLength != incoming.length() ){
    Serial.println("Erro! o tamanho da mensagem não condiz com o conteudo");
    return;
  }

  Serial.println("Tamanho da mensagem: " + String(incomingLength ) );
  Serial.println("Mensagem: " + incoming);
  Serial.println();

  String addressWeb = "http://"+IP+"/acoes.php?atualizar=";
  addressWeb += incoming;
  msg = addressWeb;
  travaSite = 1;

}

void setup(){
  IP.toCharArray(ip_conv, 14);
  setupDisplay();
  Serial.begin(115200);

  if (!loraBegin()){
    Serial.println("LoRa não conseguiu inicializar...");
    while (true);
  }

  // Ativa crc
  LoRa.enableCrc();
  // Ativa o recebimento de pacotes
  LoRa.receive();
  LoRa.onReceive(onReceive); // callback
  Serial.println("LoRa inicializado com sucesso");
  Serial.println("Aguardando conexão");

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA não conseguiu configurar");
  }

  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED ){
    delay(100);
    Serial.print(".");
  }
  Serial.println("WiFi conectado com sucesso: ");

  // Contrucao do MAC do hardware para MAC byte
  String localMac = WiFi.macAddress();
  char characterMacLocal[18];
  String(localMac).toCharArray(characterMacLocal, 18);
  char* infoMacLocal[6]; 
  
  infoMacLocal[0] = strtok(characterMacLocal,":");
  infoMacLocal[1] = strtok(NULL, ":");
  infoMacLocal[2] = strtok(NULL, ":");
  infoMacLocal[3] = strtok(NULL, ":");
  infoMacLocal[4] = strtok(NULL, ":");
  infoMacLocal[5] = strtok(NULL, ":");
  String builtMac;

  for(int i=0; i<6; i++){
    builtMac += infoMacLocal[i];
  }

  localAddress = builtMac;
  display.clear();
  display.drawString(0, 0,  "" + String(localMac));
  display.drawString(0, 16, "Novo: " + String(builtMac));
  display.drawString(0, 32, "[RECEIVER]");
  display.display();
}

void site(){
  http.begin(msg);
  int httpCode = http.GET();
  http.end();
}

void resetar(){
  if (millis() > 7200000){
    ESP.restart();
  }
}

void loop() {
  if( travaSite = 1){
    site();
    travaSite = 0;
    resetar();
  }
}
