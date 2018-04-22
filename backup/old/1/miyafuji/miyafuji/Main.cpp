#include<stdio.h>
#include<Windows.h>

class Slot{
private:
	int number[3000];
	int game[3000];
public:
	Slot();
	void Add(int n, int g);
	void Reset(int n);
	void Show();
};

Slot::Slot(){
	ZeroMemory(number, sizeof(number));
	ZeroMemory(game, sizeof(game));
}

void Slot::Add(int n, int g){
	int i = 0;
	while(number[i]){
		if(number[i] == n){
			game[i] += g;
			return;
		}
		i++;
	}
	number[i] = n;
	game[i] = g;
}

void Slot::Reset(int n){
	int i = 0;
	while(number[i]){
		if(number[i] == n){
			game[i] = 0;
			return;
		}
		i++;
	}
}

void Slot::Show(){
	int i = 0;
	while(number[i]){
		if(game[i] >= 600){
			printf("%d:%dg\n", number[i], game[i]);
		}
		i++;
	}
}

void main(){
	int i;
	Slot slot;
	for(i=0; i<2; i++){
	FILE *fp;
	char file[0x100];
	char Buffer[0x100];
	printf("ƒtƒ@ƒCƒ‹–¼:");
	scanf("%s", file);
	fp = fopen(file, "r");
	while(fgets(Buffer, 0x0100, fp)){
		if(strstr(Buffer, ":") && strstr(Buffer, "g")){
			int number, game;
			if(strstr(Buffer, "@")){
				sscanf(Buffer, "%d:@%dg", &number, &game);
				slot.Add(number, game);
			}
			else{
				sscanf(Buffer, "%d:%dg", &number, &game);
				slot.Reset(number);
				slot.Add(number, game);
			}
		}
	}
	fclose(fp);
	}
	slot.Show();
}