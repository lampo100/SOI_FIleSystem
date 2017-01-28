//
// Created by kacper on 26.01.17.
//
#include <errno.h>
#include "FileSystem.h"

int create_virtual_disk(const char *filename, WORD size) {
    SYSPOINT pointers;
    FILE *file_handler;
    WORD total_data_blocks_needed;
    WORD total_disk_size;
    WORD information_region_size;

    total_data_blocks_needed = convert_size_to_blocks(size);

    //Superblok + bitmapa inode'ów + bitmapa bloków danych + bloki danych
    information_region_size = (WORD) sizeof(SUPERBLOCK) + (WORD) sizeof(pointers.inode_bmp)
                              + (WORD) (total_data_blocks_needed * sizeof(char))
                              + (WORD) (sizeof(INODE) * MAX_FILES_NUMBER);

    //superblok + bitmapa inode'ów + bitmapa bloków danych
    total_disk_size = information_region_size + total_data_blocks_needed * sizeof(BLOCK);


    /*            Tworzenie dysku                                  */
    file_handler = fopen(filename, "w+b");
    if (!file_handler) {
        printf("Nie mozna bylo utworzyc dysku wirtualnego.\n");
        return 1;
    }
    if (strlen(filename) >= MAX_NAME_LENGTH) {
        printf("Utworzenie dysku nie powiodlo sie. Za dluga nazwa.\n");
        fclose(file_handler);
        return 1;
    }

    if (truncate(filename, total_disk_size)){
        printf("Zmiana rozmiaru dysku do docelowy nie udala sie.\n");
        return errno;
    }
    file_handler = fopen(filename, "r+b");

    /*                      Zapisywanie danych początkowych na dysk                   */
    pointers.superblock = (SUPERBLOCK *) malloc(sizeof(SUPERBLOCK));
    pointers.dblocks_bmp = (char *) malloc(total_data_blocks_needed * sizeof(char));
    memset(pointers.dblocks_bmp, 0, total_data_blocks_needed);
    memset(pointers.inode_bmp, 0, MAX_FILES_NUMBER);

    if (!pointers.superblock) {
        printf("Blad. Nie mozna bylo zarezerwowac pamieci na superblok dysku!\n");
        fclose(file_handler);
        return 2;
    }

    strncpy(pointers.superblock->disk_name, filename, 24);
    pointers.superblock->size = total_disk_size;
    pointers.superblock->inode_count = MAX_FILES_NUMBER;
    pointers.superblock->dblock_count = total_data_blocks_needed;
    // niemożliwa wartość oznaczająca brak zajętych inode'ów
    pointers.superblock->free_dblocks = total_data_blocks_needed;
    pointers.superblock->full_dblocks = 0;

    rewind(file_handler);
    if (fwrite(pointers.superblock, sizeof(SUPERBLOCK), 1, file_handler) != 1) {
        printf("Blad. Nie mozna bylo zapisac superbloku na dysku. Proba utworzenia dysku nie powiodla sie!\n");
        fclose(file_handler);
        return 3;
    }

    fseek(file_handler, INODE_BMP_ADRESS, SEEK_SET);
    if (fwrite(&pointers.inode_bmp, sizeof(pointers.inode_bmp), 1, file_handler) != 1) {
        printf("Blad. Nie mozna bylo zapisac bitmapy inode'ów na dysku. Proba utworzenia dysku nie powiodla sie!\n");
        fclose(file_handler);
        return 3;
    }
    fseek(file_handler, INODE_BMP_ADRESS + sizeof(pointers.inode_bmp), SEEK_SET);
    if (fwrite(pointers.dblocks_bmp, sizeof(pointers.dblocks_bmp), 1, file_handler) != 1) {
        printf("Blad. Nie mozna bylo zapisac bitmapy danych na dysku. Proba utworzenia dysku nie powiodla sie!\n");
        fclose(file_handler);
        return 3;
    }


    printf("Utworzono dysk wirtualny o nazwie: '%s' i pojemnosci: %uB\n", pointers.superblock->disk_name,
           total_disk_size);

    fclose(file_handler);
    free(pointers.superblock);
    free(pointers.dblocks_bmp);
    return total_disk_size;
}

int update_disk_data(FILE *file_handler, SYSPOINT *pointers){
    rewind(file_handler);
    if (fwrite(pointers->superblock, sizeof(SUPERBLOCK), 1, file_handler) != 1){
        printf("Blad zapisywania superbloku.\n");
        fclose(file_handler);
        free_system_pointers(pointers);
        return errno;
    }

    fseek(file_handler, INODE_BMP_ADRESS, SEEK_SET);
    if (fwrite(pointers->inode_bmp, MAX_FILES_NUMBER*sizeof(char), 1, file_handler) != 1){
        printf("Blad zapisywania bitmapy inode'ow.\n");
        fclose(file_handler);
        free_system_pointers(pointers);
        return errno;
    }

    fseek(file_handler, DBLOCKS_BMP_ADRESS, SEEK_SET);
    if (fwrite(pointers->dblocks_bmp, pointers->superblock->dblock_count*sizeof(char), 1, file_handler) != 1){
        printf("Blad zapisywania bitmapy blokow danych.\n");
        fclose(file_handler);
        free_system_pointers(pointers);
        return errno;
    }
    return 0;
}

int copy_to_virtual_disk(const char *filename, const char *virtual_disk_name){
    FILE *virtual_handler, *physical_handler;
    SYSPOINT pointers;
//    BLOCK *copying_block;
    WORD index, name_length;
//    unsigned long int file_size;

    if(!(virtual_handler = fopen(virtual_disk_name, "r+b"))){
        printf("cp: Blad otwierania dysku\n");
        return errno;
    }

    if((allocate_system_pointers(virtual_handler, &pointers))!= 0){
        printf("cp: blad czytania struktur informacyjnych\n");
        fclose(virtual_handler);
        return errno;
    }

    if(!(physical_handler = fopen(filename, "r"))) {
        printf("cp: blad otwierania pliku do kopiowania\n");
        fclose(virtual_handler);
        free_system_pointers(&pointers);
        return errno;
    }

    name_length = (WORD)strlen(filename);
    if(name_length >= MAX_NAME_LENGTH){
        printf("cp: nazwa pliku jest za dluga (max: %d)\n",MAX_NAME_LENGTH);
        fclose(virtual_handler);
        fclose(physical_handler);
        free_system_pointers(&pointers);
    }

    index = find_file_on_disk(filename, &pointers, virtual_handler);
    if(index != MAX_FILES_NUMBER){
        printf("cp: plik istnieje juz na dysku\n");
        fclose(virtual_handler);
        fclose(physical_handler);
        free_system_pointers(&pointers);
        return errno;
    }

  //  file_size = get_file_size(physical_handler);

    fclose(virtual_handler);
    fclose(physical_handler);
    free_system_pointers(&pointers);
    return 0;
}

int display_virtual_disk_info(char *filename) {
    SYSPOINT pointers;
    FILE *disk_handler;

    if (!(disk_handler = fopen(filename, "rb"))) {
        printf("Nie mozna otworzyc dysku '%s'\n", filename);
        return errno;
    }

    allocate_system_pointers(disk_handler, &pointers);
    printf("NAME:%s SIZE:%u INODES:%u DATA_BLOCKS:%u FREE_DATA_BLOCKS:%u FULL_DATA_BLOCKS:%u\n",
           pointers.superblock->disk_name, pointers.superblock->size,
           pointers.superblock->inode_count, pointers.superblock->dblock_count,
           pointers.superblock->free_dblocks, pointers.superblock->full_dblocks);
    int i = 0;
    for(;i<MAX_FILES_NUMBER;++i){

    }
    free_system_pointers(&pointers);
    fclose(disk_handler);
    return 0;
}

int display_main_catalog(const char *filename){
    FILE *handler;
    SYSPOINT pointers;
    WORD offset;

    if(!(handler = fopen(filename,"r+b"))){
        printf("l: Blad otwierania pliku\n");
        return errno;
    }

    if((allocate_system_pointers(handler, &pointers)) != 0){
        printf("l: Blad czytania glownych struktur informacynych\n");
        return errno;
    }

    for(WORD i = 0; i < MAX_FILES_NUMBER; ++i){
        if(pointers.inode_bmp[i] == 1){
            offset = calculate_inode_offset(i, &pointers);
            fseek(handler, offset, SEEK_SET);
            if(fread(pointers.temp_inode, sizeof(INODE), 1, handler) != 1){
                printf("l: Odczytanie inode'a nie udalo sie\n");
                return errno;
            }
            printf("N[%s]:S[%u]:DbI[%u]\t", pointers.temp_inode->name, pointers.temp_inode->size, pointers.temp_inode->dblock_index);
        }
    }
    printf("\n");
    free_system_pointers(&pointers);
    fclose(handler);
    return 0;
}

int delete_virtual_disk(const char *filename){
    if (unlink(filename) == -1){
        printf("Blad usuwania systemu plikow.\n");
        return errno;
    }

    printf("Dysk wirtualny '%s' zostal usuniety.\n",filename);

    return 0;
}

int map_virtual_disk(const char *filename){
    FILE *handler;
    SYSPOINT pointers;
    int i, free_blocks, free_inodes;

    if(!(handler = fopen(filename, "rb"))){
        printf("m: Blad podczas otwierania pliku\n");
        return errno;
    }
    if((allocate_system_pointers(handler, &pointers))!=0){
        fclose(handler);
        printf("m: Blad podczas czytania pliku\n");
        return -1;
    }

    for (i = 0, free_blocks = 0; i < pointers.superblock->dblock_count; ++i){
        if (pointers.dblocks_bmp[i] == 0){
            ++free_blocks;
        }
    }

    for (i = 0, free_inodes = 0; i < MAX_FILES_NUMBER; ++i){
        if (pointers.inode_bmp[i] == 0){
            ++free_inodes;
        }
    }
    printf("dblocks: %d\ninodes: %d\n",free_blocks, free_inodes);

    printf("OFFSET    |    SIZE    |    COUNT    |    STATUS    |    TYPE    |\n");
    printf("%10i|%12zu|%13d|          full|  superblock\n", 0, sizeof(SUPERBLOCK), 1);
    printf("%10zu|%12zu|%13d|          ----|inode bitmap\n", INODE_BMP_ADRESS, sizeof(char)*MAX_FILES_NUMBER, MAX_FILES_NUMBER);
    printf("%10zu|%12zu|%13d|          ----|block bitmap\n", DBLOCKS_BMP_ADRESS, sizeof(char)*pointers.superblock->dblock_count, pointers.superblock->dblock_count);
    printf("%10i|%12zu|%13d|          free|      inodes\n", calculate_inode_offset(find_first_free_inode(&pointers), &pointers), free_inodes*sizeof(INODE), free_inodes);
    printf("%10i|%12zu|%13d|          full|      inodes\n", calculate_inode_offset(find_first_taken_inode(&pointers), &pointers), (MAX_FILES_NUMBER - free_inodes) * sizeof(INODE), MAX_FILES_NUMBER - free_inodes);
    printf("%10i|%12zu|%13d|          free| data blocks\n", calculate_dblock_offset(0, &pointers), sizeof(BLOCK)*free_blocks, free_blocks);
    printf("      ----|%12zu|%13d|          full| data blocks\n\n", sizeof(BLOCK)*(pointers.superblock->dblock_count - free_blocks), pointers.superblock->dblock_count - free_blocks);

    free_system_pointers(&pointers);
    fclose(handler);
    return 0;

}

unsigned long int get_file_size(FILE *file_handler){
    unsigned long int size = 0;
    fseek(file_handler, 0, SEEK_END);
    size = (unsigned long) ftell(file_handler);
    rewind(file_handler);
    return size;
}

WORD convert_size_to_blocks(long size) {
    return (size % BLOCK_SIZE == 0) ? (WORD) (size / BLOCK_SIZE) : (WORD) (size / BLOCK_SIZE + 1);
}

WORD find_file_on_disk(const char *filename, SYSPOINT *pointers, FILE *handler){
    WORD index;
    for(index = 0; index < MAX_FILES_NUMBER; ++index){
        if(pointers->inode_bmp[index] == 1){
            fseek(handler, calculate_inode_offset(index, pointers), SEEK_SET);
            if((fread(pointers->temp_inode, sizeof(INODE), 1, handler))!=1){
                printf("findfile: Blad przy czytaniu inode'a\n");
                return (WORD)errno;
            }
            if(!(strcmp(pointers->temp_inode->name, filename))){
                rewind(handler);
                break;
            }
        }
    }
    return index;
}

WORD find_first_free_inode(SYSPOINT *pointers){
    WORD index = 0;
    for(;index<MAX_FILES_NUMBER;++index)
        if(pointers->inode_bmp[index] == 0)
            return index;
    return MAX_FILES_NUMBER;
}

WORD find_first_taken_inode(SYSPOINT *pointers){
    WORD index = 0;
    for(;index<MAX_FILES_NUMBER;++index)
        if(pointers->inode_bmp[index] == 1)
            return index;
    return MAX_FILES_NUMBER;
}

WORD calculate_inode_offset(WORD index, SYSPOINT *pointers){
    return DBLOCKS_BMP_ADRESS + (pointers->superblock->dblock_count * sizeof(char)) + (sizeof(INODE) * index);
}

WORD calculate_dblock_offset(WORD index, SYSPOINT *pointers){
    return DBLOCKS_BMP_ADRESS + (pointers->superblock->dblock_count * sizeof(char)) + (sizeof(INODE) * MAX_FILES_NUMBER) + (sizeof(BLOCK) * index);
}

int allocate_system_pointers(FILE *handler, SYSPOINT *sys_pointers) {
    rewind(handler);
    if(!(sys_pointers->superblock = (SUPERBLOCK *) malloc(sizeof(SUPERBLOCK)))){
        printf("Nie mozna zaalokowac miejsca na odczytany superblok.\n");
        return 1;
    }
    if(fread(sys_pointers->superblock, sizeof(SUPERBLOCK), 1, handler) != 1){
        printf("Nie mozna odczytac superbloku dysku.\n");
        return errno;
    }

    fseek(handler, INODE_BMP_ADRESS, SEEK_SET);
    if((fread(sys_pointers->inode_bmp, MAX_FILES_NUMBER * sizeof(char), 1, handler)) != 1){
        printf("Nie mozna odczytal bitmpay inode'ow.\n");
        return errno;
    }

    fseek(handler, DBLOCKS_BMP_ADRESS, SEEK_SET);
    if(!(sys_pointers->dblocks_bmp = (char *) malloc(sys_pointers->superblock->dblock_count * sizeof(char)))){
        printf("Nie mozna zaalokowac miejsca na odczytana bitmape blokow danych.\n");
        return errno;
    }
    if((fread(sys_pointers->dblocks_bmp, sys_pointers->superblock->dblock_count * sizeof(char), 1, handler)) != 1){
        printf("Nie mozna odczytam bitmapy blokow danych.\n");
        return errno;
    }

    if(!(sys_pointers->temp_inode = (INODE *)malloc(sizeof(INODE)))){
        printf("Nie mozna zaalokowac pamieci na inode'a.\n");
        return errno;
    }
    return 0;
}

void free_system_pointers(SYSPOINT *pointers){
    free(pointers->superblock);
    free(pointers->dblocks_bmp);
    free(pointers->temp_inode);
}