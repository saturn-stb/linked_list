/******************************************************************************
*
* pmt.h
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

#include "pmt.h"


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
void pmt_free_section(pmt_section_t * section)
{
	pmt_es_data_t * es;
	pmt_es_data_t * next_es;
	descriptor_t * desc;
	descriptor_t * next_desc;
	
	if (section)
	{
		desc = section->desc;		
		while (desc != NULL)
		{
			next_desc = desc->next;
			dvb_desc_free(desc);
			desc = next_desc;
		}

		es = section->es_data;		
		while (es != NULL)
		{
			next_es = es->next;
			desc = es->desc;			
			while (desc != NULL)
			{
				next_desc = desc->next;
				dvb_desc_free(desc);
				desc = next_desc;
			}

			DTV_FREE(es);
			es = next_es;
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
pmt_section_t *pmt_parse_section(unsigned char * p)
{
	pmt_section_t * sec;
	DTV_ECODE err;
	signed short section_length;
	unsigned char section_syntax_indicator, current_next_indicator;
	unsigned char temp;
	
	descriptor_t * desc;
	descriptor_t * prev_desc = NULL;
	unsigned char desc_tag;
	unsigned char desc_length;
	
	pmt_es_data_t * es_data;
	pmt_es_data_t * prev_es_data = NULL;
	signed short program_info_length;
	signed short es_info_length;

	signed short total_len;
	unsigned int get_crc, mpeg_crc;

	if(p == NULL)
	{
		return NULL;
	}

	sec = (pmt_section_t *)DTV_MALLOC(sizeof(pmt_section_t));
	if (!sec) return NULL;
	DTV_MEMSET(sec, 0, sizeof(pmt_section_t));

	/* table_id */
	sec->table_id = *p++;
	if(sec->table_id != PMT_TID) 
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
	// CRC32 verify
	get_crc = get_32bit(p + (total_len - 1 - 4));
	mpeg_crc = get_mpeg_crc32(p - 1, total_len - 4);
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

	sec->program_number = get_16bit(p);
	p += 2; section_length = (unsigned short)(section_length - 2);

	temp = *p;
	p += 1; section_length = (unsigned short)(section_length - 1);

	sec->version = (unsigned char)((temp & 0x3e) >> 1);
	current_next_indicator = (unsigned char)(temp & 0x01);
	if (current_next_indicator != 1)
	{
		// Ignore 'next' table version; currently only processing 'current' information.
	}

	sec->section = *p;
	p += 1; section_length = (unsigned short)(section_length - 1);

	sec->last_section = *p;
	p += 1; section_length = (unsigned short)(section_length - 1);

	sec->pcr_pid = get_13bit(p);
	p += 2; section_length = (unsigned short)(section_length - 2);

	program_info_length = (signed short)get_12bit(p);
	p += 2; section_length = (unsigned short)(section_length - 2);

	while ((section_length > 0)
		&& (section_length >= program_info_length)
		&& (program_info_length > 0))
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

		p += (desc_length + 2); section_length = (unsigned short)(section_length - (desc_length + 2));
		program_info_length = (unsigned short)(program_info_length - (desc_length + 2));
	}

	while (section_length > 0)
	{
		es_data = (pmt_es_data_t *)DTV_MALLOC(sizeof(pmt_es_data_t));
		if (!es_data) return sec;
		DTV_MEMSET(es_data, 0, sizeof(pmt_es_data_t));

		/* link */
		if (sec->es_data == NULL)
		{
			sec->es_data = es_data;
		}
		else
		{
			prev_es_data->next = es_data;
		}
		prev_es_data = es_data;

		es_data->stream_type = *p;
		p += 1; section_length = (unsigned short)(section_length - 1);
		
		es_data->elem_pid = get_13bit(p);
		p += 2; section_length = (unsigned short)(section_length - 2);
		
		es_info_length = (signed short)get_12bit(p);
		p += 2; section_length = (unsigned short)(section_length - 2);

		while ((section_length > 0)
			&& (section_length >= es_info_length) && (es_info_length > 0))
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
				if (es_data->desc == NULL)
				{
					es_data->desc = desc;
				}
				else
				{
					prev_desc->next = desc;
				}
				prev_desc = desc;
			}

			p += (desc_length + 2); section_length = (unsigned short)(section_length - (desc_length + 2));
			es_info_length = (unsigned short)(es_info_length - (desc_length + 2));
		}
	}
	
	return sec;
}

