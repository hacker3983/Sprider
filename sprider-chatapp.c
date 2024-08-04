#include "sprider-chatapp.h"
#define DEFAULT_WINDOW_TITLE "Sprider - Chat Application"
#define DEFAULT_WINDOW_WIDTH  900
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_FONT_FILE "Symbola.ttf"
#define DEFAULT_FONT_SIZE 18
SDL_Color DEFAULT_MESSAGEBOX_COLOR = {0x10, 0x2B, 0x3F, 0xFF};
SDL_Color DEFAULT_MESSAGETEXT_COLOR = {0x62, 0x47, 0xAA, 0xFF};

void sprider_init() {
    // Initialize the video system for SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "SDL_Init(SDL_INIT_VIDEO: Failed because %s", SDL_GetError());
        return;
    }
    // Initialize SDL ttf so we can render text
    if(TTF_Init() < 0) {
        fprintf(stderr, "SDL ttf initialization failed because %s\n", SDL_GetError());
        return;
    }
    setlocale(LC_ALL, "en_US.UTF-8");  // Set the locale to UTF-8 capable locale
}

void sprider_createapp(sprider_t* sprider) {
    sprider->window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    sprider->quit = false;
    sprider->e = (SDL_Event){0};
    sprider->win_w = DEFAULT_WINDOW_WIDTH, sprider->win_h = DEFAULT_WINDOW_HEIGHT;
    if(!sprider->window) {
        fprintf(stderr, "SDL_CreateWindow(): Failed because %s\n", SDL_GetError());
        return;
    }
    sprider->renderer = SDL_CreateRenderer(sprider->window, -1, SDL_RENDERER_ACCELERATED);
    sprider->default_font = TTF_OpenFont(DEFAULT_FONT_FILE, DEFAULT_FONT_SIZE);
    sprider->default_fontsize = DEFAULT_FONT_SIZE;
    sprider->message_list = (sprider_stringlist_t){0};
    sprider->chatbox = (sprider_chatbox_t){0};
    sprider_initialize_chatbox(sprider);

    if(!sprider->default_font) {
        fprintf(stderr, "TTF_OpenFont(): Failed because %s\n", SDL_GetError());
        return;
    }
}

void sprider_getwindow_size(sprider_t* sprider) {
    // Get the size of the window
    SDL_GetWindowSize(sprider->window, &sprider->win_w, &sprider->win_h);
}

void sprider_setwindow_color(sprider_t* sprider, SDL_Color window_color) {
    SDL_SetRenderDrawColor(sprider->renderer, COLOR_TOPARAM(window_color));
    SDL_RenderClear(sprider->renderer);
}

void sprider_setrender_drawcolor(sprider_t* sprider, SDL_Color render_color) {
    SDL_SetRenderDrawColor(sprider->renderer, COLOR_TOPARAM(render_color));
}

void sprider_handleapp_events(sprider_t* sprider) {
    while(SDL_PollEvent(&sprider->e)) {
        if(sprider->e.type == SDL_QUIT) {
            sprider->quit = 1; break;
        } else if(sprider->e.type == SDL_TEXTINPUT || sprider->e.type == SDL_KEYDOWN) {
            sprider_handle_chatbox_input(sprider);
        }
    }
}

void sprider_run(sprider_t* sprider) {
    while(!sprider->quit) {
        // Handle any application events such as window, quit events, etc
        sprider_handleapp_events(sprider);
        // Get the window size (width and height)
        sprider_getwindow_size(sprider);
        sprider_setwindow_color(sprider, (SDL_Color){0xff, 0xff, 0xff, 0xff});
        
        // create the chatbox that allows the user to type in a message
        sprider_create_chatbox(sprider);
        sprider_display_messages(sprider);
        // Update the screen with the rendering changes
        sprider_present(sprider);
    }
}

void sprider_display_messages(sprider_t* sprider) {
    int message_verticalspacing = 10;
    int message_verticalpadding = 50, message_horizontalpadding = 10;
    SDL_Rect message_outer_canvas = {0}, message_inner_canvas = {0};
    sprider_stringlist_t message_list = sprider->message_list;
    for(size_t i=0;i<sprider->message_list.count;i++) {
        // Get the width and the height of the message in pixels of the particular font and properties
        TTF_SizeUTF8(sprider->default_font, message_list.list[i], &message_inner_canvas.w, &message_inner_canvas.h);
        message_outer_canvas.w = message_inner_canvas.w + message_horizontalpadding;
        message_outer_canvas.h = message_inner_canvas.h + message_verticalpadding;
        message_outer_canvas.x = message_horizontalpadding;

        // Center the message text using inner canvas within the outer_canvas
        message_inner_canvas.x = message_outer_canvas.x + ((message_outer_canvas.w - message_inner_canvas.w) / 2);
        message_inner_canvas.y = message_outer_canvas.y + ((message_outer_canvas.h - message_inner_canvas.h) / 2);
        
        // Render the message box that contains the message the user sends
        SDL_SetRenderDrawColor(sprider->renderer, COLOR_TOPARAM(DEFAULT_MESSAGEBOX_COLOR));
        SDL_RenderDrawRect(sprider->renderer, &message_outer_canvas);
        SDL_RenderFillRect(sprider->renderer, &message_outer_canvas);

        SDL_Surface* surface = TTF_RenderUTF8_Blended(sprider->default_font, message_list.list[i], DEFAULT_MESSAGETEXT_COLOR);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(sprider->renderer, surface);
        // Copy the texture to the renderer so we can display it on the screen
        SDL_RenderCopy(sprider->renderer, texture, NULL, &message_inner_canvas);

        // Destroy the textures that and the surface that we rendrered
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        message_outer_canvas.y += message_outer_canvas.h + message_verticalspacing;
    }
}

void sprider_present(sprider_t* sprider) {
    SDL_RenderPresent(sprider->renderer);
}

void sprider_destroyapp(sprider_t* sprider) {
    TTF_CloseFont(sprider->default_font); sprider->default_font = NULL;
    SDL_DestroyRenderer(sprider->renderer); sprider->renderer = NULL;
    SDL_DestroyWindow(sprider->window); sprider->window = NULL;
    TTF_Quit();
    SDL_Quit();
}