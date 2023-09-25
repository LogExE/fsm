#include "bufreadline.h"

#include "string.h"

int buf_readline(char *buf, FILE *stream)
{
    char *gets_res = fgets(buf, LINE_SIZE, stream);
    if (gets_res == NULL)
    {
        if (feof(stream))
            return BUF_RL_EOF;
        else
            return BUF_RL_ERR;
    }
    int read = strlen(buf);
    if (buf[read - 1] != '\n' && !feof(stream))
        return BUF_RL_TOOMANY;
    if (buf[read - 1] == '\n')
    {
        buf[read - 1] = '\0';
        --read;
    }
    return read;
}