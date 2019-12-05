#include <io/mailbox.h>

int send_messages(property_message_tag_t * tags) {
    property_message_buffer_t * msg;
    mail_message_t mail;
    uint32_t bufsize = 0, i, len, bufpos;
   
    // Calculate the sizes of each tag
    for (i = 0; tags[i].proptag != NULL_TAG; i++) {
        bufsize += get_value_buffer_len(&tags[i]) + 3*sizeof(uint32_t);          
    }

    // Add the buffer size, buffer request/response code and buffer end tag sizes
    bufsize += 3*sizeof(uint32_t); 

    // buffer size must be 16 byte aligned
    bufsize += (bufsize % 16) ? 16 - (bufsize % 16) : 0;

    // kmalloc returns a 16 byte aligned address
    msg = kmalloc(bufsize);
    if (!msg)
        return -1;

    msg->size = bufsize;
    msg->req_res_code = REQUEST;

    // Copy the messages into the buffer
    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++) {
        len = get_value_buffer_len(&tags[i]);
        msg->tags[bufpos++] = tags[i].proptag;
        msg->tags[bufpos++] = len;
        msg->tags[bufpos++] = 0;
        memcpy(msg->tags+bufpos, &tags[i].value_buffer, len);
        bufpos += len/4;
    }

    msg->tags[bufpos] = 0;

    // Send the message
    mail.data = ((uint32_t)msg) >>4;
    
    mailbox_send(mail, PROPERTY_CHANNEL);
    mail = mailbox_read(PROPERTY_CHANNEL);


    if (msg->req_res_code == REQUEST) {
        kfree(msg);
        return 1;
    } else if (msg->req_res_code == RESPONSE_ERROR) {
        kfree(msg);
        return 2;
    }

    
    // Copy the tags back into the array
    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++) {
        len = get_value_buffer_len(&tags[i]);
        bufpos += 3; //skip over the tag bookkepping info
        memcpy(&tags[i].value_buffer, msg->tags+bufpos,len);
        bufpos += len/4;
    }

    kfree(msg);
    return 0;
}