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

#include "util.h"
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
	Print("\n%-10s %-20s %-15s %-10s %-10s\n", "CH", "NAME", "FAV", "LCN", "SID");
	Print("----------------------------------------------------------------------\n");
	Node* curr = head;
	while (curr != NULL)
	{
		Print("%-10u %-20s %-15s %-10u %-10u\n", 
		curr->data.ch, curr->data.name, curr->data.fav, curr->data.lcn, curr->data.sid);
		curr = curr->next;
	}
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

		int res = sscanf(line, "%hu , \"%[^\"]\" , \"%[^\"]\" , %hu , %hu",
		&newNode->data.ch,
		newNode->data.name,
		newNode->data.fav,
		&newNode->data.lcn,
		&newNode->data.sid);

		int isValid = 1;

		// [검증 1] 파싱 성공 여부 및 기본값 체크
		if (res != 5 || newNode->data.ch == 0 || strlen((char*)newNode->data.name) == 0)
		{
			isValid = 0;
		}

		// [검증 2] SID 범위 체크 (0 제외 및 SID_MASK 0x1FFF 초과 비트 체크)
		// 사용자가 제안하신 (sid & 0x2000)를 포함하여 마스크 밖의 비트가 있는지 확인합니다.
		if (isValid && ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK)))
		{
			isValid = 0;
		}

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

			// 모든 검증 통과 시 마스킹 적용 후 연결
			newNode->data.sid &= SID_MASK; 
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
		Print("Failed to create file.\n");
		return;
	}

	// 1. UTF-8 BOM 추가 (엑셀 한글 깨짐 방지 핵심)
	fputs("\xEF\xBB\xBF", file);

	// 2. CSV 헤더 작성
	fprintf(file, "Channel_No,Name,Favorite,LCN\n");

	Node* curr = head;
	int count = 0;
	while (curr != NULL)
	{
		// 3. 데이터 기록 (큰따옴표로 필드 감싸기)
		fprintf(file, "%u,\"%s\",\"%s\",%u,%u\n", 
				curr->data.ch, 
				curr->data.name, 
				curr->data.fav, 
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
	Print("%-10s %-20s %-15s %-10s\n", "CH", "NAME", "FAV", "LCN");
	Print("------------------------------------------------------------\n");

	Node* curr = head;
	while (curr != NULL)
	{
		// strstr: curr->data.name 안에 searchName이 포함되어 있는지 확인
		if (strstr((const char*)curr->data.name, name) != NULL)
		{
			Print("%-10u %-20s %-15s %-10u %-10u\n", 
			curr->data.ch, curr->data.name, curr->data.fav, curr->data.lcn, curr->data.sid);
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
void LinkedList_SortByLCN(void)
{
	if (head == NULL || head->next == NULL)
	{
		Print("\nNot enough data to sort.\n");
		return;
	}

	Node *i, *j;
	CHANNEL_LIST temp;

	// 데이터 내용만 교체하는 버블 정렬
	for (i = head; i->next != NULL; i = i->next)
	{
		for (j = i->next; j != NULL; j = j->next)
		{
			if (i->data.lcn > j->data.lcn)
			{
				// 구조체 전체 복사 (데이터 맞바꾸기)
				temp = i->data;
				i->data = j->data;
				j->data = temp;
			}
		}
	}

	Print("\n[Sort Complete] All channels have been sorted by LCN.\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_UpdateChannelName(unsigned short ch, char *name)
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
void LinkedList_SearchChannel(unsigned short ch)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->data.ch == ch)
		{
			Print("\n[Found channel!] NAME: %s, FAV: %s, LCN: %u SID: %u\n", 
			curr->data.name, curr->data.fav, curr->data.lcn, curr->data.sid);
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

	// 3. 즐겨찾기 입력
	Print("Favorite : %s\n", list.fav);
	memcpy(newNode->data.fav, list.fav, sizeof(list.fav));

	// 4. LCN 입력
	Print("Logical Channel Number : %d\n", list.lcn);
	newNode->data.lcn = list.lcn;

	// 5. Service ID 입력 및 검증
	Print("Service ID : %d\n", list.sid);
	newNode->data.sid = list.sid;

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
void LinkedList_LoadFromFile(const char* filename, int sortType) 
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

		// [검증 1] SID 범위 체크 (0 제외 및 마스크 0x1FFF 초과 비트 체크)
		if ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK))
		{
			isValid = 0;
		}

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

			newNode->data.sid &= SID_MASK; // 마스킹 적용
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

	// 3. 인자로 받은 정렬 기준에 따라 정렬 수행
	if (head != NULL && head->next != NULL)
	{
		Node *i, *j;
		CHANNEL_LIST temp;

		for (i = head; i->next != NULL; i = i->next)
		{
			for (j = i->next; j != NULL; j = j->next)
			{
				int condition = 0;
				if (sortType == 0) // CH 기준
				{
					condition = (i->data.ch > j->data.ch);
				}
				else // LCN 기준
				{
					condition = (i->data.lcn > j->data.lcn);
				}

				if (condition)
				{
					temp = i->data;
					i->data = j->data;
					j->data = temp;
				}
			}
		}
	}

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
int LinkedList_SaveToFile(const char* filename, int sortType)
{
	if (head == NULL)
	{
		Print("No data to save.\n");
		return -1;
	}

	// --- [선택적 정렬 로직] ---
	Node *i, *j;
	CHANNEL_LIST temp;

	for (i = head; i->next != NULL; i = i->next)
	{
		for (j = i->next; j != NULL; j = j->next)
		{
			int condition = 0;

			if (sortType == 0) // CH 기준
			{
				condition = (i->data.ch > j->data.ch);
			}
			else // LCN 기준
			{            
				condition = (i->data.lcn > j->data.lcn);
			}

			if (condition)
			{
				temp = i->data;
				i->data = j->data;
				j->data = temp;
			}
		}
	}

	// --- [파일 쓰기 로직] ---
	FILE* file = fopen(filename, "wb");
	if (!file) return -1;

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
	LinkedList_SaveConfig(sortType); // 정렬 기준 저장
	Print("\n[Save Success] %d channels (%s criteria)\n", count, sortType == 0 ? "CH" : "LCN");

	return sortType;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void LinkedList_SaveConfig(int sortType)
{
	FILE* file = fopen("config.dat", "wb");
	if (file)
	{
		fwrite(&sortType, sizeof(int), 1, file);
		fclose(file);
	}
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
int LinkedList_LoadConfig(void)
{
	int savedType = 0;
	FILE* file = fopen("config.dat", "rb");
	if (file) 
	{
		fread(&savedType, sizeof(int), 1, file);
		fclose(file);
	}
	return savedType;
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

