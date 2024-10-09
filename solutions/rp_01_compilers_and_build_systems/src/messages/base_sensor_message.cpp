#include "base_sensor_message.h"

namespace mini_core {
using namespace geometry3d;

BaseSensorMessage::BaseSensorMessage(const std::string& topic_,
                                     const std::string& frame_id_, int seq_,
                                     const double& timestamp_)
    : SETUP_PROPERTY(topic, topic_),
      SETUP_PROPERTY(frame_id, frame_id_),
      SETUP_PROPERTY(seq, seq_),
      SETUP_PROPERTY(timestamp, timestamp_) {}

} /* namespace mini_core */
