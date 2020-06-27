#include <DHT.h> // DHT11 temperature and humidity sensor Predefined library
#include <ESP8266WiFi.h> // Esp8266 library
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#define DHTPIN D3          // GPIO Pin where the dht11 is connected
DHT dht(DHTPIN, DHT11); // DHT 11


// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
BlynkTimer timer;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "blUltWbMo0L0FtYmt9t0WySLs94nVqKo";


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Loading...";
char pass[] = "hoilamgii";


WidgetLED led1(V2);                     //pump on: led on, pump off: led off
const int moisturePin = A0;             // moisteure sensor pin
const int motorPin = D5;               //mini pump pin
float moisturePercentage;              //moisture reading
float h;                  // humidity reading
float t;                  //temperature reading


void sendSensor()
{
  
  h = dht.readHumidity();     // read humiduty
  t = dht.readTemperature();  // read temperature


  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");   //Failed to read from DHT sensor
    return;
  }
  
  // Print the IP address on serial monitor
  Serial.println(analogRead(moisturePin));
  moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1024.00) * 100.00 ) );
  Serial.print("Soil Moisture is  = ");
  Serial.print(moisturePercentage);
  Serial.println("%");
  if (moisturePercentage < 55) {  
    digitalWrite(motorPin, LOW); //Motor will turn off if soil moisture is over 55%       
  }
  if (moisturePercentage > 56) { 
    digitalWrite(motorPin, HIGH); //Motor will turn on if soil moisture is lower than 55%       
  }
  delay(1000);
  
  Blynk.virtualWrite(V6, t);  //Virtual V6: temperature in Blynk app
  Blynk.virtualWrite(V5, h); //Virtual V5: humiduty in Blynk app
  Blynk.virtualWrite(V7, moisturePercentage);  //Virtual V7: moisturePercentage in Blynk app
}

void setup()
{
    // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass); //Begin WiFi

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW); // keep motor off initally
  dht.begin();
  timer.setInterval(2000, sendSensor);   // Setup a function to be called every second

}

void loop()
{
 Blynk.run(); // run Blynk
 timer.run(); // Initiates BlynkTimer
}
