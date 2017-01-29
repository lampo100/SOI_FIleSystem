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
   /* create_virtual_disk("test",2049);
    display_virtual_disk_info("test");*/

    //strcpy(pointers.superblock->disk_name, "sdsds");
    //pointers.superblock->size = 22;
    //pointers.superblock->free_dblocks = 999;
   // update_disk_data(handler, &pointers);
   // display_virtual_disk_info("test");
  /*
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

    delete_file("all", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    delete_file("agg","test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    copy_to_virtual_disk("a", "test");
    copy_to_virtual_disk("b", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    delete_file("a", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");

    copy_to_virtual_disk("c", "test");
    display_virtual_disk_info("test");
    map_virtual_disk("test");
    display_main_catalog("test");*/
    //display_main_catalog("test");
    //copy_from_virtual_disk("b","test");
   // create_virtual_disk("test_image_disk", 1542180);
  //  map_virtual_disk("test_image_disk");
 //   copy_to_virtual_disk("img1.jpg", "test_image_disk");
   // map_virtual_disk("test_image_disk");
  //  copy_from_virtual_disk("img1.jpg", "test_image_disk");
  //  create_virtual_disk("test", 32472580);
 //   map_virtual_disk("test");
  //  copy_to_virtual_disk("img1","test");
  //  copy_to_virtual_disk("obr","test");
   // copy_to_virtual_disk("obraz.bmp","test");

    //   display_main_catalog("test");
 //   map_virtual_disk("test");
     copy_from_virtual_disk("obraz.bmp", "test");
    copy_from_virtual_disk("obr", "test");
    copy_from_virtual_disk("img1", "test");


}