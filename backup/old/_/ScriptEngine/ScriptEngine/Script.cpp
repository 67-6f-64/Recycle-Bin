#include"Script.h"

int GetStringLength(LPSTR lpString){
	int i;

	for(i=0; lpString[i]; i++){}

	return i;
}

int GetFunctionName(LPSTR lpNewString, LPSTR lpString){
	int i, hit;

	for(i= hit = 0; lpString[i]; i++){
		if(((BYTE)lpString[i] >= '0' && (BYTE)lpString[i] <= '9') 
			|| ((BYTE)lpString[i] >= 'A' && (BYTE)lpString[i] <= 'Z') 
			|| ((BYTE)lpString[i] >= 'a' && (BYTE)lpString[i] <= 'z')){
				lpNewString[hit] = lpString[i];
				hit++;
		}
		else if((BYTE)lpString[i] == '('){
			return i;
		}
	}

	return 0;
}

int GetData(LPSTR lpNewString, LPSTR lpString){
	int i, hit;

	for(i= hit = 0; lpString[i]; i++){
		if(((BYTE)lpString[i] >= '0' && (BYTE)lpString[i] <= '9') 
			|| ((BYTE)lpString[i] >= 'A' && (BYTE)lpString[i] <= 'Z') 
			|| ((BYTE)lpString[i] >= 'a' && (BYTE)lpString[i] <= 'z')){
				lpNewString[hit] = lpString[i];
				hit++;
		}
		else if((BYTE)lpString[i] == ')'){
			return i;
		}
	}

	return 0;
}

int SearchEnding(LPSTR lpString){
	int i;

	for(i=0; lpString[i]; i++){
		if((BYTE)lpString[i] == ';'){
			if((BYTE)lpString[i+1] != '\0'){
				return i+1;
			}
		}
	}

	return 0;
}

bool Proxy(LPSTR lpString){
	LPSTR lpNewString[4] = {(LPSTR)malloc(0x100),(LPSTR)malloc(0x100),(LPSTR)malloc(0x100),(LPSTR)malloc(0x100)};
	int iPtr = 0;
	int iTmptr = 0;
	int i;

	if(lpString == NULL){
		SW.AddString(0x201, "NULL Pointer");
		return false;
	}

	ZeroMemory(lpNewString[0], 0x100);
	iTmptr = GetFunctionName(lpNewString[0], lpString);
	iPtr += iTmptr;

	if(!iTmptr){
		SW.AddString(0x201, "Script was executed perfectly!");
		return true;
	}

	SW.AddString(0x201, lpNewString[0]);

	ZeroMemory(lpNewString[0], 0x100);
	iTmptr = GetData(lpNewString[0], &lpString[iPtr]);
	iPtr += iTmptr;

	if(!iTmptr){
		SW.AddString(0x201, "Unknown Error");
		return true;
	}

	SW.AddString(0x201, lpNewString[0]);

	iPtr += SearchEnding(&lpString[iPtr]);

	if(!iPtr){
		SW.AddString(0x201, "Script was executed perfectly!");
		return true;
	}

	return Proxy(&lpString[iPtr]);
}

void StringClearner(LPSTR lpString){
	int i, hit;
	for(i=hit=0; lpString[i]; i++){
		if(((BYTE)lpString[i] >= '0' && (BYTE)lpString[i] <= '9')
		|| ((BYTE)lpString[i] >= 'A' && (BYTE)lpString[i] <= 'Z')
		|| ((BYTE)lpString[i] >= 'a' && (BYTE)lpString[i] <= 'z')
		){
			lpString[hit] = lpString[i];
			hit++;
		}
	}

	lpString[hit] = '\0';
}


bool Scripter(LPSTR lpString){
	int i;
	int iLength = 0;
	int iNext = 0;
	LPSTR lpFunction, lpTemporaryData, lpData[4];

	if(!lpString){
		SW.AddString(0x201, "Null Pointer (lpString)");
		return false;
	}

	//Get EOF or NextLine
	for(i=0; lpString[i]; i++){
		if(lpString[i] == '\r'){
			iNext = i+1;
			break;
		}
	}

	if(!iNext){
		iLength = i;
	}else{
		iLength = iNext;
	}

	int iDataEnd = 0;
	int iDataStart = 0;


	//Get Data
	for(i=0; i<iLength; i++){
		if(lpString[i] == ')' && lpString[i+1] == ';'){
			iDataEnd = i;//EndAddress for Data
			break;
		}
	}

	if(iDataEnd < 1){
			if(iNext){
				return Scripter(&lpString[iNext]);
			}
		SW.AddString(0x201, "Couldn't find (function end)");
		return false;
	}

	for(i=iDataEnd; i>0; i--){
		if(lpString[i] == '('){
			iDataStart = i+1;//StartAddress for Data
			break;
		}
	}

	if(iDataStart < 1){
			if(iNext){
				return Scripter(&lpString[iNext]);
			}
		SW.AddString(0x201, "Couldn't find (function start)");
		return false;
	}

	lpTemporaryData = (LPSTR)malloc(iDataEnd-iDataStart+1);
	ZeroMemory(&lpTemporaryData[0], (iDataEnd-iDataStart+1));
	memcpy(lpTemporaryData, &lpString[iDataStart], (iDataEnd-iDataStart));
	//StringClearner(lpTemporaryData);

	lpFunction = (LPSTR)malloc(iDataStart);
	ZeroMemory(&lpFunction[0], iDataStart);
	memcpy(lpFunction, &lpString[0], (iDataStart-1));
	StringClearner(lpFunction);

	int iDataCount = 0;

	for(i=iDataStart; i<iDataEnd; i++){
		if(lpString[i] == ','){
			lpData[iDataCount] = (LPSTR)malloc(i-iDataStart+1);
			ZeroMemory(&lpData[iDataCount][0], (i-iDataStart+1));
			memcpy(&lpData[iDataCount][0], &lpString[iDataStart], (i-iDataStart));
			iDataStart = i+1;
			iDataCount++;
		}
	}
			lpData[iDataCount] = (LPSTR)malloc(i-iDataStart+1);
			ZeroMemory(&lpData[iDataCount][0], (i-iDataStart+1));
			memcpy(&lpData[iDataCount][0], &lpString[iDataStart], (i-iDataStart));
			iDataStart = i+1;
			iDataCount++;

	SW.AddString(0x201, lpFunction, -1);

	for(i=0; i<iDataCount; i++){
		StringClearner(lpData[i]);
		SW.AddString(0x201, lpData[i]);
	}
	SW.AddString(0x201, NULL);


	if(iNext){
		return Scripter(&lpString[iNext]);
	}

	SW.AddString(0x201, "Script was executed perfectly!");

	return true;
}
