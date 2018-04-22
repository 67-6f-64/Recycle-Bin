#include"AirWindow.h"

namespace Air{
	char HexArray[] = "0123456789ABCDEF0123456789abcdef";
}

void Air::DWORDtoChar(char *temp, DWORD dw){
	int i;

	temp[0] = '0';
	temp[1] = 'x';

	for(i=3; i>=0; i--){
		temp[i*2+2] = HexArray[(BYTE)(dw >> (8*(3-i)))/0x10];
		temp[i*2+3] = HexArray[(BYTE)(dw >> (8*(3-i)))%0x10];
	}
	temp[10] = 0;
}

void Air::WORDtoChar(char *temp, WORD w){
	int i;
	temp[0] = '0';
	temp[1] = 'x';
	for(i=1; i>=0; i--){
		temp[i*2+2] = HexArray[(BYTE)(w >> (8*(1-i)))/0x10];
		temp[i*2+3] = HexArray[(BYTE)(w >> (8*(1-i)))%0x10];
	}
	temp[6] = 0;
}

void Air::BYTEtoChar(char *temp, BYTE b){
	temp[0] = '0';
	temp[1] = 'x';
	temp[2] = HexArray[(BYTE)b/0x10];
	temp[3] = HexArray[(BYTE)b%0x10];
	temp[4] = 0;
}


void Air::BYTEArraytoString(char *temp, DWORD dwSize, BYTE Array[]){
	int i;
	for(i=0; i<dwSize; i++){
		temp[i*3] = HexArray[Array[i]/0x10];
		temp[i*3+1] = HexArray[Array[i]%0x10];
		temp[i*3+2] = ' ';
	}
	temp[i*3-1] = 0;
}


void Air::INTtoChar(char *temp, DWORD d){
	int i, j, digit = 0;
	DWORD _d;
	bool IsNegative = (d < 0x80000000) ? false : true;

	if(!IsNegative){
		_d=d;
	}else{
		_d=~d+1;
		temp[0] = '-';
	}

	for(i=1; (_d/i); i*=10){
		digit++;
	}
	j=i/10;

	for(i=0; i<digit; i++){
		temp[i+IsNegative] = HexArray[_d/j];
		_d -=j*(_d/j);
		j/=10;
	}
	temp[i+IsNegative] = 0;
}

int Air::GetStringLength(char *_String){
	int i;
	for(i=0; _String[i]; i++){};
	return i;
}

void _stdcall Air::_CreateFormatString(char *_Output, char *_Format, DWORD *dwArgument){
	int i, j, k, last = 0, iSize;
	char temp[MaxString] = {0};
	for(i=j=k=0; _Format[i]; i++){
		if(_Format[i]=='%'){
			memcpy(&_Output[last], temp, j);
			last += j;
			_Output[last] = 0;
			ZeroMemory(temp, MaxString);
			j=0;

			switch(_Format[i+1]){
				case 's'://String
					iSize = GetStringLength((char *)dwArgument[k]);
					memcpy(&_Output[last], (char *)dwArgument[k], iSize);
					last += iSize;
					break;

				case 'd'://DWORD
					DWORDtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, MaxString);
					break;

				case 'w'://WORD
					WORDtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, MaxString);
					break;

				case 'b'://BYTE
					BYTEtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, MaxString);
					break;

				case 'i'://int
					INTtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, MaxString);
					break;

				case 'a':
					BYTEArraytoString(temp, dwArgument[k+1], (BYTE *)dwArgument[k]);
					iSize = dwArgument[k+1]*3-1;
					memcpy(&_Output[last], temp, iSize);
					last += dwArgument[k+1]*3-1;
					_Output[last] = 0;
					ZeroMemory(temp, MaxString);
					k++;
					break;

				default:
					temp[j] = _Format[i];
					i--;
					j++;
					k--;
					break;
			}
			i++;
			k++;
		}
		else{
			temp[j] = _Format[i];
			j++;
		}
	}
	iSize = GetStringLength(temp);
	memcpy(&_Output[last], temp, iSize);
	last += iSize;
	_Output[last] = 0;
	ZeroMemory(temp, MaxString);
}

void _stdcall Air::CreateFormatString(char *_Output, char *_Format, ...){
	_CreateFormatString(_Output, _Format, (DWORD *)(&_Format+1));
}