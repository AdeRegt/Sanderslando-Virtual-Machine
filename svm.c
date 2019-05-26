#include <stdlib.h>
#include <stdio.h>

#define MEMORYSIZE 0xFFFF
#define STACKSIZE  0xFF
#define RET_EXIT    0
#define RET_NO_EXIT 1
#define RET_NO_ARG  2
#define RET_PANIC   3

unsigned char *memory;
unsigned char register_A = 0;
unsigned char register_CP = 0;
unsigned char register_SP = 0;
unsigned char cmpbyte = 0;
unsigned short stack[STACKSIZE];

void panic(const char* message){
	printf("[PANIC] %s \n",message);
	exit(RET_PANIC);
}

void pushStack(){
	if(register_SP<STACKSIZE){
		stack[register_SP++] = register_CP;
	}else{
		panic("Stack overflow");
	}
}

void popStack(){
	if(register_SP>0){
		register_SP--;
		register_CP = stack[register_SP--];
	}else{
		panic("Stack underflow");
	}
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
	for(int i = 0 ; i < STACKSIZE ; i++){
		stack[i] = 0;
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
			register_A += getMemory(location);
		}else if(opcode==0x04){
			//
			// Distract value of memorylocation to registerA
			// DIS
			// 0x04 LOCATION
			//

			unsigned short location = getNextWord();
                        register_A -= getMemory(location);
		}else if(opcode==0x05){
                        //
                        // Multiply value of memorylocation to registerA
                        // MUL
                        // 0x05 LOCATION
                        //

                        unsigned short location = getNextWord();
                        register_A *= getMemory(location);
                }else if(opcode==0x06){
                        //
                        // Divide value of memorylocation to registerA
                        // DIV
                        // 0x06 LOCATION
                        //

                        unsigned short location = getNextWord();
                        register_A /= getMemory(location);
                }else if(opcode==0x07){
			//
			// Compare register A with memory
			// CMP
			// 0x07 LOCATION
			//

			unsigned short location = getNextWord();
			unsigned char valueA = getMemory(location);
			if(valueA==register_A){
				cmpbyte = 1;
			}else if(valueA<register_A){
				cmpbyte = 2;
			}else if(valueA>register_A){
				cmpbyte = 3;
			}else{
				cmpbyte = 0;
			}
		}else if(opcode==0x08){
			//
			// Jump if equal
			// JEQ
			// 0x08 LOCATION
			//

			unsigned short loc = getNextWord();
			if(cmpbyte==1){
				register_CP = loc;
			}
		}else if(opcode==0x09){
			//
			// Jump if cmp is not equal
			// QNE
			// 0x09 LOCATION
			//

			unsigned short loc = getNextWord();
			if(cmpbyte!=1){
				register_CP = loc;
			}
		}else if(opcode==0x0A){
			//
			// Jump if cmp is more
			// JMO
			// 0x0A LOCATION
			//

			unsigned short loc = getNextWord();
                        if(cmpbyte==2){
                                register_CP = loc;
                        }
		}else if(opcode==0x0B){
			//
                        // Jump if cmp is not more
                        // JNM
                        // 0x0B LOCATION
                        //

                        unsigned short loc = getNextWord();
                        if(cmpbyte!=2){
                                register_CP = loc;
                        }

		}else if(opcode==0x0C){
                        //
                        // Jump if cmp is less
                        // JLE
                        // 0x0C LOCATION
                        //

                        unsigned short loc = getNextWord();
                        if(cmpbyte==3){
                                register_CP = loc;
                        }
                }else if(opcode==0x0D){
                        //
                        // Jump if cmp is not less
                        // JNL
                        // 0x0D LOCATION
                        //

                        unsigned short loc = getNextWord();
                        if(cmpbyte!=3){
                                register_CP = loc;
                        }

                }else if(opcode==0x0E){
			//
			// jump
			// JMP
			// 0x0E LOCATION
			//

			unsigned short loc = getNextWord();
			register_CP = loc;
		}else if(opcode==0x0F){
			//
			// call
			// CAL
			// 0x0F LOCATION
			//

			unsigned short loc = getNextWord();
			pushStack();
			register_CP = loc;
		}else if(opcode==0x10){
			//
			// return
			// RET
			// 0x10 
			//

			popStack();
		}else if(opcode==0x11){
			//
			// shift A to the left
			// LEF
			// 0x11
			//

			register_A = register_A << 1;
		}else if(opcode==0x12){
                        //
                        // shift A to the right
                        // RIG
                        // 0x12
                        //

                        register_A = register_A >> 1;
                }else if(opcode==0x13){
			//
			// AND
			// AND
			// 0x13 LOCATION
			//

			unsigned short loc = getNextWord();
			register_A &= getMemory(loc);
		}else if(opcode==0x14){
                        //
                        // OR
                        // ORM
                        // 0x13 LOCATION
                        //

                        unsigned short loc = getNextWord();
                        register_A |= getMemory(loc);
                }else if(opcode==0x15){
			//
			// SYSTEMCALL
			// SYS
			// 0x15 LOCATION
			//
			unsigned short loc = getNextWord();
			panic("SYSTEMCALL");
		}else{
			printf("[PANIC] Unrecognised token %x \n",opcode);
			panic("CPU detected unrecognised token");
		}
	}
	return RET_NO_EXIT;
}
