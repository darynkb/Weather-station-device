

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SD.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPtimeESP.h>
#define DEBUG_ON
#include <SDConfigFile.h>

NTPtime NTPch("pool.ntp.org");    
strDateTime dateTime;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

#define DHTPIN D3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
#define CS_PIN  D8

int i = 1;
float sumH = 0;
float sumT = 0;
float avgH;
float avgT;
int val;
int sec;
int minutes;
int hours;


#define CONFIG_FILE "config.cfg" //this is my config file
SDConfigFile cfg;

char *wifiName;
char *wifiPass;
int id;
String ip;
boolean readConfiguration();
char *endRoute = "/api/v1/post2";

String dataCreation();
boolean didReadConfig = false;



void setup()
{
  Wire.begin(D2, D1); // D1 = SCL | D2 = SDA
  lcd.init(); // Iniciamos el LCD
  lcd.backlight(); 
  lcd.setCursor(0,1);
  Serial.print("Initializing SD");
  dht.begin();
  sdInit();
  Serial.begin(115200);                            //Serial connection

  didReadConfig = readConfiguration();
  ip += "/api/v1/post";
  ip += "\0";

//  Serial.print(F("IP = "));
//  Serial.println(ip);
  
  wifiInit();
 
}


void loop(){
  Serial.println(">BEGIN");
  float h = dht.readHumidity();
  float t = dht.readTemperature();
//  Serial.print("Hum");
//  Serial.println(h);
//  Serial.print("Temp");
//  Serial.println(t);
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
  lcd.setCursor(11,0);
  lcd.print(t);
  lcd.setCursor(11,1);
  lcd.print(h);

  
  
  String dataString = dataCreation(t, h);
  Serial.print("From dataCreation: ");
  Serial.print(dataString);
  
  sdWriting(dataString);

  sumH=sumH+h;
  sumT=sumT+t;
  avgH=sumH/i;
  avgT=sumT/i;
  i++;
  Serial.print("Hum:  ");
  Serial.println(avgH);
  Serial.print("Temp: ");
  Serial.println(avgT);
  saveAvg();

  String serverData = serverDataCreation(t, h);
  sendingToServer(h, t, serverData);

  Serial.println(">END");
  Serial.println(" ");
  delay(3000);
}


void saveAvg(){
  if(SD.exists("tAvg.txt")){
    SD.remove("tAvg.txt");
  }
  if(SD.exists("hAvg.txt")){
    SD.remove("hAvg.txt");
  }
  
  File avgTf= SD.open("tAvg.txt", FILE_WRITE);
  if (avgTf) {
    Serial.println("Successfully saved T in SD");
      avgTf.println(avgT);
      avgTf.close();
  }
  else {
    Serial.println("couldn't saved T in SD");
  }

  
  File avgHf = SD.open("hAvg.txt", FILE_WRITE);
  if (avgHf) {
    Serial.println("Successfully saved H in SD");
      avgHf.println(avgH);
      avgHf.close();
  }
  else {
    Serial.println("couldn't saved H in SD");
  }

 
}

boolean readConfiguration() {
  /*
   * Length of the longest line expected in the config file.
   * The larger this number, the more memory is used
   * to read the file.
   * You probably won't need to change this number.
   */
  const uint8_t CONFIG_LINE_LENGTH = 127;

  SDConfigFile cfg;
  
  if (!cfg.begin(CONFIG_FILE, CONFIG_LINE_LENGTH)) {
    Serial.print("Failed to open configuration file: ");
    Serial.println(CONFIG_FILE);
    return false;
  }
  
  // Read each setting from the file.
  while (cfg.readNextSetting()) {
      if(cfg.nameIs("wifi")){
        wifiName = cfg.copyValue();
      }else if (cfg.nameIs("password")) {
        wifiPass = cfg.copyValue();
      }else if(cfg.nameIs("id")){
        id = cfg.getIntValue();
      }else if(cfg.nameIs("ip")){
        ip = cfg.copyValue();
      }else{
        Serial.print("The name of this setting is:");
        Serial.println(cfg.getName());
      }
  }
  cfg.end();
  
  return true;
}


String dataCreation(float t, float h) {
  String dataString = "";
  do {
    dateTime = NTPch.getNTPtime(5, 1);  
  } while (!dateTime.valid);
  if(dateTime.valid){
    byte actualHour = dateTime.hour;
    byte actualMinute = dateTime.minute;
    int actualyear = dateTime.year;
    byte actualMonth = dateTime.month;
    byte actualday =dateTime.day;
    if (actualday < 10) {
      dataString += '0';
    }
    dataString += String(actualday);
    dataString += "/";
    if (actualMonth < 10) {
      dataString += '0';
    }
    dataString += String(actualMonth);
    dataString += "/";
    dataString += String(actualyear);
    dataString += "  ";
    if (actualHour < 10) {
      dataString += '0';
    }
    dataString += String(actualHour);
    dataString += ":";
    if (actualMinute < 10) {
      dataString += '0';
    }
    dataString += String(actualMinute);
    dataString += " Температура:";
    dataString += String(t);
    dataString += " Влажность:";
    dataString += String(h);
    dataString += "\r\n";
  }

  return dataString;
}


String serverDataCreation(float t, float h) {
  String dataString = "";
  do {
    dateTime = NTPch.getNTPtime(5, 1);  
  } while (!dateTime.valid);
  if(dateTime.valid){
    byte actualHour = dateTime.hour;
    byte actualMinute = dateTime.minute;
    int actualyear = dateTime.year;
    byte actualMonth = dateTime.month;
    byte actualday =dateTime.day;
    if (actualday < 10) {
      dataString += '0';
    }
    dataString += String(actualday);
    dataString += "/";
    if (actualMonth < 10) {
      dataString += '0';
    }
    dataString += String(actualMonth);
    dataString += "/";
    dataString += String(actualyear);
    dataString += "  ";
    if (actualHour < 10) {
      dataString += '0';
    }
    dataString += String(actualHour);
    dataString += ":";
    if (actualMinute < 10) {
      dataString += '0';
    }
    dataString += String(actualMinute);
  }

  return dataString;
}


void sdWriting(String dataString) {
  File humTemp = SD.open("humTemp.txt", FILE_WRITE);
  if (humTemp) {
    Serial.println("SD opened");
      humTemp.println(dataString);
      humTemp.close();
  }
  else {
    Serial.println("couldn't do it my man. Error in opening SD");
  }
}

void sendingToServer(float h, float t, String serverData) {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

//    Serial.println("*Check serverData in function: ");
//    Serial.print(serverData);
 
    DynamicJsonDocument doc(2048);
    doc["hum"] = h;
    doc["temp"] = t;
    doc["sent_date"] = serverData;
    doc["device_id"] = id;

    // Serialize JSON document
    String json;
    serializeJson(doc, json);

    WiFiClient client;  // or WiFiClientSecure for HTTPS
    HTTPClient http;

    // Send request
    http.begin(client, ip); //"http://192.168.73.200:8090/api/v1/post2" address can be changed later
    http.addHeader("Content-Type","application/json");
    http.POST(json);

    Serial.println("* serverData:");
    Serial.println(h);
    Serial.println(t);
    Serial.println(serverData);
    Serial.println(id);
    Serial.println("* Sent to server.");

// Read response
    Serial.print(http.getString());

// Disconnect
    http.end();
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
}

void sdInit() {
  if (!SD.begin(CS_PIN)) {
    Serial.println("error with SD");
    return;
  }
  Serial.println("SD Initialized");
}

void wifiInit() {
  WiFi.begin(wifiName, wifiPass);   //WiFi connection  "iPhone dora"  "PAVLODAR14"
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
  Serial.println("Wifi connected");
}
