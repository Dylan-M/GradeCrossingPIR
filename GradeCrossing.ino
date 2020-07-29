/* Grade Crossing Display Controller
 * ------------------ 
 *
 * Control circuit to flash a simulated railroad crossing
 * Potentiometer used to control flash speed
 * Activation controlled via a sensor such as a PIR
 * Designed for UNO & Leonardo boards, will require changes for other boards
 *
 */

#define potPin     A2               // select the input pin for the potentiometer
#define usePotPin   0               // If pin is grounded, use potentiometer for interval
#define ledPinA     3               // select the pin for the 1st LED
#define ledPinB     4               // select the pin for the 2nd LED
#define SensorPinA  5               // Input pin for the first Passive Infrared Receiver
#define SensorPinB  6               // Input pin for the second PIR
const unsigned long interval = 800; // Default interval 800ms if not using a pot to control interval timing
unsigned long potVal = interval;    // variable to store the value coming from the sensor
bool even = true;                   // Is the LED flash on the even stroke?
unsigned long previousMillis = 0;   // will store last time LED was updated
char buf[175];                      // Buffer for text output with sprintf()
bool activated = false;             // Has the sensor been activated
bool wasActivated = false;          // Was the sensor previously activated

void setup() {
  pinMode(ledPinA, OUTPUT);         // declare the ledPinA as an OUTPUT
  pinMode(ledPinB, OUTPUT);         // declare the ledPinB as an OUTPUT
  pinMode(SensorPinA, INPUT);       // declare the first sensor as an input
  pinMode(SensorPinB, INPUT);       // declare the second sensor as an input
  pinMode(potPin, INPUT);           // declare the pin for a pot as input
  pinMode(usePotPin, INPUT_PULLUP); // declare the pin for using a pot as input_pullup
  digitalWrite(ledPinA, HIGH);      // start with all LEDs turned off
  digitalWrite(ledPinB, HIGH);      // start with all LEDs turned off
  Serial.begin(9600);               // start a serial connection on the USB port for debugging
  Serial.println("Initializing");
}

/*
 * Main loop with decision making
 */
void loop() {
  // If we're using a pot, read the potVal in
  if (digitalRead(usePotPin) == LOW) {
    potVal = analogRead(potPin);
  }
  // Was a sensor active on this pass?
  activated = (digitalRead(SensorPinA) == HIGH || digitalRead(SensorPinB) == HIGH);

  // If a sensor was activated, start running the lights
  if (activated) {
    unsigned long milliseconds = millis(); // Get our current milliseconds
    if ((milliseconds  - previousMillis) >= potVal) {
      sprintf(buf, "Switching LEDs at %lu millis, previous millis of %lu with a interval of %lu\r\n", milliseconds, previousMillis, potVal);
      Serial.print(buf);
      switchLED();
      previousMillis = milliseconds;
      wasActivated = true;
    }
  // If we were previously activated, but aren't now, shut everything down
  } else if (wasActivated) {
    Serial.print("Turning LEDs Off\r\n");
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, HIGH);
    wasActivated = false;
  }
}

/*
 * switchLED()
 * This function will alternate the flashin of the LEDs
 * It is called from loop() when the interval in milliseconds has passed
 * Also flashes onboard LED for debugging
 */
void switchLED() {
  if (even) {
    digitalWrite(ledPinA, HIGH);  // turn the ledPin off
    digitalWrite(ledPinB, LOW);   // turn the ledPin on
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(ledPinA, LOW);   // turn the ledPin on
    digitalWrite(ledPinB, HIGH);  // turn the ledPin off
    digitalWrite(13, LOW);
  }

  even = !even; // swap even
}