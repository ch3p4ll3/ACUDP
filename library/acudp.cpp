#include "ACUDP.h"  // dichiarazione della classe
#include <WiFiUdp.h>

/* funzione di inizializzazione */
void ACUDP::begin(char * ip, int port, IPAddress ipEsp){
	udpAddress = ip;
	udpPort = port;
	udp.begin(ipEsp, udpPort);
}

void ACUDP::begin(char * ip, IPAddress ipEsp){
	udpAddress = ip;
	udpPort = 9996;
	udp.begin(ipEsp, udpPort);
}

void ACUDP::sendHandshake(){
	udp.beginPacket(udpAddress, udpPort);
	uint8_t b[sizeof(hand)];
	memcpy(b, &hand, sizeof(hand));
	udp.write(b, sizeof(hand));
	udp.endPacket();

	//receive
	uint8_t buffer[sizeof(tmpHandshake)];
	memset(buffer, 0, sizeof(tmpHandshake));
	udp.parsePacket();
	if(udp.read(buffer, sizeof(tmpHandshake)) == sizeof(tmpHandshake)){
		memcpy(&tmpHandshake, buffer, sizeof(tmpHandshake));
		Serial.println();
	}
}


void ACUDP::sendUpdate(){
	udp.beginPacket(udpAddress, udpPort);
	uint8_t b[sizeof(update)];
	memcpy(b, &update, sizeof(update));
	udp.write(b, sizeof(update));
	udp.endPacket();
	isUpdate = true;
}


void ACUDP::sendSpot(){
	udp.beginPacket(udpAddress, udpPort);
	uint8_t b[sizeof(spot)];
	memcpy(b, &update, sizeof(spot));
	udp.write(b, sizeof(spot));
	udp.endPacket();
	isUpdate = false;
}


void ACUDP::sendQuit(){
	udp.beginPacket(udpAddress, udpPort);
	uint8_t b[sizeof(quit)];
	memcpy(b, &update, sizeof(quit));
	udp.write(b, sizeof(quit));
	udp.endPacket();
}


Result ACUDP::readUpdate(){
    Result result;
	RTCarInfo carInfo;
    uint8_t buffer[sizeof(carInfo)];
    memset(buffer, 0, sizeof(carInfo));
    udp.parsePacket();
    if(udp.read(buffer, sizeof(carInfo)) == sizeof(carInfo)){
        memcpy(&carInfo, buffer, sizeof(carInfo));
        result.result = isUpdate;
        result.carInfo = carInfo;
        memset(&result.lap, 0, sizeof(result.lap));
        return result;
    }

    else{
        result.result = isUpdate;
        memset(&result.carInfo, 0, sizeof(result.carInfo));
        memset(&result.lap, 0, sizeof(result.lap));
        return result;
    }
}


Result ACUDP::readSpot(){
    Result result;
	RTLap Lap;
    uint8_t buffer[sizeof(Lap)];
    memset(buffer, 0, sizeof(Lap));
    udp.parsePacket();
    if(udp.read(buffer, sizeof(Lap)) == sizeof(Lap)){
        memcpy(&Lap, buffer, sizeof(Lap));
        result.result = isUpdate;
        result.lap = Lap;
        memset(&result.carInfo, 0, sizeof(result.carInfo));
        return result;
    }

    else{
        result.result = isUpdate;
        memset(&result.carInfo, 0, sizeof(result.carInfo));
        memset(&result.lap, 0, sizeof(result.lap));
        return result;
    }
}

Result ACUDP::read(){
    if (isUpdate) return readUpdate();

    else return readSpot();
}