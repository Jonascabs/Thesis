#include <PROTOCOL_TIMER.h>

#include <RX_REC.h>

#include <TX_SEND.h>



// Defining RTS Parameters
#define rts_length 0x04
#define rts_max 0x03

// Defining Packet Paramaters
#define packet_length 0x09
#define packet_max 0x02
#define packet_ack_counter_max 0x03

// Defining Timer

     int state=1;      
     
//PROTOCOL_TIMER timer;
TX_SEND tx;
RX_REC rx;


   uint8_t modbus_address = 0x01;

  uint8_t function_code = 0x02;
  uint8_t modbus_data = 0x03;
  uint16_t crc_byte = 0xF0;
  
  
  uint8_t rts_current = 1;
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






  
}

void loop() {

  switch (state)
{
  case 1: //idle
    
    Serial.println("Receiver Mode");
    Serial.println("Initializing Master");
    state++;
    
    
  case 2: //send_rts
    tx.tx_send_RTS(rts_length,rts_max,rts_current);
    state++;
  
  case 3: //wait_cts
   
     rx.rx_rec_CTS();
    // timer.rx_cts_timer();
     


        
        if(cts_timer == 5)                                                                   // if CTS timer expired
        {
          state = 2;                                                            // request to send RTS
          rts_current++;                                                             // increment current rts
		      Serial.println("Timer Expired! Request to Retransmit CTS");		  
            if(rts_current == rts_max)                                               // if rts is max and still no cts 
            {
            state = 2;                                                               // go back to broadcast modde (send RTS)
            Serial.println("Maximum RTS sent. No CTS signal received. Connection Aborted");
            }else{
            state = 3;
            }
        }else if(cts_received == 1){                                                   // if cts is received, proceed to send packet
          
            //timer.rx_cts_reset();                                                      // reset timer
		        Serial.println("CTS Received, Ready to send Packet");
            state = 4;
                                                              
        }else{
        state = 3;
        }
        
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
  
  case 5: //wait_packet_ack
    
	    
	             			                                    // rx packet timer            
      //timer.rx_packet_ack_timer();				
	    rx.rx_rec_PacketAck(packet_current);	
	
	    
    if(packet_ack_timer== 5){                 				                          // timer expired
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
	
      
  case 6: //response_fully_received
    Serial.println("Message Succesfully Transmitted");
    state = 2;
    
  default:
    state = 2;
  
}
}
