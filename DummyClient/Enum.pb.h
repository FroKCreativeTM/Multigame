// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Enum.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Enum_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Enum_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Enum_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

enum MsgId : int {
  S_Enter_Game = 0,
  S_Leave_Game = 1,
  S_Spawn = 2,
  S_Despawn = 3,
  C_Move = 4,
  S_Move = 5,
  C_Skill = 6,
  S_Skill = 7,
  S_Change_HP = 8,
  S_Die = 9,
  MsgId_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  MsgId_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool MsgId_IsValid(int value);
constexpr MsgId MsgId_MIN = S_Enter_Game;
constexpr MsgId MsgId_MAX = S_Die;
constexpr int MsgId_ARRAYSIZE = MsgId_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MsgId_descriptor();
template<typename T>
inline const std::string& MsgId_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MsgId>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MsgId_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MsgId_descriptor(), enum_t_value);
}
inline bool MsgId_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MsgId* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MsgId>(
    MsgId_descriptor(), name, value);
}
enum CreatureState : int {
  IDLE = 0,
  MOVING = 1,
  SKILL = 2,
  DEAD = 3,
  CreatureState_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  CreatureState_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool CreatureState_IsValid(int value);
constexpr CreatureState CreatureState_MIN = IDLE;
constexpr CreatureState CreatureState_MAX = DEAD;
constexpr int CreatureState_ARRAYSIZE = CreatureState_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* CreatureState_descriptor();
template<typename T>
inline const std::string& CreatureState_Name(T enum_t_value) {
  static_assert(::std::is_same<T, CreatureState>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function CreatureState_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    CreatureState_descriptor(), enum_t_value);
}
inline bool CreatureState_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, CreatureState* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<CreatureState>(
    CreatureState_descriptor(), name, value);
}
enum MoveDir : int {
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3,
  MoveDir_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  MoveDir_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool MoveDir_IsValid(int value);
constexpr MoveDir MoveDir_MIN = UP;
constexpr MoveDir MoveDir_MAX = RIGHT;
constexpr int MoveDir_ARRAYSIZE = MoveDir_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MoveDir_descriptor();
template<typename T>
inline const std::string& MoveDir_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MoveDir>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MoveDir_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MoveDir_descriptor(), enum_t_value);
}
inline bool MoveDir_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MoveDir* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MoveDir>(
    MoveDir_descriptor(), name, value);
}
enum GameObjectType : int {
  NONE = 0,
  PLAYER = 1,
  MONSTER = 2,
  PROJECTILE = 3,
  GameObjectType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  GameObjectType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool GameObjectType_IsValid(int value);
constexpr GameObjectType GameObjectType_MIN = NONE;
constexpr GameObjectType GameObjectType_MAX = PROJECTILE;
constexpr int GameObjectType_ARRAYSIZE = GameObjectType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* GameObjectType_descriptor();
template<typename T>
inline const std::string& GameObjectType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, GameObjectType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function GameObjectType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    GameObjectType_descriptor(), enum_t_value);
}
inline bool GameObjectType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, GameObjectType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<GameObjectType>(
    GameObjectType_descriptor(), name, value);
}
enum SkillType : int {
  SKILL_NONE = 0,
  SKILL_AUTO = 1,
  SKILL_PROJECTILE = 2,
  SkillType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  SkillType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool SkillType_IsValid(int value);
constexpr SkillType SkillType_MIN = SKILL_NONE;
constexpr SkillType SkillType_MAX = SKILL_PROJECTILE;
constexpr int SkillType_ARRAYSIZE = SkillType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* SkillType_descriptor();
template<typename T>
inline const std::string& SkillType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, SkillType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function SkillType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    SkillType_descriptor(), enum_t_value);
}
inline bool SkillType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, SkillType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<SkillType>(
    SkillType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::Protocol::MsgId> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::MsgId>() {
  return ::Protocol::MsgId_descriptor();
}
template <> struct is_proto_enum< ::Protocol::CreatureState> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::CreatureState>() {
  return ::Protocol::CreatureState_descriptor();
}
template <> struct is_proto_enum< ::Protocol::MoveDir> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::MoveDir>() {
  return ::Protocol::MoveDir_descriptor();
}
template <> struct is_proto_enum< ::Protocol::GameObjectType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::GameObjectType>() {
  return ::Protocol::GameObjectType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::SkillType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::SkillType>() {
  return ::Protocol::SkillType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
