/******************************************************************************
*
* scan.h
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
#ifndef	SCAN_H
#define	SCAN_H

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
typedef struct stream_info_s
{
	unsigned char  stream_type;    // 0x02:MPEG2, 0x1B:H.264 등
	unsigned short elementary_pid; // 해당 스트림의 PID 

	struct stream_info_s *next;    // 다음 스트림으로 연결
} stream_info_t;

/* 각 서비스(프로그램)의 스캔 상태를 담는 구조체 */
typedef struct scan_service_s
{
    unsigned short program_number; // PAT에서 얻은 Service ID
    unsigned short pmt_pid;        // PAT에서 얻은 PMT PID
    unsigned short pcr_pid;        // PCR PID는 채널당 하나이므로 여기에 배치
    unsigned char  is_scanned;     // PMT/SDT 파싱 완료 여부 (0: 대기, 1: 완료)
    unsigned char  service_type;   // TV, Radio, Data 등 (PMT/SDT 파싱 후 업데이트)
    char           service_name[32]; // 채널명 (SDT 파싱 후 업데이트)

    stream_info_t *streams;        // 비디오/오디오 리스트 시작 포인터
    
	struct scan_service_s *next;    // 다음 스트림으로 연결
} scan_service_t;

/* 전체 스캔 세션을 관리하는 메인 구조체 */
typedef struct
{
    unsigned short ts_id;          // Transport Stream ID
    unsigned short network_id;     // Network ID (NIT 파싱 시 필요)
    int            total_services; // 발견된 총 서비스 수
    int            scanned_count;  // 현재까지 스캔 완료된 서비스 수
    
    // PAT에서 발견된 서비스 리스트 (동적 할당 권장)
    scan_service_t *services;
} dvb_scan_result_t;

typedef struct
{
	FILE *fp;
	size_t size;

	void *buf;
	unsigned long dscr;

	dvb_scan_result_t scan;
	
} dvb_scan_t;

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
extern void scan_channel(void);
extern void open_channel_file(const char *file_name);


#endif	// SCAN_H
