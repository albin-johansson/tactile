#include "protobuf_context.hpp"

#include <google/protobuf/stubs/common.h>

namespace tactile {

ProtobufContext::ProtobufContext()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

ProtobufContext::~ProtobufContext()
{
  google::protobuf::ShutdownProtobufLibrary();
}

}  // namespace tactile