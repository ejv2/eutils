#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	unsigned int test = 1;
	if (htonl(test) == test) {
		puts("big");
	} else {
		puts("little");
	}
}
