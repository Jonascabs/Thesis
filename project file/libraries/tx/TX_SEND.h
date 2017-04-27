#ifndef TX_SEND_H
#define TX_SEND_H


#include <Arduino.h>

class TX_SEND
{ 
	
	public:
		TX_SEND();
		uint8_t tx_send_RTS(uint8_t RTS_length,
						uint8_t RTS_max,
						uint8_t RTS_current);
		
		
		void tx_send_CTS(uint8_t CTS_length);
		
		void tx_send_PacketAck(uint8_t ACK_current);
		
		void tx_send_Packet(uint8_t Packet_length,
							uint8_t Packet_max,
							uint8_t Packet_current,
							uint8_t Packet_ACK_counter,
							uint8_t Packet_ACK_counter_max,
							uint8_t *Modbus_Packet);
							
		uint8_t tx_send_Modbus(uint8_t Address, 
							   uint8_t Function_code,
							   uint8_t Modbus_data,
							   uint16_t CRC_byte,
							   uint8_t *Modbus_packet);
		
		
};

#endif