#ifndef RX_REC_H
#define RX_REC_H


#include <Arduino.h>

class RX_REC{ 
public:
	RX_REC();
	uint8_t rx_rec_RTS();
	uint8_t rx_rec_CTS();
	uint8_t rx_rec_Packet();
	uint8_t rx_rec_PacketAck(uint8_t current_ACK);
	
};

#endif