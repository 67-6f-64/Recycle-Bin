#include<Windows.h>
#include<stdio.h>

int GetItemID(char *s){
	char s1[] = "<int name=\"id\" value=\"";
	int i, j;
	int ItemID;
	j = 0;
	for(i=0; s[i]; i++){
		if(s[i] == s1[j]){
			i++;
			j++;
			if(s1[j] == 0){
				sscanf(&s[i], "%d", &ItemID);
				return ItemID;
			}

		}
		else{
			j = 0;
		}
	}
	return 0;
}

int NodeCheck(char *s){
	int i, j, dc, k, l, sdc;
	char s1[] = "<imgdir name=\"";
	char s2[] = "</imgdir>";
	char dust[1024];
	//char buff[1024];
	int ItemID;

	DWORD State = 0;
	dc = 0;
	j = 0;
	k = 0;
	for(i=0; s[i]; i++){
		if(s[i] == s1[j]){
			j++;
			if(s1[j] == 0){
				dc++;
				i++;
				j = 0;
				for(l=0; s[i+l]!='\"'; l++){
					dust[l] = s[i+l];
				}
				dust[l] = 0;
				if(dc == 1){
					sdc = dc;
					if(strcmp(dust, "Check") == 0){
						State = 1;
					}
					else if(strcmp(dust, "Act") == 0){
						State = 4;
					}
				}
				if(dc == 2 && strcmp(dust, "0") == 0 && State == 1){
					sdc = dc;
					State = 2;
				}
				if(dc == 3 && (strcmp(dust, "quest") == 0 || strcmp(dust, "item") == 0)&& State == 2){
					sdc = dc;
					return 0;
				}
				if(dc == 2 && strcmp(dust, "0") == 0 && State == 4){
					sdc = dc;
					State = 5;
				}
				if(dc == 3 && strcmp(dust, "item") == 0 && State == 5){
					sdc = dc;
					State = 6;
				}
				if(dc == 4 && strcmp(dust, "0") == 0 && State == 6){
					sdc = dc;
					i+= 2;
					//return true;
					//return GetItemID(&s[i]);
					sscanf(&s[i+l], "<int name=\"id\" value=\"%d", &ItemID);
					return ItemID;
				}
			//printf("%d - %s\n", dc, dust);
			}
		}
		else{
			j = 0;
		}

		if(s[i] == s2[k]){
			k++;
			if(s2[k] == 0){
				if(sdc == dc){
					State = 0;
				}
				dc--;
				k = 0;
				if(dc == 0){
					//break;
				}
			}
		}
		else{
			k = 0;
		}
	}
	return 0;
}

void QuestInfo(char *s){

}

int ReadQuest(FILE *fp){
	char s[2][256] = {"<wzimg name=\"", "</wzimg>"};
	BYTE dust[4096*8];
	BYTE *d;
	int ItemID;
	int ID;
	BYTE b;
	int i = 0, j = 0;
	while(fread(&b, 1, 1, fp) != EOF){
		if(s[j][i] == b){
			i++;
		}else{
			i = 0;
		}
		if(j == 1){
			*d++ = b;
		}
		if(s[j][i] == 0){
				switch(j){
				case 0:
					fscanf(fp, "%d", &ID);
					//if(ID == 2047){
						//printf("%d:\n", ID);
					//}
					j++;
					ZeroMemory(dust,  4096*8);
					d = dust;
					break;
				case 1:
					//if(ID == 2047){
					ItemID = NodeCheck((char *)dust);
						if(ItemID){
							FILE *fp2;
							fp2 = fopen("questlist.txt", "a");
							printf("%d:\n", ID);
							fprintf(fp2, "%d:%d\n", ID, ItemID);
							fprintf(fp2, "@00EB 00 %02X %02X 01 00 00 00 %02X %02X %02X %02X\n", ID & 0xFF, (ID >> 8) & 0xFF, ItemID & 0xFF,  (ItemID >> 8) & 0xFF,  (ItemID >> 16) & 0xFF,  (ItemID >> 24) & 0xFF);
							fclose(fp2);
						}
					//}
					j = 0;
					i = 0;
					return 0;
					break;
				default:
					break;
				}
		}
	}
	return EOF;
}

void main(){
	FILE *fp;
	BYTE b;
	int fg = 0;

	fp = fopen("test.xml", "rb");
	if(fp){
		while(fg != EOF){
			fg = ReadQuest(fp);
		}
		fclose(fp);
	}
	system("pause");
}