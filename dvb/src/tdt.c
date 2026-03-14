/******************************************************************************
*
* tdt.h
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

#include "tdt.h"


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
void tdt_free_section(tdt_section_t * section)
{
	if (section)
	{
		DTV_FREE(section);
	}

	return;
}

/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
tdt_section_t *tdt_parse_section(unsigned char * p)
{
	tdt_section_t * sec;
	signed short section_length;
	unsigned char section_syntax_indicator;

	if(p == NULL)
	{
		return NULL;
	}

	sec = (tdt_section_t *)DTV_MALLOC(sizeof(tdt_section_t));
	if (!sec) return NULL;
	DTV_MEMSET(sec, 0, sizeof(tdt_section_t));

	/* table_id */
	sec->table_id = *p++;
	if(sec->table_id != TDT_TID)
	{
		DTV_FREE(sec);
		return NULL;
	}

	section_syntax_indicator = (unsigned char)((*p & 0x80) >> 7);
	if (section_syntax_indicator == 0)
	{
	}

	section_length = (signed short)get_12bit(p);	
	p += 2;
	//hexDump("TDT", (void *)(p - 3), section_length + 3);
	
	if ((section_length == 0) || (section_length > MAX_SECTION_LENGTH))
	{
		DTV_FREE(sec);
		return NULL;
	}

	DTV_MEMCPY(sec->time, p, 5);
	p += 5; section_length = (signed short)(section_length - 5);
	
	return sec;
}


