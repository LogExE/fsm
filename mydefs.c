#include "mydefs.h"

#include "string.h"

int buf_readline(char *buf, FILE *stream)
{
    char *gets_res = fgets(buf, LINE_SIZE, stream);
    if (gets_res == NULL)
    {
        if (feof(stream))
            return 0;
        else
            return -1;
    }
    int read = strlen(buf);
    if (buf[read - 1] != '\n' && !feof(stream))
        return -2;
    if (buf[read - 1] == '\n')
    {
        buf[read - 1] = '\0';
        --read;
    }
    return read;
}