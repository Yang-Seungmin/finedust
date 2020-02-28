#include <SdsDustSensor.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

struct payload_t {                  // Structure of our payload
  uint16_t this_node;
  char pm10[8];
  char pm25[8];
};

int rxPin = 2;
int txPin = 3;
SdsDustSensor sds(rxPin, txPin);

void setup(void)
{
  
  Serial.begin(9600);
  Serial.println("RF24Network/examples/helloworld_tx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  sds.begin();
  sds.setCustomWorkingPeriod(1);
}

void loop() {
 
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    network.update();                          // Check the network regularly

    Serial.print("Sending...");
    payload_t payload = { this_node, "0.0", "0.0" };
    dtostrf(pm.pm10, 4, 2, payload.pm10);
    dtostrf(pm.pm25, 4, 2, payload.pm25);
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  
  } else {
  }

  delay(60000);
}
