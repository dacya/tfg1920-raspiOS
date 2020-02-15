/**
 * Library to communicate with the pi 2 and up hardware
 * 
 * Tomás Golomb Durán - Dec. 05, 2019
 */

#ifndef _MAILBOX_H
#define _MAILBOX_H

#include <stdint.h>
#include <io/base.h>
#include <io/framebuffer.h>

#define MAILBOX_BASE PHYSICAL_PBASE + MAILBOX_OFFSET
#define MAIL0_READ (((mailbox_message_t *)(0x00 + MAILBOX_BASE)))
#define MAIL0_STATUS (((mailbox_status_t *)(0x18 + MAILBOX_BASE)))
#define MAIL0_WRITE (((mailbox_message_t *)(0x20 + MAILBOX_BASE)))

// When sending a message the request code must be 0
#define MAILBOX_REQUEST_CODE ((uint32_t) 0)
#define MAILBOX_RESPONSE_SUCCESS ((uint32_t) 0x80000000)
#define MAILBOX_RESPONSE_ERROR   ((uint32_t) 0x80000001)

// Channel types (there are more tho)
typedef enum {
    MAILBOX_PROPERTY_CHANNEL = 8,
    MAILBOX_FRAMEBUFFER_CHANNEL = 1
} mailbox_channel_t;

typedef struct {
    uint8_t channel: 4;
    uint32_t data: 28;
} mailbox_message_t;

typedef struct {
    uint32_t reserved: 30;
    uint8_t empty: 1;
    uint8_t full: 1;
} mailbox_status_t;

typedef struct {
    uint32_t size;
    uint32_t req_res_code;
    uint32_t tags[1]; /* just the padding, it will be filled with kmalloc */
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
    void* fb_addr;
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
 * A message can contain any number of these.
 */
typedef struct {
    property_tag_t proptag;
    value_buffer_t value_buffer;
} property_message_tag_t;

/**
 * Sends a message through a mailbox channel
 * and overrides the message with the response.
 * 
 * @param msg null ended array of tags
 * @param channel the channel number where the message will be sent through
 * @return  0 if the message has been sent succesfully and the response is filled in the @param msg
 *         -1 if kmalloc failed to reserve 16 aligned bytes
 *         -2 if the message hasn't been send correctly and was cancelled
 *         -3 if a RESPONSE_ERROR has ocurred 
 * @see property_message_tag_t
 * @see mailbox_channel_t
 */
int send_message(property_message_tag_t* msg, mailbox_channel_t channel);

#endif /* _MAILBOX_H */