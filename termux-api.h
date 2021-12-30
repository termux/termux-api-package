#ifndef TERMUX_API_H
#define TERMUX_API_H

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

_Noreturn void exec_am_broadcast(int, char**, char*, char*);
_Noreturn void contact_plugin(int, char**, char*, char*);
_Noreturn void exec_callback(int);
void generate_uuid(char*);
void* transmit_stdin_to_socket(void*);
int transmit_socket_to_stdout(int);
int run_api_command(int, char**);

#if defined(__cplusplus)
}
#endif

#endif /* TERMUX_API_H */
