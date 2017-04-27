




/* Title: Slave Device 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */






#include <TX_SEND.h>

#include <PROTOCOL_TIMER.h>


// Defining RTS Parameters
#define rts_length 4
#define rts_max 3

//
#define 



PROTOCOL_TIMER timer;
TX_SEND tx;
RX_REC rx;



void setup() {
  // put your setup code here, to run once:
  volatile unsigned long z;
  int state;
  uint8_t rts_current = 1;
}

void loop() {

  switch (state)
{
  case 1:        //listen
    Serial.begin(9600);
    Serial.println("Receiver Mode");
    Serial.println("Initializing Master");
    state = 2;
    
    
  case 2: //wait_rts
   /*rx.rx_rec_RTS();
   if(
    state++;
  */
        timer.rx_rts_timer();
        unsigned long  z = rx.rx_rts_timer();
        rx.rx_rec_RTS();
        int rts_received = rx.rx_rec_RTS();

        if(z == 5)                                                                     // if RTS timer expired
        {
                                                                          
          Serial.println("Timer Expired! No RTS Received");   
             state = 2;                                                                  // request to send RTS      
        }else if(rts_received == 1){                                                  // if rts is received, proceed to send CTS
            Serial.println("RTS Received, Ready to send CTS"):
            state = 3;
            timer.rx_rts_reset();                                                     // reset RTS timer
        }else{
        state = 2;
        }
        
  case 3: //send CTS
        tx.tx_send_CTS(uint8_t cts_length);
        state++;


  
        
  case 4: //wait packet
        
        timer.rx_packet_timer();
        unsigned long  z = rx.rx_packet_timer();
        rx.rx_rec_Packet();
        uint8_t packet_received = rx.rx_rec_Packet();

        if(z == 5)                                                                  // if Packet timer expired
        {
          Serial.println("Packet Timer Expired. No packet Received"); 
            state = 2;                                                                // Go back to listen mode (wait RTS)
             
        }else if(packet_received1 == 1){                                            // if Packet1 is received, proceed to send packet
             
            state = 5;        
        }else if(packet_received == 2){
            tx.tx_send_PacketAck(Packet_received); 
            state = 5
        }else{
          state = 4;
        }
      
    
  
   
  case 5: //send packetAck
        tx.tx_send_PacketAck(uint8_t packet_received);
        uint8_t current_ACK = tx.tx_send_PacketAck(uint8_t packet_received);
        
        if(current_ACK == 1){
          state = 4;
        }else{
          state = 6;
        }
            
  case 6: 

        Serial.println("Message Successfully Received!");
    
    
  default:1;
  
 
}
