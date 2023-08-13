#include "hal.h"
#include "fat.h"

FATnoti HAL_OpenDisk(void)
{
FATnoti retStatus = DISK_OK;
    ptr = fopen("C://Users//NCPC//Desktop//New folder (2)//floppy.img", "rb");

    if (ptr == NULL)
    {
    	retStatus = DISK_ERR_OPEN;
        printf("Error: could not open file.\n");
    }
    
    return retStatus;
}

FATnoti HAL_CloseDisk(void)
{
	FATnoti retStatus = DISK_OK;
	if ( 0 != fclose( ptr))
	{
		retStatus = DISK_ERR_CLOSE;
	}
	
	return retStatus;
}

FATnoti HAL_Read_Sector(uint32_t index, uint8_t *buffer)
{
	FATnoti retStatus = READ_SECTOR_OK;
    uint32_t sector_offset = index * SECTOR_SIZE;

    if ( 0 != fseek(ptr, sector_offset, SEEK_SET))
    {
    	retStatus = SEEK_SECTOR_ERR;
    	printf("Error: Could not seek sector !");
    	return retStatus;
	}
	
	if ( SECTOR_SIZE != fread(buffer, 1, SECTOR_SIZE, ptr))
	{
		retStatus = READ_SECTOR_ERR;
		printf("Error: Read Sector !");
	}
    
    return retStatus;
}


