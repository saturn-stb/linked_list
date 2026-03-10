/******************************************************************************
*
* descriptor.h
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
#ifndef DVB_DESCRIPTOR_H
#define DVB_DESCRIPTOR_H

#include <string.h>
#include <malloc.h>

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
#define DTV_SUCCEEDED(err)			((err) >= 0)
#define DTV_FAILED(err)				((err) < 0)
	
/* success & ok, normal */
#define DTV_OK						 0
#define DTV_ALREADY_LOCK			 1
#define DTV_ALREADY_UNLOCK			 2
#define DTV_CONTINUE 				 3
#define DTV_PMT_PID_CHANGED          4
	
/* errors */
#define DTV_ERROR					-1
#define DTV_ERROR_BAD_PARAMETERS	-2
#define DTV_ERROR_UNKNOWN_DEVICE	-3
#define DTV_ERROR_NOT_INITIALIZED	-4
#define DTV_ERROR_NO_FREE_HANDLES	-5
#define DTV_ERROR_INVALID_HANDLE	-6
#define DTV_ERROR_DEVICE_BUSY		-7
#define DTV_ERROR_TIMEOUT			-8
#define DTV_ERROR_NO_MEMORY			-9
#define DTV_ERROR_ALREADY_EXIST		-10
#define DTV_ERROR_NOT_EXIST			-11
#define DTV_ERROR_FULL				-12
#define DTV_ERROR_EMPTY				-13
#define DTV_ERROR_INVALID_DATA		-14
#define DTV_ERROR_NOT_SUPPORTED		-15
#define DTV_ERROR_CANCEL			-16
#define DTV_ERROR_INVALID_SVC_ID	-17

#define DTV_MALLOC(size) 					malloc(size)
#define DTV_FREE(p) 						{ if(p){free(p); p=NULL;} }
#define DTV_MEMSET(src,init,size) 			memset(src,init,size)
#define DTV_MEMCPY(dst,src,size) 			memcpy(dst,src,size)

/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
typedef int 						DTV_ECODE;

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
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern private_data_t *
dvb_alloc_private_data(
	int len
	);

extern void
dvb_free_private_data(
	private_data_t * priv
	);

extern descriptor_t *
dvb_desc_alloc(
	unsigned char tag
	);

extern void
dvb_desc_free(
	descriptor_t *desc
	);

extern DTV_ECODE
dvb_desc_parse(
	unsigned char *p,
	descriptor_t *desc
	);

extern descriptor_t *
dvb_ait_desc_alloc(
	unsigned char tag
	);

extern void
dvb_ait_desc_free(
	descriptor_t *desc
	);

extern DTV_ECODE
dvb_ait_desc_parse(
	unsigned char *p,
	descriptor_t *desc
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DVB_DESCRIPTOR_H */
