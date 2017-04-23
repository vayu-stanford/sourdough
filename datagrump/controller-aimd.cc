#include <iostream>
#include <limits>

#include "controller-aimd.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
AIMDController::AIMDController( const bool debug )
  :  Controller(debug), cwnd_(1), cwnd_incr_(0), ssthresh_(numeric_limits<int>::max()),  rtt_gain_(3), rtt_mean_(numeric_limits<int>::max()), rtt_var_(numeric_limits<int>::max()), rto_(1000), timeout_ctr_(0) , last_sent_(0)
{
}

/* Get current window size, in datagrams */
unsigned int AIMDController::window_size( void )
{
  /* Default: fixed window size of 100 outstanding datagrams */
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
  /* Timeout occurred */
  if(send_timestamp - last_sent_ > 2*rtt_mean_){
    cwnd_ = cwnd_ / 2;
    if(cwnd_ == 0){
      cwnd_ = 1;
    }
    ssthresh_ = cwnd_;
  } 
  last_sent_ = send_timestamp;

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
  /* Update cwnd and ssthresh */
  if(false && cwnd_ <= ssthresh_){
    cwnd_ += 1;
  } else {
    cwnd_incr_ += 1;
    if(cwnd_incr_ == cwnd_){
      cwnd_ += 1;
      cwnd_incr_ = 0;
    }
  }

  /* Update RTT estimates */
  int64_t m = send_timestamp_acked - recv_timestamp_acked;
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
  return rtt_mean_; /* timeout of one second */
}
