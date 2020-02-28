#include <TinyGPS.h>
#include <SdsDustSensor.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <SoftwareSerial.h>

RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board 
SoftwareSerial gps(3, 2);

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format
const uint16_t channel = 90;

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

struct payload_t {                  // Structure of our payload
  uint16_t this_node;
  char pm10[8];
  char pm25[8];
  union {
    char lat_char[4];
    long lat_long;
  } lat;
  union {
    char lng_char[4];
    long lng_long;
  } lng;
};

SdsDustSensor sds(Serial);

TinyGPS tgps;

void setup(void)
{
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

  gps.begin(9600);

  sds.begin();
  sds.setQueryReportingMode();

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
  bool newdata = false;
  unsigned long start = millis();
  unsigned long age;

  payload_t payload = { this_node, "-1.0", "-1.0", NULL, NULL };

  digitalWrite(4, HIGH);       
  sds.wakeup();
  delay(5000);
  
  PmResult pm = sds.queryPm();
  if (pm.isOk()) {
    network.update();                          // Check the network regularly
    
    //Serial.print(F("Sending..."));
    
    dtostrf(pm.pm10, 4, 2, payload.pm10);
    dtostrf(pm.pm25, 4, 2, payload.pm25);
      
  } else {                    //Red LED will blink 5 times if Sensor cannot be activated.
    ledBlink(4, 6, 100);
  }

  sds.sleep();
  digitalWrite(4, LOW);
  delay(1000);
  digitalWrite(4, HIGH);
 
  // Every 5 seconds we print an update
  while (millis() - start < 5000) {
    if (gps.available() > 0) {
      char c = gps.read();
      // Serial.print(c);  // uncomment to see raw GPS data
      if (tgps.encode(c)) {
        newdata = true;
        // break;  // uncomment to print new data immediately!
      }
    }
  }
 
  if (newdata) {
    tgps.get_position(&payload.lat.lat_long, &payload.lng.lng_long, &age);
    digitalWrite(4, LOW);
  } else {
    ledBlink(4, 6, 100);
  }

  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);

  RF24NetworkHeader header(other_node);
  bool ok = network.write(header,&payload,sizeof(payload));
      
  if (ok)                   //Red LED will off if data is completely transferred.
    digitalWrite(5, LOW);
  else                      //Red LED will blink 3 times if data is not transferred.
    ledBlink(5, 4, 100);

  delay(55000);
}
