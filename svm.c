#include <stdlib.h>
#include <stdio.h>

#define MEMORYSIZE 0xFFFF
#define RET_EXIT    0
#define RET_NO_EXIT 1
#define RET_NO_ARG  2
#define RET_PANIC   3

unsigned char *memory;
unsigned char register_A = 0;
unsigned char register_CP = 0;
unsigned char register_SP = 0;
unsigned char cmpbyte = 0;

void panic(const char* message){
	printf("[PANIC] %s \n",message);
	exit(RET_PANIC);
}

unsigned char getMemory(unsigned short location){
        if(location<MEMORYSIZE){
                return memory[location & 0xFFFF] & 0xFF;
        }else{
                panic("Out of memory");
        }
}


unsigned char getNextByte(){
	if(register_CP<MEMORYSIZE){
		return getMemory(register_CP++);
	}else{
		panic("Out of memory");
	}
}

unsigned short getNextWord(){
	unsigned long A = getNextByte();
	unsigned long B = getNextByte();
	return (B << 8) | A;
}

void setMemoryByte(unsigned short location,unsigned char value){
	if(location<MEMORYSIZE){
		memory[location & 0xFFFF] = value & 0xFF;
	}else{
		panic("Out of memory");
	}
}


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
	unsigned char karakter = 0x00;
	unsigned int mempointer = 0;
	while(1){
		if(mempointer==MEMORYSIZE){
			panic("Out of memory");
		}
		karakter = (unsigned char)fgetc(file);
		if(karakter==0xFF){
			break;
		}
		memory[mempointer++] = karakter;
	}
	fclose(file);
	return 0;
}

int main(int argc,char** argv){
	reset();
	if(argc!=2){
		printf("FATAL: expected: %s [PATH TO IMAGE]\n",argv[0]);
		return RET_NO_ARG;
	}
	if(loadImage(argv[argc-1])){
		printf("FATAL: unable to load image\n");
		return RET_NO_ARG;
	}
	while(1){
		unsigned char opcode = getNextByte();
		if(opcode==0x00){
			//
			// End of code
			// HLT
			// 0x00
			//

			return RET_EXIT;
		}else if(opcode==0x01){
			//
			// Value of memorylocation to registerA
			// LMA
			// 0x01 LOCATION
			//

			unsigned short location = getNextWord();
			register_A = getMemory(location);
		}else if(opcode==0x02){
			//
			// RegisterA to memorylocation
			// LAM
			// 0x02 LOCATION
			//

			unsigned short location = getNextWord();
			setMemoryByte(location,register_A);
		}else if(opcode==0x03){
			//
			// Add value of memorylocation to registerA
			// ADD
			// 0x03 LOCATION
			//

			unsigned short location = getNextWord();
			register_A += getMemoryByte(location);
		}else if(opcode==0x04){
			//
			// Distract value of memorylocation to registerA
			// DIS
			// 0x04 LOCATION
			//

			unsigned short location = getNextWord();
                        register_A -= getMemoryByte(location);
		}else if(opcode==0x05){
                        //
                        // Multiply value of memorylocation to registerA
                        // MUL
                        // 0x05 LOCATION
                        //

                        unsigned short location = getNextWord();
                        register_A *= getMemoryByte(location);
                }else if(opcode==0x06){
                        //
                        // Divide value of memorylocation to registerA
                        // DIV
                        // 0x06 LOCATION
                        //

                        unsigned short location = getNextWord();
                        register_A /= getMemoryByte(location);
                }else if(opcode==0x07){
			//
			// Compare register A with memory
			// CMP
			// 0x07 LOCATION
			//

			unsigned short location = getNextWord();
			unsigned char valueA = getMemoryByte(location);
			if(valueA==register_A){
				cmpbyte = 1;
			}else if(valueA<register_A){
				cmpbyte = 2;
			}else if(valueA>register_A){
				cmpbyte = 3;
			}else{
				cmpbyte = 0;
			}
		}else{
			printf("[PANIC] Unrecognised token %x \n",opcode);
			panic("CPU detected unrecognised token");
		}
	}
	return RET_NO_EXIT;
}
