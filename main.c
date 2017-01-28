//
// Created by kacper on 26.01.17.
//
#include "FileSystem.h"

int main(){
  //  printf("sizeof(SUPERBLOCK) = %lu\n",sizeof(SUPERBLOCK));
 //   printf("sizeof(INODE) = %lu\n",sizeof(INODE));
   // printf("sizeof(BLOCK) = %lu\n",sizeof(BLOCK));

  //  printf("%u %u %u %u",convert_size_to_blocks(0),convert_size_to_blocks(1)
 //   ,convert_size_to_blocks(2049), convert_size_to_blocks(2048));
    create_virtual_disk("test",2049);
    display_virtual_disk_info("test");

    SYSPOINT pointers;
    FILE *handler = fopen("test","r+b");
    allocate_system_pointers(handler, &pointers);

    char tab[64];
    memset(tab, 0, 64);
    tab[0] = 1;

    memcpy(pointers.inode_bmp, tab, 64);
    update_disk_data(handler,&pointers);

    INODE temp;
    char name[24] = "adddd";
    strcpy(temp.name,name);
    temp.dblock_index = 23;
    temp.size = 25;

    printf("OFSSET TO INODE TO WRITE: %u\n", calculate_inode_offset(0,&pointers));
    fseek(handler,calculate_inode_offset(0,&pointers),SEEK_SET);

    fwrite(&temp,sizeof(INODE),1,handler);

    char name2[24] = "bla";
    strcpy(temp.name,name2);
    temp.dblock_index = 256;
    temp.size = 2335;

    printf("OFSSET TO INODE TO WRITE: %u\n", calculate_inode_offset(3,&pointers));
    fseek(handler,calculate_inode_offset(3,&pointers),SEEK_SET);

    fwrite(&temp,sizeof(INODE),1,handler);
    tab[3] = 1;
    tab[0] = 1;
    memcpy(pointers.inode_bmp, tab, 64);
    update_disk_data(handler,&pointers);

    printf("OFSSET TO INODE: %u\n", calculate_inode_offset(0,&pointers));
    fseek(handler,calculate_inode_offset(0,&pointers),SEEK_SET);
    if((fread(&temp,sizeof(INODE),1,handler))!=1){
        return 1;
    }

    //pointers.superblock->dblock_count = 23;
    //strcpy(pointers.superblock->disk_name, "sdsds");
    //pointers.superblock->size = 22;
    //pointers.superblock->free_dblocks = 999;
   // update_disk_data(handler, &pointers);
   // display_virtual_disk_info("test");
    //  map_virtual_disk("test");
  //  printf("First free inode: %u %u\n",find_first_free_inode(&pointers),calculate_inode_offset(find_first_free_inode(&pointers), &pointers));
  //  printf("Inode 63: %u\n",calculate_inode_offset(63,&pointers));
  //  printf("dblock 0: %u\n",calculate_dblock_offset(0, &pointers));
   // printf("dblock 1: %u\n",calculate_dblock_offset(1, &pointers));
  //  printf("Inode 2: %u\n",calculate_inode_offset(3,&pointers));
   // printf("OFSSET TO INODE: %u\n", calculate_inode_offset(0,&pointers));
   // fseek(handler,calculate_inode_offset(0,&pointers),SEEK_SET);
    printf("OFSSET TO INODE: %u\n", calculate_inode_offset(0,&pointers));
    fseek(handler,calculate_inode_offset(0,&pointers),SEEK_SET);
    if((fread(&temp,sizeof(INODE),1,handler))!=1){
        return 1;
    }
    free_system_pointers(&pointers);
  //  allocate_system_pointers(handler, &pointers);
    fclose(handler);
    display_main_catalog("test");
    copy_to_virtual_disk("bla", "test");
    delete_virtual_disk("test");
    FILE *anch = fopen("test2", "r");
    printf("rozmiar: %lu",get_file_size(anch));
    fclose(anch);
}