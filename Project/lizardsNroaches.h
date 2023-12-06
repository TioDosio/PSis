#ifndef lizardNroaches_H // Include guards to prevent multiple inclusions
#define lizardNroaches_H

#define ADDRESS_RC "tcp://127.0.0.1:6666"
#define WINDOW_SIZE 30
#define MAX_ROACHES (WINDOW_SIZE * WINDOW_SIZE / 3) // one third of possible spaces
#define MAX_LIZARDS 26
#define MAX_ROACH_PER_CLIENT 10

typedef enum entity_type_t
{
    LIZARD,
    ROACH
} entity_type_t;

typedef enum direction_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct entity_t
{
    entity_type_t entity_type;
    char ch;
    unsigned int points;
    unsigned short int pos_x, pos_y;
    direction_t direction;
    short int secrect_code;
} entity_t;

typedef struct generic_msg // connect or movement or disconnect
{
    short int header;            /* header to send */
    entity_type_t entity_type;   /* type of entity */
    unsigned short int msg_type; /* 0 - connect   1 - move   2 - disconnect*/
    char ch;                     /* value to send (points of roaches or char of lizard) */
    direction_t direction;       /* direction of movement */
    short int secrect_code;      /* secrect code to send */
} generic_msg;

typedef struct display_msg
{
    entity_t array_entities[MAX_LIZARDS + MAX_ROACHES];
} display_msg;

typedef struct response_msg
{
    unsigned short int success; /* 0 - fail, 1 - success */
    unsigned short int score;   /* score of the player */
    short int secrect_code;     /* secrect code to send */
} response_msg;

typedef struct display_update
{
    unsigned short int n_roaches; /* number of roaches */
    unsigned short int n_lizards; /* number of lizards */
    entity_t lizard[MAX_LIZARDS];
    entity_t roach[MAX_ROACHES];
} display_update;

typedef struct response_msg_display
{
    unsigned short int type; /* 0 - error, 1 - success, 2 - connect*/
    short int secrect_code;  /*  secret code to send */
} response_msg_display;

#endif
