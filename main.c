//
// Created by kacper on 26.01.17.
//
#include "FileSystem.h"

void print_description(void){
    printf("STRESZCZENIE\n");
    printf("\t./FileSystem OPCJA [DYSK] [ARGUMENT]\n");
    printf("OPIS\n");
    printf("\tProgram wykonujacy rozne czynnosci zwiazanie z tworzeniem i zarzadzaniem dyskami wirtualnymi.");
    printf("\n\n-mk");
    printf("\n\tUtworz dysk wirtualny o podanej nazwie i o podanym rozmiarze na dane");
    printf("\n-cv");
    printf("\n\tKopiuj plik o podanej nazwie z dysku fizycznego na dysk wirtualny");
    printf("\n-cp");
    printf("\n\tKopiuj plik o podanej nazwie z dysku wirtualnego na dysk fizyczny");
    printf("\n-ls");
    printf("\n\tWyswietl zawartosc katalogu glownego");
    printf("\n-rm");
    printf("\n\tUsun z dysku wirtualnego plik o podanej nazwie");
    printf("\n-dt");
    printf("\n\tUsun podany dysk wirtualny wraz z jego zawartoscia");
    printf("\n-mp");
    printf("\n\tWyswietl szczegolowe mapowanie dysku wirtualnego");
    printf("\n-i");
    printf("\n\tWyswietl ogolne parametry dysku\n");
}

int main(int argc, char *argv[]){
    if(argc<3) {
        print_description();
        return 0;
    }
    char option[4];
    char disk[24];
    char argument[24];
    strcpy(option, argv[1]);
    strcpy(disk, argv[2]);
    if(argc == 4)
        strcpy(argument, argv[3]);
    if(!(strcmp(option, "-mk"))){
        create_virtual_disk(disk, (unsigned long)atoi(argument));

    }else if(!(strcmp(option, "-cv"))){
        copy_to_virtual_disk(argument, disk);

    }else if(!(strcmp(option, "-cp"))){
        copy_from_virtual_disk(argument, disk);

    }else if(!(strcmp(option, "-ls"))){
        display_main_catalog(disk);

    }else if(!(strcmp(option, "-rm"))){
        delete_file(argument, disk);

    }else if(!(strcmp(option, "-dt"))){
        delete_virtual_disk(disk);

    }else if(!(strcmp(option, "-mp"))){
        map_virtual_disk(disk);

    }else if(!(strcmp(option, "-i"))){
        display_virtual_disk_info(disk);

    }else{
        print_description();
    }
    return 0;
}