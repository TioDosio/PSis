#define ADDRESS_RC "ipc:///tmp/s1"
#define WINDOW_SIZE 30
#define MAX_ROACHES (WINDOW_SIZE * WINDOW_SIZE / 3) // one third of possible spaces

typedef struct lizard_t
{
    char name;
    int points;
    unsigned short int pos[2];
} lizard_t;

typedef struct roach_t
{
    unsigned short int value;
    unsigned short int pos[2];
} roach_t;

typedef enum direction_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct generic_msg // connect or movement or disconnect
{
    unsigned short int entity_type; /* 0 - lizard, 1 - roach */
    unsigned short int msg_type;    /* 0 - connect   1 - move   2 - disconnect*/
    char ch;                        /* value to send (points of roaches or char of lizard) */
    direction_t direction;          /* direction of movement */

} generic_msg;

typedef struct update_msg
{
    lizard_t array_lizards[26];
    roach_t array_roaches[MAX_ROACHES];
} update_msg;

typedef struct response
{
    unsigned short int name_check;

} response;
