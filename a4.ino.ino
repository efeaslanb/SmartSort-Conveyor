#include <Stepper.h>
#include <Servo.h>


// Stepper motor setup
const int stepsPerRevolution = 2048;
const int rpm = 10;  // Slightly slower and smoother
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);  // Match your wiring


// Ultrasonic sensor pins
const int trigPin = 6;
const int echoPin = 5;


// Servo setup
Servo myServo;
const int servoPin = 3;


// Sensor-to-belt height
const float sensorToBeltHeight = 8.0;


void setup() {
  Serial.begin(9600);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  myStepper.setSpeed(rpm);
  myServo.attach(servoPin);
  myServo.write(0);  // Start at 0°
}


void loop() {
  // --- Stepper motor continuously rotates ---
  myStepper.step(stepsPerRevolution);  // 360° full turn


  // --- Ultrasonic sensor reading ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  long duration = pulseIn(echoPin, HIGH, 15000); // Timeout after 15ms
  float measuredDistance = duration * 0.034 / 2.0;
  float objectHeight = sensorToBeltHeight - measuredDistance;


  Serial.print("Distance: ");
  Serial.print(measuredDistance);
  Serial.print(" cm → Height: ");
  Serial.print(objectHeight);
  Serial.println(" cm");


  // --- Limit sensor reaction to within 0–7 cm range ---
  if (measuredDistance > 0 && measuredDistance <= 7.0) {
    if (objectHeight > 2.0) {
      Serial.println("Tall object detected → Action triggered");


      delay(2000); // Wait 2 seconds before stopping motor


      delay(2000); // Wait 2 seconds before moving servo
      myServo.write(90);   // Push
      delay(1000);         // Hold at 90° for 1 second
      myServo.write(0);    // Return to 0°
      delay(500);
    } else {
      Serial.println("Short object → Ignored");
      myServo.write(0);
    }
  } else {
    Serial.println("Out of range or no object");
    myServo.write(0);
  }


  delay(10);  // Sensor stability delay
}

