//
// Created by kacper on 26.01.17.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <glob.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>

#define BLOCK_SIZE 2048
#define MAX_NAME_LENGTH 24
#define MAX_FILES_NUMBER 64
#define INODE_BMP_ADRESS sizeof(SUPERBLOCK)
#define DBLOCKS_BMP_ADRESS sizeof(SUPERBLOCK) + sizeof(char)*MAX_FILES_NUMBER

typedef uint16_t WORD;


typedef struct Block{
    char data[BLOCK_SIZE];
    WORD next_dblock_index;
} BLOCK;

typedef struct INode{
    char name[MAX_NAME_LENGTH];
    unsigned long int size;
    WORD dblock_index;
} INODE;

typedef struct SuperBlock{
    char disk_name[MAX_NAME_LENGTH];
    WORD size;
    WORD inode_count;
    WORD dblock_count;
    WORD free_dblocks;
    WORD full_dblocks;
} SUPERBLOCK;

typedef struct SystemPointers{
    SUPERBLOCK *superblock;
    INODE *temp_inode;
    char inode_bmp[MAX_FILES_NUMBER];
    char *dblocks_bmp;
} SYSPOINT;

int create_virtual_disk(const char *filename, WORD size);
int copy_from_virtual_disk(const char *filename, const char* virtual_disk_name);
int copy_to_virtual_disk(const char *filename, const char *virtual_disk_name);
int display_main_catalog(const char *filename);
int delete_file(const char *filename);
int delete_virtual_disk(const char *filename);
int map_virtual_disk(const char *filename);

WORD convert_size_to_blocks(long size);
int update_disk_data(FILE *file_handler, SYSPOINT *pointers);
unsigned long int get_file_size(FILE *file_handler);
int check_if_file_exists(char *filename, FILE *disk_handler);
WORD find_file_on_disk(const char *filename, SYSPOINT *pointers, FILE *handler);
WORD free_dblocks_count(FILE *disk_handler);
size_t getIndexOfFirstFreeInode(char *inode_bmp);
void read_information_region(FILE *file_handler,SUPERBLOCK *superblock, char *inode_bmp, char *dblocks_bmp);
int display_virtual_disk_info(char *filename);
int allocate_system_pointers(FILE *handler, SYSPOINT *sys_pointers);
void free_system_pointers(SYSPOINT *pointers);
WORD find_first_free_inode(SYSPOINT *pointers);
WORD find_first_taken_inode(SYSPOINT *pointers);
WORD calculate_inode_offset(WORD index, SYSPOINT *pointers);
WORD find_first_free_dblock(SYSPOINT *pointers);
WORD calculate_dblock_offset(WORD index, SYSPOINT *pointers);

void aaa(const char *filename);
void bbb(const char *filename);
#endif //FILESYSTEM_FILESYSTEM_H
