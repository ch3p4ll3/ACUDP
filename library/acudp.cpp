#include "ACUDP.h"  // dichiarazione della classe
#include <WiFiUdp.h>

/* funzione di inizializzazione */
void ACUDP::initialize(char * ip, int port, IPAddress ipEsp){
	udpAddress = ip;
	udpPort = port;
	udp.begin(ipEsp, udpPort);
}

void ACUDP::initialize(char * ip, IPAddress ipEsp){
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
}


void ACUDP::sendSpot(){
	udp.beginPacket(udpAddress, udpPort);
	uint8_t b[sizeof(spot)];
	memcpy(b, &update, sizeof(spot));
	udp.write(b, sizeof(spot));
	udp.endPacket();
}


void ACUDP::sendQuit(){
	udp.beginPacket(udpAddress, udpPort);
	uint8_t b[sizeof(quit)];
	memcpy(b, &update, sizeof(quit));
	udp.write(b, sizeof(quit));
	udp.endPacket();
}


RTCarInfo ACUDP::readUpdate(){
	RTCarInfo carInfo;
	uint8_t buffer[sizeof(carInfo)];
	memset(buffer, 0, sizeof(carInfo));
	udp.parsePacket();
	if(udp.read(buffer, sizeof(carInfo)) == sizeof(carInfo)){
		memcpy(&carInfo, buffer, sizeof(carInfo));
		return carInfo;
	}
}


RTLap ACUDP::readSpot(){
	RTLap Lap;
	uint8_t buffer[sizeof(Lap)];
	memset(buffer, 0, sizeof(Lap));
	udp.parsePacket();
	if(udp.read(buffer, sizeof(Lap)) == sizeof(Lap)){
		memcpy(&Lap, buffer, sizeof(Lap));
		return Lap;
	}
}