/*
 * A utility to destroy your system and wreak
 * havoc on the kernel.
 *
 * Steadily allocates more and more memory until
 * killed or crashed by you or the kernel.
 *
 * WARNING: This program WILL allocate in the realms
 * of GIGABYTES of memory and will allocate EXPONENTIALLY
 * more if allowed to run for long. DO NOT leave to run
 * unattended. YOU WILL loose data and/or CRASH YOUR SYSTEM.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <inttypes.h>
#include <unistd.h>

const char *ver = "1.0.0";

const char *opts = "s:S:e:b:dahuv";

uint32_t sleep_time = 0;
uint32_t exit_time = 0;

bool cap = false;
uint32_t stop_at = 0;
bool byte_cap = false;
uint64_t stop_bytes = 0;

bool dry = false;
bool nacc = false;

void usage()
{
	printf("mdest [-%s]: allocate way too much memory\n", opts);
}

void help()
{
	usage();

	puts("\nOptions:");
	puts("\ts: Sleep time between passes");
	puts("\tS: Sleep time between completion and exit");
	puts("\te: Exit at this many passes");
	puts("\tb: Exit at this many bytes");
	puts("\td: Dry run; allocate no memory");
	puts("\ta: Disable acumulative mode; free memory after each pass");
	puts("\th: This message");
	puts("\tu: Usage");
	puts("\tv: Version");

	puts("\nmdest: Copyright (C) 2021 - Ethan Marshall");
	puts("Please don't use this to destroy people's computers :(");
}

void halt()
{
	sleep(exit_time);
}

int main(int argc, char **argv)
{
	char opt;
	while ((opt = getopt(argc, argv, opts)) != -1) {
		switch (opt) {
		case 's':
			sleep_time = strtoimax(optarg, NULL, 10);
			break;
		case 'S':
			exit_time = strtoimax(optarg, NULL, 10);
			atexit(halt);
			break;
		case 'e':
			cap = true;
			stop_at = strtoimax(optarg, NULL, 10);

			printf("Cap placed at %i iterations", stop_at);

			break;
		case 'b':
			byte_cap = true;
			stop_bytes = strtol(optarg, NULL, 10);
			break;
		case 'd':
			puts("Enabled dry run");
			dry = true;
			break;
		case 'a':
			puts("Disabled accumulation");
			nacc = true;
			break;
		case 'h':
			help();
			return 1;
		case 'u':
			usage();
			return 1;
		case 'v':
			printf("mdest version %s, built at %s %s\n", ver, __TIME__, __DATE__);
			return 1;
		default:
			return -1;
		}
	}

	uint64_t allocd = 0;
	uint64_t iterations = 1;
	void *ptr = 0x0;

	while (true) {
		uint64_t alloc = sizeof(uint64_t) * iterations;

		if (!dry) {
			ptr = malloc(alloc);
		}

		if (nacc) {
			free(ptr);
			allocd = alloc;
		} else {
			allocd += alloc;
		}

		printf("Pass %li: Allocated %li bytes (total %li bytes) at address %p\n",
				iterations,
				alloc,
				allocd,
				ptr);

		iterations++;

		if ((cap && iterations >= stop_at) || (byte_cap && allocd >= stop_bytes)) {
			return 0;
		}

		if (sleep_time > 0) {
			sleep(sleep_time);
		}
	}

}
