
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ihex.h"

ihex_record_t r;
char line[64];

int readline(char *buf, int size)
{
    char get;
    int  len = 0;

    do {
        get = getc(stdin);
        if (get == '\n' || get == '\r' && len != 0) {
            return len;
        } else {
            buf[len] = get;
            len++;
        }
    } while (len < size);

    return len;
}

int main(int argc, char *argv[]) {
    ihex_error_t err;
	int count = 0;

	do {
        int len = readline(line, sizeof(line));
        err = ihex_decode_record(line, len, &r);

		if (IHEX_ERROR_NONE != err) {
			printf("Got error 0x%02X, aborting due to invalid record!\n", err);
			exit(1);
		}

		switch (r.record_type) {
			case DATA_RECORD:
				printf("Got data record with length %u\n", r.byte_count);
				count++;
				break;
			case EOF_RECORD:
				puts("Got EOF record");
				break;
			case ESA_RECORD:
				puts("Got ESA record");
				break;
			case SSA_RECORD:
				puts("Got SSA record");
				break;
			case ELA_RECORD:
				puts("Got ELA record");
				printf("LA = 0x%X\n", ((r.data[0] << 8) + r.data[1]) << 16);
				break;
			case SLA_RECORD:
				puts("Got SLA record");
				break;
		}
	} while (r.record_type != EOF_RECORD);
	
	return EXIT_SUCCESS;
}

