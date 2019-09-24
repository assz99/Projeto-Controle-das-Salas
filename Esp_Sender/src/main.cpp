#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

//#include <SDD1306.h>

#define BAND 921E6

const char* ssid = "LaboratorioE117";
const char* password = "LAB@E117";

int BROKER_PORT = 1883;
String IP = "172.24.1.2";
char ip_conv[14];
const char* BROKER_MQTT;

String mensagem;
String localAddres;
char info_topico[25];



void setup() {


}
void loop() {}