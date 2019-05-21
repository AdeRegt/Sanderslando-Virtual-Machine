#include <stdlib.h>
#include <stdio.h>

#define MEMORYSIZE 0xFFFF
#define RET_NO_EXIT 1
#define RET_NO_ARG  2

unsigned char *memory;
unsigned char register_A = 0;
unsigned char register_CP = 0;
unsigned char register_SP = 0;

void reset(){
	free(memory);
	memory = (unsigned char *)malloc(MEMORYSIZE);
	for(int i = 0 ; i < MEMORYSIZE ; i++){
		memory[i] = 0x00;
	}
	register_A = 0;
	register_CP = 0;
	register_SP = 0;
}

int loadImage(char* filename){
	FILE *file = fopen(filename,"rb");
	if(file==NULL){
		return 1;
	}
	fclose(file);
	return 0;
}

int main(int argc,char** argv){
	reset();
	if(argc!=2){
		printf("FATAL: expected: ./svm [PATH TO IMAGE]\n");
		return RET_NO_ARG;
	}
	if(loadImage(argv[argc-1])){
		printf("FATAL: unable to load image\n");
		return RET_NO_ARG;
	}
	return RET_NO_EXIT;
}
