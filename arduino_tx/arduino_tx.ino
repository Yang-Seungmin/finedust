#include <TinyGPS.h>
#include <SdsDustSensor.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <SoftwareSerial.h>

RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);          // Network uses that radio

SdsDustSensor sds(Serial);
SoftwareSerial gps(3, 2);

TinyGPS tgps;

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format
const uint16_t channel = 90;

struct payload_t {                  // Structure of our payload
  uint16_t this_node;
  float pm10;
  float pm25;
  float lat;
  float lng;
};

void setup() {
  //Serial.begin(9600);
  //Serial.println(freeRam());
  //Serial.println(F("RF24Network/examples/helloworld_tx/"));

  pinMode(4, OUTPUT); //SDS Status red LED
  pinMode(5, OUTPUT); //GPS Status green LED

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ channel, /*node address*/ this_node);

  sds.begin();
  sds.setQueryReportingMode();
  
  gps.begin(9600);

  delay(1000);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void ledBlink(int pin, int count, int period) {
  int i;
  for(i = 0; i < count; i++) {
    digitalWrite(pin, HIGH);
    delay(period);
    digitalWrite(pin, LOW);
    delay(period);
  }
}

void loop() {
  bool newData = false;
  unsigned long age;

  payload_t payload = {this_node, -1.0, -1.0, -1000, -1000};

  digitalWrite(4, HIGH);       
  sds.wakeup();
  delay(5000);
  
  PmResult pm = sds.queryPm();
  if (pm.isOk()) {
    payload.pm10 = pm.pm10;
    payload.pm25 = pm.pm25;
  } else {                    //Red LED will blink 5 times if Sensor cannot be activated.
    ledBlink(4, 6, 100);
  }

  sds.sleep();
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);

  gps.listen();

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 5000;)
  {
    while (gps.available())
    {
      char c = gps.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (tgps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData) {
    unsigned long age;
    tgps.f_get_position(&payload.lat, &payload.lng, &age);
  } else {
    ledBlink(5, 6, 100);
  }
  digitalWrite(5, LOW);
  
  delay(1000);
  digitalWrite(5, HIGH);

  network.update();                          // Check the network regularly
  RF24NetworkHeader header(other_node);
  bool ok = network.write(header,&payload,sizeof(payload));
      
  if (ok)                   //Red LED will off if data is completely transferred.
    digitalWrite(5, LOW);
  else                      //Red LED will blink 3 times if data is not transferred.
    ledBlink(5, 3, 100);

  delay(5000);
}
