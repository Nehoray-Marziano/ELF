#include <stdio.h>
#include <string.h>

void digit_cnt(char* str){
    char* digits="0123456789";
    int i;
    int j;
    int counter;
    for(i=0;str[i]!='\0';i++){
        for(j=0;j<10;j++){
            if(str[i]==digits[j]){
                counter++;
                break;
            }
        }
    }
    printf("The number of digits in the string is: %d",counter);
}
int main(int argc, char** argv){
	return 0;
}
