#ifndef MYSTRINGS_H
#define MYSTRINGS_H

#include <stddef.h>

size_t my_strlen(const char *s);

char *my_strcpy(char *dest, const char *src);

char *my_strdup(const char *s);

char *my_strtok(char *str, const char *delim);

char **split(char *str, char delim);

#endif