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
#define MAX_POOL_SIZE          MAX_CHANNELS

#define SID_MASK		       0x1FFF

#define CHANNEL_LIST_ITEM      6 // CHANNEL_LIST' item number

#define CHANNEL_LIST_CONFIG_FILE "config.dat"
#define CHANNEL_LIST_FILE        "channels.dat"
#define CHANNEL_LIST_CSV_FILE    "channels_backup.csv"
#define CHANNEL_LIST_JSON_FILE   "channels.json"

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
typedef enum
{
	SORT_CH,
	SORT_LCN,
	SORT_NAME,
	SORT_SID,
	
	MAX_SORT
	
}TYPE_SORT;

// Force 1-byte alignment for binary consistency
#pragma pack(push, 1)

typedef struct
{
    unsigned char type;    // video type
    unsigned short pid;    // video PID

} VIDEO_INFO;

typedef struct
{
    unsigned char type;    // audio type
    unsigned short pid;    // audio PID
    unsigned char lang[4]; // Language Code (ISO639, kor, eng. usa, etc...)

} AUDIO_INFO;

typedef struct
{
	unsigned short ch;        // Channel Number (index)
    unsigned char name[32];   // Channel Name
	VIDEO_INFO video[2]; 
	AUDIO_INFO audio[16]; 
	unsigned short lcn;       // Logical Channel Number
	unsigned short sid;       // Service ID
	unsigned char type;       // service type (TV, RADIO, etc...)
    unsigned char country[4]; // Country code (ISO639, kr, en, ger, deu, fra, ita, etc...)

} CHANNEL_LIST;

typedef struct Node
{
    CHANNEL_LIST data;  // Channel Data
    struct Node* next;  // Next Node pointer

} Node;

typedef struct
{
    unsigned char sort; // Channel Sort

} CONFIG_LIST;

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
extern void LinkedList_ImportFromCSV(const char* filename);
extern void LinkedList_ExportToCSV(const char* filename);
extern void LinkedList_SearchChannelByName(char *name);
extern void LinkedList_SearchChannelByLcn(unsigned short lcn);
extern void LinkedList_SearchChannelByChannel(unsigned short ch);
extern void LinkedList_Sort(unsigned char sortType);
extern void LinkedList_UpdateChNameByChannel(unsigned short ch, char *name);
extern void LinkedList_UpdateChNameByLcn(unsigned short lcn, char *name);
extern void LinkedList_DeleteChannel(unsigned short ch);
extern void LinkedList_DeleteChannelByLcn(unsigned short lcn);
extern void LinkedList_SearchChannel(unsigned short ch);
extern void LinkedList_AddChannel(CHANNEL_LIST list);
extern void LinkedList_LoadFromFile(const char* filename);
extern void LinkedList_SaveToFile(const char* filename);
extern void LinkedList_Reset(const char* filename);
extern void LinkedList_SaveConfig(CONFIG_LIST config);
extern CONFIG_LIST LinkedList_LoadConfig(void);
extern int LinkedList_Free(void);
extern int LinkedList_Init(void);

#endif
