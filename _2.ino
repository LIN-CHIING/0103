#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include<Wire.h>
#include<SPI.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>


#define OLED_MOSI 16 //D1
#define OLED_CLK 5 //D0
#define OLED_DC 4
#define OLED_CS 0
#define OLED_RESET 2

Adafruit_SSD1306 display(OLED_MOSI,OLED_CLK,OLED_DC,OLED_RESET,OLED_CS);
//const char* ssid     = "Lai";           // insert your SSID
//const char* password = "22397873";   // insert your password
//const char* ssid     = "GameNetwork";           // insert your SSID
////const char* password = "805801805801";   // insert your password
//const char* ssid     = "賴映如 的 iPhone";           // insert your SSID
//const char* password = "irene423";   // insert your password
//const char* ssid     = "Cheryl";           // insert your SSID
//const char* password = "CherylH825";   // insert your password
//const char* ssid     = "Pluto";           // insert your SSID
//const char* password = "pluto27351";   // insert your password
const char* ssid     = "hi";           // insert your SSID
const char* password = "";   // insert your password
WiFiClient client;

const char* server = "api.openweathermap.org";  // server's address
const char* resource = "/data/2.5/weather?id=1668341&units=metric&APPID=5f22c3cad94d76c3def72fa3bb9baf14"; // insert your API key

char response[600]; // this fixed sized buffers works well for this project using the NodeMCU.

//rgbled
#define  REDPIN 14 
#define  GREENPIN 12
#define  BLUEPIN 13 
#define  FADESPEED 15     // make this higher to slow down

void setup(){
    pinMode(REDPIN,  OUTPUT); 
    pinMode(GREENPIN,  OUTPUT);
    pinMode(BLUEPIN,  OUTPUT); 
    delay(3000);
    // initialize serial
    Serial.begin(115200);
    while(!Serial){
        delay(100);
        }
      
    // initialize WiFi
    WiFi.begin(ssid, password);

     //Connection WiFi
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.println("Wi-Fi is Connected!!!");
    
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
    }
display.begin(SSD1306_SWITCHCAPVCC); //初始化OLED
display.clearDisplay(); //清除清除螢幕和緩衝區

   
}



void loop(){
    
        // connect to server  
        bool ok = client.connect(server, 80);
        bool beginFound = false;
        
        while(!ok){
            //Serial.print(".");
            delay(500);        
        }
    
        delay(500);
    
        //Send request to resource
        client.print("GET ");
        client.print(resource);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(server);
        client.println("Connection: close");
        client.println();
    
        delay(100);
    
        //Reading stream and remove headers
        client.setTimeout(10000);
   
        bool ok_header = ok_header = client.find("\r\n\r\n");
    
        while(!ok_header){
        // wait
        }
         
        client.readBytes(response, 800);
        
        // uncomment line below to see data received for debugging purposes or just fun    
        // Serial.println(response); 
        
        // process JSON
        DynamicJsonBuffer jsonBuffer;
        
        // But.....make sure the stream header is valid
        // Sometime OWM includes invalid data after the header
        // Parsing fails if this data is not removed
        
        if(int(response[0]) != 123){
            Serial.println("Wrong start char detected");
        int i = 0;
        while(!beginFound){
            if(int(response[i]) == 123){ // check for the "{" 
            beginFound = true;
            Serial.print("{ found at ");
            Serial.println(i);
            }
            i++;
        }

            int eol = sizeof(response);
            Serial.print("Length = ");
            Serial.println(eol);
  
    
            //restructure by shifting the correct data
            Serial.println("restructure");
            for(int c=0; c<(eol-i); c++){
                response[c] = response[((c+i)-1)];
                Serial.print(response[c]);
            }
     
            Serial.println("Done...");

                
            }
        
        
        JsonObject& root = jsonBuffer.parseObject(response);
        
        if (!root.success()) {
        Serial.println("JSON parsing failed!");
        } 
        else {
        //Serial.println("JSON parsing worked!");
        }
        
        const char* location = root["name"]; 
        const char* weather = root["weather"][0]["description"];
        const char* id = root["weather"][0]["id"];
        double temp = root["main"]["temp"];
        
        // Print data to Serial
        Serial.print("*** ");
        Serial.print(location);
        Serial.println(" ***");
        Serial.print("Type: ");
        Serial.println(weather);
        Serial.print("Temp ");
        Serial.print(temp);
        Serial.println("C");
        Serial.print(id);
        
        Serial.println("----------"); 
     
        client.stop(); // disconnect from server
    display.setTextColor(WHITE);
display.setTextSize(1);
display.setCursor(0,0);
 Serial.print("*** ");
        display.print(location);
        display.print("  NTUE DTD");
        display.setCursor(0,10);
        display.print("Weather: ");
        display.print(weather);
        display.setCursor(0,20);
        display.print("Temp: ");
        display.print(temp);
        display.print("C");
//       display.print("----------"); 
display.display(); //顯示畫面
/////////////////////////////////////////////////////////////////////////////////////////////////////      

char i;
i=*id;Serial.print(i);
        if(i=='2'){
        analogWrite(REDPIN, 255);//Red Light
        analogWrite(GREENPIN, 243);//Green Light
        analogWrite(BLUEPIN, 20);//Blue Light

        delay(200);  
       Serial.print("thunderstorm!");
       }     
       
      if(i=='3'){
        analogWrite(REDPIN, 255);//Red Light
        analogWrite(GREENPIN, 158);//Green Light
        analogWrite(BLUEPIN, 197);//Blue Light
        delay(200);  
       Serial.print("DRIZZLE!");
       }
         if(i=='5'){
        analogWrite(REDPIN, 240);//Red Light
        analogWrite(GREENPIN, 0);//Green Light
        analogWrite(BLUEPIN, 0);//Blue Light
        delay(200);  
       Serial.print("RAIN!");
       }
        if(i=='8'){
        analogWrite(REDPIN, 148);//Red Light
        analogWrite(GREENPIN, 248);//Green Light
        analogWrite(BLUEPIN, 255);//Blue Light
        delay(200);  
       Serial.print("SUNNY!");
       }
         
       if(i=='9'){
        analogWrite(REDPIN, 68);//Red Light
        analogWrite(GREENPIN, 233);//Green Light
        analogWrite(BLUEPIN, 90);//Blue Light
        delay(200);  
        Serial.print("Other!");
        }
    
      
        for(int x=0; x<10; x++){ // wait for new connection with progress indicator
            Serial.print(".");
            delay(99999); // the OWM free plan API does NOT allow more then 60 calls per minute
        }
        
        Serial.println("");

     Serial.begin(115200);

    }

