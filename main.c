#include "hal.h"
#include "fat.h"
#include "linkedlist.h"

void main()
{
	if ( DISK_OK == HAL_OpenDisk())
	{
		
	FATnoti retStatus 		= DISK_OK;
    LinkedList_t *	list 		= createLinkedList();
    uint8_t 		numFiles 	= 0;
    uint32_t 		startCluster;

    if ( READ_SECTOR_OK != HAL_Read_Sector(0, &BootData))
    {
    	return retStatus;
	}
    
    uint32_t rootDirStartSector = BootData.reserved_sectors + (BootData.number_of_fats * BootData.fat_size_sectors);
    
    if ( READ_DIR_OK != FAT_Read_Directory( rootDirStartSector, &numFiles))
    {
    	return retStatus;
	}

    addToHead(list, rootDirStartSector);                  	
    uint32_t currentAddress = list->head->currentAddress; 	

    while (1)
    {
        uint8_t selectedFile = -1;

        while (selectedFile < 0 || selectedFile > numFiles)
        {
            printf("Please select a file or folder (or press 0 to go back): ");
            scanf(" %d", &selectedFile);
            
            if (selectedFile < 0 || selectedFile > numFiles)
            {
                printf("PLEASE CHOOSE AGAIN.\n");
                break;
            }

            if (selectedFile == 0)
            {
            	system("cls");
                if (currentAddress == rootDirStartSector) 
				{		                                       	
                    printf("Exiting Program!\n");
                    freeLinkedList(list);
                    HAL_CloseDisk();
                    exit(0);
                }

                removeHead(list);   							
                currentAddress = list->head->currentAddress;    

                FAT_Read_Directory(currentAddress, &numFiles);  
            }
            else
            {
            	system("cls");
                FAT_Get_StartCluster(currentAddress, selectedFile, &startCluster); 

                addToHead(list, startCluster); 					

                currentAddress = list->head->currentAddress; 	

                if (entry.isDirectory)
                {
                    FAT_Read_Directory(currentAddress, &numFiles); 
                }
                else
                {
                    FAT_Read_FileContent(currentAddress); 			
                    numFiles = 0;
                }
            }
        }
    }
    
    return retStatus;
}
	
}
