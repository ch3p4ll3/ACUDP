[![Codacy Badge](https://api.codacy.com/project/badge/Grade/480a78185b0d4cc8b9707b5d1942ab45)](https://www.codacy.com/manual/slinucs/ACUDP?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ch3p4ll3/ACUDP&amp;utm_campaign=Badge_Grade)
# ACUDP
Library for ESP32 to receive Assetto Corsa telemetry via UDP

## How to install
Copy the `library` folder contained in the ACUDP folder to the Arduino libraries folder. Once the folder has been copied, the `ACUDP` library should appear in the Arduino IDE list of libraries. The examples should also appear in the examples menu in the Arduino IDE.

## Simple Example
```C
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
  
  // or initializes the library specifying the ip of the 
  // device in which Assetto Corsa is run, the UDP port and
  // the ip of the esp32 using:
  
  //acTelemetry.initialize("192.1168.1.10", 9996, WiFi.localIP());
  
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

    if (carInfo.result) 
    /*if result is true then only update has been selected and only the struct 
    carInfo will have data, while lap will always be 0. 
    Otherwise if result is false only lap will contain data, carInfo will contain only 0. 
    
    Note: if you put a wrong ip or AC hasn't been started, all carInfo and lap struct will be 0*/
      Serial.println(carInfo.carInfo.gear); //write the value of the gear on the serial

    else
      Serial.println("Spot!");
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
```

## ACUDP Library API
### Methods
| return type |                name of the method               |                                                                        description                                                                      |
| ----------- | ----------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| void        | initialize(char * ip, int port, IPAddress ipEsp)| This method is used to initialize the library. Accept as parameters the ip address where AC is executed, the UDP port and finally the address of esp32  |
| void        | initialize(char * ip, IPAddress ipEsp)          | This method is used to initialize the library. Accept as parameters the ip address where AC is executed and the address of esp32                        |
| void        | sendHandshake()                                 | This method is used to send the handshake to the AC server                                                                                              |
| void        | sendUpdate()                                    | This method must be used when the client wants to be updated from the specific ACServer.                                                                |
| void        | sendSpot()                                      | This method must be used when the client wants to be updated from the specific ACServer just for SPOT Events (e.g.: the end of a lap).                  |
| void        | sendQuit()                                      | This method must be used when the client wants to leave the comunication with ACServer.                                                                 |
| Result      | read()                                          | This method is used to read the packets that AC sends and returns a struct of type  [Result](#result-struct)                                            |

### Result struct
|    type   |           name         |                                                                                                                                   description                                                                                                                                                                      |
| --------- | ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| bool      | result                 | if result is true then only update has been selected and only the struct carInfo will have data while lap will always be 0. Otherwise if result is false only lap will contain data, carInfo will contain only 0. Note: if you put a wrong ip or not AC has still been started, all carInfo and lap data will be 0 |
| RTCarInfo | carInfo                | [RTCarInfo](#RTCarInfo-struct)                                                                                                                                                                                                                                                                                     |
| RTLap     | lap                    | [RTLap](#RTLap-struct)                                                                                                                                                                                                                                                                                             |

### RTCarInfo struct
|    type   |           name         |
| --------- | ---------------------- |
| char      | identifier             |
| int       | size                   |
| float     | speed_Kmh              |
| float     | speed_Mph              |
| float     | speed_Ms               |
| bool      | isAbsEnabled           |
| bool      | isAbsInAction          |
| bool      | isTcInAction           |
| bool      | isTcEnabled            |
| bool      | isInPit                |
| bool      | isEngineLimiterOn      |
| float     | accG_vertical          |
| float     | accG_horizontal        |
| float     | accG_frontal           |
| int       | lapTime                |
| int       | lastLap                |
| int       | bestLap                |
| int       | lapCount               |
| float     | gas                    |
| float     | brake                  |
| float     | clutch                 |
| float     | engineRPM              |
| float     | steer                  |
| int       | gear                   |
| float     | cgHeight               |
| float\[4] | wheelAngularSpeed      |
| float\[4] | slipAngle              |
| float\[4] | slipAngle_ContactPatch |
| float\[4] | slipRatio              |
| float\[4] | tyreSlip               |
| float\[4] | ndSlip                 |
| float\[4] | load                   |
| float\[4] | Dy                     |
| float\[4] | Mz                     |
| float\[4] | tyreDirtyLevel         |
| float\[4] | camberRAD              |
| float\[4] | tyreRadius             |
| float\[4] | tyreLoadedRadius       |
| float\[4] | suspensionHeight       |
| float    | carPositionNormalized   |
| float    | carSlope                |
| float\[3] | carCoordinates         |

### RTLap struct
|   RTLap   |         name        |
| --------- | ------------------- |
| int       | carIdentifierNumber |
| int       | lap                 |
| char\[50] | driverName          |
| char\[50] | carName             |
| int       | time                |
