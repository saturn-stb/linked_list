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
* 채널 리스트 출력
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

    // 헤더: 열별 너비를 통일 (CH:4, NAME:20, V_PID:8, V_TYPE:6, A_PID:8, A_TYPE:6, A_LANG:8, LCN:6, SID:6)
    Print("\n%-4s %-20s %-8s %-6s %-8s %-6s %-8s %-6s %-6s\n", 
          "CH", "NAME", "V_PID", "V_TYPE", "A_PID", "A_TYPE", "A_LANG", "LCN", "SID");
    Print("------------------------------------------------------------------------------------------\n");

    Node* curr = head;
    while (curr != NULL)
    {
        // 1. 비디오 스트림 (v=0)과 오디오 스트림 (a=0)을 한 줄에 출력
        // %-8.8s 등을 사용하여 문자열 길이도 고정 출력
        Print("%-4u %-20.20s 0x%04X %-6u 0x%04X %-6u %-8.8s %-6u %-6u\n", 
              curr->data.ch, 
              curr->data.name, 
              curr->data.video[0].pid, 
              curr->data.video[0].type,
              curr->data.audio[0].pid, 
              curr->data.audio[0].type, 
              curr->data.audio[0].lang, 
              curr->data.lcn, 
              curr->data.sid);

        // 2. 나머지 오디오 스트림(1~15) 출력
        for (int a = 1; a < 16; a++)
        {
            if (curr->data.audio[a].pid != 0)
            {
                // 빈 칸은 비워두고 오디오 정보만 출력 (정렬 유지)
                Print("%-4s %-20s %-8s %-6s 0x%04X %-6u %-8.8s %-6s %-6s\n",
                      "", "", "", "", 
                      curr->data.audio[a].pid, 
                      curr->data.audio[a].type, 
                      curr->data.audio[a].lang, "", "");
            }
        }
        curr = curr->next;
    }
}

/*-----------------------------------------------------------------------------
* CSV 파일에서 채널 데이터를 안전하게 로드하고 메모리 풀에 할당
*
*
*---------------------------------------------------------------------------*/
void LinkedList_ImportFromCSV(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        Print("[Error] CSV file not found: %s\n", filename);
        return;
    }

    // 1. 초기화 (메모리 풀 전체 리셋)
    memset(nodePool, 0, sizeof(Node) * MAX_POOL_SIZE);
    poolIndex = 0;
    head = tail = NULL;
    lastAssignedCh = 0;

    char line[256];
    // 헤더 행 건너뛰기
    if (!fgets(line, sizeof(line), file))
    {
        fclose(file);
        return;
    }

    int importCount = 0;
    int skipCount = 0;

    while (fgets(line, sizeof(line), file) && poolIndex < MAX_POOL_SIZE)
    {
        Node* newNode = &nodePool[poolIndex];
        
        // [수정] 임시 버퍼 사용: sscanf 결과를 임시 변수에 저장 후 구조체로 복사
        unsigned short temp_ch, temp_lcn, temp_sid;
        char temp_name[32], temp_lang[4], temp_country[4];

        int res = sscanf(line, "%hu,\"%31[^\"]\",\"%3[^\"]\",\"%3[^\"]\",%hu,%hu",
                         &temp_ch, temp_name, temp_lang, temp_country, &temp_lcn, &temp_sid);

        if (res < 6 || strlen(temp_name) == 0)
        {
            skipCount++;
            continue;
        }

        // [구조체 반영] 데이터 복사 및 구조체 정의에 맞게 할당
        newNode->data.ch = temp_ch;
        strncpy((char*)newNode->data.name, temp_name, sizeof(newNode->data.name) - 1);
        
        // AUDIO_INFO 구조체 내부의 0번째 오디오 정보에 언어 코드 할당
        strncpy((char*)newNode->data.audio[0].lang, temp_lang, sizeof(newNode->data.audio[0].lang) - 1);
        
        strncpy((char*)newNode->data.country, temp_country, sizeof(newNode->data.country) - 1);
        newNode->data.lcn = temp_lcn;
        newNode->data.sid = temp_sid;

        // [검증 2] SID 유효성 및 중복 체크
        if (isDuplicateSID(newNode->data.sid) || newNode->data.sid == 0)
        {
            skipCount++;
            continue;
        }

        // [검증 3] CH 중복 처리
        if (isDuplicateCH(newNode->data.ch))
        {
            newNode->data.ch = ++lastAssignedCh;
        }
        else
        {
            if (newNode->data.ch > lastAssignedCh)
                lastAssignedCh = newNode->data.ch;
        }

        newNode->next = NULL;
        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
        
        poolIndex++;
        importCount++;
    }

    fclose(file);
    Print("\n[CSV Import Task Completed]\n - Success: %d, Skipped: %d\n", importCount, skipCount);
}

/*-----------------------------------------------------------------------------
* CSV 데이터 내의 따옴표를 이스케이프 처리하여 구조 안정성 확보
*
*
*---------------------------------------------------------------------------*/
static void fprintf_csv_string(FILE* file, const char* str)
{
    fputc('"', file); // 시작 따옴표
    while (*str) {
        if (*str == '"') fputc('"', file); // 따옴표가 있으면 두 번 출력하여 이스케이프 (CSV 표준)
        fputc(*str, file);
        str++;
    }
    fputc('"', file); // 끝 따옴표
}

/*-----------------------------------------------------------------------------
* 채널 데이터를 CSV 형식으로 안전하게 내보내기
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
        Print("[Error] Failed to create file: %s\n", filename);
        return;
    }

    // 1. UTF-8 BOM 추가 (엑셀 한글 깨짐 방지)
    fputs("\xEF\xBB\xBF", file);

    // 2. CSV 헤더 작성
    fprintf(file, "CH,Name,Lang,Country,LCN,SID\n");

    Node* curr = head;
    int count = 0;
    while (curr != NULL)
    {
        // 3. 데이터 기록 (필드별로 이스케이프 처리)
        fprintf(file, "%u,", curr->data.ch);
        
        fprintf_csv_string(file, (char*)curr->data.name);
        fputc(',', file);
        
        fprintf_csv_string(file, (char*)curr->data.audio[0].lang);
        fputc(',', file);
        
        fprintf_csv_string(file, (char*)curr->data.country);
        
        fprintf(file, ",%u,%u\n", curr->data.lcn, curr->data.sid);

        curr = curr->next;
        count++;
    }

    fclose(file);
    Print("\n[CSV Export Complete] %d channels saved to '%s'.\n", count, filename);
}

/*-----------------------------------------------------------------------------
* 대소문자 구분 없는 문자열 검색을 위한 보조 함수
*
*
*---------------------------------------------------------------------------*/
static int str_contains_nocase(const char *haystack, const char *needle)
{
    if (!needle || *needle == '\0') return 1;
    for (; *haystack != '\0'; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h, *n;
            for (h = haystack, n = needle; *h != '\0' && *n != '\0'; h++, n++) {
                if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) break;
            }
            if (*n == '\0') return 1;
        }
    }
    return 0;
}

/*-----------------------------------------------------------------------------
*  채널 이름으로 검색 (대소문자 구분 없음)
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

    Print("\n[ Search Results for Keyword: '%s' ]\n", name);
    // 헤더: CH(8), NAME(20), LANG(6), CTRY(6), LCN(6), SID(6)
    Print("%-8s %-20s %-6s %-6s %-6s %-6s\n", "CH", "NAME", "LANG", "CTRY", "LCN", "SID");
    Print("----------------------------------------------------------------------\n");

    Node* curr = head;
    while (curr != NULL)
    {
        // 대소문자 무시 검색 (직접 구현한 str_contains_nocase 함수 사용)
        if (str_contains_nocase((const char*)curr->data.name, name))
        {
            // 구조체 정의에 맞춘 데이터 접근
            // LANG은 4바이트 배열이므로 %-6.4s를 사용하여 너비 지정
            Print("%-8u %-20.20s %-6.4s %-6.4s %-6u %-6u\n", 
                  curr->data.ch, 
                  curr->data.name, 
                  curr->data.audio[0].lang, 
                  curr->data.country, 
                  curr->data.lcn, 
                  curr->data.sid);
            
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
        Print("----------------------------------------------------------------------\n");
        Print("Total %d channel(s) found.\n", foundCount);
    }
}


/*-----------------------------------------------------------------------------
* LCN(Logical Channel Number)으로 채널 검색
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

    Print("\n[ Search Results for LCN: %u ]\n", lcn);
    // 다른 함수와 동일한 일관된 헤더 포맷 사용
    Print("%-8s %-20s %-8s %-8s %-6s %-6s\n", "CH", "NAME", "LANG", "CTRY", "LCN", "SID");
    Print("----------------------------------------------------------------------\n");

    Node* curr = head;
    while (curr != NULL)
    {
        // 입력된 LCN과 일치하는 채널 검색
        if (curr->data.lcn == lcn)
        {
            Print("%-8u %-20.20s %-8s %-8s %-6u %-6u\n", 
                  curr->data.ch, 
                  curr->data.name, 
                  curr->data.audio[0].lang, // 구조체 정의에 따른 올바른 접근
                  curr->data.country, 
                  curr->data.lcn, 
                  curr->data.sid);
            foundCount++;
        }
        curr = curr->next;
    }

    if (foundCount == 0)
    {
        Print("No channels found matching the LCN.\n");
    }
    else
    {
        Print("----------------------------------------------------------------------\n");
        Print("Total %d channel(s) found.\n", foundCount);
    }
}

/*-----------------------------------------------------------------------------
* 채널 번호(CH)로 채널 검색
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

    Print("\n[ Search Results for Channel: %u ]\n", ch);
    // 일관된 표 헤더 레이아웃 적용
    Print("%-8s %-20s %-8s %-8s %-6s %-6s\n", "CH", "NAME", "LANG", "CTRY", "LCN", "SID");
    Print("----------------------------------------------------------------------\n");

    Node* curr = head;
    while (curr != NULL)
    {
        // 입력된 채널 번호와 일치하는지 확인
        if (curr->data.ch == ch)
        {
            // 구조체 정의(audio[0].lang)에 맞춰 필드 접근 수정
            Print("%-8u %-20.20s %-8s %-8s %-6u %-6u\n", 
                  curr->data.ch, 
                  curr->data.name, 
                  curr->data.audio[0].lang, 
                  curr->data.country, 
                  curr->data.lcn, 
                  curr->data.sid);
            foundCount++;
        }
        curr = curr->next;
    }

    if (foundCount == 0)
    {
        Print("No channels found matching the channel number.\n");
    }
    else
    {
        Print("----------------------------------------------------------------------\n");
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
			Print("\n[Found channel!] NAME: %s, LCN: %u SID: %u\n", 
			curr->data.name, curr->data.lcn, curr->data.sid);
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
	Print("LinkedList_AddChannel\n");
	
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

	// 3. Vido PID
	Print("Video PID : 0x%04x, type 0x%02x\n", list.video[0].pid, list.video[0].type);
	memcpy(&newNode->data.video[0], &list.video[0], sizeof(VIDEO_INFO)*2);

	// 4. Audio PID
	Print("Audio PID : 0x%04x, type 0x%02x\n", list.audio[0].pid, list.audio[0].type);
	memcpy(&newNode->data.audio[0], &list.audio[0], sizeof(AUDIO_INFO)*16);

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

