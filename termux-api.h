#ifndef TERMUX_API_H
#define TERMUX_API_H

#include <stdlib.h>

#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

_Noreturn void exec_am_broadcast(int, char**, char*, char*);
_Noreturn void exec_am_broadcast_v2(int, char**, pid_t, uid_t, unsigned long long, char*, char*);
_Noreturn void contact_plugin(int, char**, char*, char*);
_Noreturn void contact_plugin_v2(int, char**, pid_t, uid_t, unsigned long long, char*, char*);
_Noreturn void exec_callback(int);
unsigned long long get_process_starttime(pid_t pid);
void generate_uuid(char*);
void* transmit_stdin_to_socket(void*);
int transmit_socket_to_stdout(int);
int run_api_command(int, char**);

#if defined(__cplusplus)
}
#endif

#endif /* TERMUX_API_H */
