/******************************************************************************
*
* main.c
*
* Description	:
*
* Author		: 
* First Created : 2026.01.31
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
// 데이터 구조체 정의 (linked_list.h 내용 가정)
#define MAX_CHANNELS 10000

#define SID_MASK		0x1FFF

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
// Force 1-byte alignment for binary consistency
#pragma pack(push, 1)

typedef struct
{
	unsigned short ch;        // Channel Number (index)
    unsigned char name[32];   // Channel Name
    unsigned char fav[16];    // Favorite Status
    unsigned short lcn;       // Logical Channel Number
	unsigned short sid;       // Service ID
} CHANNEL_LIST;

typedef struct Node
{
    CHANNEL_LIST data;  // 채널 데이터
    struct Node* next;  // 다음 노드 주소
} Node;

#pragma pack(pop)

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

// 전역 변수에 현재 설정된 정렬 기준 추가
int currentSortType = 0; // 기본값 0 (CH 기준)
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

void printHelp() {
    printf("\n======================= [ PROGRAM MENU ] =======================");
    printf("\n  1. Add Channel        : Register a new channel");
    printf("\n  2. Print All          : Display all registered channels");
    printf("\n  3. Search (Num)       : Find a channel by its number (CH)");
    printf("\n  4. Delete Channel     : Remove a channel from the list");
    printf("\n  5. Save to File       : Save current data with sort options");
    printf("\n  6. Load from File     : Load data and sort (CH or LCN)");
    printf("\n  7. Update Name        : Modify an existing channel's name");
    printf("\n  8. Sort by LCN        : Reorder the list by LCN number");
    printf("\n  9. Search (Name)      : Search channels by name keyword");
    printf("\n 10. CSV Export         : Export data to 'channels_backup.csv'");
    printf("\n 11. CSV Import         : Import data from 'channels_backup.csv'");
    printf("\n  0. Exit               : Save and terminate the program");
    printf("\n================================================================\n");
    printf(" Tip: Type the number or 'help' to see this menu again.\n");
}

// 입력 버퍼를 비우는 함수
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 안전하게 문자열을 입력받는 함수
void safeGets(char* str, int size) {
    if (fgets(str, size, stdin)) {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0'; // 개행 문자 제거
        } else {
            clearInputBuffer(); // 버퍼에 남은 잔여 데이터 청소
        }
    }
}

// --- 9. 설정값 저장 (Config Save) ---
void saveConfig(int sortType) {
    FILE* file = fopen("config.dat", "wb");
    if (file) {
        fwrite(&sortType, sizeof(int), 1, file);
        fclose(file);
    }
}

// --- 10. 설정값 로드 (Config Load) ---
int loadConfig() {
    int savedType = 0;
    FILE* file = fopen("config.dat", "rb");
    if (file) {
        fread(&savedType, sizeof(int), 1, file);
        fclose(file);
    }
    return savedType;
}

// 1. 채널 검색
void searchChannel() {
    unsigned short searchCh;
    printf("\nEnter Channel Number to search:");
    scanf("%hu", &searchCh);

    Node* curr = head;
    while (curr != NULL) {
        if (curr->data.ch == searchCh) {
            printf("\n[Found channel!] NAME: %s, FAV: %s, LCN: %u SID: %u\n", 
                   curr->data.name, curr->data.fav, curr->data.lcn, curr->data.sid);
            return;
        }
        curr = curr->next;
    }
    printf("\nChannel number not found.\n");
}

// 2. 채널 삭제
void deleteChannel() {
    unsigned short deleteCh;
    printf("\nEnter Channel Number to delete: ");
    if (scanf("%hu", &deleteCh) != 1) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Node *curr = head, *prev = NULL;
    int found = 0;

    // 1. 노드 찾기 및 연결 해제
    while (curr != NULL) {
        if (curr->data.ch == deleteCh) {
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

    if (!found) {
        printf("Channel not found.\n");
        return;
    }

    // 2. 전체 채널 번호 재부여 (Re-indexing)
    // 삭제 후 번호가 비지 않도록 1번부터 순서대로 다시 매깁니다.
    unsigned short newCh = 1;
    Node* reindexCurr = head;
    while (reindexCurr != NULL) {
        reindexCurr->data.ch = newCh++;
        reindexCurr = reindexCurr->next;
    }

    // 3. 자동 증가 카운터 동기화
    // 다음 addChannel 시 마지막 번호 + 1이 되도록 설정합니다.
    lastAssignedCh = (newCh - 1); 

    printf("Channel deleted and list re-indexed successfully.\n");
}

// 중복된 SID가 있으면 1, 없으면 0 반환
int isDuplicateSID(unsigned short sid) {
    Node* curr = head;
    while (curr != NULL) {
        if (curr->data.sid == sid) {
            return 1; // 중복 발견
        }
        curr = curr->next;
    }
    return 0; // 중복 없음
}

// 3. 채널 추가 (중복 체크 포함)
void addChannel() {
    Node* newNode = NULL;
    if (freeListHead != NULL) {
        newNode = freeListHead;
        freeListHead = freeListHead->next;
    } else if (poolIndex < MAX_CHANNELS) {
        newNode = &nodePool[poolIndex++];
    } else {
        printf("\n[Error] Out of storage space!\n");
        return;
    }

    // 1. 자동 채널 번호 할당 (CH)
    newNode->data.ch = ++lastAssignedCh;
    printf("\n[Assigning Channel Number: %u]\n", newNode->data.ch);

    // 2. 채널 이름 입력 및 검증
    printf("Enter Channel Name: ");
    safeGets((char *)newNode->data.name, sizeof(newNode->data.name));
    if (strlen((char*)newNode->data.name) == 0) {
        printf("[Error] Channel name cannot be empty. Cancellation...\n");
        goto cancel_add;
    }

    // 3. 즐겨찾기 입력
    printf("Favorite (ON/OFF): ");
    safeGets((char *)newNode->data.fav, sizeof(newNode->data.fav));

    // 4. LCN 입력
    printf("Logical Channel Number: ");
    if (scanf("%hu", &(newNode->data.lcn)) != 1) {
        newNode->data.lcn = 0;
    }
    clearInputBuffer();

    // 5. Service ID 입력 및 검증
    printf("Service ID (Cannot be 0): ");
    if (scanf("%hu", &(newNode->data.sid)) != 1) {
        printf("[Error] Invalid input for SID. Cancellation...\n");
        clearInputBuffer();
        goto cancel_add;
    }
    clearInputBuffer();

	// [검증 1] SID 0 체크
	// 0x2000 이상의 비트가 포함되어 있거나, 마스킹 후 0이 되는 경우 차단
	if ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK)) {
		printf("[Error] Invalid SID: %u. SID must be between 1 and %u.\n", 
			   newNode->data.sid, SID_MASK);
		goto cancel_add;
	}

    // [검증 2] SID 중복 체크 (추가된 부분)
    if (isDuplicateSID(newNode->data.sid)) {
        printf("[Error] SID %u already exists. Each channel must have a unique SID.\n", newNode->data.sid);
        goto cancel_add;
    }
	
    newNode->data.sid &= SID_MASK; // 정의된 마스크 적용

    // 6. 성공적으로 리스트에 연결
    newNode->next = NULL;
    if (head == NULL) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    printf("Channel added successfully (CH: %u, SID: %u).\n", newNode->data.ch, newNode->data.sid);
    return;

// 예외 발생 시 처리 로직
cancel_add:
    // 사용하려던 노드를 다시 Free List로 반환하고 카운터 복구
    newNode->next = freeListHead;
    freeListHead = newNode;
    lastAssignedCh--; // 할당하려던 번호 취소
    if (newNode == &nodePool[poolIndex - 1]) {
        poolIndex--; // 풀에서 가져온 경우 인덱스도 복구
    }
    printf("Channel addition failed due to invalid data.\n");
}

// 4. 전체 출력
void printAllChannels() {
    if (head == NULL) {
        printf("\nNo channels registered.\n");
        return;
    }
    printf("\n%-10s %-20s %-15s %-10s %-10s\n", "CH", "NAME", "FAV", "LCN", "SID");
    printf("----------------------------------------------------------------------\n");
    Node* curr = head;
    while (curr != NULL) {
        printf("%-10u %-20s %-15s %-10u %-10u\n", 
               curr->data.ch, curr->data.name, curr->data.fav, curr->data.lcn, curr->data.sid);
        curr = curr->next;
    }
}

// 5. 저장 (정렬 후 저장)
// type - 0: 채널(ch) 기준 정렬, 1: LCN 기준 정렬
void saveToFile(const char* filename, int sortType) {
    if (head == NULL) {
        printf("No data to save.\n");
        return;
    }

    // --- [선택적 정렬 로직] ---
    Node *i, *j;
    CHANNEL_LIST temp;

    for (i = head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            int condition = 0;
            
            if (sortType == 0) { // CH 기준
                condition = (i->data.ch > j->data.ch);
            } else {            // LCN 기준
                condition = (i->data.lcn > j->data.lcn);
            }

            if (condition) {
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }

    // --- [파일 쓰기 로직] ---
    FILE* file = fopen(filename, "wb");
    if (!file) return;

    int count = 0;
    fwrite(&count, sizeof(int), 1, file); // 개수 자리 확보

    Node* curr = head;
    while (curr != NULL) {
        fwrite(&(curr->data), sizeof(CHANNEL_LIST), 1, file);
        curr = curr->next;
        count++;
    }

    rewind(file);
    fwrite(&count, sizeof(int), 1, file);
    fclose(file);
    saveConfig(sortType); // 정렬 기준 저장
    currentSortType = sortType;
    printf("\n[Save Success] %d channels (%s criteria)\n", count, sortType == 0 ? "CH" : "LCN");
}

// 중복된 CH가 있으면 1, 없으면 0 반환
int isDuplicateCH(unsigned short ch) {
    Node* curr = head;
    while (curr != NULL) {
        if (curr->data.ch == ch) {
            return 1; // 중복 발견
        }
        curr = curr->next;
    }
    return 0; // 중복 없음
}

// 6. 불러오기 (불러온 후 정렬)
// sortType - 0: 채널(ch) 기준 정렬, 1: LCN 기준 정렬
void loadFromFile(const char* filename, int sortType) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("File not found.\n");
        return;
    }

    // 1. 기존 상태 초기화
    poolIndex = 0;
    freeListHead = NULL;
    head = tail = NULL;

	lastAssignedCh = 0;

    int count = 0;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return;
    }

	int importCount = 0;
    int skipCount = 0;

	// 2. 파일 데이터를 순차적으로 로드하며 검증 수행
    for (int i = 0; i < count && i < MAX_CHANNELS; i++) {
        Node* newNode = &nodePool[poolIndex++];
        
        // 구조체 통째로 읽기
        if (fread(&(newNode->data), sizeof(CHANNEL_LIST), 1, file) != 1) {
            poolIndex--;
            break;
        }

        int isValid = 1;

        // [검증 1] SID 범위 체크 (0 제외 및 마스크 0x1FFF 초과 비트 체크)
        if ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK)) {
            isValid = 0;
        }

        // [검증 2] SID 중복 체크
        // 불러오는 과정에서 파일 내 중복 데이터가 있을 경우 차단
        if (isValid && isDuplicateSID(newNode->data.sid)) {
            isValid = 0;
        }

 		// [검증 3] SID 중복 체크
        if (isValid && isDuplicateSID(newNode->data.sid)) isValid = 0;

        // [검증 4] 기본 필드 체크 (이름 유무 등)
        if (isValid && strlen((char*)newNode->data.name) == 0) {
            isValid = 0;
        }

        if (isValid) {
			// 1. CH 중복 체크 및 재부여
            if (isDuplicateCH(newNode->data.ch)) {
                // 중복되면 현재까지의 최대 번호 + 1 부여
                newNode->data.ch = ++lastAssignedCh;
                printf("[Info] Duplicate CH re-assigned to %u\n", newNode->data.ch);
            } else {
                // 중복이 아니면 최대 번호(lastAssignedCh) 갱신
                if (newNode->data.ch > lastAssignedCh) {
                    lastAssignedCh = newNode->data.ch;
                }
            }

            newNode->data.sid &= SID_MASK; // 마스킹 적용
            newNode->next = NULL;

            if (head == NULL) head = tail = newNode;
            else {
                tail->next = newNode;
                tail = newNode;
            }
            importCount++;
        } else {
            // 유효하지 않은 데이터는 메모리 풀 인덱스 반환
            poolIndex--;
            skipCount++;
        }
    }
    fclose(file);

    // 3. 인자로 받은 정렬 기준에 따라 정렬 수행
    if (head != NULL && head->next != NULL) {
        Node *i, *j;
        CHANNEL_LIST temp;

        for (i = head; i->next != NULL; i = i->next) {
            for (j = i->next; j != NULL; j = j->next) {
                int condition = 0;
                if (sortType == 0) { // CH 기준
                    condition = (i->data.ch > j->data.ch);
                } else {            // LCN 기준
                    condition = (i->data.lcn > j->data.lcn);
                }

                if (condition) {
                    temp = i->data;
                    i->data = j->data;
                    j->data = temp;
                }
            }
        }
    }

	printf("\n[Binary Load Complete]\n");
    printf(" - Success: %d channels\n", importCount);
    if (skipCount > 0) {
        printf(" - Skipped: %d invalid/duplicate channels\n", skipCount);
    }
    printf(" - Next CH: %u\n", lastAssignedCh + 1);
}

// --- 7. 채널 이름 수정 기능 ---
void updateChannelName() {
	unsigned short targetCh;
    printf("\nEnter Channel Number to update name:");
    if (scanf("%hu", &targetCh) != 1) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

	Node* curr = head;
    while (curr != NULL) {
        if (curr->data.ch == targetCh) {
            printf("[Current Channel Name: %s]\n", curr->data.name);
            printf("Enter new name:");
            //safeGets(curr->data.name, sizeof(curr->data.name) - 1); // NULL 제외 크기
            safeGets((char *)curr->data.name, sizeof(curr->data.name)); // unsigned char* 를 char* 로 캐스팅
            printf("Channel %u name updated to '%s'.\n", targetCh, curr->data.name);
            return;
        }
        curr = curr->next;
    }
    printf("\narget channel not found.\n");
}

// --- 8. LCN 정렬 기능 ---
void sortByLCN() {
    if (head == NULL || head->next == NULL) {
        printf("\nNot enough data to sort.\n");
        return;
    }

    Node *i, *j;
    CHANNEL_LIST temp;

    // 데이터 내용만 교체하는 버블 정렬
    for (i = head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->data.lcn > j->data.lcn) {
                // 구조체 전체 복사 (데이터 맞바꾸기)
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }

    printf("\n[Sort Complete] All channels have been sorted by LCN.\n");
}

// --- 11. 이름으로 채널 검색 기능 추가 ---
void searchChannelByName() {
    char searchName[32];
    int foundCount = 0;

    if (head == NULL) {
        printf("\nNo channels registered.\n");
        return;
    }

    printf("\nEnter channel name (keyword) to search:");
    safeGets(searchName, sizeof(searchName));

    if (strlen(searchName) == 0) {
        printf("Please enter a search term.\n");
        return;
    }

    printf("\n[ Search Results for '%s' ]\n", searchName);
    printf("%-10s %-20s %-15s %-10s\n", "CH", "NAME", "FAV", "LCN");
    printf("------------------------------------------------------------\n");

    Node* curr = head;
    while (curr != NULL) {
        // strstr: curr->data.name 안에 searchName이 포함되어 있는지 확인
        if (strstr((const char*)curr->data.name, searchName) != NULL) {
            printf("%-10u %-20s %-15s %-10u %-10u\n", 
                   curr->data.ch, curr->data.name, curr->data.fav, curr->data.lcn, curr->data.sid);
            foundCount++;
        }
        curr = curr->next;
    }

    if (foundCount == 0) {
        printf("No channels found matching the keyword.\n");
    } else {
        printf("------------------------------------------------------------\n");
        printf("Total %d channel(s) found.\n", foundCount);
    }
}

// --- 11. CSV 파일로 내보내기 기능 추가 ---
void exportToCSV(const char* filename) {
    if (head == NULL) {
        printf("\nNo channel data to export.\n");
        return;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to create file.\n");
        return;
    }

    // 1. UTF-8 BOM 추가 (엑셀 한글 깨짐 방지 핵심)
    fputs("\xEF\xBB\xBF", file);

    // 2. CSV 헤더 작성
    fprintf(file, "Channel_No,Name,Favorite,LCN\n");

    Node* curr = head;
    int count = 0;
    while (curr != NULL) {
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
    printf("\n[CSV Export Complete] %d channels saved to '%s'.\n", count, filename);
}

// --- 12. CSV 파일로부터 데이터 불러오기 기능 추가 ---
void importFromCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("CSV file not found.\n");
        return;
    }

    // 1. 초기화
    poolIndex = 0;
    freeListHead = NULL;
    head = tail = NULL;
    lastAssignedCh = 0;

    char line[256];
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    int importCount = 0;
    int skipCount = 0;

    while (fgets(line, sizeof(line), file) && poolIndex < MAX_CHANNELS) {
        Node* newNode = &nodePool[poolIndex++];
        
        int res = sscanf(line, "%hu , \"%[^\"]\" , \"%[^\"]\" , %hu , %hu",
                         &newNode->data.ch,
                         newNode->data.name,
                         newNode->data.fav,
                         &newNode->data.lcn,
                         &newNode->data.sid);

        int isValid = 1;

        // [검증 1] 파싱 성공 여부 및 기본값 체크
        if (res != 5 || newNode->data.ch == 0 || strlen((char*)newNode->data.name) == 0) {
            isValid = 0;
        }

        // [검증 2] SID 범위 체크 (0 제외 및 SID_MASK 0x1FFF 초과 비트 체크)
        // 사용자가 제안하신 (sid & 0x2000)를 포함하여 마스크 밖의 비트가 있는지 확인합니다.
        if (isValid && ((newNode->data.sid == 0) || (newNode->data.sid & ~SID_MASK) || (newNode->data.sid > SID_MASK))) {
            isValid = 0;
        }

        // [검증 3] SID 중복 체크
        if (isValid && isDuplicateSID(newNode->data.sid)) {
            isValid = 0;
        }

        if (isValid) {
			// 1. CH 중복 체크 및 재부여
            if (isDuplicateCH(newNode->data.ch)) {
                // 중복되면 현재까지의 최대 번호 + 1 부여
                newNode->data.ch = ++lastAssignedCh;
                printf("[Info] Duplicate CH re-assigned to %u\n", newNode->data.ch);
            } else {
                // 중복이 아니면 최대 번호(lastAssignedCh) 갱신
                if (newNode->data.ch > lastAssignedCh) {
                    lastAssignedCh = newNode->data.ch;
                }
            }

            // 모든 검증 통과 시 마스킹 적용 후 연결
            newNode->data.sid &= SID_MASK; 
            newNode->next = NULL;

            if (head == NULL) head = tail = newNode;
            else {
                tail->next = newNode;
                tail = newNode;
            }
            importCount++;
        } else {
            poolIndex--; // 유효하지 않은 데이터는 메모리 풀 반환
            skipCount++;
        }
    }

    fclose(file);
    printf("\n[CSV Import Task Completed]\n");
    printf(" - Success: %d channels\n", importCount);
    if (skipCount > 0) {
        printf(" - Skipped: %d channels (Invalid SID, Range, or Duplicate)\n", skipCount);
    }
    printf(" - Next CH to be assigned: %u\n", lastAssignedCh + 1);
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
int main() {
	char input[32]; // 문자열 입력을 저장할 변수
    int choice;

    // 1. 메모리 풀 할당
    nodePool = (Node*)calloc(MAX_CHANNELS, sizeof(Node));

	// 2. 자동 로드 기능 실행
	currentSortType = loadConfig(); // 마지막 저장된 설정 읽기
	printf("System initialized. Last sort criteria: %s\n", currentSortType == 0 ? "CH" : "LCN");
	loadFromFile("channels.dat", currentSortType);

	// 프로그램 시작 시 도움말 한 번 출력
    printHelp();

    while (1) {
		printf("\n[Sort:%s] Select menu (type 'help' for menu): ", currentSortType == 0 ? "CH" : "LCN");
		
		// 1. 입력을 문자열로 받음
		if (scanf("%s", input) != 1) {
            printf("Please enter a numeric value.\n");
            clearInputBuffer(); // 잘못된 문자 입력 시 버퍼 비우기
            continue;
        }
        clearInputBuffer(); // 입력된 숫자 뒤의 엔터값 제거

		// 2. 'help' 입력 여부 먼저 확인
        if (strcmp(input, "help") == 0) {
            printHelp();
            continue;
        }

		// 3. 숫자로 변환 (atoi는 숫자가 아니면 0을 반환)
        choice = atoi(input);

		// 사용자가 '0'을 입력한 것인지, 숫자가 아닌 문자를 입력해서 0이 된 것인지 구분
        if (choice == 0 && strcmp(input, "0") != 0) {
            printf("Invalid input. Type 'help' for menu.\n");
            continue;
        }

		// 4. 메뉴 처리
        switch (choice) {
            case 1: addChannel(); break;
            case 2: printAllChannels(); break;
            case 3: searchChannel(); break;
            case 4: deleteChannel(); break;
			case 5: {
				int sType;
				printf("Select Sort Type (0:CH, 1:LCN):");
				scanf("%d", &sType);
				saveToFile("channels.dat", sType); 
				break;
			}
			case 6: {
				int sType;
				printf("Select sort criteria for loading (0:BY CH, 1:BY LCN):");
				scanf("%d", &sType);
				loadFromFile("channels.dat", sType);
				break;
			}
			case 7: updateChannelName(); break;
            case 8: sortByLCN(); break; // LCN 정렬 기능 호출
			case 9: searchChannelByName(); break;
			case 10: exportToCSV("channels_backup.csv"); break;
			case 11: importFromCSV("channels_backup.csv"); break;
			case 0: free(nodePool); return 0;
            default: printf("Invalid choice.\n");
        }
    }

	free(nodePool);
    printf("System exited.\n");

    return 0;
}


