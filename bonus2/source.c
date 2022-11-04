#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int	language = 0;

void	greetuser(char *buff)
{
	char	greeting[72];

	if (language == 1)
		strcpy(greeting, "Hyvää päivää ");
	else if (language == 2)
		strcpy(greeting, "Goedemiddag! ");
	else if (language == 0)
		strcpy(greeting, "Hello ");
	puts(strcat(greeting, buff));
  return;
}

int	main(int ac, char **av)
{
	char	buff[76];
	char	*env = NULL;

	if (ac == 3){
    memset(buff, 0, 76);
    strncpy(buff, av[1], 40);
    strncpy(&buff[40], av[2], 32);
    env = getenv("LANG");
    if (env)
    {
      if (memcmp(env, "fi", 2) == 0)
        language = 1;
      else if (memcmp(env, "nl", 2) == 0)
        language = 2;
    }
    greetuser(buff);
  }
  else
		return (1);
	return (0);
}

