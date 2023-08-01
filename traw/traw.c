/*
 * traw -- view raw terminal input bytes
 * Copyright (C) 2023 - Ethan Marshall
 */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

static FILE *tty;
static struct termios termios, termios_orig;

void init_tty(int fileno)
{
	if (!isatty(fileno))
		return;

	tcgetattr(fileno, &termios_orig);
	memcpy(&termios, &termios_orig, sizeof(termios_orig));

	termios.c_lflag &= ~ICANON;
	termios.c_lflag &= ~ECHO;
	termios.c_lflag &= ~ISIG;

	tcsetattr(fileno, TCSANOW, &termios);
}

void reset_tty(int fileno)
{
	tcsetattr(fileno, TCSANOW, &termios_orig);
}

void teardown()
{
	reset_tty(fileno(tty));
	_exit(0);
}

int main(int argc, char **argv)
{
	tty = fopen("/dev/tty", "r+");
	init_tty(fileno(tty));

	signal(SIGTERM, teardown);

	for (;;) {
		printf("0x%X\n",  fgetc(tty));
	}

	reset_tty(fileno(tty));
	fclose(tty);
}
