#include<stdint.h>
#include <kernel/ata.h>
#include <kernel/com.h>

/*
BSY: Indicates the drive is preparing to send/receive data (wait for it to clear). In case of 'hang' (it never clears), do a software reset.
DRQ: Set when the drive has PIO data to transfer, or is ready to accept PIO data.
RDY: Bit is clear when drive is spun down, or after an error. Set otherwise.
ERR: Indicates an error occurred. Send a new command to clear it (or nuke it with a Software Reset).
IDX: Index. Always set to zero.
CORR: Corrected data. Always set to zero.
DF:	Drive Fault Error (does not set ERR).
*/

#define BSY_STATUS 0x80
#define RDY_STATUS 0x40
#define DRQ_STATUS 0x08
#define DF_STATUS 0x20
#define ERR_STATUS 0x01

//here assuming ATA bus 0 master 
//Source - OsDev wiki
void sectors_read_ATAPIO(uint32_t *target_address, uint32_t LBA, uint8_t sector_count)
{

	ATA_wait_BSY();
	outportb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	outportb(0x1F1,0x00);
	outportb(0x1F2,sector_count);
	outportb(0x1F3, (uint8_t) LBA);
	outportb(0x1F4, (uint8_t)(LBA >> 8));
	outportb(0x1F5, (uint8_t)(LBA >> 16)); 
	outportb(0x1F7,0x20); //Send the read command

	uint16_t *target = (uint16_t*) target_address;

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++){
			target[i] = inports(0x1F0);
		}
	}
}

void sectors_write_ATAPIO(uint32_t LBA, uint8_t sector_count, uint32_t* bytes)
{
	ATA_wait_BSY();
	outportb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	outportb(0x1F1,0x00);
	outportb(0x1F2,sector_count);
	outportb(0x1F3, (uint8_t) LBA);
	outportb(0x1F4, (uint8_t)(LBA >> 8));
	outportb(0x1F5, (uint8_t)(LBA >> 16)); 
	outportb(0x1F7,0x30); //Send the write command

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++)
		{
			outportl(0x1F0, bytes[i]);
		}
	}
}

static void ATA_wait_BSY()   //Wait for BSY to be 0
{
	while(inportb(0x1F7)&BSY_STATUS);
}
static void ATA_wait_DRQ()  //Wait fot DRQ to be 1
{
	while(!(inportb(0x1F7)&RDY_STATUS));
}


void init_ata() {
    
    printf("SnowflakeOS.\r\n");
    printf("Memory address in: 0x%x\r\n\r\n", init_ata);

    printf("here reading...\r\n");

    uint32_t* target;

    sectors_read_ATAPIO(target, 0x0, 1);
    
    int i;
    i = 0;
    while(i < 128)
    {
        printf("%x ", target[i] & 0xFF);
        printf("%x ", (target[i] >> 8) & 0xFF);
        i++;
    }

    printf("\r\n");
    printf("here writing 0...\r\n");
    char bwrite[512];
    for(i = 0; i < 512; i++)
    {
        bwrite[i] = 0x0;
    }
    sectors_write_ATAPIO(0x0, 2, bwrite);


    printf("here reading...\r\n");
    sectors_read_ATAPIO(target, 0x0, 1);
    
    i = 0;
    while(i < 128)
    {
        printf("%x ", target[i] & 0xFF);
        printf("%x ", (target[i] >> 8) & 0xFF);
        i++;
    }
}