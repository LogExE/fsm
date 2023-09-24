#pragma once

#include "stdio.h"

#define ANSI_GREEN "\033[0;32m"
#define ANSI_RED "\033[0;31m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_RESET "\033[0m"

#define LINE_SIZE 512
// > 0 - how many chars read except newline
// == 0 - EOF
// -1 - stream error
// -2 - too many chars
int buf_readline(char *buf, FILE *stream);