#include <iostream>
#include <limits>

#include "controller-delay.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
DelayController::DelayController( const bool debug )
  :  Controller(debug), rtt_thresh(100), window_size_(5), 
 rtt_gain_(2), rtt_mean_(numeric_limits<int>::max()), rtt_var_(numeric_limits<int>::max()), rtt_min_(numeric_limits<int>::max())
{
}

/* Get current window size, in datagrams */
unsigned int DelayController::window_size( void )
{
  /* Default: fixed window size of 100 outstanding datagrams */
  unsigned int the_window_size = window_size_;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

/* A datagram was sent */
void DelayController::datagram_was_sent( const uint64_t sequence_number,
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
void DelayController::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  uint64_t delay = timestamp_ack_received - send_timestamp_acked;
  int64_t m = timestamp_ack_received - send_timestamp_acked;

  if(rtt_min_ > delay){
    rtt_min_ = delay;
  }

  if(rtt_mean_ == numeric_limits<int>::max()){
    rtt_mean_ = m;
    rtt_var_ = 0;
  } 
  else {
    m -= (rtt_mean_ >> rtt_gain_);
    rtt_mean_ += m;
    if(m<0){
      m = -m;
    }
    m -= (rtt_var_ >> rtt_gain_);
    rtt_var_ += m;
  }

  if(delay > (3*rtt_min_)){
    window_size_ = (3*window_size_)/4;
    if(window_size_ == 0){
      window_size_ = 1;
    }
  } else if (delay <= rtt_thresh * 0.8){
    window_size_ += 1;
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
unsigned int DelayController::timeout_ms( void )
{
  return rtt_mean_; /* timeout of one second */
}
