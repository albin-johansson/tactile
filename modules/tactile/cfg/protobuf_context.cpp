#include "protobuf_context.hpp"

#include <google/protobuf/stubs/common.h>

namespace Tactile {

ProtobufContext::ProtobufContext()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

ProtobufContext::~ProtobufContext()
{
  google::protobuf::ShutdownProtobufLibrary();
}

}  // namespace Tactile