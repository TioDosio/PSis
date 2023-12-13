/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: balls.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "balls.pb-c.h"
void   ball_draw_display_msg__init
                     (BallDrawDisplayMsg         *message)
{
  static BallDrawDisplayMsg init_value = BALL_DRAW_DISPLAY_MSG__INIT;
  *message = init_value;
}
size_t ball_draw_display_msg__get_packed_size
                     (const BallDrawDisplayMsg *message)
{
  assert(message->base.descriptor == &ball_draw_display_msg__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t ball_draw_display_msg__pack
                     (const BallDrawDisplayMsg *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &ball_draw_display_msg__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t ball_draw_display_msg__pack_to_buffer
                     (const BallDrawDisplayMsg *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &ball_draw_display_msg__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
BallDrawDisplayMsg *
       ball_draw_display_msg__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (BallDrawDisplayMsg *)
     protobuf_c_message_unpack (&ball_draw_display_msg__descriptor,
                                allocator, len, data);
}
void   ball_draw_display_msg__free_unpacked
                     (BallDrawDisplayMsg *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &ball_draw_display_msg__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   payperview_req__init
                     (PayperviewReq         *message)
{
  static PayperviewReq init_value = PAYPERVIEW_REQ__INIT;
  *message = init_value;
}
size_t payperview_req__get_packed_size
                     (const PayperviewReq *message)
{
  assert(message->base.descriptor == &payperview_req__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t payperview_req__pack
                     (const PayperviewReq *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &payperview_req__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t payperview_req__pack_to_buffer
                     (const PayperviewReq *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &payperview_req__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
PayperviewReq *
       payperview_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (PayperviewReq *)
     protobuf_c_message_unpack (&payperview_req__descriptor,
                                allocator, len, data);
}
void   payperview_req__free_unpacked
                     (PayperviewReq *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &payperview_req__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   payperview_resp__init
                     (PayperviewResp         *message)
{
  static PayperviewResp init_value = PAYPERVIEW_RESP__INIT;
  *message = init_value;
}
size_t payperview_resp__get_packed_size
                     (const PayperviewResp *message)
{
  assert(message->base.descriptor == &payperview_resp__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t payperview_resp__pack
                     (const PayperviewResp *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &payperview_resp__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t payperview_resp__pack_to_buffer
                     (const PayperviewResp *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &payperview_resp__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
PayperviewResp *
       payperview_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (PayperviewResp *)
     protobuf_c_message_unpack (&payperview_resp__descriptor,
                                allocator, len, data);
}
void   payperview_resp__free_unpacked
                     (PayperviewResp *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &payperview_resp__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor ball_draw_display_msg__field_descriptors[3] =
{
  {
    "ch",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_BYTES,
    0,   /* quantifier_offset */
    offsetof(BallDrawDisplayMsg, ch),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "x",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(BallDrawDisplayMsg, x),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "y",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(BallDrawDisplayMsg, y),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned ball_draw_display_msg__field_indices_by_name[] = {
  0,   /* field[0] = ch */
  1,   /* field[1] = x */
  2,   /* field[2] = y */
};
static const ProtobufCIntRange ball_draw_display_msg__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor ball_draw_display_msg__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ball_draw_display_msg",
  "BallDrawDisplayMsg",
  "BallDrawDisplayMsg",
  "",
  sizeof(BallDrawDisplayMsg),
  3,
  ball_draw_display_msg__field_descriptors,
  ball_draw_display_msg__field_indices_by_name,
  1,  ball_draw_display_msg__number_ranges,
  (ProtobufCMessageInit) ball_draw_display_msg__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor payperview_req__field_descriptors[2] =
{
  {
    "subscriber_name",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(PayperviewReq, subscriber_name),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "creditcard_number",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(PayperviewReq, creditcard_number),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned payperview_req__field_indices_by_name[] = {
  1,   /* field[1] = creditcard_number */
  0,   /* field[0] = subscriber_name */
};
static const ProtobufCIntRange payperview_req__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor payperview_req__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "payperview_req",
  "PayperviewReq",
  "PayperviewReq",
  "",
  sizeof(PayperviewReq),
  2,
  payperview_req__field_descriptors,
  payperview_req__field_indices_by_name,
  1,  payperview_req__number_ranges,
  (ProtobufCMessageInit) payperview_req__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor payperview_resp__field_descriptors[1] =
{
  {
    "random_secret",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(PayperviewResp, random_secret),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned payperview_resp__field_indices_by_name[] = {
  0,   /* field[0] = random_secret */
};
static const ProtobufCIntRange payperview_resp__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor payperview_resp__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "payperview_resp",
  "PayperviewResp",
  "PayperviewResp",
  "",
  sizeof(PayperviewResp),
  1,
  payperview_resp__field_descriptors,
  payperview_resp__field_indices_by_name,
  1,  payperview_resp__number_ranges,
  (ProtobufCMessageInit) payperview_resp__init,
  NULL,NULL,NULL    /* reserved[123] */
};
