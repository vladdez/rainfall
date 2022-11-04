#include <stdio.h>
#include <string.h>
#include <unistd.h>

void p(char *s, char *str)
{
	char buff[4096];

	puts(str);
	read(0, buff, 4096);
	*strchr(buff, '\n') = 0;
	strncpy(s, buff, 20);
}

void pp(char *buff)
{
	char str1[20];
	char str2[20];

	p(str1, " - ");
	p(str2, " - ");
	strcpy(buff, str1);
	buff[strlen(buff)] = ' ';
	strcat(buff, str2);
}

int main(void)
{
	char buff[42];

	pp(buff);
	puts(buff);
	return (0);
}
