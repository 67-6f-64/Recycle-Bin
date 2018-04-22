#include<stdio.h>

typedef struct tagPosition{
int row;
int col;
}Position;

void main(){
	Position pos;
	pos.row = 1;
	pos.col = 2;
	printf("row:%d\r\ncol:%d\r\n", pos.row, pos.col);
}