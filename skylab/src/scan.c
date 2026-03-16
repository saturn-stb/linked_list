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
#include "nit.h"
#include "tdt.h"
#include "tot.h"

#include "linked_list.h"
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
#define MAX_SECTION   		  					8
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

static unsigned char *sections[MAX_SECTION] = {NULL,}; 

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
const char* get_stream_type_name(unsigned char type)
{
    switch (type) {
        case ES_MPEG1_VIDEO: return "MPEG-1 Video";
        case ES_MPEG2_VIDEO: return "MPEG-2 Video";
        case ES_MPEG1_AUDIO: return "MPEG-1 Audio";
        case ES_MPEG2_AUDIO: return "MPEG-2 Audio";
        case ES_PRIVATE_PES: return "Teletext/Data";
        case ES_AAC_AUDIO: return "AAC ADTS";
        case ES_MPEG4_PART2_VIDEO: return "MPEG-4 Video";
        case ES_14496_3_AUDIO_LATM: return "AAC LATM";
        case ES_H264_VIDEO: return "H.264 AVC";
        case ES_HEVC_VIDEO: return "H.265 HEVC";
        case ES_AC3_AUDIO: return "AC-3 Audio";
        default:   return "Unknown";
    }
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
*
*
*
*---------------------------------------------------------------------------*/
void free_service(dvb_scan_result_t *scan)
{
	if (scan == NULL) return;

	scan_service_t *svc = scan->services;
	while (svc) 
	{
		scan_service_t *next_svc = svc->next;
		
		// 1. 스트림 리스트 먼저 해제
		stream_info_t *s = svc->streams;
		while (s)
		{
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
void print_final_scan_results(dvb_scan_result_t *scan)
{
    scan_service_t *svc = scan->services;
    int count = 0;

	CHANNEL_LIST chList;

    Print("\n==================================================\n");
    Print(" FINAL CHANNEL SCAN RESULTS (Total: %d)\n", scan->total_services);
    Print("==================================================\n");

    while(svc)
    {
        // 채널 이름이 비어있으면 "Unknown"으로 표시
        char *name = (svc->service_name[0] != '\0') ? (char*)svc->service_name : "Unknown";

		Print("--------------------------------------------------\n");
		Print(" No.  | Prog ID | Channel Name        | PMT PID | LCN\n");
		Print("--------------------------------------------------\n");
        Print(" %-4d | %-7d | %-19s | 0x%04X  | %d\n", 
               count, 
               svc->program_number, 
               name, 
               svc->pmt_pid, svc->lcn);
		Print("--------------------------------------------------\n");

		memset(&chList, 0x0, sizeof(CHANNEL_LIST));
		memcpy(chList.name, name, strlen(name));
		chList.sid = svc->program_number;
		chList.lcn = svc->lcn;
		chList.type = svc->service_type;

        // 상세 스트림 정보(비디오/오디오 PID)까지 보고 싶다면 아래 주석 해제
		// 스트림 정보 출력 루프 수정	 
		stream_info_t *stream = svc->streams;
		int aud_count = 0;
		while (stream)
		{
			const char* type_str = get_stream_type_name(stream->stream_type); // 타입 이름 변환 함수가 있다면 활용
			// [수정 포인트] 간격을 맞추기 위해 필드 폭 지정 (%-12s, %04X 등)
			Print("  L [Stream] Type: 0x%02X (%-10s) | PID: 0x%04X (%4d) | LANG: %s\n",
					stream->stream_type,
					type_str ? type_str : "Unknown",
					stream->elementary_pid,
					stream->elementary_pid,
					stream->language);

			if((stream->stream_type == ES_MPEG1_VIDEO) ||
				(stream->stream_type == ES_MPEG2_VIDEO) ||
				(stream->stream_type == ES_MPEG4_PART2_VIDEO) ||
				(stream->stream_type == ES_H264_VIDEO) ||
				(stream->stream_type == ES_HEVC_VIDEO))
			{
				chList.video[0].type = stream->stream_type;
				chList.video[0].pid = stream->elementary_pid;
			}
			else if((stream->stream_type == ES_MPEG1_AUDIO) ||
					(stream->stream_type == ES_MPEG2_AUDIO) ||
					(stream->stream_type == ES_AAC_AUDIO) ||
					(stream->stream_type == ES_14496_3_AUDIO_LATM) ||
					(stream->stream_type == ES_AC3_AUDIO))
			{
				chList.audio[aud_count].type = stream->stream_type;
				chList.audio[aud_count].pid = stream->elementary_pid;
				memcpy(chList.audio[aud_count].lang, stream->language, 3);
				aud_count++;
			}

			stream = stream->next;
		}

		Print("--------------------------------------------------\n");

		LinkedList_AddChannel(chList);

        svc = svc->next;
		count++;
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
*
*
*
*---------------------------------------------------------------------------*/
void dvb_free_section(void)
{
	int sec = 0;
	
	while((sections[sec] != NULL) && (sec < MAX_SECTION))
	{
		free(sections[sec]);
		sections[sec] = NULL;
		sec++;
	}
}

/*-----------------------------------------------------------------------------
* This function is equivalent to requesting section data for a specific PID via the demux filter.
*
*
*---------------------------------------------------------------------------*/
void dvb_get_section(unsigned char *ts, size_t size, unsigned short flt_pid)
{
	int pktLen = detect_packet_len(ts);

	unsigned char sec_buf[MAX_SECTION_BUF_SIZE];

	int section_received[MAX_SECTION];
	int last_section_number = -1;
	int received_count = 0;

	int sec_pos = 0;
	int sec_len = 0;

	unsigned char *pkt = ts;

	dvb_free_section();

	memset(sec_buf, 0, sizeof(sec_buf));
	memset(section_received, 0, sizeof(section_received));

	Print("\r\ndvb_get_section (filter pid 0x%04x)\n", flt_pid);

	while (pkt < (ts + size))
	{
		if (pkt[0] != TS_SYNC_BYTE) {
			pkt++;
			continue;
		}

		if (flt_pid == (TDT_PID + 0x2000))
		{
			unsigned short pid = ((pkt[1] & 0x1F) << 8) | pkt[2];
			if (pid != (flt_pid - 0x2000)) {
				pkt += pktLen;
				continue;
			}
			
			unsigned char pusi = (pkt[1] & 0x40) >> 6;
			unsigned char afc = (pkt[3] & 0x30) >> 4;
			
			if (afc == 2) { pkt += pktLen; continue; }
			int header_len = (afc == 3) ? (4 + pkt[4] + 1) : 4;
			
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
				//Print("1. section 0x%02x length: %d\n", sec_buf[0], need);
				sec_pos += need;
				payload += need;
				payload_len -= need;

				if (sec_pos >= 3) {
					sec_len = (((sec_buf[1] & 0x0F) << 8) | sec_buf[2]) + 3;
					//Print("2. section 0x%02x length: %d\n", sec_buf[0], sec_len);
				}
			}

			if (sec_len > 0 && sec_pos < sec_len && payload_len > 0) {
				int copy = sec_len - sec_pos;
				if (copy > payload_len) copy = payload_len;
				memcpy(sec_buf + sec_pos, payload, copy);
				//Print("3. section 0x%02x length: %d\n", sec_buf[0], copy);
				if(sec_buf[0] == TDT_TID)
					sec_pos += copy;
				else
					sec_pos += copy;
				//Print("4. section pos: %d, len = %d\n", sec_pos, sec_len);
			}

			if (sec_len > 0 && sec_pos >= sec_len) {
				if (sec_buf[0] == TDT_TID){
					//hexDump("TDT", sec_buf, sec_len);
					int sec_num = 0;
					last_section_number = 0;
					sections[sec_num] = malloc(sec_len);
					
					if (sections[sec_num] != NULL)
					{
						memcpy(sections[sec_num], sec_buf, sec_len);
					
						section_received[sec_num] = 1;
						received_count++;
					
						Print("[TDT] SECTION STORED %d\n", sec_num);
					}

					if (last_section_number >= 0 &&
						received_count >= (last_section_number + 1))
					{
						Print("[TDT] Received all sections\n");
						return;
					}
				}

				sec_pos = 0;
			}
		}
		else
		{
			unsigned short pid = ((pkt[1] & 0x1F) << 8) | pkt[2];
			if (pid != flt_pid) {
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
			
			if (payload_len <= 0) {
				pkt += pktLen;
				continue;
			}
			
			if (pusi)
			{
				int pointer = payload[0];
				payload += pointer + 1;
				payload_len -= pointer + 1;
			
				sec_pos = 0;
				sec_len = 0;
			}

			while (payload_len > 0)
			{
				/* header (3 bytes) */
				if (sec_pos < 3)
				{
					int need = 3 - sec_pos;
					int copy = payload_len < need ? payload_len : need;

					memcpy(sec_buf + sec_pos, payload, copy);

					sec_pos += copy;
					payload += copy;
					payload_len -= copy;

					if (sec_pos < 3)
						break;

					if (sec_pos == 3)
					{
						sec_len = (((sec_buf[1] & 0x0F) << 8) | sec_buf[2]) + 3;
						if (sec_len < 3 || sec_len > MAX_SECTION_BUF_SIZE)
						{
							Print("Invalid section length: %d\n", sec_len);
							sec_pos = 0;
							sec_len = 0;
							break;
						}
					}
				}

				/* body */
				if (sec_pos >= 3 && sec_len > 0)
				{
					int remain = sec_len - sec_pos;
					int copy = payload_len < remain ? payload_len : remain;

					memcpy(sec_buf + sec_pos, payload, copy);

					sec_pos += copy;
					payload += copy;
					payload_len -= copy;
				}

				/* section complete */
				if (sec_len > 0 && sec_pos >= sec_len)
				{
					if (sec_buf[0] == TOT_TID)
					{
						int sec_num = 0;
						//hexDump("TOT", sec_buf, sec_len);

						last_section_number = 0;
						sections[sec_num] = malloc(sec_len);
						
						if (sections[sec_num] != NULL)
						{
							memcpy(sections[sec_num], sec_buf, sec_len);
						
							section_received[sec_num] = 1;
							received_count++;
						
							Print("[TOT] SECTION STORED %d\n", sec_num);
						}

						if (last_section_number >= 0 &&
							received_count >= (last_section_number + 1))
						{
							Print("[TOT] Received all sections\n");
							return;
						}
					}
					else
					{
						int sec_num = 0;
						
						sec_num = sec_buf[6];
						last_section_number = sec_buf[7];

						Print("section %d / last %d\n", sec_num, last_section_number);

						if (sec_num < MAX_SECTION &&
							!section_received[sec_num])
						{
							sections[sec_num] = malloc(sec_len);

							if (sections[sec_num] != NULL)
							{
								memcpy(sections[sec_num], sec_buf, sec_len);

								section_received[sec_num] = 1;
								received_count++;

								Print("SECTION STORED %d\n", sec_num);
							}
						}

						if (last_section_number >= 0 &&
							received_count >= (last_section_number + 1))
						{
							Print("Received all sections\n");
							return;
						}
					}

					/* prepare next section */
					sec_pos = 0;
					sec_len = 0;
				}
			}
		}

		pkt += pktLen;
	}
}

/*-----------------------------------------------------------------------------
* PAT 정보를 바탕으로 서비스 구조체 업데이트
*
*
*---------------------------------------------------------------------------*/
void update_service_from_pat(dvb_scan_result_t *scan, pat_section_t *pat)
{
    if (!scan || !pat) return;

	Print("\n==================================================\n");
	Print(" PAT INFORMATION SCANNING\n");
	Print("==================================================\n");

	// Transport Stream ID는 보통 16진수(0xXXXX)와 10진수를 병기하는 것이 좋습니다.
	scan->ts_id = pat->ts_id;
	Print(" [+] Transport Stream ID : 0x%04X (%d)\n", pat->ts_id, pat->ts_id);
	Print(" L Program Association List:\n");

	pat_program_data_t *prog = pat->prog_data;
	
	while(prog != NULL)
	{
		// program_number가 0이면 NIT를 의미하므로 별도 표시 
		if (prog->program_number == 0)
		{
			Print("  L [Network]  PID: 0x%04X (NIT)\n", prog->pmt_pid);
		}
		else
		{
			Print("  L [Program]  ID: 0x%04X | PMT PID: 0x%04X (%d)\n", prog->program_number, prog->pmt_pid, prog->pmt_pid);
		}
		
		scan_service_t *svc = create_service(prog->program_number, prog->pmt_pid);
		if(svc != NULL)
		{
			add_service(scan, svc);
		}

		prog = prog->next;
	}
}

/*-----------------------------------------------------------------------------
* PMT 정보를 바탕으로 서비스 구조체에 스트림(Video/Audio) 정보
*
*
*---------------------------------------------------------------------------*/
void update_service_from_pmt(scan_service_t *svc, pmt_section_t *pmt)
{
    if (!svc || !pmt) return;

	Print("\n==================================================\n");
	Print(" PMT INFORMATION SCANNING (Program ID 0x%04X, %d)\n", pmt->program_number, pmt->program_number);
	Print("==================================================\n");

    svc->pcr_pid = pmt->pcr_pid;
	Print(" [+] PCR PID : 0x%04X\n", pmt->pcr_pid);

    descriptor_t *desc = pmt->desc;

	while(desc != NULL)
	{
		if(desc->tag == DESC_TAG_CA)
		{
			ca_desc_t *ca_desc = (ca_desc_t *)desc->data;
			if(ca_desc != NULL)
			{
				Print(" L [CA]  CA system id : 0x%04X | CA PID: 0x%04X\n", ca_desc->ca_system_id, ca_desc->ca_pid);

				private_data_t *priv_data = (private_data_t *)ca_desc->priv_data;
				if(priv_data != NULL)
				{
					hexDump("CA Private Data", priv_data->data, priv_data->len);
				}
			}
		}
		
		desc = desc->next;
	}

	Print(" L ES Association List:\n");

    pmt_es_data_t *es = pmt->es_data;

    while (es != NULL)
	{
        stream_info_t *stream = (stream_info_t*)calloc(1, sizeof(stream_info_t));
        if (stream)
		{
            stream->stream_type = es->stream_type;
            stream->elementary_pid = es->elem_pid;
			Print("  L [ES]  stream type : 0x%02X | elem PID: 0x%04X\n", es->stream_type, es->elem_pid);
            
            // 리스트 하단에 추가
            if (!svc->streams)
			{
                svc->streams = stream;
            }
			else
			{
                stream_info_t *curr = svc->streams;
                while (curr->next)
                {
					curr = curr->next;
                }
				
                curr->next = stream;
            }
        }

		descriptor_t *desc = es->desc;
		while(desc != NULL)
		{
			switch(desc->tag)
			{
				case DESC_TAG_ISO_639_LANG:
				{
					iso_639_lang_desc_t *lang_desc = (iso_639_lang_desc_t *)desc->data;
					iso_639_lang_data_t *data = lang_desc->lang_data;
					if(data != NULL)
					{
						memset(stream->language, 0x0, ISO_LANGUAGE_CODE);
						memcpy(stream->language, data->lang_code, ISO_LANGUAGE_CODE);
						Print("    L language : %s | audio type : 0x%02X\n", data->lang_code, data->audio_type);
					}
				}
				break;

				case DESC_TAG_AC3:
				{
					ac3_desc_t *adc_desc = (ac3_desc_t *)desc->data;
					if(adc_desc != NULL)
					{
						Print("    L ac3 type : 0x%02X | bsid : 0x%02X\n", adc_desc->ac3_type, adc_desc->bsid);
						Print("        mainid : 0x%02X | asvc : 0x%02X\n", adc_desc->mainid, adc_desc->asvc);
						Print("          info : %s\n", adc_desc->info);
					}
				}
				break;

				default:
				break;
			}
			
			desc = desc->next;
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
void update_service_from_sdt(dvb_scan_result_t *scan, sdt_section_t *sdt)
{
	if (!scan || !sdt) return;

	Print("\n==================================================\n");
	Print(" SDT INFORMATION SCANNING\n");
	Print("==================================================\n");
	Print(" [+] Transport Stream ID : 0x%04X\n", sdt->ts_id);
	Print(" L Service Name Mapping:\n");

	sdt_service_data_t *sdt_svc = sdt->svc_data;
	while (sdt_svc)
	{
		scan_service_t *curr_svc = scan->services;
		while (curr_svc)
		{
			if (curr_svc->program_number == sdt_svc->service_id)
			{
				descriptor_t *desc = sdt_svc->desc;
				while (desc)
				{
					if (desc->tag == DESC_TAG_SERVICE)
					{
						service_desc_t *svc_desc = (service_desc_t *)desc->data;
						if (svc_desc && svc_desc->service_name_len > 0)
						{
							curr_svc->service_type = svc_desc->service_type;
							memset(curr_svc->service_name, 0, sizeof(curr_svc->service_name));
							int copy_len = (svc_desc->service_name_len < (int)sizeof(curr_svc->service_name) - 1) ? 
											svc_desc->service_name_len : (int)sizeof(curr_svc->service_name) - 1;
							memcpy(curr_svc->service_name, svc_desc->service_name, copy_len);
							curr_svc->service_name[copy_len] = '\0';
							
							Print("   L Service ID: 0x%04X | Name: %20s | Service Type 0x%02x\n", sdt_svc->service_id, svc_desc->service_name, svc_desc->service_type);
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
NIT 섹션 정보를 바탕으로 네트워크 이름 및 LCN(Logical Channel Number) 업데이트
*
*
*---------------------------------------------------------------------------*/
void update_service_from_nit(dvb_scan_result_t *scan, nit_section_t *nit)
{
	if (!scan || !nit) return;

	Print("\n==================================================\n");
	Print(" NIT INFORMATION SCANNING\n");
	Print("==================================================\n");
	
	// 1. 네트워크 이름 업데이트 (Network Name Descriptor: 0x41)
	descriptor_t *net_desc = (descriptor_t *)nit->desc;
	while (net_desc)
	{
		if (net_desc->tag == DESC_TAG_NETWORK_NAME)
		{
			if(net_desc->data != NULL)
			{
				network_name_desc_t *net_name_desc = (network_name_desc_t *)net_desc->data;
				// network_id와 이름을 매칭하여 관리할 경우 여기에 구현
				if((net_name_desc != NULL) && (net_name_desc->name != NULL))
				{
					Print("[+] Network Name : %s\n", net_name_desc->name);
				}
			}
		}
		net_desc = net_desc->next;
	}

	// 2. LCN 업데이트 (Logical Channel Descriptor: 0x83)
	nit_ts_data_t *ts_data = (nit_ts_data_t *)nit->ts_data;
	while (ts_data)
	{
		Print(" L [TS ID: 0x%04X] Original Network ID: 0x%04X\n", ts_data->ts_id, ts_data->on_id);

		descriptor_t *ts_desc = (descriptor_t *)ts_data->desc;
		while (ts_desc)
		{
			if (ts_desc->tag == DESC_TAG_DVB_LOGICAL_CHANNEL_NUMBER) // logical_channel_descriptor (유럽/한국 등 표준)
			{
				// descriptor 내 데이터를 파싱하여 service_id와 lcn 매칭
 				lcn_desc_t *lcn_desc = (lcn_desc_t *)ts_desc->data;
				lcn_data_t *lcn_data = (lcn_data_t *)lcn_desc->lcn_data;

				Print(" L LCN Mapping (Service ID : LCN : Visible)\n");
				while(lcn_data)
				{
					scan_service_t *curr_svc = scan->services;
					while (curr_svc)
					{
						if (curr_svc->program_number == lcn_data->service_id)
						{
							curr_svc->lcn = lcn_data->lcn;
						}

						curr_svc = curr_svc->next;
					}
					
					Print(" 		  -> 0x%04X : %3d : %d\n", lcn_data->service_id, lcn_data->lcn, lcn_data->visiable_flag);
					
					lcn_data = lcn_data->next;
				}
			}
			ts_desc = ts_desc->next;
		}
		ts_data = ts_data->next;
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

	Print("\n==================================================\n");
	Print(" TDT INFORMATION SCANNING\n");
	Print("==================================================\n");

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
	Print("[+] Current Date: %04d-%02d-%02d, %02d:%02d:%02d\n", year, month, day, hour, minute, second);
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

	Print("\n==================================================\n");
	Print(" TOT INFORMATION SCANNING\n");
	Print("==================================================\n");

    // 1. 기본 UTC 시간 및 날짜 추출 (TDT와 동일 로직)
    unsigned short mjd = (tot->time[0] << 8) | tot->time[1];
    int year, month, day;
    mjd_to_date(mjd, &year, &month, &day);

    int hour   = ((tot->time[2] >> 4) * 10) + (tot->time[2] & 0x0F);
    int minute = ((tot->time[3] >> 4) * 10) + (tot->time[3] & 0x0F);
    int second = ((tot->time[4] >> 4) * 10) + (tot->time[4] & 0x0F);

    Print("[+] UTC Date: %04d-%02d-%02d, Time: %02d:%02d:%02d\n", 
           year, month, day, hour, minute, second);

    // 2. Local Time Offset Descriptor 파싱 (Tag: 0x58)
    // TOT는 디스크립터 영역에 지역 시차 정보를 포함합니다.
    descriptor_t *desc = tot->desc;
    while (desc) {
        if (desc->tag == DESC_TAG_LOCAL_TIME_OFFSET) { // local_time_offset_descriptor
            // 데이터 구조에 따라 시차(offset)를 추출합니다.
            // 보통 2바이트 정보를 통해 UTC 대비 시차를 계산합니다.

			Print("Local Time Offset Descriptors Found:\n");
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
void scan_channel(void) 
{
	dvb_scan_result_t *scan = &dvb_scan.scan;
	
	if (!dvb_scan.buf) return;
	unsigned char *p = dvb_scan.buf;
	size_t file_size = dvb_scan.size;

	int sec = 0;

	// parse PAT
	sec = 0;
	dvb_get_section(p, file_size, PAT_PID);
	while((sections[sec] != NULL) && (sec < MAX_SECTION))
	{
		pat_section_t *pat = pat_parse_section(sections[sec]);
		if (pat != NULL) 
		{
			update_service_from_pat(&dvb_scan.scan, pat);
			pat_free_section(pat);
		}
		sec++;
	}
	dvb_free_section();

	// parse PMT
	if(scan->total_services != 0)
	{
		scan_service_t *scan_svc = scan->services;

		while(scan_svc != NULL)
		{
			// parse PMT
			sec = 0;
			dvb_get_section(p, file_size, scan_svc->pmt_pid);
			while((sections[sec] != NULL) && (sec < MAX_SECTION))
			{
				pmt_section_t *pmt = pmt_parse_section(sections[sec]);
				if (pmt != NULL) 
				{
					update_service_from_pmt(scan_svc, pmt);
					pmt_free_section(pmt);
				}
				sec++;
			}
			dvb_free_section();

			scan_svc = scan_svc->next;
		}
	}

	// parse SDT
	sec = 0;
	dvb_get_section(p, file_size, SDT_PID);
	while((sections[sec] != NULL) && (sec < MAX_SECTION))
	{
		sdt_section_t *sdt = sdt_parse_section(sections[sec]);
		if (sdt != NULL) 
		{
			update_service_from_sdt(&dvb_scan.scan, sdt);
			sdt_free_section(sdt);
		}
		sec++;
	}
	dvb_free_section();

	// parse NIT
	sec = 0;
	dvb_get_section(p, file_size, NIT_PID);
	while((sections[sec] != NULL) && (sec < MAX_SECTION))
	{
		nit_section_t *nit = nit_parse_section(sections[sec]);
		if (nit != NULL) 
		{
			update_service_from_nit(&dvb_scan.scan, nit);
			nit_free_section(nit);
		}
		sec++;
	}
	dvb_free_section();

	// parse TDT
	sec = 0;
	dvb_get_section(p, file_size, (TDT_PID + 0x2000));
	while((sections[sec] != NULL) && (sec < MAX_SECTION))
	{
		tdt_section_t *tdt = tdt_parse_section(sections[sec]);
		if (tdt != NULL) 
		{
			update_time_from_tdt(&dvb_scan.scan, tdt);
			tdt_free_section(tdt);
		}
		sec++;
	}
	dvb_free_section();

	// parse TOT
	sec = 0;
	dvb_get_section(p, file_size, TOT_PID);
	while((sections[sec] != NULL) && (sec < MAX_SECTION))
	{
		tot_section_t *tot = tot_parse_section(sections[sec]);
		if (tot != NULL) 
		{
			update_time_from_tot(&dvb_scan.scan, tot);
			tot_free_section(tot);
		}
		sec++;
	}
	dvb_free_section();

	print_final_scan_results(&dvb_scan.scan);

	free_service(&dvb_scan.scan);
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

