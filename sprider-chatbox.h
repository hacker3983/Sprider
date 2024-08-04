#ifndef _SPRIDER_CHATBOX_H
#define _SPRIDER_CHATBOX_H
#include "sprider-chatapp.h"
void sprider_initialize_chatbox(sprider_t* chatbox);
void sprider_create_chatbox(sprider_t* chatbox);
void sprider_handle_chatbox_input(sprider_t* chatbox);
void sprider_destroy_chatbox_data(sprider_t* sprider);
#endif