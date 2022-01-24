#pragma once

namespace tactile {

/**
 * \brief Handles the loading and unloading of the Protobuf library.
 *
 * \ingroup cfg
 */
class protobuf_context final {
 public:
  protobuf_context();

  ~protobuf_context();
};

}  // namespace tactile
