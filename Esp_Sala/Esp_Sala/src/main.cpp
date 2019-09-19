#include <LoRa.h>

#include <Arduino.h>
#include <WiFi.h>
#include <SSD1306.h>
#include <SPI.h>


#define SSID "Sala-IFMT"
#define PASSWORD "lab@26"
#define BAND 916E6 //Frequencia da rede LoRa em 916 MHz

const int LORA_SCK_PIN = 5;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 27;
const int LORA_SS_PIN = 18;
const int LORA_RST_PIN = 14;
const int LORA_DI00_PIN = 26;
String localAddress; // endereço deste dipositivo
String endBroadcast = "0xE117" ; // endereço de broadcast

String Mac_Local_Full, MAC_LOCAL;

//variaveis que indicam o núcleo
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne = 1;

WiFiServer sv(8080); //Cria o objeto servidor na porta 555
WiFiClient cl;  

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
  MAC_LOCAL = WiFi.macAddress();
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

void core(){

delay(500); //tempo para a tarefa iniciar

  //cria uma tarefa que será executada na função coreTaskOne, com prioridade 2 e execução no núcleo 1
  //coreTaskOne: atualizar as informações do display
  // xTaskCreatePinnedToCore(
  //  Proto_Socket,   /* função que implementa a tarefa */
  //  "Proto_Socket", /* nome da tarefa */
  //   10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
  //   NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
  // 1,          /* prioridade da tarefa (0 a N) */
  // NULL,       /* referência para a tarefa (pode ser NULL) */
  // taskCoreOne);         /* Núcleo que executará a tarefa */

  delay(500); //tempo para a tarefa iniciar

  //cria uma tarefa que será executada na função coreTaskTwo, com prioridade 2 e execução no núcleo 0
  //coreTaskTwo: vigiar o botão para detectar quando pressioná-lo
  //xTaskCreatePinnedToCore(
    //Comunicacao_Server,   /* função que implementa a tarefa */
    //"Comunicacao_Server", /* nome da tarefa */
   // 10000,                /* número de palavras a serem alocadas para uso com a pilha da tarefa */
   // NULL,                 /* parâmetro de entrada para a tarefa (pode ser NULL) */
   // 1,                    /* prioridade da tarefa (0 a N) */
   // NULL,                 /* referência para a tarefa (pode ser NULL) */
   // taskCoreZero);*/        /* Núcleo que executará a tarefa */
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
  
  int valorComando = atoi(infoIncoming[2]);
 
  Serial.println("Mensagem: "+incoming);
  Serial.println();

  if (macMes != localAddress && macMes != endBroadcast) {
    Serial.println("Esta mensagem nao e pra mim.");
    return;                             // skip rest of function
  }

  core();
  
}


void setup() {
  setupDisplay();
  delay(3000); //Aguarda 3 segundos
  //SCT013.current(pinSCT, 5.4);
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
  display.drawString(0, 32, "[ ESP - AR COND]");
  display.display();
  LoRa.onReceive(onReceive);
  LoRa.receive();
 
}

void enviar_Mensagem(String mensagem){
  LoRa.beginPacket();            // Inicia o pacote da mensagem
  LoRa.write(mensagem.length()); // Tamanho da mensagem em bytes
  LoRa.print(mensagem);          // Vetor da mensagem
  LoRa.endPacket();              // Finaliza o pacote e envia
  Serial.println(mensagem);
}



void Proto_Socket() // funcao para receber as informaçoes do celular
{
  while (true)
  {

    //Note the approach used to automatically calculate the size of the array.
    if (cl.connected()) //Detecta se há clientes conectados no servidor.
    {
      if (cl.available() > 0) //Verifica se o cliente conectado tem dados para serem lidos.
      {
        String req = "";
        while (cl.available() > 0) //Armazena cada Byte (letra/char) na String para formar a mensagem recebida.
        {
          char z = cl.read();
          req += z;
        }
        
        //Envia uma resposta para o cliente
        cl.print("\nO servidor recebeu sua mensagem");
        cl.print("\n...Seu IP: ");
        cl.print(cl.remoteIP());
        cl.print("\n...IP do Servidor: ");
        cl.print(WiFi.softAPIP());
        cl.print("\n...Sua mensagem: " + req + "\n");

        enviar_Mensagem(req);

      }
    }
    else //Se nao houver cliente conectado,
    {
      cl = sv.available(); //Disponabiliza o servidor para o cliente se conectar.
      delay(1);
    }
    //req
  }
}

void loop() {

}