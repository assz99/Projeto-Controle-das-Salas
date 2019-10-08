#include <SPI.h>
#include <LoRa.h>
#define BAND 921E6 //Frequencia da rede LoRa em 921 MHz

const int LORA_SCK_PIN = 5;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 27;
const int LORA_SS_PIN = 18;
const int LORA_RST_PIN = 14;
const int LORA_DI00_PIN = 26;
String localAddress; // endereço deste dipositivo
String endBroadcast = "0xE117" ; // endereço de broadcast
String Mac_Local_Full ; //mac criado para padrao do banco
String MAC_LOCAL = "71:75:61:64:72:6f";
//Variável para controlar o display
const int DISPLAY_ADDRESS_PIN = 0x3c;
const int DISPLAY_SDA_PIN = 4;
const int DISPLAY_SCL_PIN = 15;
const int DISPLAY_RST_PIN = 16;
SSD1306 display(DISPLAY_ADDRESS_PIN, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

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
  char ChMacLocal[18];
  String(MAC_LOCAL).toCharArray(ChMacLocal, 18);
  char *InfoMacLocal[6];
  InfoMacLocal[0] = strtok(ChMacLocal, ":");
  InfoMacLocal[1] = strtok(NULL, ":");
  InfoMacLocal[2] = strtok(NULL, ":");
  InfoMacLocal[3] = strtok(NULL, ":");
  InfoMacLocal[4] = strtok(NULL, ":");
  InfoMacLocal[5] = strtok(NULL, ":");
  
  for (int s = 0; s < 6; s++)
  {
    Mac_Local_Full += InfoMacLocal[s];
  }
  localAddress = Mac_Local_Full;
 

}

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
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
    while (true);
  }  
}

void comando_quadro(String macUS,int comando){  
    






}



void onReceive(int packetSize) {
  if (packetSize == 0) return;          // Se nao tiver um pacote, sai
  byte incomingLength = LoRa.read();
  String incoming = "";                 // variavel para o pacote
  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }
  if (incomingLength != incoming.length())
  {
    // checa se chegou toda a mensagem
    Serial.println("erro!: o tamanho da mensagem nao condiz com o conteudo!");
    return;
  }


  char chIncoming[50];
  String(incoming).toCharArray(chIncoming, 50);
  char *infoIncoming[3];
  infoIncoming[0] = strtok(chIncoming, "!");
  infoIncoming[1] = strtok(NULL, "!");
  infoIncoming[2] = strtok(NULL, "!");
  String macMes = String(infoIncoming[0]);
  String sala = String(infoIncoming[1]); 
  int valorComando = String(infoIncoming[2]).toInt();
 
  Serial.println("Mensagem: "+incoming);
  Serial.println();

  if (macMes != localAddress && macMes != endBroadcast) {
    Serial.println("Esta mensagem nao e pra mim.");
    return;                             // skip rest of function
  }
  comando_quadro(macMes,valorComando);
} 

void enviar_Mensagem(String mensagem){
  LoRa_txMode();
  LoRa.beginPacket();            // Inicia o pacote da mensagem
  LoRa.write(mensagem.length()); // Tamanho da mensagem em bytes
  LoRa.print(mensagem);          // Vetor da mensagem
  LoRa.endPacket();              // Finaliza o pacote e envia
  Serial.println(mensagem);
  LoRa_rxMode();
}

void loop() {

}