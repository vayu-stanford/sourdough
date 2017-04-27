#include <iostream>
#include <limits>

#include "controller-aimd.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
AIMDController::AIMDController( const bool debug )
  :  Controller(debug), cwnd_(1), cwnd_incr_(0)
{
}

/* Get current window size, in datagrams */
unsigned int AIMDController::window_size( void )
{
  unsigned int the_window_size = cwnd_;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

/* A datagram was sent */
void AIMDController::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << endl;
  }
}

/* An ack was received */
void AIMDController::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Update cwnd */
  if(timestamp_ack_received - send_timestamp_acked > timeout_ms()){
    if ( debug_ ) {
      cerr << "Timeout occurred for packet " << sequence_number_acked << endl;
    }
    cwnd_ = cwnd_ / 2;
    if(cwnd_ == 0){
      cwnd_ = 1;
    }
    cwnd_incr_ = 0;
  } 
  else{
    cwnd_incr_ += 1;
    if(cwnd_incr_ >= cwnd_){
      cwnd_ += 1;
      cwnd_incr_ = 0;
    }
  }

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int AIMDController::timeout_ms( void )
{
  return 1000;
}
