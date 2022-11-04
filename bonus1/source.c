#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char **av)
{
	char buff[40];
	int nbr;

	nbr = atoi(av[1]);
	if (nbr < 10)
	{
		memcpy(buff, av[2], nbr * 4);
		if (nbr == 0x574f4c46)
			execl("/bin/sh", "sh", NULL);
	}
	else
		return (1);
	return (0);
}
