#include"AirWindow.h"

namespace Air{
	int BYTEInput(char *Output, BYTE b);
	int WORDInput(char *Output, WORD w);
	int DWORDInput(char *Output, DWORD dw);
	int QWORDInput(char *Output, DWORD dw1, DWORD dw2);
	int StringInput(char *Output, char *s);
	int DataInput(char *Output, BYTE *b, int size);
	int xBYTEInput(char *Output, BYTE b);
	int xWORDInput(char *Output, WORD w);
	int xDWORDInput(char *Output, DWORD dw);
	int xQWORDInput(char *Output, DWORD dw1, DWORD dw2);
	int xStringInput(char *Output, char *s);
	int xDataInput(char *Output, BYTE *b, int size);
	int IntInput(char *Output, signed long int d);
}

int Air::BYTEInput(char *Output, BYTE b){
	(*Output++) = ((b/0x10) <= 0x09)?((b/0x10) + 0x30):((b/0x10) + 0x37);
	(*Output++) = ((b%0x10) <= 0x09)?((b%0x10) + 0x30):((b%0x10) + 0x37);
	return 2;
}

int Air::WORDInput(char *Output, WORD w){
	int po = 0;
	po += BYTEInput(&Output[po], w >> 8);
	po += BYTEInput(&Output[po], w);
	return po;
}

int Air::DWORDInput(char *Output, DWORD dw){
	int po = 0;
	po += WORDInput(&Output[po], dw >> 16);
	po += WORDInput(&Output[po], dw);
	return po;
}

int Air::QWORDInput(char *Output, DWORD dw1, DWORD dw2){
	int po = 0;
	po += DWORDInput(&Output[po], dw2);
	po += DWORDInput(&Output[po], dw1);
	return po;
}

int Air::StringInput(char *Output, char *s){
	int i = 0;
	while(*s){
		Output[i++] = *s++;
	}
	return i;
}

int Air::DataInput(char *Output, BYTE *b, int size){
	int i, po = 0;
	for(i=0; i<size; i++){
		po += BYTEInput(&Output[po], b[i]);
	}
	return po;
}

int Air::xBYTEInput(char *Output, BYTE b){
	int po = 0;
	Output[po++] = '0';
	Output[po++] = 'x';
	po += BYTEInput(&Output[po], b);
	return po;
}

int Air::xWORDInput(char *Output, WORD w){
	int po = 0;
	po += xBYTEInput(&Output[po], w >> 8);
	po += BYTEInput(&Output[po], w);
	return po;
}

int Air::xDWORDInput(char *Output, DWORD dw){
	int po = 0;
	po += xWORDInput(&Output[po], dw >> 16);
	po += WORDInput(&Output[po], dw);
	return po;
}

int Air::xQWORDInput(char *Output, DWORD dw1, DWORD dw2){
	int po = 0;
	po += xDWORDInput(&Output[po], dw2);
	po += DWORDInput(&Output[po], dw1);
	return po;
}

int Air::xStringInput(char *Output, char *s){
	int i = 0;
	Output[i++] = '\"';
	while(*s){
		Output[i++] = *s++;
	}
	
	Output[i++] = '\"';
	return i;
}

int Air::xDataInput(char *Output, BYTE *b, int size){
	int i, po = 0;
	for(i=0; i<size; i++){
		po += BYTEInput(&Output[po], b[i]);
		Output[po++] = ' ';
	}
	return --po;
}

int Air::IntInput(char *Output, signed long int d){
	int i, di, ti, po = 0;
	if(d < 0){//Negative
		Output[po++] = '-';
		d = ~d + 1;
	}

	i = 0;
	di = d;

	while(1){
		i++;
		if(di < 10){
			break;
		}
		di /= 10;
	}

	ti = i;
	di = 1;
	for(i=0; i<(ti-1); i++){
		di *= 10;
	}
	
	for(i=0; i<ti; i++){
		Output[po++] = d/di + 0x30;
		d = d%di;
		di /= 10;
	}

	return po;
}

void Air::_AirPrint(char *Output, char *Format, DWORD *Input){
	int po, pf, pi;

	po = 0;
	pi = 0;

	for(pf=0; Format[pf]; pf++){
		if(Format[pf] == '%'){
			pf++;
			switch(Format[pf]){
				case 'b':
					po += BYTEInput(&Output[po], Input[pi]);
					pi++;
					break;

				case 'w':
					po += WORDInput(&Output[po], Input[pi]);
					pi++;
					break;

				case 'd':
					po += DWORDInput(&Output[po], Input[pi]);
					pi++;
					break;

				case 'q':
					po += QWORDInput(&Output[po], Input[pi], Input[pi+1]);
					pi += 2;
					break;

				case 's':
					po += StringInput(&Output[po], (char *)Input[pi]);
					pi++;
					break;

				case 'a':
					po += DataInput(&Output[po], (BYTE *)Input[pi], Input[pi+1]);
					pi += 2;
					break;

				case 'i':
					po += IntInput(&Output[po], Input[pi]);
					pi++;
					break;

				case 'x':
					pf++;
						switch(Format[pf]){
							case 'b':
								po += xBYTEInput(&Output[po], Input[pi]);
								pi++;
								break;

							case 'w':
								po += xWORDInput(&Output[po], Input[pi]);
								pi++;
								break;

							case 'd':
								po += xDWORDInput(&Output[po], Input[pi]);
								pi++;
								break;

							case 'q':
								po += xQWORDInput(&Output[po], Input[pi], Input[pi+1]);
								pi += 2;
								break;

							case 's':
								po += xStringInput(&Output[po], (char *)Input[pi]);
								pi++;
								break;

							case 'a':
								po += xDataInput(&Output[po], (BYTE *)Input[pi], Input[pi+1]);
								pi += 2;
								break;

							default:
								break;
						}

					break;

				default:
					break;
			}
			continue;
		}
		else{
			Output[po++] = Format[pf];
		}
	}
		Output[po] = 0;
}

void _stdcall Air::AirPrint(char *Output, char *Format, ...){
	_AirPrint(Output, Format, (LPDWORD)&Format + 1);
}