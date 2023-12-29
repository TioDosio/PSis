#ifndef lizardNroaches_H // Include guards to prevent multiple inclusions
#define lizardNroaches_H


#define ADDRESS_REQ "tcp://127.0.0.1:6666"
#define ADDRESS_PUB "tcp://127.0.0.1:6669"
#define BACK_END_ADDRESS "inproc://back-end"
#define FRONT_END_ADDRESS "inproc://front-end"

#define WINDOW_SIZE 30
#define MAX_NPCS (WINDOW_SIZE * WINDOW_SIZE / 3) // one third of possible spaces
#define MAX_LIZARDS 26
#define MAX_ROACH_PER_CLIENT 10
#define ROACH_RESPAWN_TIME 5
#define BUFFER_SIZE 70

//ZMQ context
void *context;

typedef enum entity_type_t // entity type
{
    LIZARD,
    ROACH,
    WASP
} entity_type_t;

typedef enum direction_t // direction of movement
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef enum msg_type_t // direction of movement
{
    CONNECT,
    MOVE,
    DISCONNECT
} msg_type_t;

typedef struct entity_t
{
    entity_type_t entity_type;       /* type of entity */
    char ch;                         /* ch of entity */
    unsigned int points;             /* ammount of points */
    unsigned short int pos_x, pos_y; /* position of entity */
    direction_t direction;           /* direction of last movement */
    short int secret_code;           /* secret code to send */

} entity_t;

typedef struct client_msg // client msg (connections, movements or disconnections)
{
    entity_type_t entity_type;       /* type of entity */
    msg_type_t msg_type;             /* type of message */
    short int content;               /* value to send (points, char, movement direction, etc) */
    short int secret_code;           /* secret id for entity */
} client_msg;

typedef struct response_msg
{
    unsigned short int success; /* 0 - fail, 1 - success, 2 - success in disconnection*/
    short int score;            /* score of the player */
    short int secret_code;      /* secret id of entity */
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
    entity_t npc[MAX_NPCS];         /* array of npcs */
    int n_lizards;                  /* number of lizards */
    int n_npc;                      /* number of npcs */
    char address_port[BUFFER_SIZE]; /* address and port of publisher */
} connect_display_resp;

// Struct for arguments for threads
typedef struct thread_args
{
    entity_t *lizard_array;
    entity_t *npc_array;
    int n_lizards;
    int n_npc;
    int *roach_death_time;

} thread_args;

#endif