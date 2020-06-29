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

	else
	    Serial.println("Error, no Handshake received!");
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
    uint8_t buffer[sizeof(result.carInfo)];
    memset(buffer, 0, sizeof(result.carInfo));
    udp.parsePacket();
    if(udp.read(buffer, sizeof(result.carInfo)) == sizeof(result.carInfo)){
        memcpy(&result.carInfo, buffer, sizeof(result.carInfo));
        result.result = isUpdate;
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
    uint8_t buffer[sizeof(result.lap)];
    memset(buffer, 0, sizeof(result.lap));
    udp.parsePacket();
    if(udp.read(buffer, sizeof(result.lap)) == sizeof(result.lap)){
        memcpy(&result.lap, buffer, sizeof(result.lap));
        result.result = isUpdate;
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