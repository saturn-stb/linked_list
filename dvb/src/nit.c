/******************************************************************************
*
* nit.h
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

#include "nit.h"


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
void nit_free_section(nit_section_t * section)
{
	nit_ts_data_t * ts, * next_ts;
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

		ts = section->ts_data;
		
		while (ts != NULL)
		{
			next_ts = ts->next;
			desc = ts->desc;
			
			while (desc != NULL)
			{
				next_desc = desc->next;
				dvb_desc_free(desc);
				desc = next_desc;
			}
			
			DTV_FREE(ts);
			ts = next_ts;
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
nit_section_t *nit_parse_section(unsigned char * p)
{
	nit_section_t * sec;
	DTV_ECODE err;
	signed short section_length;
	unsigned char section_syntax_indicator, current_next_indicator;
	unsigned char temp;
	
	descriptor_t * desc, * prev_desc = NULL;
	unsigned char desc_tag;
	unsigned char desc_length;
	
	nit_ts_data_t * ts_data, * prev_ts_data = NULL;
	signed short network_desc_length;
	signed short ts_stream_loop_length;
	signed short ts_desc_length;

	signed short total_len;
	unsigned int get_crc, mpeg_crc;

	if(p == NULL) return NULL;

	sec = (nit_section_t *)DTV_MALLOC(sizeof(nit_section_t));	
	if (!sec) return NULL;
	DTV_MEMSET(sec, 0, sizeof(nit_section_t));

	/* table_id */
	sec->table_id = *p++;
	if(sec->table_id != NIT_A_TID && sec->table_id != NIT_O_TID)
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
	section_length = (unsigned short)(section_length - 4);

	sec->network_id = get_16bit(p);
	p += 2; section_length = (signed short)(section_length - 2);

	temp = *p;
	p += 1; section_length = (signed short)(section_length - 1);

	sec->version = (unsigned char)((temp & 0x3e) >> 1);
	current_next_indicator = (unsigned char)(temp & 0x01);
	if (current_next_indicator == 0)
	{
		// Ignore 'next' table version; currently only processing 'current' information.
	}

	sec->section = *p;
	p += 1; section_length = (signed short)(section_length - 1);

	sec->last_section = *p;
	p += 1; section_length = (signed short)(section_length - 1);

	network_desc_length = (signed short)get_12bit(p);
	p += 2; section_length = (signed short)(section_length - 2);

	while ((section_length > 0) 
			&& (section_length >= network_desc_length)
			&& (network_desc_length > 0))
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
		network_desc_length = (signed short)(network_desc_length - (desc_length + 2));
	}

	ts_stream_loop_length = (signed short)get_12bit(p);
	p += 2; section_length = (signed short)(section_length - 2);

	while ((section_length > 0) 
			&& (section_length >= ts_stream_loop_length)
			&& (ts_stream_loop_length > 0))
	{
		ts_data = (nit_ts_data_t *)DTV_MALLOC(sizeof(nit_ts_data_t));
		if (!ts_data) return sec;
		DTV_MEMSET(ts_data, 0, sizeof(nit_ts_data_t));

		/* link */
		if (sec->ts_data == NULL)
		{
			sec->ts_data = ts_data;
		}
		else
		{
			prev_ts_data->next = ts_data;
		}
		prev_ts_data = ts_data;

		ts_data->ts_id = get_16bit(p);
		p += 2; section_length = (signed short)(section_length - 2);
		ts_stream_loop_length = (signed short)(ts_stream_loop_length - 2);

		ts_data->on_id = get_16bit(p);
		p += 2; section_length = (signed short)(section_length - 2);
		ts_stream_loop_length = (signed short)(ts_stream_loop_length - 2);

		ts_desc_length = (signed short)get_12bit(p);
		p += 2; section_length = (signed short)(section_length - 2);
		ts_stream_loop_length = (signed short)(ts_stream_loop_length - 2);

		while ((section_length > 0)
				&& (ts_desc_length > 0)
				&& (section_length >= ts_stream_loop_length)
				&& (ts_desc_length > 0))
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
				if (ts_data->desc == NULL)
				{
					ts_data->desc = desc;
				}
				else
				{
					prev_desc->next = desc;
				}
				prev_desc = desc;
			}

			p += (desc_length + 2); section_length = (signed short)(section_length - (desc_length + 2));
			ts_stream_loop_length = (signed short)(ts_stream_loop_length - (desc_length + 2));
			ts_desc_length = (signed short)(ts_desc_length - (desc_length + 2));
		}	
	}

	return sec;
}

