#define ADDRESS_RC "ipc:///tmp/s1"
#define WINDOW_SIZE 30
#define MAX_ROACHES (WINDOW_SIZE * WINDOW_SIZE / 3) // one third of possible spaces
#define MAX_LIZARDS 26
#define MAX_ROACH_PER_CLIENT 10

typedef enum entity_type_t
{
    LIZARD,
    ROACH
} entity_type_t;

typedef struct entity_t
{
    entity_type_t entity_type;
    char ch;
    unsigned int points;
    unsigned short int pos_x, pos_y;
} entity_t;

typedef enum direction_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct generic_msg // connect or movement or disconnect
{
    short int header;                               /* header to send */
    entity_type_t entity_type;                      /* type of entity */
    unsigned short int msg_type;                    /* 0 - connect   1 - move   2 - disconnect*/
    char ch;                                        /* value to send (points of roaches or char of lizard) */
    direction_t direction;                          /* direction of movement */
} generic_msg;

typedef struct display_msg
{
    entity_t array_entities[MAX_LIZARDS + MAX_ROACHES];
} display_msg;

typedef struct response_msg
{
    unsigned short int success; /* 0 - fail, 1 - success */
    unsigned short int score;
} response_msg;
