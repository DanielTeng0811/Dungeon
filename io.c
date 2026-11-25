#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "io.h"

static bool eof_reached = false;
static bool echo_input = false;

void io_set_echo(bool on) { echo_input = on; }

bool io_eof(void) { return eof_reached; }

bool read_line(char *buf, size_t size)
{
    fflush(stdout);
    if (eof_reached || fgets(buf, (int)size, stdin) == NULL) {
        eof_reached = true;
        if (size > 0)
            buf[0] = '\0';
        if (echo_input)
            printf("<EOF>\n");
        return false;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
    } else {
        /* Line too long: discard the rest */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
    if (len > 0 && buf[len - 1] == '\r') /* Windows line ending */
        buf[--len] = '\0';
    if (echo_input)
        printf("%s\n", buf);
    return true;
}

char read_option(void)
{
    char line[128];
    while (read_line(line, sizeof line)) {
        for (char *p = line; *p != '\0'; p++) {
            if (!isspace((unsigned char)*p))
                return (char)toupper((unsigned char)*p);
        }
        printf("> ");
    }
    return '\0';
}
