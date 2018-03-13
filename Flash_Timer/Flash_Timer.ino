/*
 * Flash Timer
 *
 * A quick hack to time how long a light is of and/or off
 * 
 * This is for use for debugging a Flashing Light Prize 2018 entry
 * 
 * Alistair MacDonald 2018
 * http://alistairmacdonald.com
 *
 */

#define RXPIN    10     // Pin connected to the detector
#define GNDPIN   9      // Pin used for ground on the detector
#define VCCPIN   8      // Pin used for power to the detector


void setup() {
  // Enable serial for debugging
  Serial.begin(115200);
  // Configure the pin for input
  pinMode(RXPIN, INPUT);
  // Set the pins used for power
  pinMode(GNDPIN, OUTPUT);
  digitalWrite(GNDPIN, LOW);
  pinMode(VCCPIN, OUTPUT);
  digitalWrite(VCCPIN, HIGH);
}

unsigned long start = millis();
unsigned long end = start;

void loop() {
  while (digitalRead(RXPIN)==HIGH); // Wait for the light to turn on (high is off)
  start = millis(); // Rememebr when light turned on

  Serial.print("Off :");
  Serial.println(start-end);

  delay(40); // Wait a debounce delay
  
  while (digitalRead(RXPIN)==LOW); // Wait for the light to turn off (low is on)
  end = millis(); // Rememebr when we finished sending

  Serial.print("On  :");
  Serial.println(end-start);

  delay(40); // Wait a debounce delay

}
