#ifndef _FAT_H_
#define _FAT_H_

#include <stdio.h>
#include <stdint.h>

#define HEX_INVALID (0xFFU)

typedef struct
{
	uint8_t NumSec_Boot;
	uint8_t NumTab_FAT;
	uint8_t NumSec_FATTab;
	uint8_t NumEnt_Root;
} BootPara;

typedef struct
{
	uint64_t FileName;
	uint8_t Extension[3];
	uint8_t Attribute;
	uint8_t Reserved[10];
	uint16_t Time;
	uint16_t Date;
	uint16_t FirstCluster;
	uint32_t FileSize;
} RootPara;

typedef struct
{
	uint32_t FileFirstData[10];
	uint32_t FolderFirstData[10];
} FirstData;

BootPara CalSec(FILE *);
uint32_t CalRootAddr(FILE *);
RootPara GetRootData(FILE *);
uint8_t GetFirstDataAddr(uint32_t);
void FAT_OpenFile(uint8_t *);
void FAT_CloseFile(void);

#endif /* _FAT_H_ */


