#include <stdint.h>
#include <stdbool.h>
 

static void ATA_wait_BSY();
static void ATA_wait_DRQ();
void sectors_read_ATAPIO(uint32_t *target_address, uint32_t LBA, uint8_t sector_count);
void sectors_write_ATAPIO(uint32_t LBA, uint8_t sector_count, uint32_t *bytes);