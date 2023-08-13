#include "hal.h"
#include "fat.h"
#include "linkedlist.h"
FATnoti FAT_Read_Directory(uint32_t startSector, uint8_t *numFiles)
{
	printf("\n");	
	FATnoti retStatus = READ_DIR_OK;
    uint8_t buffer[SECTOR_SIZE];
    printf("=======================================================================================\n");
    printf("================================== READ FILE FAT GROUP 1 ==================================\n");   
    printf("=======================================================================================\n");
    printf("=======================================================================================\n");
    printf("| FILE NAME |\t| EXTENSION |\t| CREATE DATE |\t| CREATE TIME |\t| File Size |\n\n");   
    uint8_t countNumFiles = 0;
    uint32_t j;
	if ( READ_SECTOR_OK == HAL_Read_Sector(startSector, buffer))
	{
		for (j = 0; j < SECTOR_SIZE; j += ENTRY_SIZE)
    	{
        	if (buffer[j] == 0x2E)
        	{
            	continue;
        	}

        	if (buffer[j] == 0x00)
        	{
            
            	break;
        	}

        	if (buffer[j] == 0xE5)
        	{
            	
            	continue;
        	}
        	if (buffer[j + 11] == 0x0F)
        	{
            	
            	continue;
        	}

        	countNumFiles++;

        	readFat12Entry( &buffer[j], &entry);

        	printFat12Entry(&entry, countNumFiles);
    	}

    	(*numFiles) = countNumFiles;
    	retStatus = READ_DIR_OK;
    	
    	if (buffer[0] == 0x2E && buffer[33] == 0x2E && j <= 64)
    	{
        	printf("This Folder is empty. Please go back.\n");
    	}
	}
	else
	{
		retStatus = READ_DIR_ERR;
		printf("Error: Read Directory !");
	}

	printf("\n");

    return retStatus;
}

FATnoti FAT_Read_FileContent( uint32_t startCluster)
{
	printf("\n");
	
	if (entry.fileSize == 0)
    {
        printf("This file is empty. Please go back.\n");
        return;
    }
	
	FATnoti retStatus = READ_CONTENT_OK;
    uint8_t buffer[SECTOR_SIZE];
    
	if ( 0 == fseek(ptr, startCluster * SECTOR_SIZE, SEEK_SET))
	{
		while (entry.fileSize > 0)
	    {
	        uint32_t bytesRead = fread(buffer, sizeof(uint8_t), SECTOR_SIZE, ptr);
	        if (bytesRead == 0)
	        {
	            printf("End of File\n");
	            break;
	        }
	
	        // Print the buffer content to the console
	        uint32_t printSize = (entry.fileSize < bytesRead) ? entry.fileSize : bytesRead;
	
	        uint32_t i;
	        for (i = 0; i < printSize; i++)
	        {
	            printf("%c", buffer[i]);
	        }
	        entry.fileSize -= printSize;
	
	        if (entry.fileSize == 0)
	            break;
	    }
	    printf("\n");
	    printf(
		"\n");
	}
	else 
	{
		retStatus = SEEK_CONTENT_ERR;
		printf("Error: Read File Content !");
	}

    return retStatus;
}

FATnoti FAT_Get_StartCluster( uint32_t startSector, uint8_t selectedFile, uint32_t *startCluster)
{
	FATnoti retStatus 	  = GET_STARTCLUSTER_OK;
	uint32_t 	i = 0;
    uint32_t 	j = 0;
    uint8_t 	buffer[SECTOR_SIZE];
    
	
	if ( READ_SECTOR_OK == HAL_Read_Sector( startSector, (Fat12BootData*)buffer))
	{
		while (j < selectedFile)
	    {
	        if (buffer[i + 11] == 0x0F || buffer[i] == 0xE5 || buffer[i] == 0x2E)
	        {
	            i += ENTRY_SIZE;
	            continue;
	        }	
	        j++;
	
	        if (j == selectedFile)
	        {
	            readFat12Entry(&buffer[i], &entry);
	            break;
	        }	
	        i += ENTRY_SIZE;
	    }	
	    if (entry.attributes & 0x10)
	    {
	        entry.isDirectory = true;
	    }
	    else
	    {
	        entry.isDirectory = false;
	    }
	
	    uint32_t data_region_start_sector = BootData.reserved_sectors + (BootData.number_of_fats * BootData.fat_size_sectors) + (BootData.root_dir_entries * 32 / BootData.bytes_per_sector);
	    (*startCluster) = ((entry.startCluster - 2) * BootData.sectors_per_cluster) + data_region_start_sector;
	}
	else 
	{
		retStatus = GET_STARTCLUSTER_ERR;
		printf("Error: Get Start Cluster !");
	}
	
	return retStatus;    
}
 uint32_t read_fattable(uint32_t poscluster)
{ FILE*file=NULL;
	char hex[6];
	uint8_t bytes[2];
	uint32_t Fatcluster;
	uint32_t physicalcluster=poscluster*1.5+1;
	uint32_t PhysicalSector;
	if(poscluster%3==0)
	{ 
	//fseek(file, dectoHex(physicalcluster,hex,5), SEEK_SET);
	fread(&bytes,sizeof(uint8_t),2,file);
	Fatcluster=(bytes[1]&0x0f<<2)|bytes[0];
	
	}
	if(poscluster%3==2)
	{
	//fseek(file, dectoHex((physicalcluster-1),hex,5), SEEK_SET);
	fread(&bytes,sizeof(uint8_t),2,file);
	Fatcluster=(bytes[0]&0xf0)|(bytes[1]<<1);
	}
	
	return PhysicalSector=Fatcluster+33-2;;
	
}
 void readFat12Entry(uint8_t *entryData, Fat12Entry *entry)
{
    
    memcpy(entry->filename, entryData, 8);
    entry->filename[8] = '\0';
    memcpy(entry->extension, entryData + 8, 8);
    entry->extension[8] = '\0';
    entry->attributes = entryData[11];
    convertFat12DateTime(*(uint16_t *)(entryData + 16), *(uint16_t *)(entryData + 14), &entry->creationTime);
    entry->startCluster = *(uint16_t *)(entryData + 26);
    entry->fileSize = *(uint32_t *)(entryData + 28);
}

 void convertFat12DateTime(uint16_t date, uint16_t time, Fat12DateTime *dateTime)
{
    // Extract the year, month, and day from the date
    dateTime->year = ((date >> 9) & 0x7f) + 1980;
    dateTime->month = (date >> 5) & 0x0f;
    dateTime->day = date & 0x1f;

    // Extract the hour, minute, and second from the time
    dateTime->hour = (time >> 11) & 0x1f;
    dateTime->minute = (time >> 5) & 0x3f;
    dateTime->second = (time & 0x1f) * 2;
}
 void printFat12Entry(Fat12Entry *entry, uint8_t numFiles)
{
    printf("%d/ ", numFiles);
    printf("%s\t", entry->filename);
    if (entry->attributes & 0x10)
    {
        printf("FILE FOLDER\t");
    }
    else
    {
        printf("FILE %s\t", entry->extension);
    }
    printf("%04d-%02d-%02d\t",
           entry->creationTime.year, entry->creationTime.month, entry->creationTime.day
          );
    printf("%04d:%02d:%02d \t",
            entry->creationTime.hour, entry->creationTime.minute, entry->creationTime.second);
    
    if (entry->fileSize != 0)
    {
        printf("%lub\n", entry->fileSize);
    }
    else
    {
        printf("\n");
    }
}
