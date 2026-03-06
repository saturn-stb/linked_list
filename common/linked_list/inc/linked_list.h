/******************************************************************************
*
* linked_list.h
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
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

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
#define MAX_CHANNELS           10000
	
#define SID_MASK		       0x1FFF

#define CHANNEL_LIST_ITEM      5

#define CHANNEL_LIST_CONFIG_FILE "config.dat"
#define CHANNEL_LIST_FILE        "channels.dat"
#define CHANNEL_LIST_CSV_FILE    "channels_backup.csv"
#define CHANNEL_LIST_JSON_FILE   "channels.json"

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
extern void LinkedList_PrintAllChannels(void);
extern void LinkedList_JsonToCsv(const char* jsonFilename, const char* csvFilename);
extern void LinkedList_CsvToJson(const char* csvFilename, const char* jsonFilename);
extern void LinkedList_ImportFromCSV(const char* filename);
extern void LinkedList_ExportToCSV(const char* filename);
extern void LinkedList_SearchChannelByName(char *name);
extern void LinkedList_SortByLCN(void);
extern void LinkedList_UpdateChannelName(unsigned short ch, char *name);
extern void LinkedList_DeleteChannel(unsigned short ch);
extern void LinkedList_SearchChannel(unsigned short ch);
extern void LinkedList_AddChannel(CHANNEL_LIST list);
extern void LinkedList_LoadFromFile(const char* filename, int sortType);
extern int LinkedList_SaveToFile(const char* filename, int sortType);
extern void LinkedList_SaveConfig(int sortType);
extern int LinkedList_LoadConfig(void);
extern int LinkedList_Free(void);
extern int LinkedList_Init(void);

#endif
