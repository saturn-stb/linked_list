/******************************************************************************
*
* scan.c
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

#include "pat.h"
#include "pmt.h"
#include "sdt.h"

#include "scan.h"

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
static long gfileSize = 0;

/******************************************************************************
*
*
*
*****************************************************************************/
/*-----------------------------------------------------------------------------
* PAT(Program Association Table
* dscr 파일 데이터가 로드된 메모리 시작 주소
*
*---------------------------------------------------------------------------*/
unsigned short pat_parse(unsigned char *p, dvb_scan_result_t *scan)
{
    pat_section_t *pat = NULL;
    int i = 0, pktLen = 0;
	unsigned short count = 0;

	if(p == NULL) 
	{
		Print("[PAT ERROR] Received section data is NULL\n");
		return 0xFFFF;
	}

	pktLen = detect_packet_len(p);

    // MPEG-TS 패킷 크기는 일반적으로 188바이트
    // 0x47(Sync Byte)을 찾고 PID가 0x0000인 패킷을 찾아야 함
    while (i < 1000000) // 파일 전체를 검색 (범위 제한 필요)
    {
        // Sync Byte 확인
        if (p[i] == 0x47) 
        {
            // PID 추출 (13비트)
            unsigned short pid = ((p[i+1] & 0x1F) << 8) | p[i+2];
            
            if (pid == PAT_PID) // PAT PID 발견
            {
                // Payload Unit Start Indicator (PUSI) 확인
                unsigned char pusi = (p[i+1] & 0x40) >> 6;
                if (pusi)
                {
                    // Pointer field를 건너뛰고 실제 데이터 시작점 계산
                    unsigned char pointer_field = p[i+4];
                    unsigned char *section_data = &p[i+4+1+pointer_field];
					unsigned short idx = 0;
					
                    // PAT 파싱 수행
                    pat = pat_parse_section(section_data);
                    
                    if (pat != NULL)
					{
						pat_program_data_t *prog_data = pat->prog_data;

						scan->ts_id = pat->ts_id;

						while(prog_data != NULL)
						{
							if(idx < MAX_PAT_PROGRAMS)
							{
								// 1. 새로운 노드 할당 및 초기화
								scan_service_t *services = (scan_service_t *)malloc(sizeof(scan_service_t));
								if(services != NULL)
								{
									memset(services, 0x0, sizeof(scan_service_t));
									services->streams = NULL; // [중요] 스트림 리스트 시작 포인터 초기화
									services->next = NULL; // 새로운 노드는 항상 마지막이므로 next를 NULL로 설정
								
									// 2. 리스트 연결 (Queue 방식: 순서 유지)
									if (scan->services == NULL)
									{
										// 리스트가 비어있다면 바로 첫 번째 노드로 설정
										scan->services = services;
									}
									else
									{
										// 마지막 노드까지 이동
										// [수정] tail의 타입을 명확히 scan_service_t *로 선언
										scan_service_t *tail = scan->services;
										while (tail->next != NULL)
										{
											tail = tail->next;
										}

										tail->next = services; // 여기서 경고가 뜬다면 next 멤버 정의를 다시 확인하세요.
									}

									// program_number가 0x0000 (NIT)
									if(prog_data->program_number == 0x0000)
									{
										services->program_number = prog_data->program_number;
										scan->network_id = prog_data->pmt_pid;
									}
									else
									{
										services->program_number = prog_data->program_number;
										services->pmt_pid = prog_data->pmt_pid;
									}

									idx++;
								}
							}

							prog_data = prog_data->next;
						}

						scan->total_services = idx; // 총 개수 업데이트
						count = idx;
						
                        pat_free_section(pat);
                    }
                }
            }
        }
        i += pktLen; // 188, 다음 패킷으로 이동
    }

	return count;
}

/*-----------------------------------------------------------------------------
* 파일 내에서 특정 PID를 가진 첫 번째 패킷의 데이터 시작 주소를 반환
* dscr 파일 데이터가 로드된 메모리 시작 주소
* target_pid 찾고자 하는 PID (13비트)
* 패킷 데이터 시작 주소 (찾지 못하면 NULL)
*---------------------------------------------------------------------------*/
unsigned char *find_packet_by_pid(unsigned char *p, unsigned short target_pid)
{
    int pktLen = detect_packet_len(p); // 이미 구현된 패킷 길이 감지 함수 사용
    long fileSize = gfileSize; // 파일 사이즈 (실제 환경에 맞게 조정 필요)

    // 패킷 단위로 점프하며 검색
    for (int i = 0; i < (fileSize - pktLen); i += pktLen)
    {
        // Sync Byte 확인
        if (p[i] == 0x47) 
        {
            // PID 추출 (13비트)
            unsigned short pid = ((p[i+1] & 0x1F) << 8) | p[i+2];
            
            if (pid == target_pid)
            {
                // 찾은 패킷의 데이터 시작 주소 반환
                return &p[i];
            }
        }
    }

    Print("[SEARCH] PID 0x%04X not found.\n", target_pid);
    return NULL;
}

/*-----------------------------------------------------------------------------
* PMT(Program Map Table
* PMT 테이블을 파싱하여 프로그램 내의 스트림 정보(비디오/오디오 PID)를 출력
*
*---------------------------------------------------------------------------*/
unsigned short pmt_parse(unsigned char *p, scan_service_t *services)
{
    pmt_section_t *pmt = NULL;
	unsigned short count = 0;
	unsigned short idx = 0;
    
    if (p == NULL)
	{
        Print("[PMT ERROR] Received section data is NULL\n");
        return 0xFFFF;
    }

    // 1. PMT 섹션 파싱 (pmt_parse_section 내부에서 es_data 할당 및 연결)
    pmt = pmt_parse_section(p);
    if (pmt == NULL)
	{
        Print("[PMT ERROR] Failed to parse PMT section\n");
        return 0xFFFF;
    }

	if(services->program_number != pmt->program_number)
	{
		// 4. 메모리 해제 (구조체 멤버인 es_data와 desc를 모두 해제해야 함)
		pmt_free_section(pmt);
        return 0xFFFF;
	}
	
	services->pcr_pid = pmt->pcr_pid;

	descriptor_t *desc = pmt->desc;
	count = 0;
	idx = 0;
    while (desc != NULL)
    {
		//Print(" 	-> Has Descriptors\n");
        desc = desc->next;
		count++;
    }

    // 3. Elementary Stream(ES) 데이터 순회
    pmt_es_data_t *es = pmt->es_data;
	stream_info_t *last_stream = NULL; // 리스트 끝을 유지할 포인터
	count = 0;
	idx = 0;
    while (es != NULL)
    {
		// 1. 새로운 노드 할당 및 초기화
		stream_info_t *new_stream = (stream_info_t *)malloc(sizeof(stream_info_t));
		if(new_stream != NULL)
		{
			memset(new_stream, 0x0, sizeof(stream_info_t));

			new_stream->stream_type = es->stream_type;
			new_stream->elementary_pid = es->elem_pid;

			// Queue 방식 최적화 (tail을 매번 찾지 않음)	
			if (services->streams == NULL)
			{
				services->streams = new_stream;
			}
			else
			{
				last_stream->next = new_stream;
			}

			last_stream = new_stream; // 다음 노드를 위해 업데이트
			idx++;
		}
		
        // 디스크립터가 있다면 추가 출력 가능
        if (es->desc != NULL)
		{
            //Print("     -> Has Descriptors in es\n");
        }
        
        es = es->next;
        count++;
    }

    // 4. 메모리 해제 (구조체 멤버인 es_data와 desc를 모두 해제해야 함)
    pmt_free_section(pmt);

    return 0x0;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void scan_channel(unsigned long dscr) 
{
    unsigned char *p = (unsigned char *)dscr;
	unsigned short ch_count = 0, result = 0;
	dvb_scan_result_t *scan = NULL;
	int idx = 0;

	scan = (dvb_scan_result_t *)malloc(sizeof(dvb_scan_result_t));
	if(scan == NULL)
	{
		Print("Memory allocation failed\n");
		return;
	}

	memset(scan, 0x0, sizeof(dvb_scan_result_t));

	// PAT 파싱 함수 호출
	ch_count = pat_parse(p, scan);
	if((ch_count > 0) && (ch_count != 0xFFFF))
	{
		Print("==============================\n");
		Print("SCAN Channel (PAT)\n");
		Print(" ch num    : %d, %d\n", scan->total_services, ch_count);
		Print(" TS_ID     : 0x%04x\n", scan->ts_id);

		if(scan->total_services != 0)
		{
			Print("\n%-8s | %-12s | %-10s\n", "Index", "Program ID", "PMT PID");
			Print("------------------------------------------\n");

			idx = 0;
			do{
				// program_id가 0x0000인 경우는 보통 NIT 테이블을 가리키므로 예외 처리하거나 구분하여 출력
				Print("%-8d | 0x%04X       | 0x%04X\n", 
					  idx, 
					  scan->services[idx].program_number, 
					  scan->services[idx].pmt_pid);
				idx++;
			}while(idx < scan->total_services);
			
			Print("------------------------------------------\n");

			// PMT 파싱 함수 호출
			// PMT 파싱 및 출력 루프
			Print("\n%-8s | %-12s | %-10s\n", "Index", "Program ID", "PCR PID");
			Print("------------------------------------------\n");
			
			scan_service_t *curr = scan->services;
			int idx = 0;
			while (curr != NULL)
			{
				// 1. PID 검색 (PAT에서 얻은 PMT PID 사용)
				unsigned char *pmt_packet = find_packet_by_pid(p, curr->pmt_pid);
				
				if (pmt_packet != NULL && curr->program_number != 0x0000)
				{
					unsigned char pointer_field = pmt_packet[4]; 
					unsigned char *section_data = &pmt_packet[4 + 1 + pointer_field];
			
					// 2. PMT 파싱 수행
					result = pmt_parse(section_data, curr);
					if(result != 0xFFFF)
					{
						// 표 형식 출력
						Print("%-8d | 0x%04X	   | 0x%04X\n", 
							  idx, curr->program_number, curr->pcr_pid);
						
						// 스트림 정보가 있다면 상세 출력
						if (curr->streams != NULL)
						{
							stream_info_t *es = curr->streams;
							int es_idx = 0;
							while (es != NULL)
							{
								// 스트림 타입에 따른 이름 표시 (디버깅 편의성) 
								const char *type_name = "Unknown";
								if (es->stream_type == ES_H264_VIDEO) type_name = "H.264 Video";
								else if (es->stream_type == ES_PRIVATE_PES) type_name = "Audio/Data";
								else if (es->stream_type == ES_MPEG1_AUDIO) type_name = "MPEG Audio";
								
								Print(" [ES %d] Type: 0x%02X (%-12s), PID: 0x%04X\n", es_idx, es->stream_type, type_name, es->elementary_pid);
								es_idx++;
								es = es->next;
							}
						}
					}
				}
				curr = curr->next; // 다음 채널로 이동
				idx++;
			}
			Print("------------------------------------------\n");
		}
	}

	// 수정된 해제 로직
	scan_service_t *curr_services = scan->services;
	while(curr_services != NULL)
	{
		scan_service_t *next_services = curr_services->next;
		
		// 1. 해당 채널의 스트림 리스트 먼저 해제
		stream_info_t *curr_stream_info = curr_services->streams;
		while(curr_stream_info != NULL)
		{
			stream_info_t *next_stream_info = curr_stream_info->next;
			
			free(curr_stream_info);
			curr_stream_info = next_stream_info; // [수정] 올바른 다음 스트림으로 이동
		}
		
		// 2. 서비스 노드 해제
		free(curr_services);
		curr_services = next_services;
	}

	free(scan);
	scan = NULL;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void open_channel_file(const char *file_name) 
{
	FILE *fp = NULL;
	unsigned char *buffer = NULL;
	long fileSize = 0;

	// 1. TS 파일 열기
	fp = fopen(file_name, "rb"); // 읽기 전용 바이너리 모드
	if (!fp)
	{
		Print("Failed to open %s\n", file_name);
		return;
	}

	// 2. 파일 크기 계산
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);

	// 3. 파일 크기만큼 메모리 할당
	buffer = (unsigned char *)malloc(fileSize);
	if (!buffer)
	{
		Print("Memory allocation failed\n");
		fclose(fp);
		return;
	}

	// 4. 파일 데이터 읽기
	if (fread(buffer, 1, fileSize, fp) != (size_t)fileSize)
	{
		Print("Failed to read file\n");
		free(buffer);
		fclose(fp);
		return;
	}
	fclose(fp);

	gfileSize = fileSize;
	
	// 5. 메모리 주소를 unsigned long으로 변환하여 전달
	unsigned long dscr = (unsigned long)buffer;
	Print("TS File loaded at address: 0x%lX, Size: %ld bytes\n", dscr, fileSize);

	scan_channel(dscr);
	
	// 6. 사용 후 메모리 해제
	free(buffer);
}

