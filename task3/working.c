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

    myMap=mmap(myMap,length,PROT_READ | PROT_WRITE,MAP_SHARED,fileDes,0);
    Elf32_Ehdr* header=(Elf32_Ehdr*)myMap;

    if(header->e_ident[EI_CLASS]!=ELFCLASS32){
        perror("this is NOT a 32bit ELF file! \n");
        munmap(myMap,length);/*unmapping*/
        close(fileDes);
        exit(EXIT_FAILURE);
    }
    int k;
    int total=header->e_shnum;

    Elf32_Shdr* list=(Elf32_Shdr*)((header->e_shoff)+myMap);
    int offsetstr= list[header->e_shstrndx].sh_offset;
    printf("\n%s\t%s\t\t%s\t%s\t%s\n","[INDEX]","NAME","ADDRESS","OFFSET","SIZE");/*printing headlines for better understanding*/

    for(k=0;k<total;k++){
        Elf32_Shdr sec=list[k];
        if(sec.sh_type==SHT_SYMTAB||sec.sh_type==SHT_DYNSYM){
            int symTotal=sec.sh_size/ sizeof(Elf32_Sym);
            Elf32_Sym* symList=(Elf32_Sym*)(sec.sh_offset+myMap);
            char* secName=  (offsetstr+sec.sh_name+myMap);
            int symStr=list[sec.sh_link].sh_offset;
            printf ("%s\t%s\t%s\t%-12s\t%-12s\n" , "INDEX","VALUE","SEC_INDEX", "SEC_NAME", "SYM_NAME");/*headline*/
            int j;
            for(j=0;j<symTotal;j++){
                Elf32_Sym temp=symList[j];
                char* symName=(symStr+myMap+temp.st_name);
                printf ("[%d]\t%08x\t%d\t%-12s\t%-12s\n" , j,temp.st_value, k, secName, symName);
            }
            printf ("\nThe %s symbol table contains %d symbols\n", secName, symTotal);
        }
    }
    munmap(myMap,length);
    close(fileDes);
    return 0;
}
