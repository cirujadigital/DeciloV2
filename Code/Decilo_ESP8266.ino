#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "spiffs/spiffs.h"
#include <FS.h>

//#include <Arduino.h>

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"
AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;
AudioFileSourceID3 *id3;

boolean modo_carga;
const char* ssid = "DECILOV2";
const char* password = "";

 int valor_analog_pin;

ESP8266WebServer server(80);
const char* serverIndex = "<form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
File fsUploadFile;

void handleFileUpload()
{
  if(server.uri() != "/upload") return;
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
   // Serial.print("handleFileUpload Name: "); 
   // Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
   // Serial.print("handleFileUpload Data: "); 
  //  Serial.println(upload.currentSize);
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile)
      fsUploadFile.close();
   // Serial.print("handleFileUpload Size: ");
   // Serial.println(upload.totalSize);
  }
}

void handleListFiles()
{
  String FileList = "File List:\n";
  
  Dir dir = SPIFFS.openDir("/");
 // Serial.println("File List:");
  while (dir.next())
  {
    String FileName = dir.fileName();
    File f = dir.openFile("r");
    String FileSize = String(f.size());
    int whsp = 6-FileSize.length();
    while(whsp-->0)
    {
   //   Serial.print(" ");
      FileList += " ";
    }
  //  Serial.println(FileSize+" "+FileName);
    FileList += FileSize+" "+FileName+"\n";
  }     
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", FileList);
}

void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
 // Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
//    Serial.printf("%c", a);
  }
//  Serial.printf("'\n");
//  Serial.flush();
}

void setup(void)
{ //WiFi.forceSleepBegin();
 WiFi.mode(WIFI_OFF);
  if (analogRead(A0)<=11){
  modo_carga= true; // habria que poner un interruptor para activar el modo carga.
  
  }
  else
  {
    modo_carga=false;
 
    }
   // pinMode(D2,OUTPUT);
  SPIFFS.begin();
  pinMode(D1, OUTPUT); //LED
  Serial.begin(115200);
  digitalWrite(D1, HIGH);
  
  if (modo_carga)
  {
  Serial.println("Welcome to WebServer File System");
  Serial.println("");
  WiFi.mode(WIFI_AP);
 // WiFi.begin(ssid, password);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  //if (WiFi.waitForConnectResult() == WL_CONNECTED)
  if(true)
  {
   // Serial.print("Local IP: ");
   // Serial.println(myIP);
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/list", HTTP_GET, handleListFiles);
    server.onFileUpload(handleFileUpload);
    server.on("/upload", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", "OK");
    });
    server.begin();
  }
  }
  file = new AudioFileSourceSPIFFS("/funcion0.mp3");
    id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  out = new AudioOutputI2SNoDAC();
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
 
}

void loop(void) {
  if(modo_carga)
  {
  server.handleClient();
  }

 else{
  if (mp3->isRunning()) {
    digitalWrite(D1, HIGH);
    if (!mp3->loop())   mp3->stop(); digitalWrite(D1, LOW);   //delete file; delete id3; delete out; delete mp3;
  }
  else{
       valor_analog_pin= analogRead(A0);
//  Serial.println(valor_analog_pin);
   
   rep_mp3(0,11, "/funcion1.mp3");
   rep_mp3(85,110,"/funcion2.mp3");

   rep_mp3(160,185, "/funcion3.mp3");
   rep_mp3(320,340,"/funcion4.mp3");

   rep_mp3(270,290, "/funcion5.mp3");
   rep_mp3(230,250,"/funcion6.mp3");

   rep_mp3(350,370, "/funcion7.mp3");
     rep_mp3(380,395,"/funcion8.mp3");

   rep_mp3(405,430, "/funcion9.mp3");


  
  }
  }
}

void rep_mp3( int analog_min, int analog_max, const char *nombre_archivo)
{
//  valor_analog_pin= analogRead(A0);
// Serial.println(valor_analog_pin);
  if(valor_analog_pin<=analog_max && valor_analog_pin>= analog_min){
    delete file; delete id3; delete out; delete mp3;
   
   file = new AudioFileSourceSPIFFS(nombre_archivo);
    id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  out = new AudioOutputI2SNoDAC();
  mp3 = new AudioGeneratorMP3();
    
  mp3->begin(id3, out);
  
  }
 
  }


