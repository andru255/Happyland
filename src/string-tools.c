#include "string-tools.h"

void triml(char ** buffer)
{
	while((*buffer)[0] == ' ' || (*buffer)[0] == '\t')
		*buffer = *buffer + 1;
}

void trimr(char ** buffer)
{
	int i;
	for(i = strlen(*buffer) - 1 ; i > -1 ; i--)
	{
		if((*buffer)[i] == ' '  ||
		   (*buffer)[i] == '\t' ||
		   (*buffer)[i] == '\n' ||
		   (*buffer)[i] == '\r' )
			(*buffer)[i] = '\0';
		else
			i = -1;
	}
}

void trimComment(char ** buffer)
{
	int sizeStr = strlen(*buffer);
	int i;
	for(i = 0; i < sizeStr ; i++)
	{
		if((*buffer)[i] == '#')
		{
			(*buffer)[i] = '\0';
			trimr(buffer); // Trim the string
			i = sizeStr;   // Stop the for(;;)
		}
	}
}

void trim(char ** buffer)
{
	triml(buffer);
	trimr(buffer);
}

char * trimCpy(char * buffer)
{
	triml(&buffer);
	trimr(&buffer);
	return buffer;
}

char * cut(char * buffer, char delim)
{
	int i;
	int bufferSize = strlen(buffer);
	char * out = NULL;

	for(i = 0 ; i < bufferSize ; i++)
	{
		if(buffer[i] == delim)
		{
			buffer[i] = '\0';
			out = buffer + i + 1;
			i = bufferSize;
		}
	}

	return out;
}