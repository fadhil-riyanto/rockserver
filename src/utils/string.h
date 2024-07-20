#ifndef ROCKSERVER_STRING_H
#define ROCKSERVER_STRING_H

void left_string(char* targetstr, int origin_length);
static char* first_strnmv(char *src, int n);              /* return ptr */
static int find_first_text_off(const char* data, int len);

#endif