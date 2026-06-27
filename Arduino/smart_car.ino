#define IN1 13 
#define IN2 12 
#define IN3 11 
#define IN4 7 
#define ENA 5 
#define ENB 6 
 
#define TRIG 4 
#define ECHO 3 
 
char command = 'S'; 
unsigned long lastDistanceTime = 0; 
long distance = 100; 
 
long getDistance() { 
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TRIG, LOW); 
 
  long duration = pulseIn(ECHO, HIGH, 20000); 
  if (duration == 0) return 999; 
  long dist = duration * 0.034 / 2; 
  return dist; 
} 
 
void setMotorSpeed(int speed) { 
  analogWrite(ENA, speed); 
  analogWrite(ENB, speed); 
} 
 
void moveForward(int speed) { 
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW); 
  setMotorSpeed(speed); 
} 
 
void moveBackward() { 
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH); 
  setMotorSpeed(200); 
} 
 
void turnLeft() { 
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW); 
  setMotorSpeed(180); 
} 
 
void turnRight() { 
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH); 
  setMotorSpeed(180); 
} 
 
void stopCar() { 
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW); 
  setMotorSpeed(0); 
} 
 
void setup() { 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT); 
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT); 
  Serial.begin(9600); 
  stopCar(); 
} 
 
void loop() { 
  unsigned long currentTime = millis(); 
  if (currentTime - lastDistanceTime >= 60) { 
    distance = getDistance(); 
    lastDistanceTime = currentTime; 
  } 
 
  if (Serial.available() > 0) { 
    command = Serial.read(); 
  } 
 
  switch (command) { 
    case 'F': { 
      if (distance > 40 && distance < 200) { 
        int speed = map(distance, 40, 200, 0, 255); 
        speed = constrain(speed, 0, 255); 
        moveForward(speed); 
      }  
      else if (distance <= 40) { 
        stopCar(); 
        Serial.println("⚠️ Obstacle detected! Car stopped."); 
      }  
      else { 
        moveForward(255); 
      } 
      break; 
    } 
 
    case 'B': 
      moveBackward(); 
      break; 
 
    case 'L': 
      turnLeft(); 
      break; 
 
    case 'R': 
      turnRight(); 
      break; 
 
    case 'S': 
      stopCar(); 
      break; 
  } 
} 
