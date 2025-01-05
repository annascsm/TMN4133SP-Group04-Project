#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <sys/types.h>

extern pid_t keylogger_pid;

void initialize_keylogger(void);
void stop_keylogger(void);
void cleanup_keylogger(void);
void setup_signal_handlers(void);
void check_keylogger_status(void);

pid_t get_keylogger_pid(void);

#endif