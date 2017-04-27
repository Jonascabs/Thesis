


#include <RX_REC.h> //include the declaration for this class
#include <Arduino.h>
//const byte LED_PIN = 13; //use the LED @ Arduino pin 13

//<<constructor>>
RX_REC::RX_REC(){
    Serial.begin(9600); 
}

//<<destructor>>
//RX_REC::~RX_REC(){/*nothing to destruct*/}

volatile uint8_t RTS_length = 2;

//RTS_rec_Packet
uint8_t RX_REC::rx_rec_RTS()
{
	//uint8_t RTS_length = 2;
	Serial.begin(9600); 
	Serial.print("Waiting for RTS");
	delay(1000);
	unsigned long start_RTS_timer = millis();
	unsigned long RTS_timer_max = 5000;
	uint8_t RTS_Packet[RTS_length];
	uint8_t RTS = 0xAA;
	
		while ( (Serial.available()< RTS_length) && ((millis() - start_RTS_timer) < RTS_timer_max) )
		{      
		// hang in this loop until we either get 9 bytes of data or 1 second
		// has gone by
		}
		if(Serial.available() < RTS_length)
{
        // the data didn't come in - handle that problem here
		Serial.println("RTS not received");
		Serial.println("Requesting RTS");
		
		}
		else
		{
		for(int n=0; n<RTS_length; n++)
         RTS_Packet[n] = Serial.read(); // Then: Get them. 
		}
	
	
	/* assign Packet*/
	uint8_t RTS_length 	= RTS_Packet[0];
	uint8_t RTS_max    	= RTS_Packet[1];
	uint8_t RTS_current = RTS_Packet[2];
	uint8_t	RTS_new			= RTS_Packet[3];
	
	if(RTS_new == 0xAA){
		Serial.println("RTS PACKet Received");
		return 1;
		Serial.println("RTS PACKET:");
		Serial.println("RTS LENGTH|RTS MAX|CURRENT RTS|RTS BYTE");
		for(uint8_t i = 0; i<RTS_length; i++)
			{
			Serial.print(RTS_Packet[i],HEX);
			}
	}else{
		Serial.println("RTS Packet not received");
	}
	
}

//CTS_rec_Packet
uint8_t RX_REC::rx_rec_CTS()
{
	
	//Serial.begin(9600); 
	Serial.println("Waiting for CTS");
	unsigned long start_CTS_timer = millis();
	unsigned long CTS_timer_max = 1000;
	uint8_t CTS_length = 2;
	uint8_t CTS_Packet[CTS_length];
	uint8_t CTS = 0xAA;
	
		while ( (Serial.available()< CTS_length) && ((millis() - start_CTS_timer) < CTS_timer_max) )
		{      
		// hang in this loop until we either get 9 bytes of data or 1 second
		// has gone by
		}
		if(Serial.available() < CTS_length)
{
        // the data didn't come in - handle that problem here
		Serial.println("CTS not received");
		Serial.println("Requesting RTS");
		
		}
		else
		{
		for(int n=0; n<CTS_length; n++)
         CTS_Packet[n] = Serial.read(); // Then: Get them. 
		}
	
	
	/* assign Packet*/
	uint8_t rec_CTS_length 		= CTS_Packet[0];
	uint8_t	CTS_new				= CTS_Packet[1];
	
	if(CTS_new == 0x55){
		Serial.println("CTS Packet Received");
		return 1;
		Serial.println("CTS PACKET FORMAT");
		Serial.println("CTS LENGTH|CTS BYTE");
		for(uint8_t i = 0; i<2; i++)
			{
			Serial.print(CTS_Packet[i],HEX);
			}
			
	}else{
		Serial.println("CTS Packet not received");
	}
	
}

// receive Packet
uint8_t RX_REC::rx_rec_Packet()
{
	
	Serial.begin(9600); 
	Serial.println("Waiting for the series of Packets");
	unsigned long start_Packet_timer = millis();
	unsigned long Packet_timer_max = 5000;
	
	
	uint8_t Packet[9];
	uint8_t	header[5];
	uint8_t	Modbus[9];
	
	
		while ( (Serial.available()< 9) && ((millis() - start_Packet_timer) < Packet_timer_max) )
		{      
		// hang in this loop until we either get 9 bytes of data or 1 second
		// has gone by
		}
		if(Serial.available() < 9)
		{
        // the data didn't come in - handle that problem here
		Serial.println("Packet not received");
		Serial.println("Requesting for retransmit");
		return 0;
		}
		else
		{
		for(uint8_t n=0; n<RTS_length; n++)
		{
         Packet[n] = Serial.read(); // Then: Get them. 
		}
		}
		
		//stripping the header
		for(uint8_t i=0; i<5; i++){
		header[i] = Packet[i];
		}
		
		uint8_t Packet_length 	        = header[0];
		uint8_t Packet_max	 		    = header[1];										
		uint8_t Packet_current		    = header[2];											
		uint8_t Packet_ACK_counter	    = header[3];											
		uint8_t Packet_ACK_counter_max  = header[4];		

		
		
		if(Packet_current==1){
			Serial.println("Packet1 received, waiting for the other Packet!");
			for(uint8_t i=5; i<=8; i++){
				Modbus[i-5] = Packet[i];	
			}
			Serial.print("Packet1 Received:");
			for(uint8_t i = 0; i<8; i++)
			{
			Serial.print(Modbus[i],HEX);
			}
			
			return Packet_current;
			
			
		}else{
		Serial.println("Packet2 received");
			for(uint8_t i = 5; i<=8; i++){
				Modbus[i] = Packet[i];				
			}
			Serial.println("All Packets Received");
			Serial.println("MODBUS DATA");
			Serial.println("ADDRESS|FUNCTION CODE|DATA|CRC");
			for(uint8_t i = 0; i<8; i++)
			{
			Serial.print(Modbus[i],HEX);
			}
			
			return Packet_current;
			
		}
	
}	
// RX rec Packet ACK
uint8_t RX_REC::rx_rec_PacketAck(uint8_t current_ACK)
{
	
	Serial.begin(9600); 
	Serial.println("Waiting for Packet ACK");
	unsigned long start_ACK_timer = millis();
	unsigned long ACK_timer_max = 1000;
	uint8_t ACK_Packet[2];
	
		while ( (Serial.available()< 2) && ((millis() - start_ACK_timer) < ACK_timer_max) )
		{      
		// hang in this loop until we either get 9 bytes of data or 1 second
		// has gone by
		}
		if(Serial.available() < 2)
{
        // the data didn't come in - handle that problem here
		Serial.println("ACK not received");
		
		}
		else
		{
		for(int n=0; n<2; n++)
         ACK_Packet[n] = Serial.read(); // Then: Get them. 
		}
	
	
	/* assign Packet*/
	uint8_t ACK 		= ACK_Packet[0];
	uint8_t ACK_current = ACK_Packet[1];
	
	
	if(ACK == 0xFF && ACK_current == current_ACK){
		return 1;
		Serial.println("ACK PACKET FORMAT:");
		Serial.println("ACK MESSAGE |CURRENT PACKET ACK");
		for(uint8_t i = 0; i<2; i++)
		{
			Serial.print(ACK_Packet[i],HEX);
		}
		
	}else{
		Serial.println("Packet not received");
	}
	
}





