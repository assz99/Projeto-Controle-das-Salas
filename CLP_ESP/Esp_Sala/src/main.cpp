#include <Arduino.h>
#include <WiFi.h>
#include <SSD1306.h>
#include <SPI.h>
#include <Lora.h>

#define SSID "Sala-IFMT"
#define PASSWORD "lab@26"
#define BAND 916E6 //Frequencia da rede LoRa em 916 MHz

const int LORA_SCK_PIN = 5;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 27;
const int LORA_SS_PIN = 18;
const int LORA_RST_PIN = 14;
const int LORA_DI00_PIN = 26;


WiFiServer sv(8080); //Cria o objeto servidor na porta 555
WiFiClient cl;  

void setupDisplay()
{
  //O estado do GPIO16 é utilizado para controlar o display OLED
  pinMode(16, OUTPUT);
  //Reseta as configurações do display OLED
  digitalWrite(16, LOW);
  //Para o OLED permanecer ligado, o GPIO16 deve permanecer HIGH
  //Deve estar em HIGH antes de chamar o display.init() e fazer as demais configurações,
  //não inverta a ordem
  digitalWrite(16, HIGH);

  //Configurações do display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

bool loraBegin()
{
  // Iniciamos a comunicação SPI
  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
  // Setamos os pinos do lora
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI00_PIN);
  // Iniciamos o lora
  return LoRa.begin(BAND);
}



void mac(){ 
  ////////////////////////////////////////
  // Convertendo o MAC para MAC byte
  ////////////////////////////////////////
  String MAC_LOCAL = WiFi.macAddress();
  char ChMacLocal[18];
  String(MAC_LOCAL).toCharArray(ChMacLocal, 18);
  char *InfoMacLocal[6];
  InfoMacLocal[0] = strtok(ChMacLocal, ":");
  InfoMacLocal[1] = strtok(NULL, ":");
  InfoMacLocal[2] = strtok(NULL, ":");
  InfoMacLocal[3] = strtok(NULL, ":");
  InfoMacLocal[4] = strtok(NULL, ":");
  InfoMacLocal[5] = strtok(NULL, ":");
  String Mac_Local_Full;
  for (int s = 0; s < 6; s++)
  {
    Mac_Local_Full += InfoMacLocal[s];
  }
  localAddress = Mac_Local_Full;


}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // Se nao tiver um pacote, sai
  
  String incoming = "";                 // variavel para o pacote
  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }

  char chIncoming[30];
  String(incoming).toCharArray(chIncoming, 30);
  char *infoIncoming[3];
  infoIncoming[0] = strtok(chIncoming, "!");
  infoIncoming[1] = strtok(NULL, "!");
  infoIncoming[2] = strtok(NULL, "!");
  String macMes = infoIncoming[0];
  String comando = infoIncoming[1];
  int valorComando = infoIncoming[2];
 


  if (macMes != localAddress && macMes != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}


void setup() {
  setupDisplay();
  delay(3000); //Aguarda 3 segundos
  SCT013.current(pinSCT, 5.4);
  Serial.begin(115200);

  LoRa.setPins(18, 14, 26);

  if (!loraBegin())
  { // tenta inicializar a Rede LoRa
    Serial.println("LoRa nao conseguiu inicializar..");
    while (true)
      ;
  }

  WiFi.mode(WIFI_AP); //Modo Access Point
  WiFi.softAP(SSID, PASSWORD);

  sv.begin(); //Inicia o servidor TCP na porta declarada no começo.

  mac();

  display.clear();
  display.drawString(0, 0, "" + String(MAC_LOCAL));
  display.drawString(0, 16, "Novo: " + String(Mac_Local_Full));
  display.drawString(0, 32, "[ESP - AR COND]");
  display.display();
  LoRa.onReceive(onReceive);
  LoRa.receive();
 
}

void loop() {

}