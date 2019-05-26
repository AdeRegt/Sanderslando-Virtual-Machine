#include <stdlib.h>
#include <stdio.h>

unsigned char* inputfilename;
unsigned char* outputfilename;

typedef struct{
	unsigned char bytecode;
	unsigned char needswork;
	unsigned char hascode;
	unsigned char flagname[25];
}ProgramFraction;

unsigned int codepointer = 0;
ProgramFraction program[0xFFFF];
FILE *file = NULL;

void readFunName(unsigned char isfunc){
	if(fgetc(file)=='@'){
		ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
        	pf->hascode = isfunc?0:1;
        	pf->bytecode = 0;
		pf->needswork = isfunc?0:1;
		int i = 0;
		while(1){
			unsigned char xx = fgetc(file);
			if(xx==' '||xx=='\r'||xx=='\n'||xx==0xFF){
				break;
			}
			pf->flagname[i++] = xx;
			if(i==25){
				break;
			}
		}
		ProgramFraction *pf2 = (ProgramFraction *)&program[codepointer++];
        	pf2->hascode = isfunc?0:1;
        	pf2->bytecode = 0;
        	pf2->needswork = 0;
	}else{
		printf("Syntax error: missing @ \n");
		exit(1);
	}
}

int main(int argc,char** argv){
	if(argc==3){
		inputfilename = argv[1];
		outputfilename = argv[2];

		//
		// parsing file
		file = fopen(inputfilename,"rb");
		if(file){
			unsigned char dee = 0x00;
			while(1){
				//
				// remove white spaces
				while(1){
					dee = fgetc(file);
					if(dee==0xff){
						goto build;
					}
					if(!(dee==' '||dee=='\n'||dee=='\t'||dee=='\r')){
						break;
					}
				}
				//
				// get the rest of the command
				unsigned char def = fgetc(file);
				unsigned char deg = fgetc(file);

				//
				// does it meet a existing command?
				if(dee=='R'&&def=='E'&&deg=='M'){ // REMARK
					// A REMARK.... IGNORE
					unsigned char okey = 0x00;
					while(1){
						if(okey=='\n'){
							break;
						}
						if(okey==0xFF){
							goto build;
						}
					}
				}else if(dee=='H'&&def=='L'&&deg=='T'){
					ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
					pf->hascode = 1;
					pf->bytecode = 0x00;
				}else if(dee=='L'&&def=='M'&&deg=='A'){
					ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x01;
					readFunName(0);
				}else if(dee=='L'&&def=='A'&&deg=='M'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x02;
                                        readFunName(0);
                                }else if(dee=='A'&&def=='D'&&deg=='D'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x03;
                                        readFunName(0);
                                }else if(dee=='D'&&def=='I'&&deg=='S'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x04;
                                        readFunName(0);
                                }else if(dee=='M'&&def=='U'&&deg=='L'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x05;
                                        readFunName(0);
                                }else if(dee=='D'&&def=='I'&&deg=='V'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x06;
                                        readFunName(0);
                                }else if(dee=='C'&&def=='M'&&deg=='P'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x07;
                                        readFunName(0);
                                }else if(dee=='J'&&def=='E'&&deg=='Q'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x08;
                                        readFunName(0);
                                }else if(dee=='Q'&&def=='N'&&deg=='E'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x09;
                                        readFunName(0);
                                }else if(dee=='J'&&def=='M'&&deg=='O'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x0A;
                                        readFunName(0);
                                }else if(dee=='J'&&def=='N'&&deg=='M'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x0B;
                                        readFunName(0);
                                }else if(dee=='J'&&def=='L'&&deg=='E'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x0C;
                                        readFunName(0);
                                }else if(dee=='J'&&def=='N'&&deg=='L'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x0D;
                                        readFunName(0);
                                }else if(dee=='J'&&def=='M'&&deg=='P'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x0E;
                                        readFunName(0);
                                }else if(dee=='C'&&def=='A'&&deg=='L'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x0F;
                                        readFunName(0);
                                }else if(dee=='R'&&def=='E'&&deg=='T'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x10;
                                }else if(dee=='L'&&def=='E'&&deg=='F'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x11;
                                }else if(dee=='R'&&def=='I'&&deg=='G'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x12;
                                }else if(dee=='A'&&def=='N'&&deg=='D'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x13;
                                        readFunName(0);
                                }else if(dee=='O'&&def=='R'&&deg=='M'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x14;
                                        readFunName(0);
                                }else if(dee=='S'&&def=='Y'&&deg=='S'){
                                        ProgramFraction *pf = (ProgramFraction *)&program[codepointer++];
                                        pf->hascode = 1;
                                        pf->bytecode = 0x15;
                                        readFunName(0);
                                }else if(dee=='V'&&def=='A'&&deg=='R'){
                                        readFunName(1);
                                }else{
					printf("PANIC: unknown opcode %c%c%c \n",dee,def,deg);
					exit(1);
				}
			}
			build:
			fclose(file);
		}else{
			printf("Unable to open %s to read \n",inputfilename);
			return 1;
		}

		//
		// Linking
		for(int i = 0 ; i < codepointer ; i++){
			ProgramFraction *pf = (ProgramFraction *)&program[i];
			if(pf->hascode&&pf->needswork){
				int defset = 0;
				unsigned short wlol = 0;
				for(int t = 0 ; t < codepointer ; t++){
					ProgramFraction tyt = program[t];
					if(tyt.hascode){
						wlol++;
					}else if(tyt.hascode==0){
						int y = 1;
						for(int g = 0 ; g < 25 ; g++){
							if(pf->flagname[g]!=tyt.flagname[g]){
								y = 0;
							}
						}
						if(y){
							unsigned char sigmaA = wlol & 0xFF;
							unsigned char sigmaB = (wlol>>8) & 0xFF;

							ProgramFraction *pf2 = (ProgramFraction *)&program[i+1];

							pf->bytecode = sigmaB;
							pf2->bytecode= sigmaA;
							defset = 1;
						}
					}
				}
				if(defset==0){
					printf("Unable to find symbol ");
					for(int g = 0 ; g < 25 ; g++){
                                        	printf("%c",pf->flagname[g]);
					}
					printf("\n");
					exit(1);
				}
				pf->needswork = 0;
			}
		}

		//
		// writing file
		FILE *file2 = fopen(outputfilename,"w");
		if(file2){
			int cnt = 0;
			for(int i = 0 ; i < codepointer ; i++){
				ProgramFraction pf = program[i];
				if(pf.hascode){
					cnt++;
					fputc(pf.bytecode,file2);
				}
			}
			printf("Wrote %x bytes to %s \n",cnt,outputfilename);
			fclose(file2);
		}else{
			printf("Unable to open %s to write \n",outputfilename);
			return 1;
		}
	}else{
		printf("Expected: ./svma input output\n");
	}
	return 0;
}
