#pragma once

#include "stdio.h"

#define LINE_SIZE 512
#define BUF_RL_ERR -1
#define BUF_RL_TOOMANY -2
#define BUF_RL_EOF -3
// >= 0 - how many chars read except newline
// BUF_RL_ERR - stream error
// BUF_RL_TOOMANY - too many chars
// BUF_RL_EOF - EOF
int buf_readline(char *buf, FILE *stream);