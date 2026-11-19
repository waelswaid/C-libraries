#include <stdlib.h>
#include <stdio.h>
#include "mystrings.h"
#include <string.h>


int main()
{
char *str = "ab adg   dgw   c";
char delim = ' ';
char **words = split(str, delim);
for (char **p=words; *p!=NULL; p++)
    printf("%s\n",*p);

}