#include <ESP8266WebServer.h>

#define LEDn    12
#define LEDpwm   2

//Define name of the Wifi & password for creating an access point
const char* ssid = "ledTest";;
//!!!Your password MUST be a minimum of 8 characters...otherwise neither password nor ssid will be accepted -> default or old ssid&pwd will show up!!! 
const char* password = "12341234";


ESP8266WebServer server(80);

char webpage[] = R"=(
<html>
<body>
  <div style="background:#FFFFFF; width:400px; height:200px">
    <div style="background:#FFFFFF; width:100%; height:10%">
      <h2 align="center">ON</h2>
    </div>
    <div id="LEDn" style="background:#77FF77; width:100%; height:100%"></div> 
  </div>
  <div style="background:#FFFFFF; width:400px; height:200px">
    <div style="background:#FFFFFF; width:100%; height:10%">
      <h2 align="center">PWM plus</h2>
    </div>
    <div id="pwmplus" style="background:#00EE00; width:100%; height:100%"></div>
  </div>
  <div style="background:#FFFFFF; width:400px; height:200px">
    <div style="background:#FFFFFF; width:100%; height:10%">
      <h2 align="center">PWM minus</h2>
    </div>
    <div id="pwmminus" style="background:#CCFFCC; width:100%; height:100%"></div>
  </div>
</body>
<script>
  window.addEventListener('load', function(){
    var n = document.getElementById('LEDn') 
    var minus = document.getElementById('pwmminus') 
    var plus = document.getElementById('pwmplus') 
    
    var xhr = new XMLHttpRequest();

    n.onmousedown = function(){
      xhr.open("GET", "LEDon", true);
      xhr.send();  
    }

    n.onmouseup = function(){
      xhr.open("GET", "LEDoff", true);
      xhr.send();  
    }

    n.addEventListener('touchstart', function(e){
      xhr.open("GET", "LEDon", true);
      xhr.send();
    }, false)

    n.addEventListener('touchend', function(e){
      xhr.open("GET", "LEDoff", true);
      xhr.send();
    }, false)

    plus.onmousedown = function(){
      xhr.open("GET", "plus", true);
      xhr.send();  
    }

    plus.onmouseup = function(){
      xhr.open("GET", "stop", true);
      xhr.send();  
    }

    plus.addEventListener('touchstart', function(e){
      xhr.open("GET", "plus", true);
      xhr.send();
    }, false)

    plus.addEventListener('touchend', function(e){
      xhr.open("GET", "stop", true);
      xhr.send();
    }, false)

    minus.onmousedown = function(){
      xhr.open("GET", "minus", true);
      xhr.send();  
    }

    minus.onmouseup = function(){
      xhr.open("GET", "stop", true);
      xhr.send();  
    }

    minus.addEventListener('touchstart', function(e){
      xhr.open("GET", "minus", true);
      xhr.send();
    }, false)

    minus.addEventListener('touchend', function(e){
      xhr.open("GET", "stop", true);
      xhr.send();
    }, false)
  },false)
</script>
</html>
)=";

bool led;
int pwm;
int pwmCount;

void ledON(){
  led=1;
}

void ledOFF(){
  led=0;  
}
//pwm functions - if pwm is set to 1/-1 it will in-/decrease pwmCounter in loop()
void plus(){
  pwm=3;  
}

void minus(){
  pwm=-3;  
}

void stopPWM(){
  pwm=0;  
}
//----------------------------------------------------------------------------------
void handleRoot(){
  server.send(200, "text/html", webpage);  
}

void setup() {
  Serial.begin(9600);
  //Set LED pins to output________________
  pinMode(LEDn,OUTPUT);
  pinMode(LEDpwm,OUTPUT);
  //Wifi as access point__________________
  WiFi.mode(WIFI_AP);
  IPAddress apLocalIp(2,2,2,2);
  IPAddress apSubnetMask(255,255,255,0);
  WiFi.softAPConfig(apLocalIp,apLocalIp,apSubnetMask);
  WiFi.softAP(ssid, password);
  //Server________________________________
  server.begin();
  server.on("/", handleRoot);
  server.on("/LEDon", ledON);     
  server.on("/LEDoff", ledOFF);   
  server.on("/minus", minus);   
  server.on("/plus", plus);   
  server.on("/stop", stopPWM);   
  //initialize variables__________________
  pwm=0;
  pwmCount=0;
  led=0;
}

void loop() {
  server.handleClient();
  //turns LED ON/OFF (GPIO 2/D4)
  digitalWrite(LEDn, led);
  
  //in-/decreases pwmCount(=brightness)
  pwmCount+=pwm;
  //to hold pwmCount in it's range (min 0 - max 1024)
  if(pwmCount>1024)pwmCount=1024;
  if(pwmCount<0)pwmCount=0;
  //Serial.println(pwmCount);
  //sets pwmCount to PWM value of GPIO 12/D6
  analogWrite(LEDpwm, pwmCount);
  delay(10);
}
