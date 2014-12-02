#include "list.h"

// Compare functions
int Int_cmp(int n1, int n2)
{
	return n1 - n2;
}

int Float_cmp(float n1, float n2)
{
	return n1 - n2;
}

int Double_cmp(double n1, double n2)
{
	return n1 - n2;
}

int Ptr_cmp(void * pt1, void * pt2)
{
	return pt1 - pt2;
}

int Str_cmp(char * str1, char * str2)
{
	return strcmp(str1, str2);
}

// Copy functions
void Int_copy(int * dest, int * src)
{
	*dest = *src;
}

void Float_copy(float * dest, float * src)
{
	*dest = *src;
}

void Double_copy(double * dest, double * src)
{
	*dest = *src;
}

void Ptr_copy(void ** dest, void ** src)
{
	*dest = *src;
}

void Str_copy(char ** dest, char ** src)
{
	int i = 0;
	int len = strlen(*src);
	*dest = malloc(sizeof(char) * len);
	for(i = 0 ; i < len ; i++)
		(*dest)[i] = (*src)[i];
	(*dest)[len] = '\0';
}

// Free functions
void Int_free(int val)       { (void)(val); }
void Float_free(float val)   { (void)(val); }
void Double_free(double val) { (void)(val); }
void Str_free(char * str)    { free(str);   }