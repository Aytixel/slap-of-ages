

typedef struct{

    int nb_frames;

    SDL_Rect *summon;
    SDL_Rect *idle;
    SDL_Rect *disappear;

    SDL_Texture *color;

    int state;
    int current_frame;
}anim_portal_t;