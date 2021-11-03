// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: session.proto

#include "session.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace Tactile {
namespace Proto {
constexpr Session::Session(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : files_(){}
struct SessionDefaultTypeInternal {
  constexpr SessionDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~SessionDefaultTypeInternal() {}
  union {
    Session _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT SessionDefaultTypeInternal _Session_default_instance_;
}  // namespace Proto
}  // namespace Tactile
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_session_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_session_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_session_2eproto = nullptr;

const uint32_t TableStruct_session_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Tactile::Proto::Session, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Tactile::Proto::Session, files_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::Tactile::Proto::Session)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Tactile::Proto::_Session_default_instance_),
};

const char descriptor_table_protodef_session_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rsession.proto\022\rTactile.Proto\"\030\n\007Sessio"
  "n\022\r\n\005files\030\001 \003(\tb\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_session_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_session_2eproto = {
  false, false, 64, descriptor_table_protodef_session_2eproto, "session.proto", 
  &descriptor_table_session_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_session_2eproto::offsets,
  file_level_metadata_session_2eproto, file_level_enum_descriptors_session_2eproto, file_level_service_descriptors_session_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_session_2eproto_getter() {
  return &descriptor_table_session_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_session_2eproto(&descriptor_table_session_2eproto);
namespace Tactile {
namespace Proto {

// ===================================================================

class Session::_Internal {
 public:
};

Session::Session(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned),
  files_(arena) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:Tactile.Proto.Session)
}
Session::Session(const Session& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      files_(from.files_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:Tactile.Proto.Session)
}

inline void Session::SharedCtor() {
}

Session::~Session() {
  // @@protoc_insertion_point(destructor:Tactile.Proto.Session)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void Session::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void Session::ArenaDtor(void* object) {
  Session* _this = reinterpret_cast< Session* >(object);
  (void)_this;
}
void Session::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Session::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Session::Clear() {
// @@protoc_insertion_point(message_clear_start:Tactile.Proto.Session)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  files_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Session::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // repeated string files = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_files();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "Tactile.Proto.Session.files"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Session::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Tactile.Proto.Session)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated string files = 1;
  for (int i = 0, n = this->_internal_files_size(); i < n; i++) {
    const auto& s = this->_internal_files(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "Tactile.Proto.Session.files");
    target = stream->WriteString(1, s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Tactile.Proto.Session)
  return target;
}

size_t Session::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Tactile.Proto.Session)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated string files = 1;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(files_.size());
  for (int i = 0, n = files_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      files_.Get(i));
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Session::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    Session::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Session::GetClassData() const { return &_class_data_; }

void Session::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<Session *>(to)->MergeFrom(
      static_cast<const Session &>(from));
}


void Session::MergeFrom(const Session& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Tactile.Proto.Session)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  files_.MergeFrom(from.files_);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Session::CopyFrom(const Session& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Tactile.Proto.Session)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Session::IsInitialized() const {
  return true;
}

void Session::InternalSwap(Session* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  files_.InternalSwap(&other->files_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Session::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_session_2eproto_getter, &descriptor_table_session_2eproto_once,
      file_level_metadata_session_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace Proto
}  // namespace Tactile
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::Tactile::Proto::Session* Arena::CreateMaybeMessage< ::Tactile::Proto::Session >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Tactile::Proto::Session >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
