#include <PROTOCOL_TIMER.h>
#include <inttypes.h>
#include <RX_REC.h>

#include <TX_SEND.h>



// Defining RTS Parameters
#define RTS_length 0x04
#define RTS_max 0x03

// Defining Packet Paramaters
#define packet_length 0x09
#define packet_max 0x02
#define packet_ack_counter_max 0x03

	// Defining Timer

int state=1;      
int wait_cts_status=0;
//PROTOCOL_TIMER timer;
TX_SEND tx;
RX_REC rx;
uint8_t modbus_address = 0x01;
uint8_t function_code = 0x02;
uint8_t modbus_data = 0x03;
uint16_t crc_byte = 0xF0;
uint8_t RTS_current = 1;
uint8_t packet_ack_counter = 1;
uint8_t packet_current;

uint8_t cts_timer;
uint8_t cts_received;

unsigned long packet_ack_timer;
uint8_t ack_received;  



void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	// unsigned long cts_timer = timer.rx_cts_timer();
	//uint8_t cts_received = rx.rx_rec_CTS();
	//unsigned long packet_ack_timer = timer.rx_packet_ack_timer();       
	//uint8_t ack_received = rx.rx_rec_PacketAck(packet_current); 
	/*Serial.println("Starting in 3..");
	delay(1000);
	Serial.println("Starting in 2..");
	delay(1000);
	Serial.println("Starting in 1..");
	delay(1000);*/
}

void loop(){

	switch (state)
	{
	case 1: //idle
		Serial.println("Receive_Modbus Mode");
		Serial.println("Initializing Master-side Transceiver");
    RTS_current = 0;
    wait_cts_status = 0;
		state++;
	break;

	case 2: //send_rts
		tx.tx_send_RTS(RTS_length,RTS_max,RTS_current);
		state++;
	break;

	case 3: //wait_cts
		rx.rx_rec_CTS();
    //RECEIVE MULTIPLE VALUES, wait_cts_status & RTS_current
		// timer.rx_cts_timer();
		if(wait_cts_status == 1)			// if CTS timer expired
		{
			// increment current rts
      //Serial.write("%" PRIu8 "\n",RTS_current);
      RTS_current++;

				if(RTS_current == RTS_max)	// if rts is max and still no cts 
				{
					state = 7;     	//PROCEED TO FAIL CASE # 7
					Serial.println("Maximum RTS sent. No CTS signal received. Connection Aborted");
				}
				else
				{
					state = 2;
					 // go back to broadcast modde (send RTS)
					Serial.println("Timer Expired! Request to Retransmit CTS");
				}
		}
		else if(wait_cts_status == 2)
		{
			// if cts is received, proceed to send packet
			//timer.rx_cts_reset();
			// reset timer
			Serial.println("CTS Received, Ready to send Packet");
			state = 4;
		}
		else{
      Serial.println("For some reason, we are stuck in this loop.");
			state = 3;
		}
	break;

	case 4: //send_packet
		// tx mode
		uint8_t modbus_packet[5];

		tx.tx_send_Modbus(modbus_address,
		function_code, 
		modbus_data,
		crc_byte,
		modbus_packet);


		tx.tx_send_Packet(packet_length,
		packet_max,
		packet_current,
		packet_ack_counter,
		packet_ack_counter_max,
		modbus_packet); 

		packet_current++;                 
		state = 5;                                             
	break;
	
	case 5: //wait_packet_ack
		// rx packet timer            
		//timer.rx_packet_ack_timer();				
		rx.rx_rec_PacketAck(packet_current);	


		if(packet_ack_timer== 5){ // timer expired
		if(packet_ack_counter == packet_ack_counter_max){
		Serial.println("No acknowledgment received, Connection Terminated");
		state = 2;  
		}else{
		packet_ack_counter++;	  
		}
		}else if(ack_received == 1){  
		packet_current++;
		state = 4; 	  
		}else if(packet_current== packet_max){

		state = 6;

		}else{

		rx.rx_rec_PacketAck(packet_current);

		}
	break;

	case 6: //response_fully_received
		Serial.println("Message Succesfully Transmitted");
		state = 2;
	break;
	
	case 7: //response_fully_received
		Serial.println("Fail Handling Case");
		wait_cts_status == 0;
		RTS_current == 0;
		state = 1;
	break;
	
	default:
		state = 1;
	break;
	}
}
