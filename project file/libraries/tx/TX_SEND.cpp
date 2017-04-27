
#ifndef TX_SEND_cpp
#define TX_SEND_cpp


#include <TX_SEND.h> //include the declaration for this class
#include <Arduino.h>
//const byte LED_PIN = 13; //use the LED @ Arduino pin 13




//<<constructor>>
TX_SEND::TX_SEND()
{
    Serial.begin(9600); 
}

//<<destructor>>
//TX_SEND::~TX_SEND(){/*nothing to destruct*/}

//RTS_Packet
uint8_t TX_SEND::tx_send_RTS(uint8_t RTS_length,uint8_t RTS_max,uint8_t RTS_current)
{
	//Serial.begin(9600); 
	Serial.println("Sending RTS");
	uint8_t RTS_Packet[RTS_length];
	uint8_t RTS = 0xAA;
	
	
	/* Create Packet  */
	RTS_Packet[0] = (uint8_t)(RTS_length);
	RTS_Packet[1] = (uint8_t)(RTS_max);
	RTS_Packet[2] = (uint8_t)(RTS_current);
	RTS_Packet[3] = (uint8_t)(RTS);
	
	Serial.write(RTS_Packet, 20);
	
	return RTS_current;
	
}

/*
//RTS ACK_Packet
void TX_SEND::tx_send_RTS_ACK(uint8_t ACK_length,uint8_t ACK_current)
{
	Serial.begin(9600); 
	Serial.println("Sending RTS ACK");
	uint8_t RTS_Packet[ACK_length];
	uint8_t ACK = 0xFF;
	
	
	// Create ACK Packet
	RTS_Packet[0] = (uint8_t)(ACK);
	RTS_Packet[1] = (uint8_t)(ACK_Packet);
	
	Serial.write(ACK_Packet, ACK_length);
	
}
*/



//send Acknowledgement for packets
void TX_SEND::tx_send_PacketAck(uint8_t ACK_current)
{
	Serial.begin(9600); 
	Serial.println("Sending PACKET ACK");
	uint8_t ACK_Packet[2];
	uint8_t ACK = 0xFF;
	
	
	// Create ACK Packet  
	ACK_Packet[0] = (uint8_t)(ACK);
	ACK_Packet[1] = (uint8_t)(ACK_current);
	
	Serial.println("Sending PACKET ACK");
	
	Serial.write(ACK_Packet, 2);
	
}




//CTS_Packet
void TX_SEND::tx_send_CTS(uint8_t CTS_length)
{
	
	Serial.begin(9600); 
	uint8_t CTS_Packet[2];
	uint8_t CTS = 0x55;
	
	/* Create Packet  */
	
	CTS_Packet[0] = (uint8_t)(CTS_length);
	CTS_Packet[1] = (uint8_t)(CTS);
	
	Serial.write(CTS_Packet, 2);
	
	Serial.println("Sending CTS Packet");
}

// Modbus Packet
uint8_t TX_SEND::tx_send_Modbus(uint8_t Address, uint8_t Function_code, uint8_t Modbus_data,uint16_t CRC_byte, uint8_t *Modbus_packet)
{
	Serial.begin(9600);
	static uint8_t Modbus[5];
	uint8_t CTS = 0x55;
	
	/* Create Modbus Packet  */
	Modbus[0] = (uint8_t)(Address);
	Modbus[1] = (uint8_t)(Function_code);
	Modbus[2] = (uint8_t)(Modbus_data);
	Modbus[3] = (uint8_t)(CRC_byte>>8);
	Modbus[4] = (uint8_t)(CRC_byte);
	
	for(uint8_t i = 0; i<5; i++)
	{
		Modbus_packet[i] = Modbus[i];
	}
	
}

// send Packet

void TX_SEND::tx_send_Packet(uint8_t Packet_length,
							 uint8_t Packet_max,
							 uint8_t Packet_current,
							 uint8_t Packet_ACK_counter,
							 uint8_t Packet_ACK_counter_max,
							 uint8_t *Modbus_Packet)
{
	
	
	Serial.begin(9600);
	uint8_t Modbus[5];
	uint8_t Packet[Packet_length+5];
	uint8_t header[5];
	
	for(uint8_t i = 0; i<8; i++)
	{
	Modbus[i] = Modbus_Packet[i];
	}
	
	//* Create Packet header  
	header[0] = (uint8_t)(Packet_length);
	header[1] = (uint8_t)(Packet_max);
	header[2] = (uint8_t)(Packet_current);
	header[3] = (uint8_t)(Packet_ACK_counter);
	header[4] = (uint8_t)(Packet_ACK_counter_max);
	
	
	
	// create Packet buffer and then send 
	uint8_t Packet_buffer[(Packet_length/2)+5];
	
		switch (Packet_current)
		{
		case 1:
			for(uint8_t i=0; i<((Packet_length/2)+5); i++)
			{	
				if(i<5){
				Packet_buffer[i] = header[i];
				}else{
				Packet_buffer[i] = Modbus[i-5];
				}
			}
		Serial.write(Packet_buffer,(Packet_length/2)+5);
		break;
		
		case 2:
			for(uint8_t i=0; i<((Packet_length/2)+5); i++)
			{
				if(i<5){
				Packet_buffer[i] = header[i];
				}else{
				Packet_buffer[i] = Modbus[i];
				}
			}
		Serial.write(Packet_buffer,((Packet_length/2)+5));
		default:
			Packet_current = 1;
		}
}


#endif



