#include <Arduino.h>

//RGB LED pins
int Rlight = 5;
int Glight = 6;
int Blight = 3;

class LED {
  protected:
    char name;
    int pin;
    int intensity;

  public:
    static int delayTime;
    static int bright;
    
    int getPin(){
      return this->pin;
    }

    char getName(){
      return this->name;
    }

    int getIntensity(){
      return this->intensity;
    }

    void setIntensity(int in){
      this->intensity = in;
    }

    void set(int in){
      analogWrite(this->pin, in);
      this->intensity = in;
    }

    void transition(int value){
      if (this->intensity >= value){
        for (int i = this->intensity; i >= value; i--){
          if (i == value){
            this->intensity = i;
          }

          if (Serial.available() > 0){
            String data = Serial.readString();

            if (data.charAt(0) == 's'){
              this->intensity = i;
              Serial.print(i);
              break;
            }
          }
          
          analogWrite(this->pin, i);
          delay(delayTime);
        }
      }

      else {
        for (int i = this->intensity; i <= value; i++){
          if (i == value){
            this->intensity = i;
          }

          if (Serial.available() > 0){
            String data = Serial.readString();

            if (data.charAt(0) == 's'){
              this->intensity = i;
              Serial.print(i);
              break;
            }
          }
          
          analogWrite(this->pin, i);
          delay(delayTime);
        }
      }
    }

    void init(){
      pinMode(this->pin, OUTPUT);
    }

    LED(char name, int pin){
      this->name = name;
      this->pin = pin;
    }
    
};

int steps = 0;

class Control {
  protected:
    int ord;
    int val;
    int target;

  public:
    bool going = true;
    Control(int value, int target){
      if (target <= value){
        this->ord = 1;
        steps += value - target;
      }
      else {
        this->ord = 0;
        steps += target - value;
      }
      this->val = value;
      this->target = target;
    }

    //~Control();
    int getTarget() {
      return this->target;
    }
    int getOrd(){
      return this->ord;
    }
    int getVal(){
      return this->val;
    }

    void increment(){
      this->val++;
    }

    void decriment(){
      this->val--;
    }

};

//default 
int LED::delayTime = 50;
int LED::bright = 10;

LED Red('R', Rlight);
LED Green ('G', Glight);
LED Blue ('B', Blight);

void checkRGB(Control *obj, LED *led){

  if (obj->getOrd() == 1){
    if (obj->getVal() == obj->getTarget()){
      led->set(obj->getVal());
      obj->going = false;
    }
    else {
      led->set(obj->getVal());
      obj->decriment();
    }
  }

  else if (obj->getOrd() == 0){
    if (obj->getVal() == obj->getTarget()){
      led->set(obj->getVal());
      obj->going = false;
    }
    else {
      led->set(obj->getVal());
      obj->increment();
    }
  }
}

void changeColor(int red, int green, int blue){
  Control CRed(Red.getIntensity(), red);
  Control CGreen(Green.getIntensity(), green);
  Control CBlue(Blue.getIntensity(), blue);

  while (CRed.going || CGreen.going || CBlue.going){

    checkRGB(&CRed, &Red);
    checkRGB(&CGreen, &Green);
    checkRGB(&CBlue, &Blue);

    delay(LED::delayTime);
  }
}

void setup() {
  
  Serial.begin(9600);

  //turns off the built in light
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //sets pinMode of LED's to OUTPUT
  Red.init();
  Green.init();
  Blue.init();

  //sets default light
  Red.set(0);
  Green.set(0);
  Blue.set(0);
}

void loop() {
  

  if (Serial.available() > 0) {
    
    String incoming = Serial.readString();

    //adress character
    char adress = incoming.charAt(0);

    //reads the value of serial messege
    int value = incoming.substring(1, sizeof(incoming)).toInt();
    
    if (adress == '5'){
      Blue.transition(value);
      Red.transition(0);
      Green.transition(0);
    }
    if (adress == '4'){
      Blue.transition(0);
      Red.transition(value);
      Green.transition(0);
    }

    if (adress == '3'){
      Blue.transition(0);
      Red.transition(0);
      Green.transition(value);
    }

    if (adress == '6'){
      Blue.transition(random(0, LED::bright));
      Red.transition(random(0, LED::bright));
      Green.transition(random(0, LED::bright));
      Serial.print(Red.getIntensity());
      delay(100);
      Serial.print(Green.getIntensity());
      delay(100);
      Serial.print(Blue.getIntensity());
    }

    if (adress == '7'){
      Red.transition(value);
    }

    if (adress == '8'){
      Green.transition(value);
    }

    if (adress == '9'){
      Blue.transition(value);
    }

    if (adress == 'b'){
      LED::bright = value;
    }

    if (adress == 'd'){
      LED::delayTime = value;
    }
    
  }
}