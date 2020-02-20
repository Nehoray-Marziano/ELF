#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
    printf("%s",argv[1]);
    int fileDes=open(argv[1],O_RDWR);
    char* myMap=NULL;
    if(fileDes==-1){/*opening failed*/
        perror("failed to open file \n");
        return 0;
    }
    int length= lseek(fileDes,0,SEEK_END);
    lseek(fileDes,0,SEEK_SET);/*takes the pointer back to the beginning of the file*/

    myMap=mmap(myMap,length,PROT_READ,MAP_SHARED,fileDes,0);
    Elf32_Ehdr* header;
    header=(Elf32_Ehdr*)myMap;

    if(header->e_ident[EI_CLASS]!=ELFCLASS32){
        perror("this is NOT a 32bit ELF file! \n");
        munmap(myMap,length);/*unmapping*/
        close(fileDes);
        return 0;
    }
    int k;
    int total=header->e_shnum;

    Elf32_Shdr* list=(Elf32_Shdr*)((header->e_shoff)+myMap);
    int offsetstr= list[header->e_shstrndx].sh_offset;
    printf("\n%s\t%s\t\t%s\t%s\t%s\n","[INDEX]","NAME","ADDRESS","OFFSET","SIZE");/*printing headlines for better understanding*/

    for(k=0;k<total;k++){
        Elf32_Shdr sec=list[k];
        char* name=(myMap+offsetstr+sec.sh_name);
        printf("[%d]\t%-12s\t%08x %06x\t%06x\n",k,name,sec.sh_addr,sec.sh_offset,sec.sh_size);
    }
    printf("\n");
    munmap(myMap,length);
    close(fileDes);
    return 0;
}
