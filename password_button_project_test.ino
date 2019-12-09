#include <Servo.h>  //servo header file
#include <Wire.h>  //ultrasonic sensor header file
#include <LiquidCrystal_I2C.h>  //LCD_I2C header file
Servo servo; 
LiquidCrystal_I2C lcd(0x3F, 16, 2); 

#define but1 12
#define but2 11
#define but3 10
#define led 8
#define trig 7
#define echo 6
int deg = 0;  //angle parameter, use in open_ and close_

void door();  //door open, close
void detect_object();  //detect object when door closed
void door_sudo();  //open door if someone press but3
void light_on();  //light on
void light_off();  //light off
void open_();  //door open
void close_();  //door close
int distance();  //measure distance

int pass1 = 0, pass2 = 0;  //use in password algorithm
int state = 0;  //show up door state

void setup() {
  lcd.begin();  //start LCD
  lcd.backlight();  //turn backlight on
  servo.attach(9);  //use 9pin for servo motor
  pinMode(but1, INPUT);
  pinMode(but2, INPUT);
  pinMode(but3, INPUT);
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);  //firing ultrasonic from trig
  pinMode(echo, INPUT);  //reflected ultrasonic input in echo
  servo.write(0);
}

void loop() {
  int dis = distance();
  detect_object();
  if(dis > 10){
    detect_object();
    state = 0;
  }
  if(digitalRead(but3) == HIGH){
    lcd.clear();
    door_sudo();
    state = 0;
  }
start:  //simple button password algorithm
  if(pass1 == 0 && digitalRead(but1) == HIGH){
    lcd.clear();
    lcd.print("Correct!");
    pass1++;
    delay(500);
    goto start;
  }
  else if(pass1 == 1 && pass2 != 2 &&digitalRead(but2) == HIGH){
    lcd.clear();
    lcd.print("Correct!");
    pass2++;
    delay(500);
    goto start;
  }
  else if(pass1 == 1 && pass2 != 2 && digitalRead(but1) == HIGH){
    lcd.clear();
    lcd.print("Wrong password!");
    pass2 = 0;
    pass1 = 0;
    delay(500);
    goto start;
  }
  else if(pass1 != 1 && digitalRead(but2) == HIGH){
    lcd.clear();
    lcd.print("Wrong password!");
    delay(500);
    goto start;
  }
  else if(pass1 == 1 && pass2 == 1 && digitalRead(but2) == HIGH){
    lcd.clear();
    lcd.print("Correct!");
    pass2++;
    delay(500);
    goto start;
  }
  else if(pass1 == 1 && pass2 == 2 && digitalRead(but2) == HIGH){
    lcd.clear();
    lcd.print("Wrong password!");
    pass1 = 0;
    pass2 = 0;
    delay(500);
    goto start;
  }
  else if(pass1 == 1 && pass2 == 2 && digitalRead(but1) == HIGH){
    door();
    pass1 = 0;
    pass2 = 0;
  }
}

void detect_object(){
  if(deg == 0 ){
    while(state == 0){
      lcd.clear();
      lcd.print("Locked");
      int dis1 = distance();
      delay(1000);
      int dis2 = distance();
      if(digitalRead(but3) == HIGH){
        lcd.clear();
        door_sudo();
        state = 0;
      }
      if(10 > dis1 && 10 > dis2){
        light_on();
        lcd.clear();
        lcd.print("Hello!");
        delay(2000);
        lcd.clear();
        lcd.print("Input password!");
        state++;
        break;
      }
      else if(10 < dis1 || 10 < dis2){
        light_off();
        lcd.clear();
      }
    }
  }
}

void light_off(){
  digitalWrite(led,LOW);
}

void light_on(){
  digitalWrite(led,HIGH);
}

void door(){
  lcd.clear();
  lcd.print("Welcome owner!!");
  open_();
  delay(5000);
  close_();
  lcd.clear();
  light_off();
}

void door_sudo(){
  open_();
  light_on(); 
  lcd.print("Good bye owner!");
  delay(5000);
  close_();
  light_off();
  lcd.clear();
  state = 0;
}

int distance(){
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  int duration = pulseIn(echo, HIGH);
  int dist = duration / (29 * 2);  //ultrasonic moves 1cm at 29us speed *2 (forth and back)
  return dist;
}

void open_(){
  deg = 90;
  for(int i = 0; i < deg; i++){
    servo.write(i);
    delay(30);
  }
}

void close_(){
  deg = 0;
  for(int i = 90; i > deg; i--){
    servo.write(i);
    delay(30);
  }
}
