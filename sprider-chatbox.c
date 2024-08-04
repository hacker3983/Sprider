#include "sprider-chatapp.h"
#include "sprider-chatbox.h"
#include "sprider-string.h"

#define DEFAULT_CHATBOX_CURSORWIDTH 1
#define DEFAULT_CHATBOX_PLACEHOLDER "Enter a message here..."
SDL_Color DEFAULT_CHATBOX_COLOR = {0x00, 0x00, 0x00, 0xff};
SDL_Color DEFAULT_CHATBOX_CURSORCOLOR = {0x00, 0xff, 0x00, 0x40};
/*
   * Default Color Palettes for chatbox area:
   * https://coolors.co/090809-f40000-f44e3f-f4796b-f4998d
   * https://coolors.co/03b5aa-037971-023436-00bfb3-049a8f
*/
SDL_Color DEFAULT_CHATBOX_TEXTCOLOR = {0x45, 0xF0, 0xDF, 0xeb};
SDL_Color DEFAULT_CHATBOX_PLACEHOLDERCOLOR = {0x45, 0xF0, 0xDF, 0x80};

void sprider_initialize_chatbox(sprider_t* sprider) {
    sprider->chatbox.outer_canvas_color = DEFAULT_CHATBOX_COLOR;
    sprider->chatbox.input_textcolor = DEFAULT_CHATBOX_TEXTCOLOR;
    sprider->chatbox.placeholder_textcolor = DEFAULT_CHATBOX_PLACEHOLDERCOLOR;
    sprider->chatbox.cursor_color = DEFAULT_CHATBOX_CURSORCOLOR;
    sprider->chatbox.cursor.w = DEFAULT_CHATBOX_CURSORWIDTH;
}

void sprider_handle_chatbox_input(sprider_t* sprider) {
    sprider_chatbox_t *chatbox = &sprider->chatbox;
    switch(sprider->e.type) {
        case SDL_TEXTINPUT:
            int utf8_input_bytesize = 0;
            int utf8_relative_inputsize = strlen(sprider->e.text.text), index = 0;
            char* current_utf8char = NULL;
            // Parse out any emojis or multibyte sequence characters that could be input using one single buffer
            // example in the windows operating system windows plus. key would allow you to input multiple emojis at
            // once
            if(utf8_relative_inputsize > 1) {
                while(index < utf8_relative_inputsize) {
                    current_utf8char = calloc(utf8_input_bytesize+1, sizeof(char));
                    chatbox->datalist_count++;
                    chatbox->datalist_cursor++;
                    chatbox->datalist = (!chatbox->datalist) ? calloc(chatbox->datalist_count,
                                                                      sizeof(sprider_chatbox_data_t))
                                                             : realloc(chatbox->datalist, chatbox->datalist_count *
                                                                      sizeof(sprider_chatbox_data_t));
                    utf8_input_bytesize = sprider_string_getutf8char_len(sprider->e.text.text[index]);
                    strncpy(current_utf8char, sprider->e.text.text + index, utf8_input_bytesize);
                    chatbox->datalist[chatbox->datalist_count-1].data = current_utf8char;
                    chatbox->datalist[chatbox->datalist_count-1].datalen = utf8_input_bytesize;
                    chatbox->datalist[chatbox->datalist_count-1].cursor_pos++;
                    printf("current_utf8char is %s, datalist_count is %ld\n", current_utf8char,
                        chatbox->datalist_count);
                    index += utf8_input_bytesize;
                }
                break;
            }
            // determine whether the current datalist index is an ascii utf8 char
            current_utf8char = calloc(utf8_relative_inputsize+1, sizeof(char));
            strncpy(current_utf8char, sprider->e.text.text, utf8_relative_inputsize);
            chatbox->datalist_count++;
            chatbox->datalist = (!chatbox->datalist) ? calloc(chatbox->datalist_count, sizeof(sprider_chatbox_data_t))
                                                     : realloc(chatbox->datalist, chatbox->datalist_count *
                                                            sizeof(sprider_chatbox_data_t));
            chatbox->datalist[chatbox->datalist_count-1].data = current_utf8char;
            chatbox->datalist[chatbox->datalist_count-1].datalen = utf8_relative_inputsize;
            chatbox->total_inputlen += utf8_relative_inputsize;
            break;
        case SDL_KEYDOWN: {
            switch(sprider->e.key.keysym.sym) {
                case SDLK_LEFT:
                    break;
                case SDLK_RIGHT:
                    break;
                case SDLK_RETURN:
                    if(chatbox->datalist_count > 0) {
                        //printf("The user pressed enter on their keyboard to send a message from the chatbox\n");
                        char* message = calloc(chatbox->total_inputlen+1, sizeof(char));
                        for(size_t i=0;i<chatbox->datalist_count;i++) {
                            strncat(message, chatbox->datalist[i].data, chatbox->datalist[i].datalen);
                        }
                        sprider_stringlist_append(&sprider->message_list, message);
                        sprider_destroy_chatbox_data(sprider);

                        printf("The amount of messages present in the message_list is %zu\n", sprider->message_list.count);
                        printf("messages are:\n");
                        for(size_t i=0;i<sprider->message_list.count;i++) {
                            printf("%s\n", sprider->message_list.list[i]);
                        }
                    }
                    break;
            }
            break;
        }
    }
}

void sprider_create_chatbox(sprider_t* sprider) {
    sprider_chatbox_t* chatbox = &sprider->chatbox;
    // Set the properties for the chatbox
    chatbox->outer_canvas.x = 10, chatbox->outer_canvas.w = sprider->win_w - (chatbox->outer_canvas.x*2);
    chatbox->outer_canvas.h = 50, chatbox->outer_canvas.y = sprider->win_h - chatbox->outer_canvas.h - 10;

    // Draw or Render the box that will contain the text that the user types on their keyboard
    sprider_setrender_drawcolor(sprider, chatbox->outer_canvas_color);
    SDL_RenderDrawRect(sprider->renderer, &chatbox->outer_canvas);
    SDL_RenderFillRect(sprider->renderer, &chatbox->outer_canvas);
    
    int text_start_xpos = chatbox->outer_canvas.x + 5,
        text_start_ypos = chatbox->outer_canvas.y,
        cursor_xpos = 0, cursor_width = DEFAULT_CHATBOX_CURSORWIDTH;
    SDL_Rect current_textcanvas = {0};

    // Render UTF8 text so and convert it into a texture that can be shown on the screen
    if(!chatbox->datalist) {
        SDL_Rect placeholder_textcanvas = {0};
        chatbox->placeholder = DEFAULT_CHATBOX_PLACEHOLDER;
        TTF_SizeUTF8(sprider->default_font, chatbox->placeholder, &placeholder_textcanvas.w, &placeholder_textcanvas.h);
        placeholder_textcanvas.x = text_start_xpos,
        placeholder_textcanvas.y = text_start_ypos + ((chatbox->outer_canvas.h - placeholder_textcanvas.h) / 2);

        current_textcanvas.w = 0, current_textcanvas.h = placeholder_textcanvas.h,
        current_textcanvas.x = placeholder_textcanvas.x, current_textcanvas.y = placeholder_textcanvas.y;
        SDL_Surface* placeholder_surface = TTF_RenderUTF8_Blended(sprider->default_font, chatbox->placeholder,
                chatbox->placeholder_textcolor);
        SDL_Texture* placeholder_texture = SDL_CreateTextureFromSurface(sprider->renderer, placeholder_surface);
        SDL_RenderCopy(sprider->renderer, placeholder_texture, NULL, &placeholder_textcanvas);
    } else {
        bool textcanvas_initialized = false;
        SDL_Rect start_textcanvas = {0};
        if(!textcanvas_initialized) {
            start_textcanvas.x = text_start_xpos,
            start_textcanvas.y = chatbox->outer_canvas.y + ((chatbox->outer_canvas.h -
            start_textcanvas.h) / 2);
        }
        for(size_t i=0;i<chatbox->datalist_count;i++) {
            TTF_SizeUTF8(sprider->default_font, chatbox->datalist[i].data, &start_textcanvas.w, &start_textcanvas.h);
            start_textcanvas.y = chatbox->outer_canvas.y + ((chatbox->outer_canvas.h -
            start_textcanvas.h) / 2);
            SDL_Surface* chatbox_textsurface = TTF_RenderUTF8_Blended(sprider->default_font, chatbox->datalist[i].data,
                chatbox->input_textcolor);
            SDL_Texture* chatbox_texttexture = SDL_CreateTextureFromSurface(sprider->renderer, chatbox_textsurface);
            SDL_RenderCopy(sprider->renderer, chatbox_texttexture, NULL, &start_textcanvas);
            SDL_FreeSurface(chatbox_textsurface);
            SDL_DestroyTexture(chatbox_texttexture);
            start_textcanvas.x += start_textcanvas.w;
        }
        current_textcanvas.x = start_textcanvas.x, current_textcanvas.y = start_textcanvas.y,
        current_textcanvas.w = start_textcanvas.w, current_textcanvas.h = start_textcanvas.h;
    }

    // set the properties of the chatbox
    chatbox->cursor.w = cursor_width;
    chatbox->cursor.h = current_textcanvas.h;
    chatbox->cursor.x = text_start_xpos + cursor_xpos;
    chatbox->cursor.y = text_start_ypos + ((chatbox->outer_canvas.h - current_textcanvas.h) / 2);

    // Render the cursor to the screen
    sprider_setrender_drawcolor(sprider, chatbox->cursor_color);
    SDL_RenderDrawRect(sprider->renderer, &chatbox->cursor);
    SDL_RenderFillRect(sprider->renderer, &chatbox->cursor);
}

void sprider_destroy_chatbox_data(sprider_t* sprider) {
    for(size_t i=0;i<sprider->chatbox.datalist_count;i++) {
        free(sprider->chatbox.datalist[i].data); sprider->chatbox.datalist[i].data = NULL;
    }
    free(sprider->chatbox.datalist); sprider->chatbox.datalist = NULL;
    sprider->chatbox.datalist_count = 0;
    sprider->chatbox.datalist_cursor = 0;
}