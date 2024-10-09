#include <data_structures/matrix.h>
#include <messages/odometry_message.h>
#include <messages/pose_message.h>
#include <utils/deserializer.h>
#include <utils/identifiable.h>
#include <utils/object_data.h>
#include <utils/property_container_manager.h>
#include <utils/serialization_context.h>

#include <iostream>

using namespace mini_core;

int main(int argc_, char** argv_) {
  PoseMessage pose_msg;
  OdometryMessage odom_msg;
  Deserializer deserializer;
  Identifiable identifiable;
  BoolData data(true);
  DynamicLoaderConfig dynloader;
  SerializationContext ctx;
  MatrixInt m;

  std::cout << "Congratulations, you solved this homework!" << std::endl;
  return 0;
}