#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "fat.h"

static uint8_t FAT_ChartoDec(uint8_t Character);
static uint32_t FAT_StrtoDec(uint8_t * pString, uint8_t Len);

uint32_t RootAddr;
uint32_t* pDecimal = 0;
FILE *ptr = NULL;

/* void CheckBoot(ptr)
{
	char * OutCheck[2];
	fseek(ptr, 0x1FE, SEEK_SET);
	fgets(OutCheck, 2, ptr);
	if ((OutCheck[0] != '55') || (OutCheck[1] != 'AA'))
	else CalSec(ptr)
} */

BootPara CalSec(FILE *ptr)
{
	BootPara Sec;
	
	// Calculate number of Boot sectors
	char NumSecBoot[2];
	fseek(ptr, 0x0e, SEEK_SET);
	fgets(NumSecBoot, 2, ptr);
	Sec.NumSec_Boot = FAT_StrtoDec(NumSecBoot, 2);
	
	//Calculate number of sectors in FAT tables
	char *NumTabFAT;
	fseek(ptr, 0x10, SEEK_SET);
	fgets(NumTabFAT, 1, ptr);
	Sec.NumTab_FAT = FAT_StrtoDec(NumTabFAT, 1);
	
	char *NumSecFATTab;
	fseek(ptr, 0x16, SEEK_SET);
	fgets(NumSecFATTab, 2, ptr);
	Sec.NumSec_FATTab = FAT_StrtoDec(NumSecFATTab, 2);
	
	//Calculate entries of Root Directory
	char * NumEntRoot;
	fseek(ptr, 0x11, SEEK_SET);
	fgets(NumEntRoot, 2, ptr);
	Sec.NumEnt_Root = FAT_StrtoDec(NumEntRoot, 2);
	
	return Sec;
}

uint32_t CalRootAddr(FILE *ptr)
{
	BootPara Sec = CalSec(ptr);
	RootAddr = Sec.NumSec_Boot * 512 + Sec.NumTab_FAT * Sec.NumSec_FATTab * 512;
	return RootAddr;
}

RootPara GetRootData(FILE *ptr)
{
	ptr = NULL;
	uint8_t i, k = CalSec(ptr).NumEnt_Root;
	RootPara Root[k];
	ptr = CalRootAddr(ptr);
	for (i = 0; i < k; i++)
	{
		fgets(FileName, 8, ptr);
		Root[k].FileName = FAT_StrtoDec(FileName, 8);
		fgets(Extension, 3, ptr);
		Root[k].Extension = FAT_StrtoDec(Extension, 3);
		fgets(Extension, 3, ptr);
		Root[k].Attribute = FAT_StrtoDec(Attribute, 1);
		fgets(Extension, 3, ptr);
		Root[k].Reserved = FAT_StrtoDec(ptr, 10);
		fgets(Extension, 3, ptr);
		Root[k].Time = FAT_StrtoDec(ptr, 2);
		Root[k].Date = FAT_StrtoDec(ptr, 2);
		Root[k].FirstCluster = FAT_StrtoDec(ptr, 2);
		Root[k].FileSize = FAT_StrtoDec(ptr, 4);
	}
	return Root[k];
}

uint8_t GetFirstDataAddr(uint32_t m)
{
	uint32_t FirstDataAddr;
	FirstDataAddr = 512 * (31 + (GetRootData(ptr)[m].FirstCluster));
	return FirstDataAddr;
}

void FAT_OpenFile(uint8_t * FileName)
{
	fopen(FileName, "r");
}

void FAT_CloseFile(void)
{
	fclose(ptr);
}
//uint32_t ReadFile(uint8_t * Root[32])
//{
//	uint32_t count = 0;
//	uint32_t FirstCluster, FirstData;
//	uint32_t i;
//	count++;
//	FirstCluster == 16 * &(*Root + 0x1B) + &(*Root + 0x1A);
//	FirstData == 512 * (FirstCluster + 31);
//	return FirstData;
//}
//
//ReadEntryList(void)
//{
//	
//}

//FirstData CheckAttribute(Root[])
//{
//	uint8_t i;
//	uint8_t j = 0, k = 0;
//	FirstData Out;
//	for (i = 0x260B; i <= 0x41FF; i += 32)
//	{
//		if (Root[i] = 0x0F)
//		continue;
//		else if (Root[i] == 0x00)
//		{
//			FirstCluster = 16 * Root[0x1B] + Root[0x1A];
//			Out.FileFirstData[j] = 512 * (FirstCluster + 31);
//			j++;
//		}
//		else if (Root[i] == 0x10)
//		{
//			FirstCluster = 16 * Root[0x1B] + Root[0x1A];
//			Out.FolderFirstData[k] = 512 * (FirstCluster + 31);
//			k++;
//		}
//	}
//	return Out;
//}


//uint8_t CalDataAddr(uint16_t Num)
//{
//	uint16_t DataAddr;
//	DataAddr = 512 * (Num + 31);
//	return DataAddr;
//}

static uint8_t FAT_ChartoDec(uint8_t Character)
{
	uint8_t retVal;
	// '0' - '9'
	if ( ('0' <= Character) && (Character <= '9') )
	{
		retVal = Character - '0';
	}
	else if ( ('A' <= Character) && (Character <= 'F') )
	{
		retVal = Character - 'A' + 10;
	}
	else
	{
		retVal = HEX_INVALID;
	}
	return retVal;
}

static uint32_t FAT_StrtoDec(uint8_t * pString, uint8_t Len)
{
	// "13" = 1 * 16^1 + 3 * 16^0 = 19
	// "7AF0" = 
	// 	7 * 16^3 + 0xA * 16^2 + 0xF * 16^1 + 0 * 16^0 = 31472
	uint8_t idx;
	uint8_t dec;
	// Reset value of output data. When declare the variable * pDecimal, the value of variable != 0;
	for (idx = Len -1; idx >= 0; idx--)
	{
		dec = FAT_ChartoDec(pString[idx]);
		if (HEX_INVALID == dec)
		{
			break;
		}
		else
		{
			*pDecimal += dec * pow(16, (Len-1-2*idx));
		}
	}
	return *pDecimal;
}




