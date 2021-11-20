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

const char *ver = "1.1.0";

const char *opts = "s:S:e:b:B:dwaVhuv";

uint32_t sleep_time = 0;		/* enable sleeping between iterations */
uint32_t exit_time = 0;			/* enable sleeping before exit */

bool cap = false;			/* enable iteration cap */
uint32_t stop_at = 0;
bool byte_cap = false;			/* enable byte terminator */
uint64_t stop_bytes = 0;

bool dry = false;			/* enable dry run */
bool nacc = false;			/* disable accumulation */
bool wt = false;			/* enable write testing */
size_t blk = sizeof(uint64_t);		/* byte size of each allocation block */

char header = '\r';

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
	puts("\tB: Memblock size");
	puts("\td: Dry run; allocate no memory");
	puts("\ta: Disable acumulative mode; free memory after each pass");
	puts("\tw: Write test memory; write a test value to the end of allocated "
		 "buffer");
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
			printf("Cap placed at %u iterations\n", stop_at);
			break;
		case 'b':
			byte_cap = true;
			stop_bytes = strtol(optarg, NULL, 10);
			printf("Byte cap placed at %lu bytes\n", stop_bytes);
			break;
		case 'B':
			blk = strtol(optarg, NULL, 10);
			printf("Set memblock size to %li bytes\n", blk);
			break;
		case 'd':
			puts("Enabled dry run");
			dry = true;
			break;
		case 'w':
			puts("Memory write test enabled");
			wt = true;
			break;
		case 'a':
			puts("Disabled accumulation");
			nacc = true;
			break;
		case 'V':
			puts("Verbosity enabled");
			header = '\n';
			break;
		case 'h':
			help();
			return 1;
		case 'u':
			usage();
			return 1;
		case 'v':
			printf("mdest version %s, built at %s %s\n", ver, __TIME__,
				   __DATE__);
			return 1;
		default:
			return -1;
		}
	}

	uint64_t allocd = 0;
	uint64_t iterations = 1;
	char *ptr = 0x0;

	while (true) {
		uint64_t alloc;
		if (nacc)
			alloc = blk;
		else
			alloc = blk * iterations;

		if (!dry) {
			ptr = malloc(alloc);
		}

		allocd += alloc;

		printf(
			"%cPass %li: Allocated %li bytes (total %li bytes) at address %p",
			header, iterations, alloc, allocd, ptr);

		if (wt && !dry)
			ptr[alloc - 1] = 127;

		iterations++;

		if ((cap && iterations >= stop_at) ||
			(byte_cap && allocd >= stop_bytes)) {
			putchar('\n');
			return 0;
		}

		if (sleep_time > 0) {
			sleep(sleep_time);
		}
	}
}
