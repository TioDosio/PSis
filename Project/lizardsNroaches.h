
typedef enum direction_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct generic_msg
{
    int entity_type;       /* 0 - lizard, 1 - roach */
    int msg_type;          /* 0 - connect   1 - move */
    int points_roach;      /* points of roach */
    char ch_lizards;       /* character of lizard */
    direction_t direction; /* direction of movement */
    /* data */
} generic_msg;

#define ADDRESS_RC "ipc:///tmp/s1"
