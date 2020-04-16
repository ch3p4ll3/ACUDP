#include <ACUDP.h>
#include <WiFi.h>

// WiFi network name and password:
const char* networkName = "Your wifi name";
const char* networkPswd =  "your wifi psw";

// Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;
ACUDP acTelemetry;

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  
  /* Initializes the library specifying the ip of the device
  in which Assetto Corsa is run, the ip of the esp32.*/
  acTelemetry.initialize("127.0.0.1", WiFi.localIP());
  
  // or initializes the library specifying the ip of the 
  // device in which Assetto Corsa is run, the UDP port and
  // the ip of the esp32 using:
  
  //acTelemetry.initialize("127.0.0.1", 9996, WiFi.localIP());
  
  //send the handshake to Assetto Corsa
  acTelemetry.sendHandshake();
  
  // use sendUpdate if the client wants to be updated from the specific ACServer.
  acTelemetry.sendUpdate();
  
  // use sendSpot if the client wants to be updated from the specific 
  // ACServer just for SPOT Events (e.g. the end of a lap). using:
  
  //acTelemetry.sendSpot();
  
  // use sendQuit if the client wants to leave the communication with ACServer.
  //using:
  
  //acTelemetry.sendQuit();
}


void loop(){
  if(connected){
    RTCarInfo carInfo; //create a struct to save all the data
    carInfo = acTelemetry.readPacket(); //save on the new struct all the data returned by readPacket
    Serial.println(carInfo.gear); //write the value of the gear on the serial
    //You can also access only one information by doing:
    //Serial.println(acTelemetry.readPacket().gear);
  }
  delay(10);
}


void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  connected = true;
}
