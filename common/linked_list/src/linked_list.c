/******************************************************************************
*
* linked_list.c
*
* Description	:
*
* Author		: 
* First Created : 2026.03.04
*
* Copyleft (c) 2026 Every Rights Released.
* All Rights Reversed. 누구나 자유롭게 사용, 수정 및 배포할 수 있습니다.
* 이 소프트웨어는 공유의 가치를 위해 조건 없이 제공됩니다.
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "iso639.h"
#include "iso3166.h"
#include "linked_list.h"

/******************************************************************************
*
*
*
*****************************************************************************/
/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
// 전역 관리 변수
Node* nodePool = NULL;
int poolIndex = 0;
Node* head = NULL;
Node* tail = NULL;
Node* freeListHead = NULL;
unsigned short lastAssignedCh = 0; // 마지막으로 부여된 채널 번호 추적

/******************************************************************************
*
*
*
*****************************************************************************/
/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
static int isDuplicateSID(unsigned short sid)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.sid == sid)
		{
			return 1; // 중복 발견
		}
		curr = curr->next;
	}
	return 0; // 중복 없음
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
static int isDuplicateCH(unsigned short ch)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.ch == ch)
		{
			return 1; // 중복 발견
		}
		curr = curr->next;
	}
	return 0; // 중복 없음
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_PrintAllChannels(void)
{
	if (head == NULL)
	{
		Print("\nNo channels registered.\n");
		return;
	}

	// 헤더와 데이터 개수를 6개로 맞춤 (CH, NAME, LANG, CODE, COUNTRY, CODE, LCN, SID)
	Print("\n%-8s %-20s %-4s %-4s %-4s %-4s %-8s %-8s\n", "CH", "NAME", "LANG", "CODE", "CTRY", "CODE", "LCN", "SID");
	Print("----------------------------------------------------------------------\n");

	Node* curr = head;
	while (curr != NULL)
	{
		// 6개 필드 모두 출력
		Print("%-8u %-20s %-4s %-4u %-4s %-4u %-8u %-8u\n", 
				curr->data.ch, 
				curr->data.name, 
				curr->data.lang, ISO639_Alph3ToLang((const char *)curr->data.lang), // ISO 639
				curr->data.country, ISO3166_GetCountryCodeByAlpha2((const char *)curr->data.country),// ISO 3166
				curr->data.lcn, 
				curr->data.sid);
		
		curr = curr->next;
	}
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_JsonToCsv(const char* jsonFilename, const char* csvFilename)
{
	FILE* jsonFile = fopen(jsonFilename, "r");
	FILE* csvFile = fopen(csvFilename, "w");
	if (!jsonFile || !csvFile)
	{
		Print("Error: File open failed.\n");
		if (jsonFile) fclose(jsonFile);
		if (csvFile) fclose(csvFile);
		return;
	}

	char line[512];

	// [수정] 헤더를 6개 필드로 확장
	fprintf(csvFile, "CH,Name,Lang,Country,LCN,SID\n");

	while (fgets(line, sizeof(line), jsonFile))
	{
		if (strstr(line, "{"))
		{
			// [수정] country 변수 추가 선언
			char ch[16] = "", name[64] = "", lang[64] = "", country[64] = "", lcn[16] = "", sid[16] = "";

			while (fgets(line, sizeof(line), jsonFile) && !strstr(line, "}"))
			{
				if (strstr(line, "\"CH\"")) sscanf(line, "%*[^:]: \"%[^\"]\"", ch);
				else if (strstr(line, "\"Name\"")) sscanf(line, "%*[^:]: \"%[^\"]\"", name);
				else if (strstr(line, "\"Lang\"")) sscanf(line, "%*[^:]: \"%[^\"]\"", lang);
				else if (strstr(line, "\"Country\"")) sscanf(line, "%*[^:]: \"%[^\"]\"", country); // [수정] country 저장
				else if (strstr(line, "\"LCN\"")) sscanf(line, "%*[^:]: \"%[^\"]\"", lcn);
				else if (strstr(line, "\"SID\"")) sscanf(line, "%*[^:]: \"%[^\"]\"", sid);
			}

			// [수정] 6개 필드 출력
			fprintf(csvFile, "%s,\"%s\",\"%s\",\"%s\",%s,%s\n", ch, name, lang, country, lcn, sid);
		}
	}

	fclose(jsonFile);
	fclose(csvFile);
	Print("Successfully converted JSON file to CSV.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_CsvToJson(const char* csvFilename, const char* jsonFilename)
{
	FILE* csvFile = fopen(csvFilename, "r");
	FILE* jsonFile = fopen(jsonFilename, "w");
	if (!csvFile || !jsonFile)
	{
		Print("Error opening files.\n");
		if (csvFile) fclose(csvFile);
		if (jsonFile) fclose(jsonFile);
		return;
	}

	char line[512];
	char headers[CHANNEL_LIST_ITEM][32]; 
	int colCount = 0;

	// 1. 헤더 파싱
	if (fgets(line, sizeof(line), csvFile))
	{
		char* token = strtok(line, ",\n\r"); // 공백 제외(이름에 공백 포함 가능)
		while (token && colCount < CHANNEL_LIST_ITEM)
		{
			strcpy(headers[colCount++], token);
			token = strtok(NULL, ",\n\r");
		}
	}

	fprintf(jsonFile, "[\n");

	// 2. 데이터 읽기
	int first = 1;
	while (fgets(line, sizeof(line), csvFile))
	{
		// 빈 줄 처리
		if (line[0] == '\n' || line[0] == '\r') continue;

		if (!first) fprintf(jsonFile, ",\n");

		// [수정] 배열 크기 수정
		char val[CHANNEL_LIST_ITEM][64]; 
		int valCount = 0;
		char* token = strtok(line, ",\n\r");
		while (token && valCount < CHANNEL_LIST_ITEM)
		{
			strcpy(val[valCount++], token);
			token = strtok(NULL, ",\n\r");
		}

		fprintf(jsonFile, "  {\n");
		for (int i = 0; i < valCount; i++)
		{
			fprintf(jsonFile, "    \"%s\": \"%s\"%s\n", 
			headers[i], val[i], (i == valCount - 1) ? "" : ",");
		}
		fprintf(jsonFile, "  }");
		first = 0;
	}

	fprintf(jsonFile, "\n]");

	fclose(csvFile);
	fclose(jsonFile);
	Print("Successfully converted CSV to JSON.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_ImportFromCSV(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		Print("CSV file not found.\n");
		return;
	}

	// 1. 초기화
	poolIndex = 0;
	freeListHead = NULL;
	head = tail = NULL;
	lastAssignedCh = 0;

	char line[256];
	if (!fgets(line, sizeof(line), file))
	{
		fclose(file);
		return;
	}

	int importCount = 0;
	int skipCount = 0;

	while (fgets(line, sizeof(line), file) && poolIndex < MAX_CHANNELS)
	{
		Node* newNode = &nodePool[poolIndex++];

		int res = sscanf(line, "%hu,\"%63[^\"]\",\"%63[^\"]\",\"%63[^\"]\",%hu,%hu",
								&newNode->data.ch,
								newNode->data.name,
								newNode->data.lang,
								newNode->data.country,
								&newNode->data.lcn,
								&newNode->data.sid);

		int isValid = 1;

		// [검증 1] 파싱 성공 여부 및 기본값 체크
		if (res != CHANNEL_LIST_ITEM/* || newNode->data.ch == 0*/ || strlen((char*)newNode->data.name) == 0)
		{
			isValid = 0;
		}

#if 0
		// [검증 2] SID 범위 체크 (0 제외 및 SID_MASK 0x1FFF 초과 비트 체크)
		// 사용자가 제안하신 (sid & 0x2000)를 포함하여 마스크 밖의 비트가 있는지 확인합니다.
		if (isValid && ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK)))
		{
			isValid = 0;
		}
#endif

		// [검증 3] SID 중복 체크
		if (isValid && isDuplicateSID(newNode->data.sid))
		{
			isValid = 0;
		}

		if (isValid)
		{
			// 1. CH 중복 체크 및 재부여
			if (isDuplicateCH(newNode->data.ch))
			{
				// 중복되면 현재까지의 최대 번호 + 1 부여
				newNode->data.ch = ++lastAssignedCh;
				Print("[Info] Duplicate CH re-assigned to %u\n", newNode->data.ch);
			}
			else
			{
				// 중복이 아니면 최대 번호(lastAssignedCh) 갱신
				if (newNode->data.ch > lastAssignedCh)
				{
					lastAssignedCh = newNode->data.ch;
				}
			}

#if 0
			// 모든 검증 통과 시 마스킹 적용 후 연결
			newNode->data.sid &= SID_MASK; 
#endif
			newNode->next = NULL;

			if (head == NULL)
			{
				head = tail = newNode;
			}
			else
			{
				tail->next = newNode;
				tail = newNode;
			}
			importCount++;
		}
		else
		{
			poolIndex--; // 유효하지 않은 데이터는 메모리 풀 반환
			skipCount++;
		}
	}

	fclose(file);
	Print("\n[CSV Import Task Completed]\n");
	Print(" - Success: %d channels\n", importCount);
	if (skipCount > 0)
	{
		Print(" - Skipped: %d channels (Invalid SID, Range, or Duplicate)\n", skipCount);
	}
	Print(" - Next CH to be assigned: %u\n", lastAssignedCh + 1);
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_ExportToCSV(const char* filename)
{
	if (head == NULL)
	{
		Print("\nNo channel data to export.\n");
		return;
	}

	FILE* file = fopen(filename, "w");
	if (!file)
	{
		Print("Failed to create file: %s\n", filename);
		return;
	}

	// 1. UTF-8 BOM 추가 (엑셀 한글 깨짐 방지 핵심)
	fputs("\xEF\xBB\xBF", file);

	// 2. CSV 헤더 작성
	fprintf(file, "Channel_No,Name,Lang,Country,LCN,SID\n");

	Node* curr = head;
	int count = 0;
	while (curr != NULL)
	{
		// 3. 데이터 기록 (큰따옴표로 필드 감싸기)
		fprintf(file, "%u,\"%s\",\"%s\",\"%s\",%u,%u\n",
				curr->data.ch,
				curr->data.name,
				curr->data.lang,	 // ISO 639 코드
				curr->data.country, // ISO 3166 코드
				curr->data.lcn,
				curr->data.sid);

		curr = curr->next;
		count++;
	}

	fclose(file);
	Print("\n[CSV Export Complete] %d channels saved to '%s'.\n", count, filename);
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_SearchChannelByName(char *name)
{
	int foundCount = 0;

	if (head == NULL)
	{
		Print("\nNo channels registered.\n");
		return;
	}

	Print("\n[ Search Results for '%s' ]\n", name);
	// 헤더와 데이터 개수를 6개로 맞춤 (CH, NAME, LANG, COUNTRY, LCN, SID)
	Print("%-10s %-20s %-15s %-15s %-10s %-10s\n", "CH", "NAME", "LANG", "CTRY", "LCN", "SID");
	Print("------------------------------------------------------------\n");

	Node* curr = head;
	while (curr != NULL)
	{
#if 0
		// 대소문자 무시 버전 (strcasestr 사용 가능 환경 시)
		if (strcasestr((const char*)curr->data.name, name) != NULL)
		{
			Print("%-10u %-20s %-15s %-15s %-10u %-10u\n", 
					curr->data.ch, curr->data.name, curr->data.lang, curr->data.country, curr->data.lcn, curr->data.sid);
			foundCount++;
		}
#else
		// strstr: curr->data.name 안에 searchName이 포함되어 있는지 확인
		if (strstr((const char*)curr->data.name, name) != NULL)
		{
			Print("%-10u %-20s %-15s %-15s %-10u %-10u\n", 
					curr->data.ch, curr->data.name, curr->data.lang, curr->data.country, curr->data.lcn, curr->data.sid);
			foundCount++;
		}
#endif
		curr = curr->next;
	}

	if (foundCount == 0)
	{
		Print("No channels found matching the keyword.\n");
	}
	else
	{
		Print("------------------------------------------------------------\n");
		Print("Total %d channel(s) found.\n", foundCount);
	}
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_SearchChannelByLcn(unsigned short lcn)
{
	int foundCount = 0;

	if (head == NULL)
	{
		Print("\nNo channels registered.\n");
		return;
	}

	Print("\n[ Search Results for '%u' ]\n", lcn);
	Print("%-10s %-20s %-15s %-15s %-10s %-10s\n", "CH", "NAME", "LANG", "CTRY", "LCN", "SID");
	Print("------------------------------------------------------------\n");

	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.lcn == lcn)
		{
			Print("%-10u %-20s %-15s %-15s %-10u %-10u\n", 
					curr->data.ch, curr->data.name, curr->data.lang, curr->data.country, curr->data.lcn, curr->data.sid);
			foundCount++;
		}
		curr = curr->next;
	}

	if (foundCount == 0)
	{
		Print("No channels found matching the keyword.\n");
	}
	else
	{
		Print("------------------------------------------------------------\n");
		Print("Total %d channel(s) found.\n", foundCount);
	}
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_SearchChannelByChannel(unsigned short ch)
{
	int foundCount = 0;

	if (head == NULL)
	{
		Print("\nNo channels registered.\n");
		return;
	}

	Print("\n[ Search Results for '%u' ]\n", ch);
	Print("%-10s %-20s %-15s %-15s %-10s %-10s\n", "CH", "NAME", "LANG", "CTRY", "LCN", "SID");
	Print("------------------------------------------------------------\n");

	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.ch == ch)
		{
			Print("%-10u %-20s %-15s %-15s %-10u %-10u\n", 
					curr->data.ch, curr->data.name, curr->data.lang, curr->data.country, curr->data.lcn, curr->data.sid);
			foundCount++;
		}
		curr = curr->next;
	}

	if (foundCount == 0)
	{
		Print("No channels found matching the keyword.\n");
	}
	else
	{
		Print("------------------------------------------------------------\n");
		Print("Total %d channel(s) found.\n", foundCount);
	}
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_Sort(unsigned char sortType)
{
	if (head == NULL || head->next == NULL)
	{
		Print("\nNot enough data to sort.\n");
		return;
	}

	// --- [선택적 정렬 로직] ---
	Node *i, *j;
	CHANNEL_LIST temp;

	for (i = head; i->next != NULL; i = i->next)
	{
		for (j = i->next; j != NULL; j = j->next)
		{
			int condition = 0;

			if (sortType == SORT_CH) // CH 기준
			{
				condition = (i->data.ch > j->data.ch);
			}
			else if(sortType == SORT_LCN) // LCN 기준
			{            
				condition = (i->data.lcn > j->data.lcn);
			}
			else if(sortType == SORT_SID) // Service ID 기준
			{            
				condition = (i->data.sid > j->data.sid);
			}
			else if(sortType == SORT_NAME) // NAME 기준 (추가)
			{
				// strcmp(s1, s2) > 0 이면 s1이 사전순으로 뒤에 있음
				condition = (strcmp((char*)i->data.name, (char*)j->data.name) > 0);
			}

			if (condition)
			{
				temp = i->data;
				i->data = j->data;
				j->data = temp;
			}
		}
	}

	Print("\n[Sort Complete] Channels have been sorted.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_UpdateChNameByChannel(unsigned short ch, char *name)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.ch == ch)
		{
			memcpy((char *)curr->data.name, name, sizeof(curr->data.name)); // unsigned char* 를 char* 로 캐스팅
			Print("Channel %u name updated to '%s'.\n", ch, name);
			return;
		}
		curr = curr->next;
	}
	Print("\nTarget channel not found.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_UpdateChNameByLcn(unsigned short lcn, char *name)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.lcn == lcn)
		{
			memcpy((char *)curr->data.name, name, sizeof(curr->data.name)); // unsigned char* 를 char* 로 캐스팅
			Print("LCN %u name updated to '%s'.\n", lcn, name);
			return;
		}
		curr = curr->next;
	}
	Print("\nTarget channel not found.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_DeleteChannel(unsigned short ch)
{
	Node *curr = head, *prev = NULL;
	int found = 0;

	// 1. 노드 찾기 및 연결 해제
	while (curr != NULL)
	{
		if (curr->data.ch == ch)
		{
			if (prev == NULL) head = curr->next;
			else prev->next = curr->next;

			if (curr == tail) tail = prev;

			// Free List에 추가하여 메모리 재사용
			curr->next = freeListHead;
			freeListHead = curr;

			found = 1;
			break; 
		}
		prev = curr;
		curr = curr->next;
	}

	if (!found)
	{
		Print("Channel not found.\n");
		return;
	}

	// 2. 전체 채널 번호 재부여 (Re-indexing)
	// 삭제 후 번호가 비지 않도록 1번부터 순서대로 다시 매깁니다.
	unsigned short newCh = 1;
	Node* reindexCurr = head;
	while (reindexCurr != NULL)
	{
		reindexCurr->data.ch = newCh++;
		reindexCurr = reindexCurr->next;
	}

	// 3. 자동 증가 카운터 동기화
	// 다음 LinkedList_AddChannel 시 마지막 번호 + 1이 되도록 설정합니다.
	lastAssignedCh = (newCh - 1); 

	Print("Channel deleted and list re-indexed successfully.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_DeleteChannelByLcn(unsigned short lcn)
{
	Node *curr = head, *prev = NULL;
	int found = 0;

	// 1. 노드 찾기 및 연결 해제
	while (curr != NULL)
	{
		if (curr->data.lcn == lcn)
		{
			if (prev == NULL) head = curr->next;
			else prev->next = curr->next;

			if (curr == tail) tail = prev;

			// Free List에 추가하여 메모리 재사용
			curr->next = freeListHead;
			freeListHead = curr;

			found = 1;
			break; 
		}
		prev = curr;
		curr = curr->next;
	}

	if (!found)
	{
		Print("Channel not found.\n");
		return;
	}

	// 2. 전체 채널 번호 재부여 (Re-indexing)
	// 삭제 후 번호가 비지 않도록 1번부터 순서대로 다시 매깁니다.
	unsigned short newCh = 1;
	Node* reindexCurr = head;
	while (reindexCurr != NULL)
	{
		reindexCurr->data.ch = newCh++;
		reindexCurr = reindexCurr->next;
	}

	// 3. 자동 증가 카운터 동기화
	// 다음 LinkedList_AddChannel 시 마지막 번호 + 1이 되도록 설정합니다.
	lastAssignedCh = (newCh - 1); 

	Print("Channel deleted and list re-indexed successfully.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_SearchChannel(unsigned short ch)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.ch == ch)
		{
			Print("\n[Found channel!] NAME: %s, LANG: %s, CTRY: %s, LCN: %u SID: %u\n", 
			curr->data.name, curr->data.lang, curr->data.country, curr->data.lcn, curr->data.sid);
			return;
		}
		curr = curr->next;
	}
	Print("\nChannel number not found.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_AddChannel(CHANNEL_LIST list)
{
	Node* newNode = NULL;
	if (freeListHead != NULL)
	{
		newNode = freeListHead;
		freeListHead = freeListHead->next;
	}
	else if (poolIndex < MAX_CHANNELS)
	{
		newNode = &nodePool[poolIndex++];
	}
	else
	{
		Print("\n[Error] Out of storage space!\n");
		return;
	}

	// 1. 자동 채널 번호 할당 (CH)
	newNode->data.ch = ++lastAssignedCh;
	Print("Assigning Channel Number : %u\n", newNode->data.ch);

	// 2. 채널 이름 입력 및 검증
	Print("Channel Name : %s\n", list.name);
	memcpy(newNode->data.name, list.name, sizeof(list.name));
	if (strlen((char*)newNode->data.name) == 0)
	{
		sprintf((char*)newNode->data.name, "channel_%d", newNode->data.ch);
	}

	// 3. Language
	Print("Language : %s\n", list.lang);
	memcpy(newNode->data.lang, list.lang, sizeof(list.lang));

	// 4. Country
	Print("Country : %s\n", list.country);
	memcpy(newNode->data.country, list.country, sizeof(list.country));

	// 5. LCN
	Print("Logical Channel Number : %d\n", list.lcn);
	newNode->data.lcn = list.lcn;

	// 6. Service ID 
	Print("Service ID : %d\n", list.sid);
	newNode->data.sid = list.sid;

#if 1
	// [검증 2] SID 중복 체크 (추가된 부분)
	if (isDuplicateSID(newNode->data.sid))
	{
		Print("[Error] SID %u already exists. Each channel must have a unique SID.\n", newNode->data.sid);
		goto cancel_add;
	}
#else
	// [검증 1] SID 0 체크
	// 0x2000 이상의 비트가 포함되어 있거나, 마스킹 후 0이 되는 경우 차단
	if ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK))
	{
		Print("[Error] Invalid SID: %u. SID must be between 1 and %u.\n", 
		newNode->data.sid, SID_MASK);
		goto cancel_add;
	}

	// [검증 2] SID 중복 체크 (추가된 부분)
	if (isDuplicateSID(newNode->data.sid))
	{
		Print("[Error] SID %u already exists. Each channel must have a unique SID.\n", newNode->data.sid);
		goto cancel_add;
	}

	newNode->data.sid &= SID_MASK; // 정의된 마스크 적용
#endif

	// 6. 성공적으로 리스트에 연결
	newNode->next = NULL;
	if (head == NULL)
	{
		head = tail = newNode;
	}
	else
	{
		tail->next = newNode;
		tail = newNode;
	}
	Print("Channel added successfully (CH: %u, SID: %u).\n", newNode->data.ch, newNode->data.sid);
	return;

	// 예외 발생 시 처리 로직
	cancel_add:
	// 사용하려던 노드를 다시 Free List로 반환하고 카운터 복구
	newNode->next = freeListHead;
	freeListHead = newNode;
	lastAssignedCh--; // 할당하려던 번호 취소
	if (newNode == &nodePool[poolIndex - 1])
	{
		poolIndex--; // 풀에서 가져온 경우 인덱스도 복구
	}
	Print("Channel addition failed due to invalid data.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_LoadFromFile(const char* filename) 
{
	FILE* file = fopen(filename, "rb");
	if (!file) 
	{
		Print("File not found.\n");
		return;
	}

	// 1. 기존 상태 초기화
	poolIndex = 0;
	freeListHead = NULL;
	head = tail = NULL;

	lastAssignedCh = 0;

	int count = 0;
	if (fread(&count, sizeof(int), 1, file) != 1)
	{
		fclose(file);
		return;
	}

	int importCount = 0;
	int skipCount = 0;

	// 2. 파일 데이터를 순차적으로 로드하며 검증 수행
	for (int i = 0; i < count && i < MAX_CHANNELS; i++)
	{
		Node* newNode = &nodePool[poolIndex++];

		// 구조체 통째로 읽기
		if (fread(&(newNode->data), sizeof(CHANNEL_LIST), 1, file) != 1)
		{
			poolIndex--;
			break;
		}

		int isValid = 1;

#if 0
		// [검증 1] SID 범위 체크 (0 제외 및 마스크 0x1FFF 초과 비트 체크)
		if ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK))
		{
			isValid = 0;
		}
#endif

		// [검증 2] SID 중복 체크
		// 불러오는 과정에서 파일 내 중복 데이터가 있을 경우 차단
		if (isValid && isDuplicateSID(newNode->data.sid))
		{
			isValid = 0;
		}

		// [검증 3] SID 중복 체크
		if (isValid && isDuplicateSID(newNode->data.sid)) 
		{
			isValid = 0;
		}

		// [검증 4] 기본 필드 체크 (이름 유무 등)
		if (isValid && strlen((char*)newNode->data.name) == 0)
		{
			isValid = 0;
		}

		if (isValid)
		{
			// 1. CH 중복 체크 및 재부여
			if (isDuplicateCH(newNode->data.ch))
			{
				// 중복되면 현재까지의 최대 번호 + 1 부여
				newNode->data.ch = ++lastAssignedCh;
				Print("[Info] Duplicate CH re-assigned to %u\n", newNode->data.ch);
			}
			else
			{
				// 중복이 아니면 최대 번호(lastAssignedCh) 갱신
				if (newNode->data.ch > lastAssignedCh)
				{
					lastAssignedCh = newNode->data.ch;
				}
			}

#if 0
			newNode->data.sid &= SID_MASK; // 마스킹 적용
#endif
			newNode->next = NULL;

			if (head == NULL)
			{
				head = tail = newNode;
			}
			else
			{
				tail->next = newNode;
				tail = newNode;
			}
			importCount++;
		}
		else
		{
			// 유효하지 않은 데이터는 메모리 풀 인덱스 반환
			poolIndex--;
			skipCount++;
		}
	}
	fclose(file);

	Print("\n[Binary Load Complete]\n");
	Print(" - Success: %d channels\n", importCount);
	if (skipCount > 0)
	{
		Print(" - Skipped: %d invalid/duplicate channels\n", skipCount);
	}
	Print(" - Next CH: %u\n", lastAssignedCh + 1);
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_SaveToFile(const char* filename)
{
	if (head == NULL)
	{
		Print("No data to save.\n");
		return;
	}

	// --- [파일 쓰기 로직] ---
	FILE* file = fopen(filename, "wb");
	if (!file) return;

	int count = 0;
	fwrite(&count, sizeof(int), 1, file); // 개수 자리 확보

	Node* curr = head;
	while (curr != NULL)
	{
		fwrite(&(curr->data), sizeof(CHANNEL_LIST), 1, file);
		curr = curr->next;
		count++;
	}

	rewind(file);
	fwrite(&count, sizeof(int), 1, file);
	fclose(file);

	Print("\n[Save Success] %d channels\n", count);

	return;
}

/*-----------------------------------------------------------------------------
*  메모리 풀 기반의 링크드 리스트 및 관련 파일 초기화
*
*
*---------------------------------------------------------------------------*/
void LinkedList_Reset(const char* filename) 
{
    // 1. 메모리 풀 초기화 (데이터 영역 전체 0으로 세팅)
    if (nodePool != NULL)
    {
        // pool 전체 크기를 알고 있다면 그만큼 초기화, 모른다면 멤버 변수 리셋
        memset(nodePool, 0, sizeof(Node) * MAX_CHANNELS); // MAX_POOL_SIZE는 헤더 정의 참조
    }

    // 2. 리스트 관리 변수들 리셋 (중요)
    head = NULL;
    tail = NULL;
    freeListHead = NULL;
    poolIndex = 0;
    lastAssignedCh = 0;

    Print("[Reset] Memory pool and list pointers initialized.\n");

    // 3. 파일 내용 비움
    FILE* file = fopen(filename, "wb");
    if (file)
    {
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
        Print("[Reset] File %s cleared.\n", filename);
    }
    else
    {
        Print("[Error] Failed to open file for reset: %s\n", filename);
    }
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_SaveConfig(CONFIG_LIST config)
{
	FILE* file = fopen(CHANNEL_LIST_CONFIG_FILE, "wb");
	if (file)
	{
		// 구조체 자체를 바이너리 형태로 파일에 기록
		fwrite(&config, sizeof(CONFIG_LIST), 1, file);
		fclose(file);
	}
	else
	{
		Print("[Error] Failed to open config file for saving.\n");
	}
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
CONFIG_LIST LinkedList_LoadConfig(void)
{
	CONFIG_LIST config = {0}; // 기본값으로 초기화
	FILE* file = fopen(CHANNEL_LIST_CONFIG_FILE, "rb");
	if (file)
	{
		// 파일에서 구조체 크기만큼 읽어오기
		if (fread(&config, sizeof(CONFIG_LIST), 1, file) != 1)
		{
			Print("[Warning] Failed to read config, using default values.\n");
		}
		fclose(file);
	}
	return config;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
int LinkedList_Free(void)
{
	if(nodePool != NULL)
	{
		free(nodePool);
		Print("Memory pool cleared successfully.\n");
		return 0;
	}

	return 0;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
int LinkedList_Init(void)
{
    nodePool = (Node*)calloc(MAX_CHANNELS, sizeof(Node));
	if(nodePool == NULL)
	{
		Print("Memory Pool exhausted! No more channels can be added.\n");
		return -1;
	}

	Print("Memory pool initialized successfully.\n");

	return 0;
}

