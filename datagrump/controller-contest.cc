#include <iostream>
#include <limits>
#include <math.h>

#include "controller-contest.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
ContestController::ContestController( const bool debug )
  :  Controller(debug), rtt_thresh(100), window_size_(5), 
 rtt_gain_(1), rtt_mean_(numeric_limits<int>::max()), rtt_var_(numeric_limits<int>::max()), rtt_min_(numeric_limits<int>::max()), consec_delays_(0), consec_no_delays_(0), rtt_decreased_(false),
  delay_cooloff_(0), window_incr_(0)
{
}

/* Get current window size, in datagrams */
unsigned int ContestController::window_size( void )
{
  unsigned int the_window_size = window_size_;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

/* A datagram was sent */
void ContestController::datagram_was_sent( const uint64_t sequence_number,
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
void ContestController::ack_received( const uint64_t sequence_number_acked,
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
    rtt_mean_ = (rtt_mean_*8+delay*2)/10;
  }

  if(delay > (4.5*rtt_min_)){
    consec_delays_+=1;
    if(consec_no_delays_ > 8){
      if(window_size_ > consec_no_delays_/8){
        window_size_ -= consec_no_delays_/8;
      } else {
        window_size_ = 1;
      }
      delay_cooloff_ = window_size_/4;
    } else if (delay_cooloff_ != 0 && delay < 7*rtt_min_){
      delay_cooloff_ -= 1;
      window_size_ -= 1;
    } else {
      uint64_t log_consec = log(consec_delays_);
      if(log_consec > 100){
        log_consec = 100;
      }
      //window_size_ -= 2.0*rtt_min_/delay*(log_consec)/100.0;
      window_size_ *= ((100.0-log_consec))/100.0;
      delay_cooloff_ = 0;
    }
    consec_no_delays_=0;
  }  else {
    consec_delays_ = 0;
    delay_cooloff_ = 0;
  }

  if (9* delay <= rtt_min_ * 10 ){
    window_size_ += 1;
    consec_no_delays_ += 1;
  }  

  if(sequence_number_acked % 10 == 0 && window_size_ > 1.2*delay/rtt_min_){
    window_size_ -= delay/rtt_min_;
  } 

  if(window_size_ == 0){
    window_size_ = 1;
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
unsigned int ContestController::timeout_ms( void )
{
  return rtt_mean_; /* timeout of one second */
}
