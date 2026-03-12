/******************************************************************************
*
* sdt.h
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

#include "sdt.h"


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
void sdt_free_section(sdt_section_t * section)
{
	sdt_service_data_t * svc, * next_svc;
	descriptor_t * desc, * next_desc;
	
	if (section)
	{
		svc = section->svc_data;		
		while (svc != NULL)
		{
			next_svc = svc->next;
			desc = svc->desc;			
			while (desc != NULL)
			{
				next_desc = desc->next;
				dvb_desc_free(desc);
				desc = next_desc;
			}
			
			DTV_FREE(svc);
			svc = next_svc;
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
sdt_section_t * sdt_parse_section(unsigned char * p)
{
	sdt_section_t * sec;
	DTV_ECODE err;
	signed short section_length;
	unsigned char section_syntax_indicator, current_next_indicator;
	unsigned char temp;
	
	descriptor_t * desc, * prev_desc = NULL;
	unsigned char desc_tag;
	unsigned char desc_length;
	
	sdt_service_data_t * svc_data, * prev_svc_data = NULL;
	signed short desc_loop_length;

	signed short total_len;
	unsigned int get_crc, mpeg_crc;

	if(p == NULL)
	{
		return NULL;
	}

	sec = (sdt_section_t *)DTV_MALLOC(sizeof(sdt_section_t));
	if (!sec) return NULL;
	DTV_MEMSET(sec, 0, sizeof(sdt_section_t));
	
	/* table_id */
	sec->table_id = *p++;
	if(sec->table_id != SDT_A_TID && sec->table_id != SDT_O_TID)
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
	
	if ((section_length == 0) || (section_length > 1024))
	{
		DTV_FREE(sec);
		return NULL;
	}

	/* CRC_32 */
	section_length = (unsigned short)(section_length - 4);

	sec->ts_id = get_16bit(p);
	p += 2; section_length = (unsigned short)(section_length - 2);

	temp = *p;
	p += 1; section_length = (unsigned short)(section_length - 1);

	sec->version = (unsigned char)((temp & 0x3e) >> 1);
	current_next_indicator = (unsigned char)(temp & 0x01);
	if (current_next_indicator == 0)
	{
		// Ignore 'next' table version; currently only processing 'current' information.
	}

	sec->section = *p;
	p += 1; section_length = (unsigned short)(section_length - 1);

	sec->last_section = *p;
	p += 1; section_length = (unsigned short)(section_length - 1);

	sec->on_id = get_16bit(p);
	p += 2; section_length = (unsigned short)(section_length - 2);

	/* reserved */
	p += 1; section_length = (unsigned short)(section_length - 1);
	
	while (section_length > 0)
	{
		svc_data = (sdt_service_data_t *)DTV_MALLOC(sizeof(sdt_service_data_t));
		if (!svc_data) return sec;
		DTV_MEMSET(svc_data, 0, sizeof(sdt_service_data_t));

		/* link */
		if (sec->svc_data == NULL)
		{
			sec->svc_data = svc_data;
		}
		else
		{
			prev_svc_data->next = svc_data;
		}
		prev_svc_data = svc_data;

		svc_data->service_id = get_16bit(p);
		p += 2; section_length = (unsigned short)(section_length - 2);

		svc_data->eit_sch = (unsigned char)((*p & 0x02) >> 1);
		svc_data->eit_pf = (unsigned char)(*p & 0x01);
		p += 1; section_length = (unsigned short)(section_length - 1);

		svc_data->running_status = (unsigned char)((*p & 0xe0) >> 5);
		svc_data->ca_mode = (unsigned char)((*p & 0x10) >> 4);
		desc_loop_length = (signed short)get_12bit(p);
		p += 2; section_length = (unsigned short)(section_length - 2);

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
				if (svc_data->desc == NULL)
				{
					svc_data->desc = desc;
				}
				else
				{
					prev_desc->next = desc;
				}
				prev_desc = desc;
			}

			p += (desc_length + 2); section_length = (unsigned short)(section_length - (desc_length + 2));
			desc_loop_length = (unsigned short)(desc_loop_length - (desc_length + 2));
		}
	}
	
	return sec;
}

