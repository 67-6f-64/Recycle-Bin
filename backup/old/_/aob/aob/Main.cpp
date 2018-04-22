#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct{
	LPBYTE Array;
	LPBOOL Wild;
}Pattern;

int ChartoByte(char *Array, unsigned char *b){
	int i, hit = 0;
	unsigned char *Arraycpy;
	for(i=0; Array[i]!=0x00; i++){
		Arraycpy[i] = Array[i];
	}

	for(i=0; Arraycpy[i]!=0x00; i++){
		if(Arraycpy[i]==0x20){continue;}//space

		if(Arraycpy[i]==0x3F || Arraycpy[i]==0x2A){
			b[hit/2] = 0xFE;
			hit++;
			hit++;
			i++;
		}//WildCard


		if(Arraycpy[i] >= 0x61 && Arraycpy[i] <=0x66){
			Arraycpy[i]-=0x20;
		}//a to A

		if(hit%2==0){
			if(Arraycpy[i] >= 0x41 && Arraycpy[i] <=0x46){
				b[hit/2]=(Arraycpy[i]-0x37)*0x10;
				hit++;
			}
			else if(Arraycpy[i] >= 0x30 && Arraycpy[i] <=0x39){
				b[hit/2]=(Arraycpy[i]-0x30)*0x10;
				hit++;
			}
		}else{
			if(Array[i] >= 0x41 && Array[i] <=0x46){
				b[hit/2]+=Array[i]-0x37;
				hit++;
			}
			else if(Arraycpy[i] >= 0x30 && Arraycpy[i] <=0x39){
				b[hit/2]+=Arraycpy[i]-0x30;
				hit++;
			}

		}
	}

	return hit/2;
		//if(Array[i] >= 0x61 && Array[i] <=0x66) {Array[i]-=0x20;}//a to A
		//if(Array[i] >= 0x41 && Array[i] <=0x46) {b[hit]+=Array[i]-0x37; hit++;}
		//if(Array[i] >= 0x30 && Array[i] <=0x39) {b[hit]+=Array[i]-0x30; hit++;}

}

int main(){
	int i;
	LPBYTE Test;
	int hit;
	
	hit = ChartoByte("11 22 a1 b5", Test);
	for(i=0; i<hit; i++){
		printf("%02X ", Test[i]);
	}
	//unsigned char aa = '*';
	//printf("%02X", aa);
	printf("\n");
	system("pause");
}