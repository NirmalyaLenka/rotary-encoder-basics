#define CLK_PIN 25                                  // Clock pin
#define DT_PIN  26                                  // Data pin
#define SW_PIN  27                                  // Button pin

int counter = 0;                                    // Encoder count
int lastStateCLK;                                   // Previous CLK state
int currentStateCLK;                                // Current CLK state
int swState = 0;                                    // Button state
int lastSwState = 0;                                // Last button state

void setup() {
  Serial.begin(115200);                             // Start serial
  pinMode(CLK_PIN, INPUT);                          // CLK as input
  pinMode(DT_PIN, INPUT);                           // DT as input
  pinMode(SW_PIN, INPUT_PULLUP);                    // Button pullup
  lastStateCLK = digitalRead(CLK_PIN);              // Read initial state
}

void loop() {
  currentStateCLK = digitalRead(CLK_PIN);           // Read CLK

  if (currentStateCLK != lastStateCLK) {            // State changed
    if (digitalRead(DT_PIN) != currentStateCLK) {   // Check direction
      counter++;                                    // Count clockwise
      Serial.println("Direction: CLOCKWISE");       // Print direction
    } else {
      counter--;                                    // Count counter-CW
      Serial.println("Direction: COUNTER-CLOCKWISE"); // Print direction
    }
    Serial.print("Counter: ");                      // Print label
    Serial.println(counter);                        // Print count
  }

  lastStateCLK = currentStateCLK;                   // Save CLK state

  swState = digitalRead(SW_PIN);                    // Read button

  if (swState == LOW && lastSwState == HIGH) {      // Button pressed
    counter = 0;                                    // Reset counter
    Serial.println("Button Pressed! Counter Reset."); // Print reset
    delay(300);                                     // Debounce delay
  }

  lastSwState = swState;                            // Save button state
  delay(5);                                         // Stability delay
}
