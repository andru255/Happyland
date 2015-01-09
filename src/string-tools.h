#ifndef __STRING_TOOLS_H__
#define __STRING_TOOLS_H__

#include <string.h>
#include <stdio.h>
#include <assert.h>

void triml(char ** buffer);
void trimr(char ** buffer);
void trim(char ** buffer);
void trimComment(char ** buffer);
char * trimCpy(char * buffer);

char * cut(char * str, char delim);

#endif // __STRING_TOOLS_H__