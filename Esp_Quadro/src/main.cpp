#include <SPI.h>
#include <LoRa.h>
#include <QueueList.h>


void setup() {
  Serial.begin(9600);
  QueueList<int> c = QueueList<int>();
  c.push(1);
  c.push(2);
  Serial.printf("%d",c.pop());
}

void loop() {

}