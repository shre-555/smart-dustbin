#include <Servo.h>

#define ledPin 13       // LED for motion detection
#define ledPin2 12      // New LED for ultrasonic distance detection
#define sensorPin A0    // Moisture sensor pin
#define servoPin 9      // Servo motor pin
#define THRESHOLD 5     // Moisture level threshold
#define pirSensor 2     // PIR motion sensor pin
#define triggerPin 7    // Ultrasonic trigger pin
#define echoPin 6       // Ultrasonic echo pin

Servo myServo;
int state = LOW; // Motion detection state
int distanceThreshold = 7; // Distance threshold for new LED

void setup() {
    Serial.begin(9600);
    
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    
    pinMode(pirSensor, INPUT);
    myServo.attach(servoPin);
    
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    int motionDetected = digitalRead(pirSensor);
    int cm = measureDistance();

    // Motion detection
    if (motionDetected == HIGH) {
        digitalWrite(ledPin, HIGH);  // Turn on LED when motion is detected
        delay(100);

        if (state == LOW) {
            Serial.println("Motion detected!");
            state = HIGH;

            int moistureLevel = readSensor();
            Serial.print("Moisture Level: ");
            Serial.println(moistureLevel);

            if (moistureLevel > THRESHOLD) {
                rotateAndReturn();
            }
        }
    } else {
        digitalWrite(ledPin, LOW);
        delay(200);
        if (state == HIGH) {
            Serial.println("Motion stopped!");
            state = LOW;
        }
    }

    // Ultrasonic distance measurement
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");

    // Turn on ledPin2 if distance < 20 cm
    if (cm < distanceThreshold) {
        digitalWrite(ledPin2, HIGH);
    } else {
        digitalWrite(ledPin2, LOW);
    }

    delay(100);
}

// Read moisture sensor
int readSensor() {
    int sensorValue = analogRead(sensorPin);
    int outputValue = map(sensorValue, 0, 1023, 255, 0);
    analogWrite(ledPin, outputValue);
    return outputValue;
}

// Rotate the servo when conditions are met
void rotateAndReturn() {
    myServo.write(110);
    delay(700);
    myServo.write(90);
    delay(4000);
    myServo.write(70);
    delay(325);
    myServo.write(90);
}

// Measure distance using ultrasonic sensor
long measureDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    return 0.01723 * pulseIn(echoPin, HIGH);
}