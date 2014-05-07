
#ifndef __IHEX_H__
#define __IHEX_H__

#include <stdint.h>

typedef enum {
	DATA_RECORD = 0x00,
	EOF_RECORD  = 0x01, /* End Of File */
	ESA_RECORD  = 0x02, /* Extended Segment Address */
	SSA_RECORD  = 0x03, /* Start Segment Address */
	ELA_RECORD  = 0x04, /* Extended Linear Address */
	SLA_RECORD  = 0x05  /* Start Linear Address */
} ihex_record_type_t;

/* Represents an Intel .hex record. */
typedef struct {
	uint8_t byte_count;
	uint16_t address;
	uint8_t record_type;
	uint8_t data[UINT8_MAX + 1];
	uint8_t checksum;
} ihex_record_t;

typedef enum {
	/* None error */
	IHEX_ERROR_NONE,

	/* Miscellaneous format error. */
	IHEX_ERROR_INVALID_FORMAT,

	/* Invalid hexadecimal character encountered (other than line endings and
	 * record-delimiting colon (':') characters). Unable to extract meaningful
	 * data. */
	IHEX_ERROR_NON_HEX_CHARACTER,

	/* Individual record checksum failure. */
	IHEX_ERROR_INVALID_CHECKSUM,
} ihex_error_t;

ihex_error_t ihex_decode_record(char *line, int len, ihex_record_t *r);

#endif // __IHEX_H__

