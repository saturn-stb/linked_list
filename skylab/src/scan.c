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
static dvb_scan_t dvb_scan;

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
* PAT(Program Association Table)
*
*
*---------------------------------------------------------------------------*/
pat_section_t* dvb_get_pat(unsigned char *ts, size_t size)
{
	int pktLen = detect_packet_len(ts);
	unsigned char sec_buf[4096];
	int sec_pos = 0;
	int sec_len = 0;
	unsigned char *pkt = ts;

	Print("[PAT_SCAN] Start Scanning. FileSize: %ld, PacketLen: %d\n", size, pktLen);

	while (pkt < ts + size)
	{
		if (pkt[0] != 0x47) {
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
			// Print("[PAT_SCAN] New Section Started (PUSI=1)\n");
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
    unsigned char sec_buf[4096];
    int sec_pos = 0;
    int sec_len = 0;
    unsigned char *pkt = ts;

    while (pkt < ts + size)
    {
        if (pkt[0] != 0x47) {
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
            if (sec_buf[0] == 0x02) {
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
    unsigned char sec_buf[4096];
    int sec_pos = 0;
    int sec_len = 0;
    unsigned char *pkt = ts;

    while (pkt < ts + size)
    {
        if (pkt[0] != 0x47) { pkt++; continue; }

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
            if ((sec_buf[0] == 0x42) || (sec_buf[0] == 0x46)){
                return sdt_parse_section(sec_buf);
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

		sdt_section_t *sdt = dvb_get_sdt(p, file_size);
		if (sdt) {
			update_service_name_from_sdt(&dvb_scan.scan, sdt);
			sdt_free_section(sdt);
		}
		
		print_final_scan_results(&dvb_scan.scan);
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

