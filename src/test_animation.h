
typedef enum
{
    ANIMATION_IDLE,
    ANIMATION_SPAWN,
    ANIMATION_DESPAWN,
    ANIMATION_DELETE,
    ANIMATION_NULL

} portal_e;

typedef struct
{

    int *state_frame_count;
    int state_count;

    SDL_Rect **anims;
    SDL_Rect *size;

    SDL_Texture *sprite;

    int current_state;
    int current_frame;
} anim_t;