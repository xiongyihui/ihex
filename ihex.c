
#include "ihex.h"

static ihex_error_t ihex_get_byte(char *src, uint8_t *dst, uint8_t *checksum)
{
    int i;
    uint8_t digital;
    uint8_t result;
    

    digital = *src - '0';
    if (digital < 10) {
        result = digital << 4;
    } else {
        digital = *src - 'A';
        if (digital < 6) {
            result = (digital + 10) << 4;
        } else {
            return IHEX_ERROR_NON_HEX_CHARACTER;
        }
    }
    
    src++;
    digital = *src - '0';
    if (digital < 10) {
        result += digital;
    } else {
        digital = *src - 'A';
        if (digital < 6) {
            result += (digital + 10);
        } else {
            return IHEX_ERROR_NON_HEX_CHARACTER;
        }
    }
    
    *dst = result;
    *checksum += result;
    
    return IHEX_ERROR_NONE;
}

static ihex_error_t ihex_get_2bytes(char *src, uint16_t *dst, uint8_t *checksum)
{
    ihex_error_t err;
    uint8_t b1, b2;
    
    err = ihex_get_byte(src, &b1, checksum);
    if (IHEX_ERROR_NONE == err) {
		err = ihex_get_byte(src + 2, &b2, checksum);
		if (IHEX_ERROR_NONE == err) {
			*dst = (b1 << 8) + b2;
		}
	}
	
	return err;
}

static ihex_error_t ihex_get_nbytes(char *src, uint8_t *dst, uint8_t nbytes, uint8_t *checksum)
{
	ihex_error_t err = IHEX_ERROR_NONE;
	int i;

	for (i = 0; i < nbytes; i++) {
		err = ihex_get_byte(src + 2 * i, dst + i, checksum);
		if (IHEX_ERROR_NONE != err) {
			break;
		}
	}

	return err;
}

ihex_error_t ihex_decode_record(char *line, int len, ihex_record_t *r)
{
    uint8_t checksum = 0;
    uint8_t *ptr = line;
    
    if (*ptr != ':') {
        return IHEX_ERROR_INVALID_FORMAT;
    }
    ptr++;
    
    ihex_get_byte(ptr, &(r->byte_count), &checksum);
    if (len != (11 + 2 * r->byte_count)) {       // 1 + 2 + 4 + 2 + 2 * n + 2
        return IHEX_ERROR_INVALID_FORMAT;
    }  
    ptr += 2;

    
    ihex_get_2bytes(ptr, &(r->address), &checksum);   
    ptr += 4;
    
    ihex_get_byte(ptr, &(r->record_type), &checksum);
    ptr += 2;
    
    ihex_get_nbytes(ptr, (uint8_t *)&(r->data), r->byte_count, &checksum);
    ptr+= 2 * r->byte_count;
    
    ihex_get_byte(ptr, &(r->checksum), &checksum);
    if (0 == checksum) {
        return IHEX_ERROR_NONE;
    } else {
        return IHEX_ERROR_INVALID_CHECKSUM;
    }
}


