#pragma once

#include "stdio.h"

#define ANSI_GREEN "\033[0;32m"
#define ANSI_RED "\033[0;31m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_RESET "\033[0m"

#define LINE_SIZE 512
#define BUF_RL_ERR -1
#define BUF_RL_TOOMANY -2
#define BUF_RL_EOF -3
// >= 0 - how many chars read except newline
// BUF_RL_ERR - stream error
// BUF_RL_TOOMANY - too many chars
// BUF_RL_EOF - EOF
int buf_readline(char *buf, FILE *stream);