#include <ACUDP.h>
#include <WiFi.h>

// WiFi network name and password:
const char* networkName = "Your wifi name";
const char* networkPswd =  "Your wifi psw";

// Are we currently connected?
boolean connected = false;

//The ACUDP library class
ACUDP acTelemetry;

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);

  /* Initializes the library specifying the ip of the device
  in which Assetto Corsa is run, the ip of the esp32.*/
  acTelemetry.begin("192.168.178.110", WiFi.localIP());

  /* or initializes the library specifying the ip of the
     device in which Assetto Corsa is run, the UDP port and
     the ip of the esp32 using:

  acTelemetry.initialize("192.1168.1.10", 9996, WiFi.localIP());*/

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
    Result carInfo; //create a struct to save all the data
    carInfo = acTelemetry.read(); //save on the new struct all the data returned by read

    if (!carInfo.result)
    /*if result is true then only update has been selected and only the struct
    carInfo will have data, while lap will always be 0.
    Otherwise if result is false only lap will contain data, carInfo will contain only 0.

    Note: if you put a wrong ip or AC hasn't been started, all carInfo and lap struct will be 0*/
      Serial.println(carInfo.lap.time); //write the value of the gear on the serial

    else
      Serial.println("Update!");
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