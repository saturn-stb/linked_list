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

#include "scan.h"

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
	Print("\n======================= [ PROGRAM MENU ] =======================");
	Print("\n  1. Add Channel        : Register a new channel");
	Print("\n  2. Search (Num)       : Find a channel by its number (CH)");
	Print("\n  3. Delete Channel     : Remove a channel from the list");
	Print("\n  4. Save to File       : Save current data with sort options");
	Print("\n  5. Load from File     : Load data");
	Print("\n  6. Reset from File    : Reset data");
	Print("\n  7. Update Name        : Modify an existing channel's name");
	Print("\n  8. Sort               : Reorder the list by sort (CH or LCN, Name)");
	Print("\n  9. Search (Name)      : Search channels by name keyword");
	Print("\n 10. Print All          : Display all registered channels");
	Print("\n 11. CSV Export         : Export data to 'channels_backup.csv'");
	Print("\n 12. CSV Import         : Import data from 'channels_backup.csv'");
	Print("\n 13. Convert to JSON    : Convert data from 'channels_backup.json");
	Print("\n 14. Convert to CSV     : Convert data from 'channels_backup.csv");
	Print("\n 21. DVB SI Parse       : DVB SI Parsing from input file");
	Print("\n  0. Exit               : Save and terminate the program");
	Print("\n================================================================\n");
	Print(" Tip: Type the number or 'help' to see this menu again.\n");
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
	CONFIG_LIST configList;

	ret = LinkedList_Init();
	if(ret < 0)
	{
		return -1;
	}

	// 2. 자동 로드 기능 실행
	configList = LinkedList_LoadConfig(); // 마지막 저장된 설정 읽기
	Print("System initialized. Last sort criteria: %s\n", (configList.sort == 0) ? "CH" : "LCN");
	LinkedList_LoadFromFile(CHANNEL_LIST_FILE);

	// 프로그램 시작 시 도움말 한 번 출력
    printHelp();

    while (1)
	{
		Print("\n[Sort:%s] Select menu (type 'help' for menu): ", (configList.sort == 0) ? "CH" : "LCN");
		
		// 1. 입력을 문자열로 받음
		if (scanf("%s", input) != 1)
		{
            Print("Please enter a numeric value.\n");
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
            Print("Invalid input. Type 'help' for menu.\n");
            continue;
        }

		// 4. 메뉴 처리
        switch (choice)
		{
            case 1:
			{
				int tempType;
				CHANNEL_LIST chList;
				memset(&chList, 0x0, sizeof(CHANNEL_LIST));

				// 2. Channel Name 입력
				Print("\nEnter Channel Name: ");
				safeGets((char *)chList.name, sizeof(chList.name));
				if (strlen((char*)chList.name) == 0)
				{
					Print("Channel name cannot be empty. Cancellation...\n");
					break;
				}

				// 3. Video pid 입력
				Print("Video PID : ");
				if (scanf("%hu", &(chList.video[0].pid)) != 1)
				{
					Print("Invalid input for video PID. Cancellation...\n");
					clearInputBuffer();
					break;
				}

				// 4. Video type 입력
				Print("Video type : ");
				if (scanf("%u", &tempType) != 1)
				{
					Print("Invalid input for video type. Cancellation...\n");
					clearInputBuffer();
					break;
				}
				chList.video[0].type = (unsigned char)tempType;

				// 5. Audio PID 입력
				Print("Audio PID : ");
				if (scanf("%hu", &(chList.audio[0].pid)) != 1)
				{
					Print("Invalid input for audio PID. Cancellation...\n");
					clearInputBuffer();
					break;
				}

				// 6. Audio type 입력
				Print("Audio type : ");
				if (scanf("%d", &tempType) != 1)
				{
					Print("Invalid input for audio type. Cancellation...\n");
					clearInputBuffer();
					break;
				}
				chList.audio[0].type = (unsigned char)tempType;

				// 7. Audio language 입력
				Print("Audio language : ");
				if (scanf("%3s", (char *)(chList.audio[0].lang)) != 1)
				{
					Print("Invalid input for audio language. Cancellation...\n");
					clearInputBuffer();
					break;
				}

				// 8. LCN 입력
				Print("LCN (Logical Channel Number): ");
				if (scanf("%hu", &(chList.lcn)) != 1)
				{
					chList.lcn = 0;
				}
				clearInputBuffer();
				
				// 9. Service ID 입력 및 검증
				Print("Service ID (Cannot be 0): ");
				if (scanf("%hu", &(chList.sid)) != 1)
				{
					Print("Invalid input for SID. Cancellation...\n");
					clearInputBuffer();
					break;
				}
				clearInputBuffer();

				LinkedList_AddChannel(chList);
            }
			break;

            case 2:
			{
				unsigned short searchCh;
				Print("\nEnter Channel Number to search:");
				scanf("%hu", &searchCh);

				LinkedList_SearchChannel(searchCh);
            }
			break;

            case 3:
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

			case 4:
			{
				LinkedList_SaveToFile(CHANNEL_LIST_FILE);
			}
			break;

			case 5:
			{
				LinkedList_LoadFromFile(CHANNEL_LIST_FILE);
			}
			break;

			case 6:
			{
				LinkedList_Reset(CHANNEL_LIST_FILE);
				//LinkedList_LoadFromFile(CHANNEL_LIST_FILE);
			}
			break;

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

				LinkedList_UpdateChNameByChannel(targetCh, searchName);
			}
			break;

			case 8:
			{
				unsigned short sortType;
				Print("\nEnter Channel Sort: ");
				if (scanf("%hu", &sortType) != 1)
				{
					clearInputBuffer();
					break;
				}
				clearInputBuffer();

				LinkedList_Sort(sortType);
			}
			break;
			
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

            case 10: LinkedList_PrintAllChannels(); break;
			case 11: LinkedList_ExportToCSV(CHANNEL_LIST_CSV_FILE); break;
			case 12: LinkedList_ImportFromCSV(CHANNEL_LIST_CSV_FILE); break;

			case 21:
			{
				char fileName[64]; // 문자열 입력을 저장할 변수
				Print("Enter the TS file name: ");
				if (scanf("%63s", fileName) != 1)
				{
					// 63자까지 안전하게 입력
					clearInputBuffer(); 
					break;
				}
				clearInputBuffer();

				open_channel_file(fileName);
			}
			break;
			
			case 0: LinkedList_Free(); return 0;
            default: Print("Invalid choice.\n");
        }
    }

	LinkedList_Free();
    Print("System exited.\n");

    return 0;
}

