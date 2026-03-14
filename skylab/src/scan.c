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
#include "tdt.h"
#include "tot.h"

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
#define MAX_SECTION_BUF_SIZE  					4096

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
static dvb_scan_t dvb_scan;

/******************************************************************************
*
*
*
*****************************************************************************/
/*-----------------------------------------------------------------------------
* MJD(Modified Julian Date)를 YYYY-MM-DD로 변환합니다.
* DVB 표준 (EN 300 468) 기준
*
*---------------------------------------------------------------------------*/
void mjd_to_date(unsigned short mjd, int *year, int *month, int *day)
{
    int Y, M, D;
    int K;

    Y = (int)((mjd - 15078.2) / 365.25);
    M = (int)((mjd - 14956.1 - (int)(Y * 365.25)) / 30.6001);
    D = mjd - 14956 - (int)(Y * 365.25) - (int)(M * 30.6001);

    if (M == 14 || M == 15) {
        K = 1;
    } else {
        K = 0;
    }

    *year = Y + K;
    *month = M - 1 - K * 12;
    *day = D;

    // Y를 1900년 기준으로 변환 (DVB MJD는 1858년 11월 17일 시작)
    *year += 1900; 
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void print_scan_pat_services(dvb_scan_result_t *scan)
{
    scan_service_t *svc = scan->services;

    Print("==================================================\n");
    Print(" PAT\n");
    Print("==================================================\n");
    Print("TS_ID : %d\n", scan->ts_id);
    Print("Total services : %d\n", scan->total_services);

    while(svc)
    {
		Print(" Program: %-5d | PMT PID: 0x%04X\n", svc->program_number, svc->pmt_pid);

        svc = svc->next;
    }
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void print_scan_pmt_services(scan_service_t *svc)
{
    stream_info_t *s;

    Print("Program %d\n", svc->program_number);
    Print("PCR PID : 0x%X\n", svc->pcr_pid);

    s = svc->streams;

    while(s)
    {
        Print("  Stream Type: 0x%X  PID: 0x%X\n",
               s->stream_type,
               s->elementary_pid);

        s = s->next;
    }
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void print_final_scan_results(dvb_scan_result_t *scan)
{
    scan_service_t *svc = scan->services;
    int count = 0;

    Print("\n==================================================\n");
    Print(" FINAL CHANNEL SCAN RESULTS (Total: %d)\n", scan->total_services);
    Print("==================================================\n");
    Print(" No.  | Prog ID | Channel Name        | PMT PID\n");
    Print("--------------------------------------------------\n");

    while(svc)
    {
        count++;
        // 채널 이름이 비어있으면 "Unknown"으로 표시
        char *name = (svc->service_name[0] != '\0') ? (char*)svc->service_name : "Unknown";

        Print(" %-4d | %-7d | %-19s | 0x%04X\n", 
               count, 
               svc->program_number, 
               name, 
               svc->pmt_pid);
        
        // 상세 스트림 정보(비디오/오디오 PID)까지 보고 싶다면 아래 주석 해제
        /*
        stream_info_t *s = svc->streams;
        while(s) {
            Print("    -> Stream Type: 0x%02X, PID: 0x%04X\n", s->stream_type, s->elementary_pid);
            s = s->next;
        }
        */

        svc = svc->next;
    }
    Print("==================================================\n\n");
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void print_time_of_change(unsigned char *toc)
{
    // 1. MJD 변환
    unsigned short mjd = (toc[0] << 8) | toc[1];
    int year, month, day;
    mjd_to_date(mjd, &year, &month, &day);

    // 2. UTC 시간 변환 (BCD)
    int hour   = ((toc[2] >> 4) * 10) + (toc[2] & 0x0F);
    int minute = ((toc[3] >> 4) * 10) + (toc[3] & 0x0F);
    int second = ((toc[4] >> 4) * 10) + (toc[4] & 0x0F);

    Print(" Change Time: %04d-%02d-%02d %02d:%02d:%02d\n", 
           year, month, day, hour, minute, second);
}

/*-----------------------------------------------------------------------------
* PAT(Program Association Table)
*
*
*---------------------------------------------------------------------------*/
pat_section_t* dvb_get_pat(unsigned char *ts, size_t size)
{
	int pktLen = detect_packet_len(ts);
	unsigned char sec_buf[MAX_SECTION_BUF_SIZE];
	int sec_pos = 0;
	int sec_len = 0;
	unsigned char *pkt = ts;

	Print("[PAT_SCAN] Start Scanning. FileSize: %ld, PacketLen: %d\n", size, pktLen);

	while (pkt < (ts + size))
	{
		if (pkt[0] != TS_SYNC_BYTE) {
			pkt++;
			continue;
		}

		unsigned short pid = ((pkt[1] & 0x1F) << 8) | pkt[2];
		if (pid != PAT_PID) {
			pkt += pktLen;
			continue;
		}

		unsigned char pusi = (pkt[1] & 0x40) >> 6;
		unsigned char afc = (pkt[3] & 0x30) >> 4;
		int header_len = 4;

		if (afc == 2) { // Adaptation Field only
			pkt += pktLen;
			continue;
		}

		if (afc == 3) { // Adaptation Field + Payload
			header_len += pkt[4] + 1;
		}

		if (header_len >= pktLen) { // 잘못된 헤더 길이 방어
			pkt += pktLen;
			continue;
		}

		unsigned char *payload = pkt + header_len;
		int payload_len = pktLen - header_len;

		// PUSI가 1이면 새로운 섹션이 시작됨
		if (pusi) {
			int pointer = payload[0];
			if (pointer + 1 > payload_len) { // pointer_field 오류 방어
				pkt += pktLen;
				continue;
			}
			payload += pointer + 1;
			payload_len -= pointer + 1;
			sec_pos = 0;
			sec_len = 0;
			Print("[PAT_SCAN] New Section Started (PUSI=1)\n");
		}

		// 섹션 헤더(3바이트: table_id + length) 수집
		if (sec_pos < 3 && payload_len > 0) {
			int need = 3 - sec_pos;
			if (need > payload_len) need = payload_len;

			memcpy(sec_buf + sec_pos, payload, need);
			sec_pos += need;
			payload += need;
			payload_len -= need;

			if (sec_pos >= 3) {
				sec_len = (((sec_buf[1] & 0x0F) << 8) | sec_buf[2]) + 3;
				if (sec_len > (int)sizeof(sec_buf)) {
					Print("[PAT_ERR] Section too large: %d\n", sec_len);
					return NULL;
				}
			}
		}

		// 나머지 데이터 수집
		if (sec_len > 0 && sec_pos < sec_len && payload_len > 0) {
			int copy = sec_len - sec_pos;
			if (copy > payload_len) copy = payload_len;

			memcpy(sec_buf + sec_pos, payload, copy);
			sec_pos += copy;
		}

		// 섹션 조립 완료 확인
		if (sec_len > 0 && sec_pos >= sec_len) {
			Print("[PAT_SCAN] Section Reassembled. TableID: 0x%02X, TotalLen: %d\n", sec_buf[0], sec_len);
			return pat_parse_section(sec_buf); // 파싱 및 반환
		}

		pkt += pktLen;
	}

	Print("[PAT_SCAN] PAT not found or Incomplete.\n");
	return NULL;
}

/*-----------------------------------------------------------------------------
* PMT(Program Map Table)
*
*
*---------------------------------------------------------------------------*/
pmt_section_t* dvb_get_pmt(unsigned char *ts, size_t size, unsigned short target_pid)
{
    int pktLen = detect_packet_len(ts);
    unsigned char sec_buf[MAX_SECTION_BUF_SIZE];
    int sec_pos = 0;
    int sec_len = 0;
    unsigned char *pkt = ts;

    while (pkt < (ts + size))
    {
        if (pkt[0] != TS_SYNC_BYTE) {
            pkt++;
            continue;
        }

        unsigned short pid = ((pkt[1] & 0x1F) << 8) | pkt[2];
        if (pid != target_pid) {
            pkt += pktLen;
            continue;
        }

        unsigned char pusi = (pkt[1] & 0x40) >> 6;
        unsigned char afc = (pkt[3] & 0x30) >> 4;
        int header_len = 4;

        if (afc == 2) { pkt += pktLen; continue; }
        if (afc == 3) header_len += pkt[4] + 1;
        if (header_len >= pktLen) { pkt += pktLen; continue; }

        unsigned char *payload = pkt + header_len;
        int payload_len = pktLen - header_len;

        if (pusi) {
            int pointer = payload[0];
            if (pointer + 1 > payload_len) { pkt += pktLen; continue; }
            payload += pointer + 1;
            payload_len -= pointer + 1;
            sec_pos = 0;
        }

        // 섹션 헤더 수집 (3바이트)
        if (sec_pos < 3 && payload_len > 0) {
            int need = 3 - sec_pos;
            if (need > payload_len) need = payload_len;
            memcpy(sec_buf + sec_pos, payload, need);
            sec_pos += need;
            payload += need;
            payload_len -= need;

            if (sec_pos >= 3) {
                sec_len = (((sec_buf[1] & 0x0F) << 8) | sec_buf[2]) + 3;
            }
        }

        // 데이터 수집
        if (sec_len > 0 && sec_pos < sec_len && payload_len > 0) {
            int copy = sec_len - sec_pos;
            if (copy > payload_len) copy = payload_len;
            memcpy(sec_buf + sec_pos, payload, copy);
            sec_pos += copy;
        }

        if (sec_len > 0 && sec_pos >= sec_len) {
            // Table ID 0x02가 PMT임을 확인
            if (sec_buf[0] == PMT_TID) {
                return pmt_parse_section(sec_buf);
            }
            sec_pos = 0; // 다른 테이블일 경우 초기화 후 계속 검색
        }
        pkt += pktLen;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
* SDT(Service Description Table)
*
*
*---------------------------------------------------------------------------*/
sdt_section_t* dvb_get_sdt(unsigned char *ts, size_t size)
{
    int pktLen = detect_packet_len(ts);
    unsigned char sec_buf[MAX_SECTION_BUF_SIZE];
    int sec_pos = 0;
    int sec_len = 0;
    unsigned char *pkt = ts;

    while (pkt < (ts + size))
    {
        if (pkt[0] != TS_SYNC_BYTE) {
            pkt++;
            continue;
        }

        unsigned short pid = ((pkt[1] & 0x1F) << 8) | pkt[2];
        if (pid != SDT_PID) { // 0x0011
            pkt += pktLen;
            continue;
        }

        unsigned char pusi = (pkt[1] & 0x40) >> 6;
        unsigned char afc = (pkt[3] & 0x30) >> 4;
        int header_len = 4;

        if (afc == 2) { pkt += pktLen; continue; }
        if (afc == 3) header_len += pkt[4] + 1;
        
        unsigned char *payload = pkt + header_len;
        int payload_len = pktLen - header_len;

        if (pusi) {
            int pointer = payload[0];
            payload += pointer + 1;
            payload_len -= pointer + 1;
            sec_pos = 0;
        }

        if (sec_pos < 3 && payload_len > 0) {
            int need = 3 - sec_pos;
            if (need > payload_len) need = payload_len;
            memcpy(sec_buf + sec_pos, payload, need);
            sec_pos += need;
            payload += need;
            payload_len -= need;

            if (sec_pos >= 3) {
                sec_len = (((sec_buf[1] & 0x0F) << 8) | sec_buf[2]) + 3;
            }
        }

        if (sec_len > 0 && sec_pos < sec_len && payload_len > 0) {
            int copy = sec_len - sec_pos;
            if (copy > payload_len) copy = payload_len;
            memcpy(sec_buf + sec_pos, payload, copy);
            sec_pos += copy;
        }

        if (sec_len > 0 && sec_pos >= sec_len) {
            // SDT Table ID: 0x42 (Actual TS)
            if ((sec_buf[0] == SDT_A_TID) || (sec_buf[0] == SDT_O_TID)){
                return sdt_parse_section(sec_buf);
            }
            sec_pos = 0;
        }
        pkt += pktLen;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
* TDT(Time and Date Table)
*
*
*---------------------------------------------------------------------------*/
tdt_section_t* dvb_get_tdt(unsigned char *ts, size_t size)
{
    int pktLen = detect_packet_len(ts);
    unsigned char sec_buf[MAX_SECTION_BUF_SIZE];
    int sec_pos = 0;
    int sec_len = 0;
    unsigned char *pkt = ts;

    while (pkt < (ts + size))
    {
        if (pkt[0] != TS_SYNC_BYTE) {
            pkt++;
            continue;
        }

        unsigned short pid = ((pkt[1] & 0x1F) << 8) | pkt[2];
        if (pid != TDT_PID) {
            pkt += pktLen;
            continue;
        }

        unsigned char pusi = (pkt[1] & 0x40) >> 6;
        unsigned char afc = (pkt[3] & 0x30) >> 4;
        int header_len = 4;

        if (afc == 2) { pkt += pktLen; continue; }
        if (afc == 3) header_len += pkt[4] + 1;
        
        unsigned char *payload = pkt + header_len;
        int payload_len = pktLen - header_len;

        if (pusi) {
            int pointer = payload[0];
            payload += pointer + 1;
            payload_len -= pointer + 1;
            sec_pos = 0;
        }

        if (sec_pos < 3 && payload_len > 0) {
            int need = 3 - sec_pos;
            if (need > payload_len) need = payload_len;
            memcpy(sec_buf + sec_pos, payload, need);
            sec_pos += need;
            payload += need;
            payload_len -= need;

            if (sec_pos >= 3) {
                sec_len = (((sec_buf[1] & 0x0F) << 8) | sec_buf[2]) + 3;
            }
        }

        if (sec_len > 0 && sec_pos < sec_len && payload_len > 0) {
            int copy = sec_len - sec_pos;
            if (copy > payload_len) copy = payload_len;
            memcpy(sec_buf + sec_pos, payload, copy);
            sec_pos += copy;
        }

        if (sec_len > 0 && sec_pos >= sec_len) {
            // TDT Table ID: 0x70
            if (sec_buf[0] == TDT_TID){
                return tdt_parse_section(sec_buf);
            }
            sec_pos = 0;
        }
        pkt += pktLen;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
* TOT(Time Offset Table)
*
*
*---------------------------------------------------------------------------*/
tot_section_t* dvb_get_tot(unsigned char *ts, size_t size)
{
    int pktLen = detect_packet_len(ts);
    unsigned char sec_buf[MAX_SECTION_BUF_SIZE];
    int sec_pos = 0;
    int sec_len = 0;
    unsigned char *pkt = ts;

    while (pkt < (ts + size))
    {
        if (pkt[0] != TS_SYNC_BYTE) {
            pkt++;
            continue;
        }

        unsigned short pid = ((pkt[1] & 0x1F) << 8) | pkt[2];
        if (pid != TOT_PID) {
            pkt += pktLen;
            continue;
        }

        unsigned char pusi = (pkt[1] & 0x40) >> 6;
        unsigned char afc = (pkt[3] & 0x30) >> 4;
        int header_len = 4;

        if (afc == 2) { pkt += pktLen; continue; }
        if (afc == 3) header_len += pkt[4] + 1;
        
        unsigned char *payload = pkt + header_len;
        int payload_len = pktLen - header_len;

        if (pusi) {
            int pointer = payload[0];
            payload += pointer + 1;
            payload_len -= pointer + 1;
            sec_pos = 0;
        }

        if (sec_pos < 3 && payload_len > 0) {
            int need = 3 - sec_pos;
            if (need > payload_len) need = payload_len;
            memcpy(sec_buf + sec_pos, payload, need);
            sec_pos += need;
            payload += need;
            payload_len -= need;

            if (sec_pos >= 3) {
                sec_len = (((sec_buf[1] & 0x0F) << 8) | sec_buf[2]) + 3;
            }
        }

        if (sec_len > 0 && sec_pos < sec_len && payload_len > 0) {
            int copy = sec_len - sec_pos;
            if (copy > payload_len) copy = payload_len;
            memcpy(sec_buf + sec_pos, payload, copy);
            sec_pos += copy;
        }

        if (sec_len > 0 && sec_pos >= sec_len) {
            // TOT Table ID: 0x73
            if (sec_buf[0] == TOT_TID){
                return tot_parse_section(sec_buf);
            }
            sec_pos = 0;
        }
        pkt += pktLen;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
scan_service_t* create_service(unsigned short program_number, unsigned short pmt_pid)
{
    scan_service_t *svc = (scan_service_t*)calloc(1, sizeof(scan_service_t));
    if(!svc) return NULL;

    svc->program_number = program_number;
    svc->pmt_pid = pmt_pid;
    svc->pcr_pid = 0xFFFF;

    svc->is_scanned = 0;
    svc->service_type = 0;

    svc->streams = NULL;
    svc->next = NULL;

    return svc;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void add_service(dvb_scan_result_t *scan, scan_service_t *svc)
{
    if(!scan->services)
    {
        scan->services = svc;
    }
    else
    {
        scan_service_t *cur = scan->services;

        while(cur->next)
        {
            cur = cur->next;
        }

        cur->next = svc;
    }

    scan->total_services++;
}

/*-----------------------------------------------------------------------------
* PMT 정보를 바탕으로 서비스 구조체에 스트림(Video/Audio) 정보
*
*
*---------------------------------------------------------------------------*/
void update_service_with_pmt(scan_service_t *svc, pmt_section_t *pmt)
{
    if (!svc || !pmt) return;

    svc->pcr_pid = pmt->pcr_pid;
    pmt_es_data_t *es = pmt->es_data;

    while (es) {
        stream_info_t *stream = (stream_info_t*)calloc(1, sizeof(stream_info_t));
        if (stream) {
            stream->stream_type = es->stream_type;
            stream->elementary_pid = es->elem_pid;
            
            // 리스트 하단에 추가
            if (!svc->streams) {
                svc->streams = stream;
            } else {
                stream_info_t *curr = svc->streams;
                while (curr->next) curr = curr->next;
                curr->next = stream;
            }
        }
        es = es->next;
    }
    svc->is_scanned = 1;
}

/*-----------------------------------------------------------------------------
* SDT 섹션 정보를 바탕으로 각 서비스의 실제 채널 이름을 업데이트
*
*
*---------------------------------------------------------------------------*/
void update_service_name_from_sdt(dvb_scan_result_t *scan, sdt_section_t *sdt)
{
	if (!scan || !sdt) return;

	sdt_service_data_t *sdt_svc = sdt->svc_data;
	while (sdt_svc) {
		scan_service_t *curr_svc = scan->services;
		while (curr_svc) {
			if (curr_svc->program_number == sdt_svc->service_id) {
				descriptor_t *desc = sdt_svc->desc;
				while (desc) {
					if (desc->tag == 0x48) {
						service_desc_t *svc_desc = (service_desc_t *)desc->data;
						if (svc_desc && svc_desc->service_name_len > 0) {
							memset(curr_svc->service_name, 0, sizeof(curr_svc->service_name));
							int copy_len = (svc_desc->service_name_len < (int)sizeof(curr_svc->service_name) - 1) ? 
											svc_desc->service_name_len : (int)sizeof(curr_svc->service_name) - 1;
							memcpy(curr_svc->service_name, svc_desc->service_name, copy_len);
							curr_svc->service_name[copy_len] = '\0';
						}
						break;
					}
					desc = desc->next;
				}
				break;
			}
			curr_svc = curr_svc->next;
		}
		sdt_svc = sdt_svc->next;
	}
}

/*-----------------------------------------------------------------------------
* TDT(Time and Date Table) 섹션 정보를 바탕으로 System time( UTC ) 업데이트
* time[0,1]: MJD (Modified Julian Date)
* time[2,3,4]: UTC Time (HH, MM, SS)
*---------------------------------------------------------------------------*/
void update_time_from_tdt(dvb_scan_result_t *scan, tdt_section_t *tdt)
{
	if (!scan || !tdt) return;

	// 1. MJD 추출 및 변환
	unsigned short mjd = (tdt->time[0] << 8) | tdt->time[1];
	int year, month, day;
	mjd_to_date(mjd, &year, &month, &day);

	// 2. UTC 시간 추출 (BCD 포맷 처리를 포함한 예시)
	// DVB BCD: (val >> 4) * 10 + (val & 0x0F)
	int hour   = ((tdt->time[2] >> 4) * 10) + (tdt->time[2] & 0x0F);
	int minute = ((tdt->time[3] >> 4) * 10) + (tdt->time[3] & 0x0F);
	int second = ((tdt->time[4] >> 4) * 10) + (tdt->time[4] & 0x0F);

	// 3. 결과 출력
	Print("\n[TDT_SCAN] Current Date: %04d-%02d-%02d\n", year, month, day);
	Print("[TDT_SCAN] Current UTC Time: %02d:%02d:%02d\n", hour, minute, second);
}

/*-----------------------------------------------------------------------------
* TOT(Time Offset Table) 섹션 정보를 바탕으로 현지 시간(Local Time)을 업데이트
*
*
*---------------------------------------------------------------------------*/
void update_time_from_tot(dvb_scan_result_t *scan, tot_section_t *tot)
{
	int count  = 0;
	
    if (!scan || !tot) return;

    // 1. 기본 UTC 시간 및 날짜 추출 (TDT와 동일 로직)
    unsigned short mjd = (tot->time[0] << 8) | tot->time[1];
    int year, month, day;
    mjd_to_date(mjd, &year, &month, &day);

    int hour   = ((tot->time[2] >> 4) * 10) + (tot->time[2] & 0x0F);
    int minute = ((tot->time[3] >> 4) * 10) + (tot->time[3] & 0x0F);
    int second = ((tot->time[4] >> 4) * 10) + (tot->time[4] & 0x0F);

    Print("\n[TOT_SCAN] UTC Date: %04d-%02d-%02d, Time: %02d:%02d:%02d\n", 
           year, month, day, hour, minute, second);

    // 2. Local Time Offset Descriptor 파싱 (Tag: 0x58)
    // TOT는 디스크립터 영역에 지역 시차 정보를 포함합니다.
    descriptor_t *desc = tot->desc;
    while (desc) {
        if (desc->tag == DESC_TAG_LOCAL_TIME_OFFSET) { // local_time_offset_descriptor
            // 데이터 구조에 따라 시차(offset)를 추출합니다.
            // 보통 2바이트 정보를 통해 UTC 대비 시차를 계산합니다.

			Print("\n[TOT_SCAN] Local Time Offset Descriptors Found:\n");
			Print("--------------------------------------------------\n");
			Print(" No. | Country | Reg ID | Polarity | Offset | Time of Change      | Next Time Offset \n");
			Print("--------------------------------------------------\n");

			local_time_offset_desc_t *local_time_offset_desc = (local_time_offset_desc_t *)desc->data;
			local_time_offset_data_t *offset_data = local_time_offset_desc->offset_data;

			while(offset_data != NULL)
			{
				// Polarity: 0은 양수(+), 1은 음수(-)
				char polarity = (offset_data->local_time_offset_polarity == 0) ? '+' : '-';

				// time_of_change[5] 중 앞의 2바이트를 MJD로 추출
				unsigned short mjd_val = (offset_data->time_of_change[0] << 8) | offset_data->time_of_change[1];

				int year, month, day;
				mjd_to_date(mjd_val, &year, &month, &day); // 앞서 만든 mjd_to_date 함수 활용

				// 수정 제안 예시: time_of_change의 시간(HH:MM:SS) 부분을 10진수로 변환하여 출력
				int toc_h = ((offset_data->time_of_change[2] >> 4) * 10) + (offset_data->time_of_change[2] & 0x0F);
				int toc_m = ((offset_data->time_of_change[3] >> 4) * 10) + (offset_data->time_of_change[3] & 0x0F);
				int toc_s = ((offset_data->time_of_change[4] >> 4) * 10) + (offset_data->time_of_change[4] & 0x0F);
				// Print 문에서 toc_h, toc_m, toc_s 사용
				
				// Offset (2바이트 BCD) 및 Time of Change (5바이트 BCD)
				// 시차는 보통 HH:MM 형태로 출력
				Print(" %-3d | %c%c%c     | 0x%02X   | %c        | %02X:%02X  | %04d-%02d-%02d %02X:%02X:%02X | %02X:%02X\n",
					   count,
					   offset_data->country_code[0], offset_data->country_code[1], offset_data->country_code[2],
					   offset_data->country_region_id,
					   polarity,
					   offset_data->local_time_offset[0], offset_data->local_time_offset[1],
					   year, month, day, toc_h, toc_m, toc_s,
					   offset_data->next_time_offset[0], offset_data->next_time_offset[1]);

				print_time_of_change(offset_data->time_of_change);
				
				offset_data = offset_data->next;
				count++;
			}

			Print("--------------------------------------------------\n");
        }
        desc = desc->next;
    }
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void free_scan_results(dvb_scan_result_t *scan)
{
	if (scan == NULL) return;

	scan_service_t *svc = scan->services;
	while (svc) {
		scan_service_t *next_svc = svc->next;
		
		// 1. 스트림 리스트 먼저 해제
		stream_info_t *s = svc->streams;
		while (s) {
			stream_info_t *next_s = s->next;
			void *ptr_s = (void *)s;
			Safe_Free(&ptr_s); 
			s = next_s;
		}
		
		// 2. 서비스 노드 해제
		void *ptr_svc = (void *)svc;
		Safe_Free(&ptr_svc);
		svc = next_svc;
	}

	scan->services = NULL;
	scan->total_services = 0;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void scan_channel(void) 
{
	if (!dvb_scan.buf) return;
	unsigned char *p = dvb_scan.buf;
	size_t file_size = dvb_scan.size;

	// parse PAT
	pat_section_t *pat = dvb_get_pat(p, file_size);
	if(pat) {
		pat_program_data_t *prog = pat->prog_data;
		dvb_scan.scan.ts_id = pat->ts_id;

		while(prog) {
			scan_service_t *svc = create_service(prog->program_number, prog->pmt_pid);
			if(svc) add_service(&dvb_scan.scan, svc);
			prog = prog->next;
		}
		pat_free_section(pat);
		print_scan_pat_services(&dvb_scan.scan);
		
		// parse PMT
		scan_service_t *curr_svc = dvb_scan.scan.services;
		while (curr_svc) {
			pmt_section_t *pmt = dvb_get_pmt(p, file_size, curr_svc->pmt_pid);
			if (pmt) {
				update_service_with_pmt(curr_svc, pmt);
				print_scan_pmt_services(curr_svc);
				pmt_free_section(pmt);
			}
			curr_svc = curr_svc->next;
		}

		// parse SDT
		sdt_section_t *sdt = dvb_get_sdt(p, file_size);
		if (sdt) {
			update_service_name_from_sdt(&dvb_scan.scan, sdt);
			sdt_free_section(sdt);
		}
		
		print_final_scan_results(&dvb_scan.scan);
	}

	// parse TDT
	tdt_section_t *tdt = dvb_get_tdt(p, file_size);
	if (tdt) {
		update_time_from_tdt(&dvb_scan.scan, tdt);
		tdt_free_section(tdt);
	}

	// parse TOT
	tot_section_t *tot = dvb_get_tot(p, file_size);
	if (tot) {
		update_time_from_tot(&dvb_scan.scan, tot);
		tot_free_section(tot);
	}

	free_scan_results(&dvb_scan.scan);
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
void open_channel_file(const char *file_name) 
{
	memset(&dvb_scan, 0x0, sizeof(dvb_scan_t));

	dvb_scan.fp = File_Open(file_name, "rb");
	if(!dvb_scan.fp) return;
	
	dvb_scan.size = File_GetSize(dvb_scan.fp);
	if(dvb_scan.size <= 0) {
		File_Close(dvb_scan.fp);
		return;
	}

	dvb_scan.buf = (unsigned char *)Safe_Malloc(dvb_scan.size);
	if (!dvb_scan.buf) {
		File_Close(dvb_scan.fp);
		return;
	}

	if (File_Read(dvb_scan.fp, dvb_scan.buf, dvb_scan.size) != 0) {
		Safe_Free((void**)&dvb_scan.buf);
		File_Close(dvb_scan.fp);
		return;
	}
	
	File_Close(dvb_scan.fp);
	scan_channel();
	
	if (dvb_scan.buf) Safe_Free((void**)&dvb_scan.buf);
}

