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

    //strcpy(pointers.superblock->disk_name, "sdsds");
    //pointers.superblock->size = 22;
    //pointers.superblock->free_dblocks = 999;
   // update_disk_data(handler, &pointers);
   // display_virtual_disk_info("test");
      map_virtual_disk("test");
    display_main_catalog("test");
    copy_to_virtual_disk("agg", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    copy_to_virtual_disk("all", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    copy_to_virtual_disk("bla", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    copy_to_virtual_disk("malpka", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    copy_to_virtual_disk("mniejszy", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");


}