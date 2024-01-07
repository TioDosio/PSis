/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: messages.proto */

#ifndef PROTOBUF_C_messages_2eproto__INCLUDED
#define PROTOBUF_C_messages_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Entity Entity;
typedef struct _ClientMessage ClientMessage;
typedef struct _ResponseMessage ResponseMessage;
typedef struct _DisplayUpdate DisplayUpdate;
typedef struct _ConnectDisplayResponse ConnectDisplayResponse;


/* --- enums --- */

typedef enum _EntityType {
  ENTITY_TYPE__LIZARD = 0,
  ENTITY_TYPE__ROACH = 1,
  ENTITY_TYPE__WASP = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(ENTITY_TYPE)
} EntityType;
typedef enum _Direction {
  DIRECTION__UP = 0,
  DIRECTION__DOWN = 1,
  DIRECTION__LEFT = 2,
  DIRECTION__RIGHT = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(DIRECTION)
} Direction;
typedef enum _MessageType {
  MESSAGE_TYPE__CONNECT = 0,
  MESSAGE_TYPE__MOVE = 1,
  MESSAGE_TYPE__DISCONNECT = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(MESSAGE_TYPE)
} MessageType;

/* --- messages --- */

struct  _Entity
{
  ProtobufCMessage base;
  EntityType entity_type;
  char *ch;
  uint32_t points;
  uint32_t pos_x;
  uint32_t pos_y;
  Direction direction;
  int32_t secret_code;
};
#define ENTITY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&entity__descriptor) \
    , 0, NULL, 0, 0, 0, 0, 0 }


struct  _ClientMessage
{
  ProtobufCMessage base;
  EntityType entity_type;
  MessageType msg_type;
  int32_t content;
  int32_t secret_code;
  int32_t client_code;
};
#define CLIENT_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&client_message__descriptor) \
    , 0, 0, 0, 0, 0 }


struct  _ResponseMessage
{
  ProtobufCMessage base;
  /*
   * 0 - fail, 1 - success, 2 - success in disconnection
   */
  uint32_t success;
  /*
   * score of the player
   */
  int32_t score;
  /*
   * secret id of entity
   */
  int32_t secret_code;
};
#define RESPONSE_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&response_message__descriptor) \
    , 0, 0, 0 }


struct  _DisplayUpdate
{
  ProtobufCMessage base;
  Entity *entity;
  /*
   * 0 - no disconnect, 1 - disconnect
   */
  uint32_t disconnect;
  /*
   * id of lizard bumped
   */
  int32_t id_l_bumped;
};
#define DISPLAY_UPDATE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&display_update__descriptor) \
    , NULL, 0, 0 }


struct  _ConnectDisplayResponse
{
  ProtobufCMessage base;
  size_t n_lizard;
  Entity **lizard;
  size_t n_npc;
  Entity **npc;
  int32_t n_lizards;
  int32_t n__npc;
  char *address_port;
};
#define CONNECT_DISPLAY_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&connect_display_response__descriptor) \
    , 0,NULL, 0,NULL, 0, 0, NULL }


/* Entity methods */
void   entity__init
                     (Entity         *message);
size_t entity__get_packed_size
                     (const Entity   *message);
size_t entity__pack
                     (const Entity   *message,
                      uint8_t             *out);
size_t entity__pack_to_buffer
                     (const Entity   *message,
                      ProtobufCBuffer     *buffer);
Entity *
       entity__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   entity__free_unpacked
                     (Entity *message,
                      ProtobufCAllocator *allocator);
/* ClientMessage methods */
void   client_message__init
                     (ClientMessage         *message);
size_t client_message__get_packed_size
                     (const ClientMessage   *message);
size_t client_message__pack
                     (const ClientMessage   *message,
                      uint8_t             *out);
size_t client_message__pack_to_buffer
                     (const ClientMessage   *message,
                      ProtobufCBuffer     *buffer);
ClientMessage *
       client_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   client_message__free_unpacked
                     (ClientMessage *message,
                      ProtobufCAllocator *allocator);
/* ResponseMessage methods */
void   response_message__init
                     (ResponseMessage         *message);
size_t response_message__get_packed_size
                     (const ResponseMessage   *message);
size_t response_message__pack
                     (const ResponseMessage   *message,
                      uint8_t             *out);
size_t response_message__pack_to_buffer
                     (const ResponseMessage   *message,
                      ProtobufCBuffer     *buffer);
ResponseMessage *
       response_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   response_message__free_unpacked
                     (ResponseMessage *message,
                      ProtobufCAllocator *allocator);
/* DisplayUpdate methods */
void   display_update__init
                     (DisplayUpdate         *message);
size_t display_update__get_packed_size
                     (const DisplayUpdate   *message);
size_t display_update__pack
                     (const DisplayUpdate   *message,
                      uint8_t             *out);
size_t display_update__pack_to_buffer
                     (const DisplayUpdate   *message,
                      ProtobufCBuffer     *buffer);
DisplayUpdate *
       display_update__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   display_update__free_unpacked
                     (DisplayUpdate *message,
                      ProtobufCAllocator *allocator);
/* ConnectDisplayResponse methods */
void   connect_display_response__init
                     (ConnectDisplayResponse         *message);
size_t connect_display_response__get_packed_size
                     (const ConnectDisplayResponse   *message);
size_t connect_display_response__pack
                     (const ConnectDisplayResponse   *message,
                      uint8_t             *out);
size_t connect_display_response__pack_to_buffer
                     (const ConnectDisplayResponse   *message,
                      ProtobufCBuffer     *buffer);
ConnectDisplayResponse *
       connect_display_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   connect_display_response__free_unpacked
                     (ConnectDisplayResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Entity_Closure)
                 (const Entity *message,
                  void *closure_data);
typedef void (*ClientMessage_Closure)
                 (const ClientMessage *message,
                  void *closure_data);
typedef void (*ResponseMessage_Closure)
                 (const ResponseMessage *message,
                  void *closure_data);
typedef void (*DisplayUpdate_Closure)
                 (const DisplayUpdate *message,
                  void *closure_data);
typedef void (*ConnectDisplayResponse_Closure)
                 (const ConnectDisplayResponse *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    entity_type__descriptor;
extern const ProtobufCEnumDescriptor    direction__descriptor;
extern const ProtobufCEnumDescriptor    message_type__descriptor;
extern const ProtobufCMessageDescriptor entity__descriptor;
extern const ProtobufCMessageDescriptor client_message__descriptor;
extern const ProtobufCMessageDescriptor response_message__descriptor;
extern const ProtobufCMessageDescriptor display_update__descriptor;
extern const ProtobufCMessageDescriptor connect_display_response__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_messages_2eproto__INCLUDED */
