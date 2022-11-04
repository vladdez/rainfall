#include <stdio.h>
#include <unistd.h>
#include <string.h>

int	main(int ac, char **av)
{
	char	buff[132];
	FILE	*pass;

	pass = fopen("/home/user/end/.pass", "r");
	memset(buff, 0, 132);
	if ((!pass) || (ac != 2))
		return (-1);
	else
	{
		fread(buff, 1, 66, pass);
		buff[65] = 0;
		buff[atoi(av[1])] = 0;
		fread(&buff[66], 1, 65, pass);
		fclose(pass);
		if (strcmp(buff, av[1]) == 0)
			execl("/bin/sh", "sh", NULL);
		else
			puts(&buff[66]);
	}
	return (0);
}

