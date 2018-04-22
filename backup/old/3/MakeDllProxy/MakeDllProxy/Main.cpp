#include<Windows.h>
#include<imagehlp.h>
#include<stdio.h>
#include<stdlib.h>
#include<direct.h>
#pragma comment(lib, "imagehlp.lib")

void MakeDllProxy(char *FileName){
	int i;
	DWORD Address, dw;
	HMODULE hModule = LoadLibraryExA(FileName, NULL, DONT_RESOLVE_DLL_REFERENCES);
	DWORD BaseAddress = (DWORD)hModule;
	IMAGE_EXPORT_DIRECTORY *IED;

	printf("DLL:%s\n", FileName);

	if(!hModule){
		printf("Failed to load the DLL\n");
		return;
	}

	printf("DLL is loaded at %p\n", hModule);

	IED = (IMAGE_EXPORT_DIRECTORY *)ImageDirectoryEntryToData(hModule, FALSE, IMAGE_DIRECTORY_ENTRY_EXPORT, &dw);
	if(!IED){
		printf("wut\n");
		return;
	}

	DWORD *AddressList = (DWORD *)(BaseAddress + IED->AddressOfFunctions);
	DWORD *NamePointer = (DWORD *)(BaseAddress + IED->AddressOfNames);
	char **NameList = (char **)malloc(sizeof(DWORD) * IED->NumberOfNames);
	WORD *OrdinalList = (WORD *)(BaseAddress + IED->AddressOfNameOrdinals);

	for(i=0; i<IED->NumberOfNames; i++){
		NameList[i] = (char *)BaseAddress + NamePointer[i];
	}

	for(i=0; i<IED->NumberOfNames; i++){
		printf("%X:%s@%d\n", AddressList[i], NameList[i], IED->Base + OrdinalList[i]);
	}

	FILE *fp, *data;
	char *DllName = (char *)malloc(256);
	char *NewFile = (char *)malloc(256);
	int lastdot = 0, lastyen = 0;
	BYTE b;

	ZeroMemory(DllName, 256);
	ZeroMemory(NewFile, 256);

	for(i=0; FileName[i]; i++){
		if(FileName[i] == '\\'){
			lastyen = i;
		}
		else if(FileName[i] == '.'){
			lastdot = i;
		}
	}
	for(i=0; i<lastdot - (lastyen+1); i++){
		DllName[i] = FileName[(lastyen+1) + i];
	}

	_mkdir(DllName);
	sprintf(NewFile, "%s\\%s\.def", DllName, DllName);
	fp = fopen(NewFile, "w");
	fprintf(fp, "LIBRARY\nEXPORTS\n");
	for(i=0; i<IED->NumberOfNames; i++){
		fprintf(fp, "\t\%s = %s_Export_%d @%d\n", NameList[i], DllName, IED->Base + OrdinalList[i], IED->Base + OrdinalList[i]);
	}
	fclose(fp);

	sprintf(NewFile, "%s\\%s\.cpp", DllName, DllName);
	fp = fopen(NewFile, "w");
	fprintf(fp, "#include\"%s\.h\"\n", DllName);
	fprintf(fp, "#include\"%s_org_data\.h\"\n\n", DllName);
	fprintf(fp, "DWORD %s_ExportList[0x%X];\n\n", DllName, IED->NumberOfNames);
	for(i=0; i<IED->NumberOfNames; i++){
		fprintf(fp, "void _declspec(naked) %s_Export_%d(){\n\t_asm{\n\t\tmov eax,0x%X\n\t\tjmp dword ptr [%s_ExportList+eax*0x04]\n\t}\n}\n\n", DllName, IED->Base + OrdinalList[i], i, DllName);
	}
	fprintf(fp, "void CreateExports(){\n");
	fprintf(fp, "\tDWORD dw;\n\tHMODULE hModule;\n");
	fprintf(fp, "\tHANDLE hFile = CreateFileA(\"%s\.old\", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL);\n\n", DllName);
	fprintf(fp, "\tif(hFile){\n");
	fprintf(fp, "\t\tWriteFile(hFile, %s_org_data, sizeof(%s_org_data), &dw, NULL);\n", DllName, DllName);
	fprintf(fp, "\t\tCloseHandle(hFile);\n\t}\n");
	fprintf(fp, "\thModule = LoadLibraryA(\"%s\.old\");\n\n", DllName);
	for(i=0; i<IED->NumberOfNames; i++){
		fprintf(fp, "\t%s_ExportList[0x%X] = (DWORD)GetProcAddress(hModule, \"%s\");\n", DllName, i, NameList[i]);
	}
	fprintf(fp, "\n\n}\n\n");
	fclose(fp);

	sprintf(NewFile, "%s\\%s\.h", DllName, DllName);
	fp = fopen(NewFile, "w");
	fprintf(fp, "#pragma once\n");
	fprintf(fp, "#include<Windows\.h>\n\n");
	fprintf(fp, "void CreateExports();\n");

	fclose(fp);
	sprintf(NewFile, "%s\\%s_org_data\.h", DllName, DllName);
	fp = fopen(NewFile, "w");
	data = fopen(FileName, "rb");
	fprintf(fp, "#pragma once\n\n");
	fprintf(fp, "unsigned char %s_org_data[] = {", DllName);
	if(fread(&b, 1, 1, data) == 1){
		fprintf(fp, "0x%02X", b);
		while(fread(&b, 1, 1, data) == 1){
			fprintf(fp, ", 0x%02X", b);
		}
	}
	fprintf(fp, "};");

	fclose(fp);
	fclose(data);
	sprintf(NewFile, "%s\\DllMain.cpp", DllName);
	fp = fopen(NewFile, "w");
	fprintf(fp, "#include\"%s.h\"\n\n", DllName);
	fprintf(fp, "BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){\n\n\tif(fdwReason != DLL_PROCESS_ATTACH){\n\t\treturn FALSE;\n\t}\n\n\tCreateExports();\n\n\treturn TRUE;\n}\n");
	fclose(fp);

	free(NewFile);
	free(DllName);
	printf("Source code is generated XD\n");

	printf(">> %X << \n", (DWORD)IED - BaseAddress);
}

int main(int argc, char *argv[]){
	int i;
	printf("this file doesn\'t support packed dlls\n");
	if(argc > 1){
		MakeDllProxy(argv[1]);
		/*
		for(i=0; i<argc; i++){
			printf("%d:%s\n", i, argv[i]);
		}
		*/
	}
	else{
		printf("Where is DLL?\n");
	}
	system("pause");
	return 0;
}