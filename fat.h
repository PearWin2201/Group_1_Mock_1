#ifndef __FAT12_H__
#define __FAT12_H__
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#pragma pack(push, 1) // exact fit - no padding
typedef struct
{
    uint8_t jmp[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t number_of_fats;
    uint16_t root_dir_entries;
    uint16_t total_sectors_short; // if zero, later field is used
    uint8_t media_descriptor;
    uint16_t fat_size_sectors;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_long;

    // used by FAT12 and FAT16
    uint8_t drive_number;
    uint8_t current_head;
    uint8_t boot_signature;
    uint32_t volume_id;
    char volume_label[11];
    char fs_type[8]; // typically contains "FAT12   "
}  Fat12BootData;


typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} Fat12DateTime;

typedef struct
{
    uint8_t filename[9];   // +1 for null terminator
    uint8_t extension[4]; // +1 for null terminator
    uint8_t attributes;
    Fat12DateTime creationTime;
    uint16_t lastAccessTime;
    uint16_t lastModifiedTime;
    uint16_t startCluster;
    uint32_t fileSize;
    bool isDirectory;
} Fat12Entry;
Fat12BootData 	BootData;
Fat12Entry 		entry;
FATnoti FAT_Read_Directory(uint32_t startSector, uint8_t *numFiles);
FATnoti FAT_Read_FileContent(uint32_t startCluster);
FATnoti FAT_Get_StartCluster(uint32_t startSector, uint8_t selectedFile, uint32_t *startCluster);
void readFat12Entry(uint8_t *entryData, Fat12Entry *entry);
void convertFat12DateTime(uint16_t time, uint16_t date, Fat12DateTime *dateTime);
void printFat12Entry(Fat12Entry *entry, uint8_t numFiles);
uint32_t read_fattable(uint32_t poscluster);
#endif
