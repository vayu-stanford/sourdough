#ifndef CONTROLLER_DELAY_HH
#define CONTROLLER_DELAY_HH

#include <cstdint>
#include "controller.hh"

/* Congestion controller interface */

class DelayController : public Controller
{
private:
  uint64_t window_size_;
  uint64_t window_size_incr_;
  uint64_t rtt_gain_;
  uint64_t rtt_mean_;
  uint64_t rtt_var_;
  uint64_t rtt_min_;
  uint64_t rto_;
  double upper_thresh_;
  double lower_thresh_;
public:
  /* Public interface for the congestion controller */
  /* You can change these if you prefer, but will need to change
     the call site as well (in sender.cc) */

  DelayController( const bool debug );

  /* Get current window size, in datagrams */
  unsigned int window_size( void ) override;

  /* A datagram was sent */
  void datagram_was_sent( const uint64_t sequence_number,
			  const uint64_t send_timestamp ) override;

  /* An ack was received */
  void ack_received( const uint64_t sequence_number_acked,
		     const uint64_t send_timestamp_acked,
		     const uint64_t recv_timestamp_acked,
		     const uint64_t timestamp_ack_received ) override;

  /* How long to wait (in milliseconds) if there are no acks
     before sending one more datagram */
  unsigned int timeout_ms( void ) override;

};

#endif
