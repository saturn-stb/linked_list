/******************************************************************************
*
* tot.h
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

#include "tot.h"


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
void tot_free_section(tot_section_t * section)
{
	descriptor_t * desc, * next_desc;
	
	if (section)
	{
		desc = section->desc;		
		while (desc != NULL)
		{
			next_desc = desc->next;
			dvb_desc_free(desc);
			desc = next_desc;
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
tot_section_t *tot_parse_section(unsigned char * p)
{
	tot_section_t * sec;
	DTV_ECODE err;
	signed short section_length;
	unsigned char section_syntax_indicator;

	descriptor_t * desc, * prev_desc = NULL;
	unsigned char desc_tag;
	unsigned char desc_length;
	
	signed short desc_loop_length;

	signed short total_len;
	unsigned int get_crc, mpeg_crc;

	if(p == NULL)
	{
		return NULL;
	}

	sec = (tot_section_t *)DTV_MALLOC(sizeof(tot_section_t));
	if (!sec) return NULL;
	DTV_MEMSET(sec, 0, sizeof(tot_section_t));

	/* table_id */
	sec->table_id = *p++;
	if(sec->table_id != TOT_TID)
	{
		DTV_FREE(sec);
		return NULL;
	}

	section_syntax_indicator = (unsigned char)((*p & 0x80) >> 7);
	if (section_syntax_indicator == 0)
	{
	}

	section_length = (signed short)get_12bit(p);	
	// table_id 1byte + section_length 2bytes = 3bytes)
	total_len = (signed short)(section_length + 3);
	//hexDump("TOT", (void *)(p - 1), total_len);
	// CRC32 verify
	get_crc = get_32bit(p + (total_len - 1 - 4));
	mpeg_crc = get_mpeg_crc32(p - 1, total_len - 4);
	//Print("[TOT INFO] section_length=%d, total_len=%d, table_id=0x%02x, CRC_pos=%d\n", section_length, total_len, *(p-1), (total_len - 4));
	//Print("[TOT INFO] mpeg_crc 0x%04X, 0x%04X\n", get_crc, mpeg_crc);
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

	DTV_MEMCPY(sec->time, p, 5);
	p += 5; section_length = (signed short)(section_length - 5);

	desc_loop_length = (signed short)get_12bit(p);
	p += 2; section_length = (signed short)(section_length - 2);
	
	while ((section_length > 0)
		&& (section_length >= desc_loop_length) && (desc_loop_length > 0))
	{
		desc_tag = *p;
		desc_length = *(p + 1);

		desc = dvb_desc_alloc(desc_tag);
		if (!desc) return sec;
			
		err = dvb_desc_parse(p, desc);
		
		if (DTV_FAILED(err))
		{
			dvb_desc_free(desc);
		}
		else
		{
			/* link */
			if (sec->desc == NULL)
			{
				sec->desc = desc;
			}
			else
			{
				prev_desc->next = desc;
			}
			prev_desc = desc;
		}

		p += (desc_length + 2); section_length = (signed short)(section_length - (desc_length + 2));
		desc_loop_length = (signed short)(desc_loop_length - (desc_length + 2));
	}
	
	return sec;
}

