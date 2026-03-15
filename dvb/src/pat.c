/******************************************************************************
*
* pat.h
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
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#include "pat.h"


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
void pat_free_section(pat_section_t * section)
{
	pat_program_data_t * prog, * next;
	
	if (section)
	{
		prog = section->prog_data;		
		while (prog != NULL)
		{
			next = prog->next;
			DTV_FREE(prog);
			prog = next;
		}		
		DTV_FREE(section);
	}

	return;
}

/*-----------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------*/
pat_section_t *pat_parse_section(unsigned char * p)
{
	pat_section_t * sec;
	signed short section_length;
	unsigned char section_syntax_indicator, current_next_indicator;
	unsigned char temp;

	pat_program_data_t * prog_data, * prev_prog_data = NULL;

	signed short total_len;
	unsigned int get_crc, mpeg_crc;

	if(p == NULL)
	{
		return NULL;
	}

	sec = (pat_section_t *)DTV_MALLOC(sizeof(pat_section_t));
	if (!sec) return NULL;
	DTV_MEMSET(sec, 0, sizeof(pat_section_t));

	/* table_id */
	sec->table_id = *p++;
	if(sec->table_id != PAT_PID)
	{
		DTV_FREE(sec);
		return NULL;
	}

	section_syntax_indicator = (unsigned char)((*p & 0x80) >> 7);
	if (section_syntax_indicator == 0)
	{
		DTV_FREE(sec);
		return NULL;
	}
	
	section_length = (signed short)get_12bit(p);	
	// table_id 1byte + section_length 2bytes = 3bytes)
	total_len = (signed short)(section_length + 3);
	//hexDump("PAT", (void *)(p - 1), total_len);
	// CRC32 verify
	get_crc = get_32bit(p + (total_len - 1 - 4));
	mpeg_crc = get_mpeg_crc32(p - 1, total_len - 4);
	//Print("[PAT INFO] section_length=%d, total_len=%d, table_id=0x%02x, CRC_pos=%d\n", section_length, total_len, *(p-1), (total_len - 4));
	//Print("[PAT INFO] mpeg_crc 0x%04X, 0x%04X\n", get_crc, mpeg_crc);
	if (mpeg_crc != get_crc)
	{
		DTV_FREE(sec);
		return NULL;
	}

	p += 2;
	
	if ((section_length == 0) || (section_length > MAX_SECTION_LENGTH))
	{
		DTV_FREE(sec);
		return NULL;
	}

	/* CRC_32 */
	section_length = (signed short)(section_length - 4);
	
	sec->ts_id = get_16bit(p);
	p += 2; section_length = (signed short)(section_length - 2);

	temp = *p;
	p += 1; section_length = (signed short)(section_length - 1);
	
	sec->version = (unsigned char)((temp & 0x3e) >> 1);
	current_next_indicator = (unsigned char)(temp & 0x01);
	if (current_next_indicator != 1)
	{
		// Ignore 'next' table version; currently only processing 'current' information.
	}

	sec->section = *p;
	p += 1; section_length = (signed short)(section_length - 1);

	sec->last_section = *p;
	p += 1; section_length = (signed short)(section_length - 1);
	
	while (section_length > 0)
	{
		prog_data = (pat_program_data_t *)DTV_MALLOC(sizeof(pat_program_data_t));
		if (!prog_data) return sec;
		DTV_MEMSET(prog_data, 0, sizeof(pat_program_data_t));

		/* link */
		if (sec->prog_data == NULL)
		{
			sec->prog_data = prog_data;
		}
		else
		{
			prev_prog_data->next = prog_data;
		}
		prev_prog_data = prog_data;
	
		prog_data->program_number = get_16bit(p);
		p += 2; section_length = (signed short)(section_length - 2);

		prog_data->pmt_pid = get_13bit(p);
		p += 2; section_length = (signed short)(section_length - 2);
	}

	return sec;
}

