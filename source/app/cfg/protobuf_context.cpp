#include "protobuf_context.hpp"

#include <google/protobuf/stubs/common.h>

namespace tactile {

protobuf_context::protobuf_context()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

protobuf_context::~protobuf_context()
{
  google::protobuf::ShutdownProtobufLibrary();
}

}  // namespace tactile