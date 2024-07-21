#ifndef ROCKSERVER_STRING_H
#define ROCKSERVER_STRING_H

void left_string(char* targetstr, int origin_length);
void first_strnmv(char *src, char *dest, int n);              /* return ptr */
int find_first_text_off(const char* data, int len);
void zerostr(char* src, int start, int length);

#endif