#include "../common-files/lizardsNroachesNwasps.h"
#include "../common-files/display-funcs.h"
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <zmq.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

void *context;

pthread_mutex_t pause_display_mutex = PTHREAD_MUTEX_INITIALIZER;

int pause_display = 0;
int quit_display = 0;

//Read update function
void read_update(thread_args *game, void *subscriber){
    // Local variable for incoming update message
    display_update update;

    // Store in local variables
    entity_t *array_lizards = game->lizard_array;
    entity_t *array_npc = game->npc_array;
    WINDOW *my_win = game->game_win;
    WINDOW *lines_win = game->lines_win;

    //Cycle
    while (1)
    {
        char *cap[4];
        int rc = zmq_recv(subscriber, cap, 3, ZMQ_NOBLOCK);
        if(rc != -1){
            int i;

            // Clear the screen
            wclear(my_win);
            box(my_win, 0, 0);

            rc = zmq_recv(subscriber, &update, sizeof(update), 0);
            assert(rc != -1);
            short id = -1;
            switch (update.entity.entity_type)
            {
            case LIZARD:
                id = find_entity_id(array_lizards, game->n_lizards, update.entity.secret_code);
                if (id == -1 && update.disconnect == -1) // If it's a new lizard and disconnect == -1, add it to the array
                {
                    array_lizards[game->n_lizards] = update.entity;
                    game->n_lizards++;
                }
                else
                {   
                    array_lizards[id] = update.entity;
                    if (update.disconnect == 1)
                    {
                        remove_entity(array_lizards, &game->n_lizards, id);
                    }
                }
                break;
            case ROACH:
            case WASP:
                id = find_entity_id(array_npc, game->n_npc, update.entity.secret_code);
                if (id == -1) // If it's a new npc, add it to the array
                {
                    array_npc[game->n_npc] = update.entity;
                    game->n_npc++;
                }
                else
                {
                    array_npc[id] = update.entity;
                    if (update.disconnect == 1)
                    {
                        remove_entity(array_npc, &game->n_npc, id);
                    }
                }
                break;

            default:
                id = -1;
                continue;
                break;
            }

            // Check if roaches got eaten
            for (int i = 0; i < game->n_npc; i++)
            {
                for (int j = 0; j < game->n_lizards; j++)
                {
                    if (array_lizards[j].pos_x == array_npc[i].pos_x && array_lizards[j].pos_y == array_npc[i].pos_y)
                    {
                        array_npc[i].ch = ' ';
                    }
                }
            }

            // Check if lizards collided, and update points of collided lizard too
            if (update.id_l_bumped != -1)
            {
                array_lizards[update.id_l_bumped].points = update.entity.points;
            }
            pthread_mutex_lock(&pause_display_mutex);
            int value = pause_display;
            pthread_mutex_unlock(&pause_display_mutex);
            if(value == 0)
                disp_update(game);
        } else if (errno == EAGAIN) {
            pthread_mutex_lock(&pause_display_mutex);
            int value = quit_display;
            pthread_mutex_unlock(&pause_display_mutex);
            if (value == 1)
            {
                break; // Exit the loop on quit
            }
        } else {
            perror("zmq_recv");
            break;  // Exit the loop on error
        }
        
    }
    zmq_close(subscriber);
}

//Display thread function
void *display_thread_func(void *args)
{
    thread_args *game = (thread_args *)args;

    //ZMQ initializations
    void *subscriber = zmq_socket(context, ZMQ_SUB);

    //Connect to the broadcast port
    int rc = zmq_connect(subscriber, game->broadcast_address);
    assert(rc != -1);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "dis", 3);

    // First draw
    disp_update(game);

    // Loop for receiving updates
    read_update(game, subscriber);

}

int main(int argc, char *argv[])
{
    char server_address[256];
    char *server_ip;
    char *server_port;
    int client_id = -1;
    // para colocar o ip e a porta como argumentos
    if (argc == 3) 
    {
        server_ip = argv[1];
        server_port = argv[2];
    }
    else
    {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]); // "Usage: %s <server_ip> <server_port>\n
        return 1;
    }

    // creating request socket
    printf("Connecting to server…\n");
    context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ); 
    snprintf(server_address, sizeof(server_address), "tcp://%s:%s", server_ip, server_port);
    int rc;
    rc = zmq_connect(requester, server_address);
    assert(rc != -1);
    response_msg r;
    connect_display_resp disp_start_msg;

    // send connection message
    client_msg m;
    m.msg_type = CONNECT;
    m.entity_type = LIZARD;
    m.content = 0;
    m.secret_code = -1;
    rc = zmq_send(requester, &m, sizeof(m), 0);
    assert(rc != -1);

    rc = zmq_recv(requester, &disp_start_msg, sizeof(disp_start_msg), 0);
    assert(rc != -1);
    if (client_id == -1){
        client_id = disp_start_msg.secret_code;
    }
    if (disp_start_msg.success == 0)
    {
        printf("Server Full, try again later\n");
        exit(0);
    }

    // Initialize the screen
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    WINDOW *game_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    WINDOW *lines_win = newwin(MAX_LIZARDS, WINDOW_SIZE, WINDOW_SIZE, 0);

    // Load game variables
    int secret_code = disp_start_msg.secret_code;
    thread_args game;
    game.lizard_array = disp_start_msg.lizard;
    game.n_lizards = disp_start_msg.n_lizards;
    game.npc_array = disp_start_msg.npc;
    game.n_npc = disp_start_msg.n_npc;
    strcpy(game.broadcast_address, disp_start_msg.address_port);
    game.game_win = game_win;
    game.lines_win = lines_win;

    //Call display thread
    pthread_t display_thread;
    pthread_create(&display_thread, NULL, display_thread_func, &game);

    int n = 0;
    int disconnect = 0;

    int key;
    do
    {    
        //  prepare the movement message
        m.msg_type = 1;
        m.content = 0;
        m.secret_code = secret_code;
        key = getch();
        usleep(10000);
        n++;
        switch (key)
        {
        case KEY_LEFT:
            //  prepare the movement message
            m.content = LEFT;
            break;
        case KEY_RIGHT:
            //  prepare the movement message
            m.content = RIGHT;
            break;
        case KEY_DOWN:
            //  prepare the movement message
            m.content = DOWN;
            break;
        case KEY_UP:
            //  prepare the movement message
            m.content = UP;
            break;
        case 'q':
        case 'Q':
            m.msg_type = DISCONNECT;
            disconnect = 1;
            break;
        default:
            key = 'x';
            break;
        }

        // send the movement message
        if (key != 'x')
        {
            m.client_code = client_id;
            if (zmq_send(requester, &m, sizeof(m), 0) == -1)
            {
                continue;
            }
            if (zmq_recv(requester, &r, sizeof(r), 0) == -1)
            {
                continue;
            }
            
            if(r.success == 0){
                //Clear the screen and pause display
                pthread_mutex_lock(&pause_display_mutex);
                pause_display = 1;
                pthread_mutex_unlock(&pause_display_mutex);
                wclear(game_win);
                wclear(lines_win);
                wrefresh(game_win);
                wrefresh(lines_win);
                wprintw(game_win, "Invalid operation. Timed out?\n");
                wrefresh(game_win);
                sleep(3);
                pthread_mutex_lock(&pause_display_mutex);
                pause_display = 0;
                pthread_mutex_unlock(&pause_display_mutex);
                continue;
            }

            if (disconnect == 1 && r.success == 1)
            {
                refresh();
                key = 27;
                break;
            }
        }
    } while (key != 27);

    pthread_mutex_lock(&pause_display_mutex);
    quit_display = 1;
    pthread_mutex_unlock(&pause_display_mutex);

    // Wait for the thread to finish
    if (pthread_join(display_thread, NULL) != 0) {
        perror("Error joining thread");
        return 1;
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);
    endwin(); /* End curses mode		  */

    return 0;
}