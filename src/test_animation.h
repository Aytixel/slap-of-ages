
typedef enum
{
    IDLE,
    SPAWN,
    DESPAWN

} portal_e;

typedef struct
{

    int *state_frame_count;
    int state_count;

    SDL_Rect **anims;

    SDL_Texture *sprite;

    int current_state;
    int current_frame;
} anim_t;