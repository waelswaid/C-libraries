
#include <stdlib.h>
#include "mystrings.h"
#include <string.h>


/*
calculates the length of the string pointed to by s,
excluding the terminating null byte ('\0')

*/
size_t my_strlen(const char *s)
{
size_t cnt = 0;

while(*s)
    {
        cnt++;
        s++;
    }
return cnt;

}

char *my_strcpy(char *dest, const char *src)
{
char *start = dest;
while(*src)
{
    *(dest++) = *(src++);
}
*dest = '\0';
return start;

}

char *my_strdup(const char *s)
{
    char *new_s = malloc(sizeof(char)*strlen(s)+1);
    if(new_s==NULL)
        return NULL;
    char *p = new_s;
    while ((*p++ = *s++) != '\0')
        ;
    return new_s;
}

static int is_delim(char c, const char *delim)
{
while(*delim)
{
    if(c == *delim) return 1;
    delim++;
}
return 0;
}

char *my_strtok(char *str, const char *delim)
{
static char *next = NULL;
// fresh string
if(str!=NULL) next = str;
// NULL passed as a fresh string returns NULL
if(next == NULL) return NULL;

// skip leading delimiters
while(*next && is_delim(*next, delim))
    next++;
if(*next=='\0')
    return NULL;

char *start = next;

while(*next && !is_delim(*next, delim))
    next++;

if(*next)
{
    *next = '\0';
    next++;
}
return start;
}

char **split(char *str, char delim)
{
//empty string? return null
if(!*str)
    return NULL;
//skip initial delimiters
while(*str == delim)
    str++;
int i =0, j=0;
int words_cap = 1;
char **words = malloc(words_cap*sizeof(char*));
if (!words) return NULL;
char *current_word = malloc(strlen(str)*sizeof(char)+1);
if (!current_word) return NULL;
while(*str)
{

    if (*str!=delim)
        current_word[j++] = *(str++);
    if(*str == delim)
    {
        current_word[j] = '\0';
        //add the word to **words
       words[i++] = strdup(current_word);
       //increase word cap and reallocate memory for **words
       words_cap++;
       char **tmp = realloc(words, words_cap*sizeof(char*));
       if (!tmp) return NULL;
       words = tmp;
       j = 0;
       while(*str==delim)
            str++;
    }

}
if (j>0)
{
    current_word[j] = '\0';
    words[i++] = strdup(current_word);
    char **tmp = realloc(words, (words_cap+1)*sizeof(char*));
    if (!tmp) return NULL;
    words = tmp;
}
words[i] = NULL;
free(current_word);
return words;
}