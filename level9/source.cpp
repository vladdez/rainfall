#include <stdlib.h>
#include <cstring>

class N {
	public:
		char buff[100];
		int nbr;

		N(int n) {
			nbr = n;
		}
		void setAnnotation(char *str) {
			memcpy(buff, str, strlen(str));
		}
		virtual int operator+(N &n) {
			return (nbr + n.nbr);
		}
		virtual int operator-(N &n) {
			return (nbr - n.nbr);
		}
};

int main(int argc, char **argv) {
	if (argc <= 1)
		exit(1);

	N *n1 = new N(5);
	N *n2 = new N(6);

	n1->setAnnotation(argv[1]);
	return (*n2 + *n1);
}