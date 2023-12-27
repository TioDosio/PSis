#ifndef lizardNroaches_H // Include guards to prevent multiple inclusions
#define lizardNroaches_H

#define ADDRESS_REQ "tcp://127.0.0.1:6666"
#define ADDRESS_PUB "tcp://127.0.0.1:6669"
#define WINDOW_SIZE 30
#define MAX_ROACHES (WINDOW_SIZE * WINDOW_SIZE / 3) // one third of possible spaces
#define MAX_LIZARDS 26
#define MAX_ROACH_PER_CLIENT 10
#define ROACH_RESPAWN_TIME 5
#define BUFFER_SIZE 70

typedef enum entity_type_t // entity type
{
    LIZARD,
    ROACH,
    DISPLAY
} entity_type_t;

typedef enum direction_t // direction of movement
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct entity_t
{
    entity_type_t entity_type;       /* type of entity */
    char ch;                         /* value to send (points of roaches) */
    unsigned int points;             /* points of roaches */
    unsigned short int pos_x, pos_y; /* position of entity */
    direction_t direction;           /* direction of movement */
    short int secret_code;           /* secret code to send */
} entity_t;

typedef struct generic_msg // connect or movement or disconnect
{
    entity_type_t entity_type;   /* type of entity */
    unsigned short int msg_type; /* 0 - connect   1 - move   2 - disconnect*/
    char ch;                     /* value to send (points of roaches or char of lizard) */
    direction_t direction;       /* direction of movement */
    short int secret_code;       /* secret code to send */
} generic_msg;

typedef struct response_msg
{
    unsigned short int success; /* 0 - fail, 1 - success, 2 - success in disconnection*/
    unsigned short int score;   /* score of the player */
    short int secret_code;      /* secret code to send */
} response_msg;

typedef struct display_update
{
    entity_t entity;               /* entity to update */
    unsigned short int disconnect; /* 0 - no disconnect, 1 - disconnect */
    short int id_l_bumped;         /* id of lizard bumped */
} display_update;

typedef struct connect_display_resp
{
    entity_t lizard[MAX_LIZARDS];   /* array of lizards */
    entity_t roach[MAX_ROACHES];    /* array of roaches */
    int n_lizards;                  /* number of lizards */
    int n_roaches;                  /* number of roaches */
    char address_port[BUFFER_SIZE]; /* address and port of publisher */
} connect_display_resp;

#endif
