/*
 * Arduino Shocker
 *
 * Application that emulates the a dog shock collar remote
 * control on the Arduino.
 * 
 * This is for use in a Flashing Light Prize 2018 entry and
 * not intended for use on poor defencelss and fluuffy animals.
 * 
 * Alistair MacDonald 2018
 * http://alistairmacdonald.com
 *
 */


#define TRANSMITTERPIN   4      // Pin connected to the 433MH transmitter
#define TGNDPIN          2      // Pin used for ground on the transmitter
#define TVCCPIN          3      // Pin used for power to the transmitter
#define DETECTORPIN      A2     // Pin connected to the light detector
#define DGNDPIN          A1     // Pin used for ground on the detector
#define DVCCPIN          A0     // Pin used for power to the detector

#define DURATIONSHORT    340   // Short pulse length
#define DURATIONSYNCLONG 1130  // Lond sync pusle length


// Send a virtual low
void sendLow () {
  digitalWrite(TRANSMITTERPIN, HIGH);
  delayMicroseconds(DURATIONSHORT);
  digitalWrite(TRANSMITTERPIN, LOW);
  delayMicroseconds(DURATIONSHORT);
  delayMicroseconds(DURATIONSHORT);
  delayMicroseconds(DURATIONSHORT);
}

// Send a virtual high
void sendHigh () {
  digitalWrite(TRANSMITTERPIN, HIGH);
  delayMicroseconds(DURATIONSHORT);
  delayMicroseconds(DURATIONSHORT);
  delayMicroseconds(DURATIONSHORT);
  digitalWrite(TRANSMITTERPIN, LOW);
  delayMicroseconds(DURATIONSHORT);
}

// Send a sync pulse
void sendSync () {
  digitalWrite(TRANSMITTERPIN, LOW);
  delayMicroseconds(DURATIONSYNCLONG);
}

// Send the command
void sendPacket ( int inMode ) {
  // Let's get some contrast for the reciever
  sendLow();
  delay(10);
  // First Byte (I am guessing a framing header)
  sendHigh();
  sendLow();
  sendLow();
  sendLow();
  sendLow();
  sendLow();
  sendLow();
  sendHigh();
  // Second Byte (I am guessing a ID for the coller)
  sendLow();
  sendHigh();
  sendLow();
  sendHigh();
  sendLow();
  sendLow();
  sendHigh();
  sendLow();
  // Third Byte (The function requested)
  sendLow();
  sendLow();
  sendLow();
  sendLow();
  if (inMode==4) { sendHigh(); } else { sendLow(); } // Electric Shock
  if (inMode==3) { sendHigh(); } else { sendLow(); } // Vibrate
  if (inMode==2) { sendHigh(); } else { sendLow(); } // Sounder
  if (inMode==1) { sendHigh(); } else { sendLow(); } // Flash Light
  // End of command sync
  sendSync();
}

// Send the sommand multiple times
void sendPackets( int inMode, int inCount ) {
  int i;
  for ( i=0; i < inCount; i++) {
    sendPacket(inMode);
  }
}

// Setup the hardware
void setup() {
  // Configure the pin for output
  pinMode(TRANSMITTERPIN, OUTPUT);
  digitalWrite(TRANSMITTERPIN, LOW);
  // Configure the pin for input
  pinMode(DETECTORPIN, INPUT);
  // Set the pins used for power
  pinMode(DGNDPIN, OUTPUT);
  digitalWrite(DGNDPIN, LOW);
  pinMode(TGNDPIN, OUTPUT);
  digitalWrite(TGNDPIN, LOW);
  pinMode(DVCCPIN, OUTPUT);
  digitalWrite(DVCCPIN, HIGH);
  pinMode(TVCCPIN, OUTPUT);
  digitalWrite(TVCCPIN, HIGH);
}

// The main application
void loop() {

  // Wait for the light to turn off (start of second)
  while (digitalRead(DETECTORPIN)==LOW); // Low is on

  // Send the command
  sendPackets( 4, 2 ); // Trial and error shows two iterations results in output for 500ms.

  // Ignore the rest of the frame data
  delay(300);
  
  // Wait for the light to turn on
  while (digitalRead(DETECTORPIN)==HIGH); // High is off

  // Debounce
  delay(40);

}

