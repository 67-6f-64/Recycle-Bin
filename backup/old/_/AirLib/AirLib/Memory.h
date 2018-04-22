#include<Windows.h>
#include<psapi.h>
#pragma comment(lib,"psapi.lib")

namespace Sage{
	void MemorySage(HANDLE This);
	void StartMemorySage();
}