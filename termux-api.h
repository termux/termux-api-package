#include <stdlib.h>

_Noreturn void exec_am_broadcast(int, char**, char*, char*);
_Noreturn void exec_callback(int);
void generate_uuid(char*);
void* transmit_stdin_to_socket(void*);
int transmit_socket_to_stdout(int);
int run_api_command(int, char**);
