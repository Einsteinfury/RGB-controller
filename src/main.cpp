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

    void init(){
      pinMode(this->pin, OUTPUT);
    }

    LED(char name, int pin){
      this->name = name;
      this->pin = pin;
    }
    
};

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
      }
      else {
        this->ord = 0;
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

void checkSerial(LED *led){
  if (Serial.available() > 0){
    String data = Serial.readString();

    if (data.charAt(0) == 's'){
      Serial.print(led->getIntensity());
    }
  }
}

void checkRGB(Control *obj, LED *led){

  checkSerial(led);

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

void transition(LED *led, int val){
  Control Cled(led->getIntensity(), val);

  while (Cled.going){
    checkRGB(&Cled, led);

    delay(LED::delayTime);
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

  changeColor(4, 0 , 7);
}

void loop() {
  

  if (Serial.available() > 0) {
    
    String incoming = Serial.readString();

    //adress character
    char adress = incoming.charAt(0);

    //reads the value of serial messege
    int value = incoming.substring(1, sizeof(incoming)).toInt();
    
    if (adress == '5'){
      changeColor(0, 0, value);
    }
    if (adress == '4'){
      changeColor(value, 0, 0);
    }

    if (adress == '3'){
      changeColor(0, value, 0);
    }

    if (adress == '6'){
      changeColor(random(0, LED::bright), random(0, LED::bright), random(0, LED::bright));
      String mes = (String)Red.getIntensity() + "#" + (String)Green.getIntensity() + "#" + (String)Blue.getIntensity() + "#";
      String sen = (String)mes.length() +  mes;
      Serial.print(sen);
    }

    if (adress == '7'){
      transition(&Red, value);
    }

    if (adress == '8'){
      transition(&Green, value);
    }

    if (adress == '9'){
      transition(&Blue, value);
    }

    if (adress == 'b'){
      LED::bright = value;
    }

    if (adress == 'd'){
      LED::delayTime = value;
    }
    
  }
}