#ifndef _SPRIDERCHATAPP_H
#define _SPRIDERCHATAPP_H
#include <locale.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>
#include "sprider-string.h"

// Converts an SDL_Color to a paramater that can be passed to one of the functions such as
// SDL_SetRenderDrawColor, etc
#define COLOR_TOPARAM(color) color.r, color.g, color.b, color.a

typedef struct sprider_chatbox_data {
    char* data;
    int datalen;
    int cursor_pos;
} sprider_chatbox_data_t;

typedef struct sprider_chatbox {
    sprider_chatbox_data_t* datalist;
    size_t datalist_count;
    size_t total_inputlen;
    size_t datalist_cursor;
    // A short message prompting the user to type within the checkbox
    // it is only displayed whenever the chatbox is empty
    const char* placeholder;
    // The color of each properties that make up the chat box such as text color etc
    SDL_Color input_textcolor,
              placeholder_textcolor,
              cursor_color,
              outer_canvas_color;
    // The canvases representing the cursor, the chatbox
    SDL_Rect cursor,
             outer_canvas;
} sprider_chatbox_t;

typedef struct sprider {
    SDL_Renderer* renderer; // A pointer to the renderer that we can draw to
    SDL_Window* window; // A pointer to the window that is displayed on the screen
    SDL_Event e; // The events that the application receives
    int win_w, win_h, default_fontsize; // the current window size (width and height) and default_fontsize for text
    TTF_Font* default_font; // The default font family that the application uses
    sprider_chatbox_t chatbox; // a structure containing information on the input within the chatbox
    sprider_stringlist_t message_list; // A list of messages that were sent from the chatbox when the user presses enter
    bool quit; // whether the application should exit or not
} sprider_t;

void sprider_init();
void sprider_initialize_chatbox(sprider_t* sprider);
void sprider_createapp(sprider_t* app);
void sprider_handleapp_events(sprider_t* sprider);
void sprider_handle_chatbox_input(sprider_t* sprider);
void sprider_create_chatbox(sprider_t* chatbox);
void sprider_getwindow_size(sprider_t* app);
void sprider_setrender_drawcolor(sprider_t* sprider, SDL_Color render_color);
void sprider_setwindow_color(sprider_t* app, SDL_Color window_color);
void sprider_run(sprider_t* app);
void sprider_display_messages(sprider_t* app);
void sprider_present(sprider_t* app);
void sprider_destroyapp(sprider_t* app);
#endif