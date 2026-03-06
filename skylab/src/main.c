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
// 전역 변수에 현재 설정된 정렬 기준 추가
int currentSortType = 0; // 기본값 0 (CH 기준)

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
void printHelp(void)
{
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

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void clearInputBuffer(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void safeGets(char* str, int size)
{
	if (fgets(str, size, stdin))
	{
		size_t len = strlen(str);
		if (len > 0 && str[len - 1] == '\n')
		{
			str[len - 1] = '\0'; // 개행 문자 제거
		}
		else
		{
			clearInputBuffer(); // 버퍼에 남은 잔여 데이터 청소
		}
	}
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
int main() 
{
	int ret = -1;
	char input[32]; // 문자열 입력을 저장할 변수
    int choice;

	ret = LinkedList_Init();
	if(ret < 0)
	{
		return -1;
	}

	// 2. 자동 로드 기능 실행
	currentSortType = LinkedList_LoadConfig(); // 마지막 저장된 설정 읽기
	printf("System initialized. Last sort criteria: %s\n", currentSortType == 0 ? "CH" : "LCN");
	LinkedList_LoadFromFile("channels.dat", currentSortType);

	// 프로그램 시작 시 도움말 한 번 출력
    printHelp();

    while (1)
	{
		printf("\n[Sort:%s] Select menu (type 'help' for menu): ", currentSortType == 0 ? "CH" : "LCN");
		
		// 1. 입력을 문자열로 받음
		if (scanf("%s", input) != 1)
		{
            printf("Please enter a numeric value.\n");
            clearInputBuffer(); // 잘못된 문자 입력 시 버퍼 비우기
            continue;
        }
        clearInputBuffer(); // 입력된 숫자 뒤의 엔터값 제거

		// 2. 'help' 입력 여부 먼저 확인
        if (strcmp(input, "help") == 0)
		{
            printHelp();
            continue;
        }

		// 3. 숫자로 변환 (atoi는 숫자가 아니면 0을 반환)
        choice = atoi(input);

		// 사용자가 '0'을 입력한 것인지, 숫자가 아닌 문자를 입력해서 0이 된 것인지 구분
        if (choice == 0 && strcmp(input, "0") != 0)
		{
            printf("Invalid input. Type 'help' for menu.\n");
            continue;
        }

		// 4. 메뉴 처리
        switch (choice)
		{
            case 1:
			{
				CHANNEL_LIST chList;
				memset(&chList, 0x0, sizeof(CHANNEL_LIST));

				printf("\nEnter Channel Name: ");
				safeGets((char *)chList.name, sizeof(chList.name));
				if (strlen((char*)chList.name) == 0)
				{
					printf("Channel name cannot be empty. Cancellation...\n");
					break;
				}
				
				// 3. 즐겨찾기 입력
				printf("Favorite : ");
				safeGets((char *)chList.fav, sizeof(chList.fav));
				
				// 4. LCN 입력
				printf("Logical Channel Number: ");
				if (scanf("%hu", &(chList.lcn)) != 1)
				{
					chList.lcn = 0;
				}
				clearInputBuffer();
				
				// 5. Service ID 입력 및 검증
				printf("Service ID (Cannot be 0): ");
				if (scanf("%hu", &(chList.sid)) != 1)
				{
					printf("Invalid input for SID. Cancellation...\n");
					clearInputBuffer();
					break;
				}
				clearInputBuffer();

				LinkedList_AddChannel(chList);
            }
			break;
            case 2: LinkedList_PrintAllChannels(); break;
            case 3: LinkedList_SearchChannel(); break;
            case 4:
			{
				unsigned short deleteCh;
				Print("\nEnter Channel Number to delete: ");
				if (scanf("%hu", &deleteCh) != 1)
				{
					clearInputBuffer();
					break;
				}
				clearInputBuffer();
				
				LinkedList_DeleteChannel(deleteCh);
            }
			break;
			case 5:
			{
				int sType;
				printf("Select Sort Type (0:CH, 1:LCN):");
				scanf("%d", &sType);
				if(LinkedList_SaveToFile("channels.dat", sType) == 0)
				{
					currentSortType = sType;
				}
				break;
			}
			case 6:
			{
				int sType;
				printf("Select sort criteria for loading (0:BY CH, 1:BY LCN):");
				scanf("%d", &sType);
				LinkedList_LoadFromFile("channels.dat", sType);
				break;
			}
			case 7:
			{
				unsigned short targetCh;
				Print("\nEnter Channel Number to update name:");
				if (scanf("%hu", &targetCh) != 1)
				{
					clearInputBuffer();
					break;
				}
				clearInputBuffer();

				char searchName[32];
				Print("Enter new name:");
				//safeGets(searchName, sizeof(searchName) - 1); // NULL 제외 크기
				safeGets((char *)searchName, sizeof(searchName)); // unsigned char* 를 char* 로 캐스팅

				LinkedList_UpdateChannelName(targetCh, searchName);
			}
			break;
            case 8: LinkedList_SortByLCN(); break; // LCN 정렬 기능 호출
			case 9:
			{
				char searchName[32];
				Print("\nEnter channel name (keyword) to search:");
				safeGets(searchName, sizeof(searchName));
				
				if (strlen(searchName) == 0)
				{
					Print("Please enter a search term.\n");
					break;
				}
				
				LinkedList_SearchChannelByName(searchName);
			}
			break;
			case 10: LinkedList_ExportToCSV("channels_backup.csv"); break;
			case 11: LinkedList_ImportFromCSV("channels_backup.csv"); break;
			case 0: LinkedList_Free(); return 0;
            default: printf("Invalid choice.\n");
        }
    }

	LinkedList_Free();
    printf("System exited.\n");

    return 0;
}

