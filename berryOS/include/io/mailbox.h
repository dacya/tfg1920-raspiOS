/**
 * Library to communicate with the pi 2 and up hardware
 * 
 * Tomás Golomb Durán - Dec. 05, 2019
 */

#ifndef _MAILBOX_H
#define _MAILBOX_H

#include <stdint.h>

#define MAILBOX_RESPONSE_SUCCESS ((uint32_t) 0x80000000)
#define MAILBOX_RESPONSE_ERROR   ((uint32_t) 0x80000001)

typedef struct {
    uint8_t channel: 4;
    uint32_t data: 28;
} mail_message_t;

typedef struct {
    uint32_t reserved: 30;
    uint8_t empty: 1;
    uint8_t full:1;
} mail_status_t;

typedef struct {
    uint32_t size;
    uint32_t req_res_code;
    property_message_tag_t tags[];
} property_message_buffer_t;


typedef enum {
    NULL_TAG = 0,
    FB_ALLOCATE_BUFFER = 0x00040001,
    FB_RELESE_BUFFER = 0x00048001,
    FB_GET_PHYSICAL_DIMENSIONS = 0x00040003,
    FB_SET_PHYSICAL_DIMENSIONS = 0x00048003,
    FB_GET_VIRTUAL_DIMENSIONS = 0x00040004,
    FB_SET_VIRTUAL_DIMENSIONS = 0x00048004,
    FB_GET_BITS_PER_PIXEL = 0x00040005,
    FB_SET_BITS_PER_PIXEL = 0x00048005,
    FB_GET_BYTES_PER_ROW = 0x00040008
} property_tag_t;

typedef struct {
    void * fb_addr;
    uint32_t fb_size;
} fb_allocate_res_t;

typedef struct {
    uint32_t width;
    uint32_t height;
} fb_screen_size_t;

/*
 * The value buffer can be any one of these types
 */
typedef union {
    uint32_t fb_allocate_align;
    fb_allocate_res_t fb_allocate_res;
    fb_screen_size_t fb_screen_size;
    uint32_t fb_bits_per_pixel;
    uint32_t fb_bytes_per_row;
} value_buffer_t;

/*
 * A message_buffer can contain any number of these
 */
typedef struct {
    property_tag_t proptag;
    value_buffer_t value_buffer;
} property_message_tag_t;

/**
 * Get the size for a mailbox message type as
 * defined in the protocol.
 * 
 * @param tag the tag to get the size of
 * @return bytes of a mailbox message type
 * @see propery_message_tag_t
 */
static uint32_t get_value_buffer_len(property_message_tag_t* tag) {
    switch(tag->proptag) {
        case FB_ALLOCATE_BUFFER: 
        case FB_GET_PHYSICAL_DIMENSIONS:
        case FB_SET_PHYSICAL_DIMENSIONS:
        case FB_GET_VIRTUAL_DIMENSIONS:
        case FB_SET_VIRTUAL_DIMENSIONS:
            return 8;
        case FB_GET_BITS_PER_PIXEL:
        case FB_SET_BITS_PER_PIXEL:
        case FB_GET_BYTES_PER_ROW:
            return 4;
        case FB_RELESE_BUFFER:
        default:
            return 0;
    }
}

/**
 * Sends a message through the property mailbox channel
 * 
 * @param tags null ended array of tags
 * @return 0 if the message has been sent succesfully.
 *         -1 if kmalloc failed to reserve 16 aligned bytes
 *         1 if ?
 *         2 if the 
 */
int send_messages(property_message_tag_t * tags);


#endif /* _MAILBOX_H */