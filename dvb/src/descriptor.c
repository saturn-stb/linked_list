/******************************************************************************
*
* descriptor.c
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
#include <string.h>
#include <malloc.h>

#include "util.h"

#include "dvb.h"
#include "descriptor.h"

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
unsigned char
get_8bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFF;
	
	// 11111111
	return (unsigned char)(*p);
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned short
get_10bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFF;
	
	// 11 11111111
	return (unsigned short)(((*p << 8) | (*(p+1))) & 0x03FF);
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned short
get_12bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFF;
	
	// 1111 11111111
	return (unsigned short)(((*p << 8) | (*(p+1))) & 0x0FFF);
}	


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned short
get_13bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFF;
	
	// 11111 11111111
	return (unsigned short)(((*p << 8) | (*(p+1))) & 0x1FFF);
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned short
get_14bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFF;
	
	// 111111 11111111	
	return (unsigned short)(((*p << 8) | (*(p+1))) & 0x3FFF);
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned short
get_15bit(
	unsigned char *p
	)
{
	// 1111111 11111111	
	return (unsigned short)(((*p << 8) | (*(p+1))) & 0x7FFF);
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned short
get_16bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFF;
	
	// 11111111 11111111	
	return (unsigned short)(((*p) << 8) | (*(p+1)));
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned int
get_20bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFFFFFF;
	
	// 1111 11111111 11111111
	return (unsigned int)(((*p << 16) | (*(p+1) << 8) | (*(p+2))) & 0xFFFFF);
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned int
get_22bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFFFFFF;
	
	// 111111 11111111 11111111
	return (unsigned int)(((*p << 16) | (*(p+1) << 8) | (*(p+2))) & 0x3FFFFF);
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned int
get_24bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFFFFFF;
	
	// 11111111 11111111 11111111
	return (unsigned int)((*p << 16) | (*(p+1) << 8) | (*(p+2)));
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned int
get_28bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFFFFFF;
	
	// 1111 11111111 11111111 11111111
	return (unsigned int)(((*p) << 24) | (*(p+1) << 16) | (*(p+2) << 8) | ((*(p+3))&0x0F));
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned int
get_29bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFFFFFF;
	
	// 1 1111 11111111 11111111 11111111
	return (unsigned int)(((*p) << 24) | (*(p+1) << 16) | (*(p+2) << 8) | ((*(p+3))&0x1F));
}

/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
unsigned int
get_32bit(
	unsigned char *p
	)
{
	if(p == NULL) return 0xFFFFFFFF;
	
	// 11111111 11111111 11111111 11111111
	return (unsigned int)(((*p) << 24) | (*(p+1) << 16) | (*(p+2) << 8) | (*(p+3)));
}

/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
private_data_t *
dvb_alloc_private_data(
	int len
	)
{
	private_data_t *priv;

	if (len <= 0)
	{
		return NULL;
	}

	priv = (private_data_t *)DTV_MALLOC(sizeof(private_data_t));
	
	if (priv == NULL) return NULL;

	DTV_MEMSET(priv, 0, sizeof(private_data_t));

	priv->data = (unsigned char *)DTV_MALLOC(len + 1);
	
	if (priv->data == NULL)
	{
		DTV_FREE(priv);
		return NULL;
	}

	DTV_MEMSET(priv->data, 0, len + 1);

	return priv;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
video_stream_desc_t *
dvb_alloc_video_stream_desc(
	void
	)
{
	video_stream_desc_t *desc;
	
	desc = (video_stream_desc_t *)DTV_MALLOC(sizeof(video_stream_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(video_stream_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
audio_stream_desc_t *
dvb_alloc_audio_stream_desc(
	void
	)
{
	audio_stream_desc_t *desc;
	
	desc = (audio_stream_desc_t *)DTV_MALLOC(sizeof(audio_stream_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(audio_stream_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
hierarchy_desc_t *
dvb_alloc_hierarchy_desc(
	void
	)
{
	hierarchy_desc_t *desc;
	
	desc = (hierarchy_desc_t *)DTV_MALLOC(sizeof(hierarchy_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(hierarchy_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
registration_desc_t *
dvb_alloc_registration_desc(
	void
	)
{
	registration_desc_t *desc;
	
	desc = (registration_desc_t *)DTV_MALLOC(sizeof(registration_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(registration_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
data_stream_align_desc_t *
dvb_alloc_data_stream_align_desc(
	void
	)
{
	data_stream_align_desc_t *desc;
	
	desc = (data_stream_align_desc_t *)DTV_MALLOC(sizeof(data_stream_align_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(data_stream_align_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
target_bg_grid_desc_t *
dvb_alloc_target_bg_grid_desc(
	void
	)
{
	target_bg_grid_desc_t *desc;
	
	desc = (target_bg_grid_desc_t *)DTV_MALLOC(sizeof(target_bg_grid_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(target_bg_grid_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
video_window_desc_t *
dvb_alloc_video_window_desc(
	void
	)
{
	video_window_desc_t *desc;
	
	desc = (video_window_desc_t *)DTV_MALLOC(sizeof(video_window_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(video_window_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ca_desc_t *
dvb_alloc_ca_desc(
	void
	)
{
	ca_desc_t *desc;
	
	desc = (ca_desc_t *)DTV_MALLOC(sizeof(ca_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ca_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
iso_639_lang_desc_t *
dvb_alloc_iso_639_lang_desc(
	void
	)
{
	iso_639_lang_desc_t *desc;
	
	desc = (iso_639_lang_desc_t *)DTV_MALLOC(sizeof(iso_639_lang_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(iso_639_lang_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
system_clock_desc_t *
dvb_alloc_system_clock_desc(
	void
	)
{
	system_clock_desc_t *desc;
	
	desc = (system_clock_desc_t *)DTV_MALLOC(sizeof(system_clock_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(system_clock_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
multi_buffer_util_desc_t *
dvb_alloc_multi_buffer_util_desc(
	void
	)
{
	multi_buffer_util_desc_t *desc;
	
	desc = (multi_buffer_util_desc_t *)DTV_MALLOC(sizeof(multi_buffer_util_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(multi_buffer_util_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
copyright_desc_t *
dvb_alloc_copyright_desc(
	void
	)
{
	copyright_desc_t *desc;
	
	desc = (copyright_desc_t *)DTV_MALLOC(sizeof(copyright_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(copyright_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
max_bitrate_desc_t *
dvb_alloc_max_bitrate_desc(
	void
	)
{
	max_bitrate_desc_t *desc;
	
	desc = (max_bitrate_desc_t *)DTV_MALLOC(sizeof(max_bitrate_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(max_bitrate_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
private_data_indicator_desc_t *
dvb_alloc_private_data_indicator_desc(
	void
	)
{
	private_data_indicator_desc_t *desc;
	
	desc = (private_data_indicator_desc_t *)DTV_MALLOC(sizeof(private_data_indicator_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(private_data_indicator_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
smoothing_buffer_desc_t *
dvb_alloc_smoothing_buffer_desc(
	void
	)
{
	smoothing_buffer_desc_t *desc;
	
	desc = (smoothing_buffer_desc_t *)DTV_MALLOC(sizeof(smoothing_buffer_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(smoothing_buffer_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
avc_desc_t *
dvb_alloc_avc_desc(
	void
	)
{
	avc_desc_t *desc;
	
	desc = (avc_desc_t *)DTV_MALLOC(sizeof(avc_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(avc_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
network_name_desc_t *
dvb_alloc_network_name_desc(
	void
	)
{
	network_name_desc_t *desc;
	
	desc = (network_name_desc_t *)DTV_MALLOC(sizeof(network_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(network_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
service_list_desc_t *
dvb_alloc_service_list_desc(
	void
	)
{
	service_list_desc_t *desc;
	
	desc = (service_list_desc_t *)DTV_MALLOC(sizeof(service_list_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(service_list_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
sat_delivery_system_desc_t *
dvb_alloc_sat_delivery_system_desc(
	void
	)
{
	sat_delivery_system_desc_t *desc;
	
	desc = (sat_delivery_system_desc_t *)DTV_MALLOC(sizeof(sat_delivery_system_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(sat_delivery_system_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
cable_delivery_system_desc_t *
dvb_alloc_cable_delivery_system_desc(
	void
	)
{
	cable_delivery_system_desc_t *desc;
	
	desc = (cable_delivery_system_desc_t *)DTV_MALLOC(sizeof(cable_delivery_system_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(cable_delivery_system_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
bouquet_name_desc_t *
dvb_alloc_bouquet_name_desc(
	void
	)
{
	bouquet_name_desc_t *desc;
	
	desc = (bouquet_name_desc_t *)DTV_MALLOC(sizeof(bouquet_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(bouquet_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
service_desc_t *
dvb_alloc_service_desc(
	void
	)
{
	service_desc_t *desc;
	
	desc = (service_desc_t *)DTV_MALLOC(sizeof(service_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(service_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
country_availability_desc_t *
dvb_alloc_country_availability_desc(
	void
	)
{
	country_availability_desc_t *desc;
	
	desc = (country_availability_desc_t *)DTV_MALLOC(sizeof(country_availability_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(country_availability_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
linkage_desc_t *
dvb_alloc_linkage_desc(
	void
	)
{
	linkage_desc_t *desc;
	
	desc = (linkage_desc_t *)DTV_MALLOC(sizeof(linkage_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(linkage_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
nvod_ref_desc_t *
dvb_alloc_nvod_ref_desc(
	void
	)
{
	nvod_ref_desc_t *desc;
	
	desc = (nvod_ref_desc_t *)DTV_MALLOC(sizeof(nvod_ref_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(nvod_ref_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
time_shifted_service_desc_t *
dvb_alloc_time_shifted_service_desc(
	void
	)
{
	time_shifted_service_desc_t *desc;
	
	desc = (time_shifted_service_desc_t *)DTV_MALLOC(sizeof(time_shifted_service_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(time_shifted_service_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
short_event_desc_t *
dvb_alloc_short_event_desc(
	void
	)
{
	short_event_desc_t *desc;
	
	desc = (short_event_desc_t *)DTV_MALLOC(sizeof(short_event_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(short_event_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
extended_event_desc_t *
dvb_alloc_extended_event_desc(
	void
	)
{
	extended_event_desc_t *desc;
	
	desc = (extended_event_desc_t *)DTV_MALLOC(sizeof(extended_event_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(extended_event_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
time_shifted_event_desc_t *
dvb_alloc_time_shifted_event_desc(
	void
	)
{
	time_shifted_event_desc_t *desc;
	
	desc = (time_shifted_event_desc_t *)DTV_MALLOC(sizeof(time_shifted_event_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(time_shifted_event_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
component_desc_t *
dvb_alloc_component_desc(
	void
	)
{
	component_desc_t *desc;
	
	desc = (component_desc_t *)DTV_MALLOC(sizeof(component_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(component_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
mosaic_desc_t *
dvb_alloc_mosaic_desc(
	void
	)
{
	mosaic_desc_t *desc;
	
	desc = (mosaic_desc_t *)DTV_MALLOC(sizeof(mosaic_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(mosaic_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
stream_identifier_desc_t *
dvb_alloc_stream_identifier_desc(
	void
	)
{
	stream_identifier_desc_t *desc;
	
	desc = (stream_identifier_desc_t *)DTV_MALLOC(sizeof(stream_identifier_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(stream_identifier_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ca_identifier_desc_t *
dvb_alloc_ca_identifier_desc(
	void
	)
{
	ca_identifier_desc_t *desc;
	
	desc = (ca_identifier_desc_t *)DTV_MALLOC(sizeof(ca_identifier_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ca_identifier_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
content_desc_t *
dvb_alloc_content_desc(
	void
	)
{
	content_desc_t *desc;
	
	desc = (content_desc_t *)DTV_MALLOC(sizeof(content_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(content_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
parental_rating_desc_t *
dvb_alloc_parental_rating_desc(
	void
	)
{
	parental_rating_desc_t *desc;
	
	desc = (parental_rating_desc_t *)DTV_MALLOC(sizeof(parental_rating_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(parental_rating_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
teletext_desc_t *
dvb_alloc_teletext_desc(
	void
	)
{
	teletext_desc_t *desc;
	
	desc = (teletext_desc_t *)DTV_MALLOC(sizeof(teletext_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(teletext_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
telephone_desc_t *
dvb_alloc_telephone_desc(
	void
	)
{
	telephone_desc_t *desc;
	
	desc = (telephone_desc_t *)DTV_MALLOC(sizeof(telephone_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(telephone_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
local_time_offset_desc_t *
dvb_alloc_local_time_offset_desc(
	void
	)
{
	local_time_offset_desc_t *desc;
	
	desc = (local_time_offset_desc_t *)DTV_MALLOC(sizeof(local_time_offset_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(local_time_offset_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
subtitling_desc_t *
dvb_alloc_subtitling_desc(
	void
	)
{
	subtitling_desc_t *desc;
	
	desc = (subtitling_desc_t *)DTV_MALLOC(sizeof(subtitling_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(subtitling_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ter_delivery_system_desc_t *
dvb_alloc_ter_delivery_system_desc(
	void
	)
{
	ter_delivery_system_desc_t *desc;
	
	desc = (ter_delivery_system_desc_t *)DTV_MALLOC(sizeof(ter_delivery_system_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ter_delivery_system_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
multi_network_name_desc_t *
dvb_alloc_multi_network_name_desc(
	void
	)
{
	multi_network_name_desc_t *desc;
	
	desc = (multi_network_name_desc_t *)DTV_MALLOC(sizeof(multi_network_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(multi_network_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
multi_bouquet_name_desc_t *
dvb_alloc_multi_bouquet_name_desc(
	void
	)
{
	multi_bouquet_name_desc_t *desc;
	
	desc = (multi_bouquet_name_desc_t *)DTV_MALLOC(sizeof(multi_bouquet_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(multi_bouquet_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
multi_service_name_desc_t *
dvb_alloc_multi_service_name_desc(
	void
	)
{
	multi_service_name_desc_t *desc;
	
	desc = (multi_service_name_desc_t *)DTV_MALLOC(sizeof(multi_service_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(multi_service_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
multi_component_desc_t *
dvb_alloc_multi_component_desc(
	void
	)
{
	multi_component_desc_t *desc;
	
	desc = (multi_component_desc_t *)DTV_MALLOC(sizeof(multi_component_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(multi_component_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
private_data_specifier_desc_t *
dvb_alloc_private_data_specifier_desc(
	void
	)
{
	private_data_specifier_desc_t *desc;
	
	desc = (private_data_specifier_desc_t *)DTV_MALLOC(sizeof(private_data_specifier_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(private_data_specifier_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
service_move_desc_t *
dvb_alloc_service_move_desc(
	void
	)
{
	service_move_desc_t *desc;
	
	desc = (service_move_desc_t *)DTV_MALLOC(sizeof(service_move_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(service_move_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
short_smoothing_buffer_desc_t *
dvb_alloc_short_smoothing_buffer_desc(
	void
	)
{
	short_smoothing_buffer_desc_t *desc;
	
	desc = (short_smoothing_buffer_desc_t *)DTV_MALLOC(sizeof(short_smoothing_buffer_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(short_smoothing_buffer_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
frequency_list_desc_t *
dvb_alloc_frequency_list_desc(
	void
	)
{
	frequency_list_desc_t *desc;
	
	desc = (frequency_list_desc_t *)DTV_MALLOC(sizeof(frequency_list_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(frequency_list_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
data_broadcast_desc_t *
dvb_alloc_data_broadcast_desc(
	void
	)
{
	data_broadcast_desc_t *desc;
	
	desc = (data_broadcast_desc_t *)DTV_MALLOC(sizeof(data_broadcast_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(data_broadcast_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
data_broadcast_id_desc_t *
dvb_alloc_data_broadcast_id_desc(
	void
	)
{
	data_broadcast_id_desc_t *desc;
	
	desc = (data_broadcast_id_desc_t *)DTV_MALLOC(sizeof(data_broadcast_id_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(data_broadcast_id_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ac3_desc_t *
dvb_alloc_ac3_desc(
	void
	)
{
	ac3_desc_t *desc;
	
	desc = (ac3_desc_t *)DTV_MALLOC(sizeof(ac3_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ac3_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
enhanced_ac3_desc_t *
dvb_alloc_enhanced_ac3_desc(
	void
	)
{
	enhanced_ac3_desc_t *desc;
	
	desc = (enhanced_ac3_desc_t *)DTV_MALLOC(sizeof(enhanced_ac3_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(enhanced_ac3_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
aac_desc_t *
dvb_alloc_aac_desc(
	void
	)
{
	aac_desc_t *desc;
	
	desc = (aac_desc_t *)DTV_MALLOC(sizeof(aac_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(aac_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
fta_content_management_desc_t *
dvb_alloc_fta_content_management_desc(
	void
	)
{
	fta_content_management_desc_t *desc;
	
	desc = (fta_content_management_desc_t *)DTV_MALLOC(sizeof(fta_content_management_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(fta_content_management_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
channel_desc_t *
dvb_alloc_dvbc_lcn_desc(
	void
	)
{
	channel_desc_t *desc;
	
	desc = (channel_desc_t *)DTV_MALLOC(sizeof(channel_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(channel_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
lcn_desc_t *
dvb_alloc_lcn_desc(
	void
	)
{
	lcn_desc_t *desc;
	
	desc = (lcn_desc_t *)DTV_MALLOC(sizeof(lcn_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(lcn_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
service_attribute_desc_t *
dvb_alloc_service_attribute_desc(
	void
	)
{
	service_attribute_desc_t *desc;
	
	desc = (service_attribute_desc_t *)DTV_MALLOC(sizeof(service_attribute_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(service_attribute_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
short_name_desc_t *
dvb_alloc_short_name_desc(
	void
	)
{
	short_name_desc_t *desc;
	
	desc = (short_name_desc_t *)DTV_MALLOC(sizeof(short_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(short_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
lcn_v2_desc_t *
dvb_alloc_lcn_v2_desc(
	void
	)
{
	lcn_v2_desc_t *desc;
	
	desc = (lcn_v2_desc_t *)DTV_MALLOC(sizeof(lcn_v2_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(lcn_v2_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
hd_simulcast_lcn_desc_t *
dvb_alloc_hd_simulcast_lcn_desc(
	void
	)
{
	hd_simulcast_lcn_desc_t *desc;
	
	desc = (hd_simulcast_lcn_desc_t *)DTV_MALLOC(sizeof(hd_simulcast_lcn_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(hd_simulcast_lcn_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
scrambling_desc_t *
dvb_alloc_scrambling_desc(
	void
	)
{
	scrambling_desc_t *desc;
	
	desc = (scrambling_desc_t *)DTV_MALLOC(sizeof(scrambling_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(scrambling_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
paritial_transport_stream_desc_t *
dvb_alloc_partial_transport_stream_desc(
	void
	)
{
	paritial_transport_stream_desc_t *desc;
	
	desc = (paritial_transport_stream_desc_t *)DTV_MALLOC(sizeof(paritial_transport_stream_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(paritial_transport_stream_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
digital_sat_news_gathering_desc_t *
dvb_alloc_digital_sat_news_gethering_desc(
	void
	)
{
	digital_sat_news_gathering_desc_t *desc;
	
	desc = (digital_sat_news_gathering_desc_t *)DTV_MALLOC(sizeof(digital_sat_news_gathering_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(digital_sat_news_gathering_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
pdc_desc_t *
dvb_alloc_pdc_desc(
	void
	)
{
	pdc_desc_t *desc;
	
	desc = (pdc_desc_t *)DTV_MALLOC(sizeof(pdc_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(pdc_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ancillary_data_desc_t *
dvb_alloc_ancillary_data_desc(
	void
	)
{
	ancillary_data_desc_t *desc;
	
	desc = (ancillary_data_desc_t *)DTV_MALLOC(sizeof(ancillary_data_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ancillary_data_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
cell_list_desc_t *
dvb_alloc_cell_list_desc(
	void
	)
{
	cell_list_desc_t *desc;
	
	desc = (cell_list_desc_t *)DTV_MALLOC(sizeof(cell_list_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(cell_list_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
cell_frequency_link_desc_t *
dvb_alloc_cell_frequency_link_desc(
	void
	)
{
	cell_frequency_link_desc_t *desc;
	
	desc = (cell_frequency_link_desc_t *)DTV_MALLOC(sizeof(cell_frequency_link_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(cell_frequency_link_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
announcement_support_desc_t *
dvb_alloc_announcement_support_desc(
	void
	)
{
	announcement_support_desc_t *desc;
	
	desc = (announcement_support_desc_t *)DTV_MALLOC(sizeof(announcement_support_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(announcement_support_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
application_signalling_desc_t *
dvb_alloc_application_signalling_desc(
	void
	)
{
	application_signalling_desc_t *desc;
	
	desc = (application_signalling_desc_t *)DTV_MALLOC(sizeof(application_signalling_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(application_signalling_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
adaptation_field_data_desc_t *
dvb_alloc_adaptation_filed_data_desc(
	void
	)
{
	adaptation_field_data_desc_t *desc;
	
	desc = (adaptation_field_data_desc_t *)DTV_MALLOC(sizeof(adaptation_field_data_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(adaptation_field_data_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
service_availability_desc_t *
dvb_alloc_service_availability_desc(
	void
	)
{
	service_availability_desc_t *desc;
	
	desc = (service_availability_desc_t *)DTV_MALLOC(sizeof(service_availability_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(service_availability_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
service_list_name_desc_t *
dvb_alloc_service_list_name_desc(
	void
	)
{
	service_list_name_desc_t *desc;
	
	desc = (service_list_name_desc_t *)DTV_MALLOC(sizeof(service_list_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(service_list_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
bouquet_list_desc_t *
dvb_alloc_bouquet_list_desc(
	void
	)
{
	bouquet_list_desc_t *desc;
	
	desc = (bouquet_list_desc_t *)DTV_MALLOC(sizeof(bouquet_list_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(bouquet_list_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
virtual_service_id_desc_t *
dvb_alloc_virtual_service_id_desc(
	void
	)
{
	virtual_service_id_desc_t *desc;
	
	desc = (virtual_service_id_desc_t *)DTV_MALLOC(sizeof(virtual_service_id_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(virtual_service_id_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
canal_audio_name_desc_t *
dvb_alloc_canal_audio_name_desc(
	void
	)
{
	canal_audio_name_desc_t *desc;
	
	desc = (canal_audio_name_desc_t *)DTV_MALLOC(sizeof(canal_audio_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(canal_audio_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ciplus_content_label_desc_t *
dvb_alloc_ciplus_content_label_desc(
	void
	)
{
	ciplus_content_label_desc_t *desc;
	
	desc = (ciplus_content_label_desc_t *)DTV_MALLOC(sizeof(ciplus_content_label_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ciplus_content_label_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ciplus_service_desc_t *
dvb_alloc_ciplus_service_desc(
	void
	)
{
	ciplus_service_desc_t *desc;
	
	desc = (ciplus_service_desc_t *)DTV_MALLOC(sizeof(ciplus_service_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ciplus_service_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ci_protection_desc_t *
dvb_alloc_ci_protection_desc(
	void
	)
{
	ci_protection_desc_t *desc;
	
	desc = (ci_protection_desc_t *)DTV_MALLOC(sizeof(ci_protection_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ci_protection_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ac3_audio_stream_desc_t *
dvb_alloc_ac3_audio_stream_desc(
	void
	)
{
	ac3_audio_stream_desc_t *desc;
	
	desc = (ac3_audio_stream_desc_t *)DTV_MALLOC(sizeof(ac3_audio_stream_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ac3_audio_stream_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
psip_caption_service_desc_t *
dvb_alloc_caption_service_desc(
	void
	)
{
	psip_caption_service_desc_t *desc;
	
	desc = (psip_caption_service_desc_t *)DTV_MALLOC(sizeof(psip_caption_service_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(psip_caption_service_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
enhanced_ac3_audio_stream_desc_t *
dvb_alloc_enhanced_ac3_audio_stream_desc(
	void
	)
{
	enhanced_ac3_audio_stream_desc_t *desc;
	
	desc = (enhanced_ac3_audio_stream_desc_t *)DTV_MALLOC(sizeof(enhanced_ac3_audio_stream_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(enhanced_ac3_audio_stream_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
descriptor_t *
dvb_desc_alloc(
	unsigned char tag
	)
{
	descriptor_t *desc;

	desc = (descriptor_t *)DTV_MALLOC(sizeof(descriptor_t));

	if (desc == NULL)
	{
		return NULL;
	}

	DTV_MEMSET(desc, 0, sizeof(descriptor_t));

	switch (tag)
	{
		case DESC_TAG_VIDEO_STREAM:
			desc->data = (void *)dvb_alloc_video_stream_desc();
		break;
		
		case DESC_TAG_AUDIO_STREAM:
			desc->data = (void *)dvb_alloc_audio_stream_desc();
		break;
		
		case DESC_TAG_HIERARCHY:
			desc->data = (void *)dvb_alloc_hierarchy_desc();
		break;
		
		case DESC_TAG_REGISTRATION:
			desc->data = (void *)dvb_alloc_registration_desc();
		break;
		
		case DESC_TAG_DATA_STREAM_ALIGN:
			desc->data = (void *)dvb_alloc_data_stream_align_desc();
		break;
		
		case DESC_TAG_TARGET_BG_GRID:
			desc->data = (void *)dvb_alloc_target_bg_grid_desc();
		break;
		
		case DESC_TAG_VIDEO_WINDOW:
			desc->data = (void *)dvb_alloc_video_window_desc();
		break;
		
		case DESC_TAG_CA:
			desc->data = (void *)dvb_alloc_ca_desc();
		break;
		
		case DESC_TAG_ISO_639_LANG:
			desc->data = (void *)dvb_alloc_iso_639_lang_desc();
		break;
		
		case DESC_TAG_SYSTEM_CLOCK:
			desc->data = (void *)dvb_alloc_system_clock_desc();
		break;
		
		case DESC_TAG_MULTI_BUFFER_UTIL:
			desc->data = (void *)dvb_alloc_multi_buffer_util_desc();
		break;
		
		case DESC_TAG_COPYRIGHT:
			desc->data = (void *)dvb_alloc_copyright_desc();
		break;
		
		case DESC_TAG_MAX_BITRATE:
			desc->data = (void *)dvb_alloc_max_bitrate_desc();
		break;
		
		case DESC_TAG_PRIVATE_DATA_INDICATOR:
			desc->data = (void *)dvb_alloc_private_data_indicator_desc();
		break;
		
		case DESC_TAG_SMOOTHING_BUFFER:
			desc->data = (void *)dvb_alloc_smoothing_buffer_desc();
		break;

		case DESC_TAG_MPEG4_AAC_PLUS:
		break;

		case DESC_TAG_AVC:
			desc->data = (void *)dvb_alloc_avc_desc();
		break;

		case DESC_TAG_MPEG2_AAC:
		break;
		
		case DESC_TAG_NETWORK_NAME:
			desc->data = (void *)dvb_alloc_network_name_desc();
		break;
		
		case DESC_TAG_SERVICE_LIST:
			desc->data = (void *)dvb_alloc_service_list_desc();
		break;
		
		case DESC_TAG_STUFFING:
		break;
		
		case DESC_TAG_SAT_DELIVERY_SYSTEM:
			desc->data = (void *)dvb_alloc_sat_delivery_system_desc();
		break;
		
		case DESC_TAG_CABLE_DELIVERY_SYSTEM:
			desc->data = (void *)dvb_alloc_cable_delivery_system_desc();
		break;
		
		case DESC_TAG_VBI_DATA:
		break;
		
		case DESC_TAG_VBI_TELETEXT:
		break;
		
		case DESC_TAG_BOUQUET_NAME:
			desc->data = (void *)dvb_alloc_bouquet_name_desc();
		break;
		
		case DESC_TAG_SERVICE:
			desc->data = (void *)dvb_alloc_service_desc();
		break;
		
		case DESC_TAG_COUNTRY_AVAILABILITY:
			desc->data = (void *)dvb_alloc_country_availability_desc();
		break;
		
		case DESC_TAG_LINKAGE:
			desc->data = (void *)dvb_alloc_linkage_desc();
		break;
		
		case DESC_TAG_NVOD_REF:
			desc->data = (void *)dvb_alloc_nvod_ref_desc();
		break;
		
		case DESC_TAG_TIME_SHIFTED_SERVICE:
			desc->data = (void *)dvb_alloc_time_shifted_service_desc();
		break;
		
		case DESC_TAG_SHORT_EVENT:
			desc->data = (void *)dvb_alloc_short_event_desc();
		break;
		
		case DESC_TAG_EXTENDED_EVENT:
			desc->data = (void *)dvb_alloc_extended_event_desc();
		break;
		
		case DESC_TAG_TIME_SHIFTED_EVENT:
			desc->data = (void *)dvb_alloc_time_shifted_event_desc();
		break;
		
		case DESC_TAG_COMPONENT:
			desc->data = (void *)dvb_alloc_component_desc();
		break;
		
		case DESC_TAG_MOSAIC:
			desc->data = (void *)dvb_alloc_mosaic_desc();
		break;
		
		case DESC_TAG_STREAM_IDENTIFIER:
			desc->data = (void *)dvb_alloc_stream_identifier_desc();
		break;
		
		case DESC_TAG_CA_IDENTIFIER:
			desc->data = (void *)dvb_alloc_ca_identifier_desc();
		break;
		
		case DESC_TAG_CONTENT:
			desc->data = (void *)dvb_alloc_content_desc();
		break;
		
		case DESC_TAG_PARENTAL_RATING:
			desc->data = (void *)dvb_alloc_parental_rating_desc();
		break;
		
		case DESC_TAG_TELETEXT:
			desc->data = (void *)dvb_alloc_teletext_desc();
		break;
		
		case DESC_TAG_TELEPHONE:
			desc->data = (void *)dvb_alloc_telephone_desc();
		break;
		
		case DESC_TAG_LOCAL_TIME_OFFSET:
			desc->data = (void *)dvb_alloc_local_time_offset_desc();
		break;
		
		case DESC_TAG_SUBTITLING:
			desc->data = (void *)dvb_alloc_subtitling_desc();
		break;
		
		case DESC_TAG_TER_DELIVERY_SYSTEM:
			desc->data = (void *)dvb_alloc_ter_delivery_system_desc();
		break;
		
		case DESC_TAG_MULTILINGUAL_NETWORK_NAME:
			desc->data = (void *)dvb_alloc_multi_network_name_desc();
		break;
		
		case DESC_TAG_MULTILINGUAL_BOUQUET_NAME:
			desc->data = (void *)dvb_alloc_multi_bouquet_name_desc();
		break;
		
		case DESC_TAG_MULTILINGUAL_SERVICE_NAME:
			desc->data = (void *)dvb_alloc_multi_service_name_desc();
		break;
		
		case DESC_TAG_MULTILINGUAL_COMPONNT:
			desc->data = (void *)dvb_alloc_multi_component_desc();
		break;
		
		case DESC_TAG_PRIVATE_DATA_SPECIFIER:
			desc->data = (void *)dvb_alloc_private_data_specifier_desc();
		break;
		
		case DESC_TAG_SERVICE_MOVE:
			desc->data = (void *)dvb_alloc_service_move_desc();
		break;
		
		case DESC_TAG_SHORT_SMOOTHING_BUFFER:
			desc->data = (void *)dvb_alloc_short_smoothing_buffer_desc();
		break;
		
		case DESC_TAG_FREQENCY_LIST:
			desc->data = (void *)dvb_alloc_frequency_list_desc();
		break;
	
		case DESC_TAG_DATA_BROADCAST:
			desc->data = (void *)dvb_alloc_data_broadcast_desc();
		break;
	
		case DESC_TAG_DATA_BROADCAST_ID:
			desc->data = (void *)dvb_alloc_data_broadcast_id_desc();
		break;
	
		case DESC_TAG_AC3:
			desc->data = (void *)dvb_alloc_ac3_desc();
		break;

		case DESC_TAG_SCRAMBLING:
			desc->data = (void *)dvb_alloc_scrambling_desc();
		break;

		case DESC_TAG_PARTIAL_TRANSPORT_STREAM:
			desc->data = (void *)dvb_alloc_partial_transport_stream_desc();
		break;

		case DESC_TAG_DIGITAL_SAT_NEWS_GETHERING:
			desc->data = (void *)dvb_alloc_digital_sat_news_gethering_desc();
		break;

		case DESC_TAG_PDC:
			desc->data = (void *)dvb_alloc_pdc_desc();
		break;

		case DESC_TAG_ANCILLARY_DATA:
			desc->data = (void *)dvb_alloc_ancillary_data_desc();
		break;

		case DESC_TAG_CELL_LIST:
			desc->data = (void *)dvb_alloc_cell_list_desc();
		break;

		case DESC_TAG_CELL_FREQUENCY_LINK:
			desc->data = (void *)dvb_alloc_cell_frequency_link_desc();
		break;

		case DESC_TAG_ANNOUNCEMENT_SUPPORT:
			desc->data = (void *)dvb_alloc_announcement_support_desc();
		break;

		case DESC_TAG_APPLICATION_SIGNALLING:
			desc->data = (void *)dvb_alloc_application_signalling_desc();
		break;

		case DESC_TAG_ADAPTATION_FIELD_DATA:
			desc->data = (void *)dvb_alloc_adaptation_filed_data_desc();
		break;

		case DESC_TAG_SERVICE_IDENTIFIER:
		break;

		case DESC_TAG_SERVICE_AVAILABLITY:
			desc->data = (void *)dvb_alloc_service_availability_desc();
		break;

		case DESC_TAG_DEFAULT_AUTHORITY:
		break;

		case DESC_TAG_RELATED_CONTENT:
		break;

		case DESC_TAG_TVA_ID:
		break;

		case DESC_TAG_CONTENT_IDENTIFIER:
		break;

		case DESC_TAG_TIME_SLICE_FEC_IDENTIFIER:
		break;

		case DESC_TAG_ECM_REPETITION_RATE:
		break;

		case DESC_TAG_S2_SAT_DELIVERY_SYSTEM:
		break;

		case DESC_TAG_ENHANCED_AC3:
			desc->data = (void *)dvb_alloc_enhanced_ac3_desc();
		break;

		case DESC_TAG_DTS:
		break;

		case DESC_TAG_AAC:
			desc->data = (void *)dvb_alloc_aac_desc();
		break;

		case DESC_TAG_XAIT_LOCATION:
		break;

		case DESC_TAG_FTA_CONTENT_MANAGEMENT:
			desc->data = (void *)dvb_alloc_fta_content_management_desc();
		break;

		case DESC_TAG_EXTENSION:
		break;

		case DESC_TAG_DVBC_LOGICAL_CHANNEL_NUMBER:
			desc->data = (void *)dvb_alloc_dvbc_lcn_desc();
		break;
		
		case DESC_TAG_DVB_LOGICAL_CHANNEL_NUMBER:
			desc->data = (void *)dvb_alloc_lcn_desc();
		break;

#if 0
		// DTG_SIPSI
		case DESC_TAG_SERVICE_ATTRIBUTE:
			desc->data = (void *)dvb_alloc_service_attribute_desc();
		break;
#endif

#if 1
		case DESC_TAG_SHORTNAME:
			desc->data = (void *)dvb_alloc_short_name_desc();
		break;
#else
		case DESC_TAG_LOGICAL_CHANNEL_NUM_V2:
			desc->data = (void *)dvb_alloc_lcn_v2_desc();
		break;
#endif

		// DTG_SIPSI, HD_SIMULCAST_LCN
		case DESC_TAG_HD_SIMULCAST_LOGICAL_CHANNEL:
			desc->data = (void *)dvb_alloc_hd_simulcast_lcn_desc();
		break;

		// BSKYB_CHANNEL_NUMBERING
		case DESC_TAG_LOGICAL_CHANNEL_NUMBER:
			desc->data = (void *)dvb_alloc_lcn_desc();
		break;

#if 0
		/* ASTRA APS(ASTRA PLATFORM SERVICE) - SGT(SERVICE GUIDE TABLE) */
		// HD_PLUS_SCAN
		case DESC_TAG_SERVICE_LIST_NAME:
			desc->data = (void *)dvb_alloc_service_list_name_desc();
		break;
#endif

		case DESC_TAG_BOUQUET_LIST:
			desc->data = (void *)dvb_alloc_bouquet_list_desc();
		break;

		case DESC_TAG_VIRTUAL_SERVICE_ID:
			desc->data = (void *)dvb_alloc_virtual_service_id_desc();
		break;

		// CANAL
		case DESC_TAG_CANAL_AUDIO_NAME:
			desc->data = (void *)dvb_alloc_canal_audio_name_desc();
		break;

		case DESC_TAG_CANAL_DATA:
		break;

#if 0
		// CIPLUS_SUPPORT
		case DESC_TAG_CIPLUS_CONTENT_LABEL:
			desc->data = (void *)dvb_alloc_ciplus_content_label_desc();
		break;
		
		case DESC_TAG_CIPLUS_SERVICE:
			desc->data = (void *)dvb_alloc_ciplus_service_desc();
		break;
		
		case DESC_TAG_CIPLUS_PROTECTION:
			desc->data = (void *)dvb_alloc_ci_protection_desc();
		break;
#endif

		/* ATSC - PSIP(Program and System Information Protocol for Terrestrial and Cable */
		case DESC_TAG_ATSC_STUFFING:
		break;

		case DESC_TAG_ATSC_AC3:
			desc->data = (void *)dvb_alloc_ac3_audio_stream_desc();
		break;

		case DESC_TAG_ATSC_CAPTION_SERVICE:
			desc->data = (void *)dvb_alloc_caption_service_desc();
		break;

#if 0
		case DESC_TAG_ATSC_CONTENT_ADVISORY:
		break;

		case DESC_TAG_ATSC_EXTENDED_CHANNEL_NAME:
		break;

		case DESC_TAG_ATSC_SERVICE_LOCATION:
		break;

		case DESC_TAG_ATSC_TIMESHIFTED_SERVICE:
		break;

		case DESC_TAG_ATSC_COMPONENT_NAME:
		break;

		case DESC_TAG_ATSC_DCC_DEPARTING_REQUEST:
		break;

		case DESC_TAG_ATSC_DCC_ARRIVING_REQUEST:
		break;

		case DESC_TAG_ATSC_REDISTRIBUTION_CONTROL:
		break;

		case DESC_TAG_ATSC_GENRE:
		break;

		case DESC_TAG_ATSC_PRIVATE_INFORMATION:
		break;
#endif

		case DESC_TAG_ATSC_ENHANCED_AC3:
			desc->data = (void *)dvb_alloc_enhanced_ac3_audio_stream_desc();
		break;

		// ARIB_SUBTITLE
		case DESC_TAG_ARIB_SUBTITLE:
		break;

		default:
		break;
	}

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_private_data(
	private_data_t *priv
	)
{
	if (priv)
	{
		if (priv->data)
		{
			DTV_FREE(priv->data);
		}
		
		DTV_FREE(priv);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_video_stream_desc(
	video_stream_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_audio_stream_desc(
	audio_stream_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_hierarchy_desc(
	hierarchy_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_registration_desc(
	registration_desc_t *desc
	)
{
	if (desc)
	{
		dvb_free_private_data(desc->priv_data);
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_data_stream_align_desc(
	data_stream_align_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_target_bg_grid_desc(
	target_bg_grid_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_video_window_desc(
	video_window_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ca_desc(
	ca_desc_t *desc
	)
{
	if (desc)
	{
		dvb_free_private_data(desc->priv_data);
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_iso_639_lang_desc(
	iso_639_lang_desc_t *desc
	)
{
	iso_639_lang_data_t	*data, *next;
	
	if (desc)
	{
		data = desc->lang_data;
		
		while (data != NULL)
		{
			next = data->next;

			DTV_FREE(data);

			data = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_system_clock_desc(
	system_clock_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_multi_buffer_util_desc(
	multi_buffer_util_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_copyright_desc(
	copyright_desc_t *desc
	)
{
	if (desc)
	{
		dvb_free_private_data(desc->priv_data);
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_max_bitrate_desc(
	max_bitrate_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_private_data_indicator_desc(
	private_data_indicator_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_smoothing_buffer_desc(
	smoothing_buffer_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_avc_desc(
	avc_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_network_name_desc(
	network_name_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->name)
		{
			DTV_FREE(desc->name);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_service_list_desc(
	service_list_desc_t *desc
	)
{
	service_list_data_t	*svc, *next;
	
	if (desc)
	{
		svc = desc->service_data;
		
		while (svc != NULL)
		{
			next = svc->next;
			
			DTV_FREE(svc);

			svc = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_sat_delivery_system_desc(
	sat_delivery_system_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_cable_delivery_system_desc(
	cable_delivery_system_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_bouquet_name_desc(
	bouquet_name_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->name)
		{
			DTV_FREE(desc->name);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_service_desc(
	service_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->provider_name)
		{
			DTV_FREE(desc->provider_name);
		}

		if (desc->service_name)
		{
			DTV_FREE(desc->service_name);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_country_availability_desc(
	country_availability_desc_t *desc
	)
{
	country_code_data_t	*code_data, *next;
	
	if (desc)
	{
		code_data = desc->code_data;
		
		while (code_data != NULL)
		{
			next = code_data->next;

			DTV_FREE(code_data);

			code_data = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_linkage_desc(
	linkage_desc_t *desc
	)
{
	if (desc)
	{
		dvb_free_private_data(desc->priv_data);
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_nvod_ref_desc(
	nvod_ref_desc_t *desc
	)
{
	nvod_ref_data_t	*nvod, *next;
	
	if (desc)
	{
		nvod = desc->nvod_data;
		
		while (nvod != NULL)
		{
			next = nvod->next;

			DTV_FREE(nvod);

			nvod = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_time_shifted_service_desc(
	time_shifted_service_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_short_event_desc(
	short_event_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->name)
		{
			DTV_FREE(desc->name);
		}

		if (desc->text)
		{
			DTV_FREE(desc->text);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_extended_event_desc(
	extended_event_desc_t *desc
	)
{
	extended_event_data_t	*ext_evt, *next;
	
	if (desc)
	{
		ext_evt = desc->ext_evt_data;
		
		while (ext_evt != NULL)
		{
			next = ext_evt->next;

			if (ext_evt->description)
			{
				DTV_FREE(ext_evt->description);
			}

			if (ext_evt->name)
			{
				DTV_FREE(ext_evt->name);
			}

			DTV_FREE(ext_evt);

			ext_evt = next;
		}

		if (desc->text)
		{
			DTV_FREE(desc->text);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_time_shifted_event_desc(
	time_shifted_event_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_component_desc(
	component_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->text)
		{
			DTV_FREE(desc->text);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_mosaic_desc(
	mosaic_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_stream_identifier_desc(
	stream_identifier_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ca_identifier_desc(
	ca_identifier_desc_t *desc
	)
{
	ca_identifier_data_t	*ca_id, *next;
	
	if (desc)
	{
		ca_id = desc->ca_id_data;
		
		while (ca_id != NULL)
		{
			next = ca_id->next;

			DTV_FREE(ca_id);

			ca_id = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_content_desc(
	content_desc_t *desc
	)
{
	content_data_t *content, *next;
		
	if (desc)
	{
		content = desc->content_data;
		
		while (content != NULL)
		{
			next = content->next;

			DTV_FREE(content);

			content = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_parental_rating_desc(
	parental_rating_desc_t *desc
	)
{
	parental_rating_data_t	*rating, *next;
	
	if (desc)
	{
		rating = desc->rating_data;
		
		while (rating != NULL)
		{
			next = rating->next;

			DTV_FREE(rating);

			rating = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_teletext_desc(
	teletext_desc_t *desc
	)
{
	teletext_data_t	*txt, *next;
	
	if (desc)
	{
		txt = desc->txt_data;
		
		while (txt != NULL)
		{
			next = txt->next;

			DTV_FREE(txt);

			txt = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_telephone_desc(
	telephone_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->country_prefix)
		{
			DTV_FREE(desc->country_prefix);
		}

		if (desc->international_area_code)
		{
			DTV_FREE(desc->international_area_code);
		}

		if (desc->operator_code)
		{
			DTV_FREE(desc->operator_code);
		}

		if (desc->national_area_code)
		{
			DTV_FREE(desc->national_area_code);
		}

		if (desc->core_number)
		{
			DTV_FREE(desc->core_number);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_local_time_offset_desc(
	local_time_offset_desc_t *desc
	)
{
	local_time_offset_data_t	*offset, *next;
	
	if (desc)
	{
		offset = desc->offset_data;
		
		while (offset != NULL)
		{
			next = offset->next;

			DTV_FREE(offset);

			offset = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_subtitling_desc(
	subtitling_desc_t *desc
	)
{
	subtitling_data_t	*subt, *next;
	
	if (desc)
	{
		subt = desc->subt_data;
		
		while (subt != NULL)
		{
			next = subt->next;

			DTV_FREE(subt);

			subt = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ter_delivery_system_desc(
	ter_delivery_system_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_multi_network_name_desc(
	multi_network_name_desc_t *desc
	)
{
	multi_network_name_data_t	*name, *next;
	
	if (desc)
	{
		name = desc->name_data;
		
		while (name != NULL)
		{
			next = name->next;

			if (name->name)
			{
				DTV_FREE(name->name);
			}

			DTV_FREE(name);

			name = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_multi_bouquet_name_desc(
	multi_bouquet_name_desc_t *desc
	)
{
	multi_bouquet_name_data_t	*name, *next;
	
	if (desc)
	{
		name = desc->name_data;
		
		while (name != NULL)
		{
			next = name->next;

			if (name->name)
			{
				DTV_FREE(name->name);
			}

			DTV_FREE(name);

			name = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_multi_service_name_desc(
	multi_service_name_desc_t *desc
	)
{
	multi_service_name_data_t	*name, *next;
	
	if (desc)
	{
		name = desc->name_data;
		
		while (name != NULL)
		{
			next = name->next;

			if (name->provider_name)
			{
				DTV_FREE(name->provider_name);
			}

			if (name->service_name)
			{
				DTV_FREE(name->service_name);
			}

			DTV_FREE(name);

			name = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_multi_component_desc(
	multi_component_desc_t *desc
	)
{
	multi_component_data_t	*comp, *next;
	
	if (desc)
	{
		comp = desc->component_data;
		
		while (comp != NULL)
		{
			next = comp->next;

			if (comp->description)
			{
				DTV_FREE(comp->description);
			}

			DTV_FREE(comp);

			comp = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_private_data_specifier_desc(
	private_data_specifier_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_service_move_desc(
	service_move_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_short_smoothing_buffer_desc(
	short_smoothing_buffer_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_frequency_list_desc(
	frequency_list_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->frequency)
		{
			DTV_FREE(desc->frequency);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_data_broadcast_desc(
	data_broadcast_desc_t *desc
	)
{
	if (desc)
	{
		dvb_free_private_data(desc->selector_byte);

		if (desc->text)
		{
			DTV_FREE(desc->text);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_data_broadcast_id_desc(
	data_broadcast_id_desc_t *desc
	)
{
	if (desc)
	{
		dvb_free_private_data(desc->selector_byte);

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ac3_desc(
	ac3_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->info)
		{
			DTV_FREE(desc->info);
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_enhanced_ac3_desc(
	enhanced_ac3_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->info)
		{
			DTV_FREE(desc->info);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_aac_desc(
	aac_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->info)
		{
			DTV_FREE(desc->info);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_fta_content_management_desc(
	fta_content_management_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_dvbc_lcn_desc(
	channel_desc_t *desc
	)
{
	channel_data_t	*lcn, *next;
	
	if (desc)
	{
		lcn = desc->channel_data;
		
		while (lcn != NULL)
		{
			next = lcn->next;
			
			DTV_FREE(lcn);

			lcn = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_lcn_desc(
	lcn_desc_t *desc
	)
{
	lcn_data_t	*lcn, *next;
	
	if (desc)
	{
		lcn = desc->lcn_data;
		
		while (lcn != NULL)
		{
			next = lcn->next;
			
			DTV_FREE(lcn);

			lcn = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_service_attribute_desc(
	service_attribute_desc_t *desc
	)
{
	service_attribute_data_t	*svc_attr, *next;
	
	if (desc)
	{
		svc_attr = desc->svc_attr_data;
		
		while (svc_attr != NULL)
		{
			next = svc_attr->next;
			
			DTV_FREE(svc_attr);

			svc_attr = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_short_name_desc(
	short_name_desc_t *desc
	)
{
	//short_name_desc_t	*name;
	
	if (desc)
	{
		if(desc->name != NULL)
		{
			DTV_FREE(desc->name);
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_lcn_v2_desc(
	lcn_v2_desc_t *desc
	)
{
	lcn_v2_list_data_t	*list;
	
	if (desc)
	{
		list = desc->lcn_list_data;
		
		if (list != NULL)
		{
			lcn_v2_data_t	*lcn, *next;

			if(list->channel_list_name != NULL)
			{
				DTV_FREE(list->channel_list_name);
			}

			lcn = list->lcn_data;
			
			while(lcn != NULL)
			{
				next = lcn->next;
				
				DTV_FREE(lcn);
				
				lcn = next;
			}
			
			DTV_FREE(list);
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_hd_simulcast_lcn_desc(
	hd_simulcast_lcn_desc_t *desc
	)
{
	hd_simulcast_lcn_data_t	*lcn, *next;
	
	if (desc)
	{
		lcn = desc->lcn_data;
		
		while (lcn != NULL)
		{
			next = lcn->next;
			
			DTV_FREE(lcn);

			lcn = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_scrambling_desc(
	scrambling_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_partial_transport_stream_desc(
	paritial_transport_stream_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_digital_sat_news_gathering_desc(
	digital_sat_news_gathering_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->dsng)
		{
			DTV_FREE(desc->dsng);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_pdc_desc(
	pdc_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ancillary_data_desc(
	ancillary_data_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_cell_list_desc(
	cell_list_desc_t *desc
	)
{
	cell_list_t	*list, *next;
	subcell_list_t *sublist, *subnext;
	
	if (desc)
	{
		list = desc->list;
		
		while (list != NULL)
		{
			sublist = list->sublist;
			
			while (sublist != NULL)
			{
				subnext = sublist->next;
				
				DTV_FREE(sublist);
			
				sublist = subnext;
			}
			
			next = list->next;

			DTV_FREE(list);

			list = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_cell_frequency_link_desc(
	cell_frequency_link_desc_t *desc
	)
{
	cell_frequency_link_t	*link, *next;
	subcell_frequency_link_t *sublink, *subnext;
	
	if (desc)
	{
		link = desc->frequency_link;
		
		while (link != NULL)
		{
			sublink = link->subfrequency_link;
			
			while (sublink != NULL)
			{
				subnext = sublink->next;
				
				DTV_FREE(sublink);
			
				sublink = subnext;
			}
			
			next = link->next;

			DTV_FREE(link);

			link = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_announcement_support_desc(
	announcement_support_desc_t *desc
	)
{
	announcement_support_t	*announcement_support, *next;
	
	if (desc)
	{
		announcement_support = desc->announcement_support;
		
		while (announcement_support != NULL)
		{
			next = announcement_support->next;

			DTV_FREE(announcement_support);

			announcement_support = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_application_signalling_desc(
	application_signalling_desc_t *desc
	)
{
	application_signalling_data_t *application_signalling, *next;

	if (desc)
	{
		application_signalling = desc->application_signalling_data;
		
		while (application_signalling != NULL)
		{
			next = application_signalling->next;

			DTV_FREE(application_signalling);

			application_signalling = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_adaptation_filed_data_desc(
	adaptation_field_data_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_service_availability_desc(
	service_availability_desc_t *desc
	)
{
	service_availability_t	*service_availability, *next;
	
	if (desc)
	{
		service_availability = desc->service_availability;
		
		while (service_availability != NULL)
		{
			next = service_availability->next;

			DTV_FREE(service_availability);

			service_availability = next;
		}

		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_service_list_name_desc(
	service_list_name_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->name)
		{
			DTV_FREE(desc->name);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_bouquet_list_desc(
	bouquet_list_desc_t *desc
	)
{
	bouq_list_t	*list, *next;
	
	if (desc)
	{
		list = desc->list;
		
		while (list != NULL)
		{
			next = list->next;

			DTV_FREE(list);

			list = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_virtual_service_id_desc(
	virtual_service_id_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_canal_audio_name_desc(
	canal_audio_name_desc_t *desc
	)
{
	if (desc)
	{
		if(desc->audio_name != NULL)
		{
			DTV_FREE(desc->audio_name);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ciplus_content_label_desc(
	ciplus_content_label_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ciplus_service_desc(
	ciplus_service_desc_t *desc
	)
{
	if (desc)
	{
		if (desc->provider_name)
		{
			DTV_FREE(desc->provider_name);
		}

		if (desc->service_name)
		{
			DTV_FREE(desc->service_name);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ci_protection_desc(
	ci_protection_desc_t *desc
	)
{
	ci_protection_brand_t	*brand, *next;

	if (desc)
	{
		brand = desc->brand;
		
		while (brand != NULL)
		{
			next = brand->next;

			DTV_FREE(brand);

			brand = next;
		}

		dvb_free_private_data(desc->priv_data);
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_ac3_audio_stream_desc(
	ac3_audio_stream_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_caption_service_desc(
	psip_caption_service_desc_t *desc
	)
{
	psip_caption_service_t	*caption, *next;

	if (desc)
	{
		caption = desc->caption_service;
		
		while (caption != NULL)
		{
			next = caption->next;

			DTV_FREE(caption);

			caption = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_free_enhanced_ac3_audio_stream_desc(
	enhanced_ac3_audio_stream_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_desc_free(
	descriptor_t *desc
	)
{
	if (desc == NULL)
	{
		return;
	}
	
	switch(desc->tag)
	{
		case DESC_TAG_VIDEO_STREAM:
			dvb_free_video_stream_desc((video_stream_desc_t *)desc->data);
		break;

		case DESC_TAG_AUDIO_STREAM:
			dvb_free_audio_stream_desc((audio_stream_desc_t *)desc->data);
		break;

		case DESC_TAG_HIERARCHY:
			dvb_free_hierarchy_desc((hierarchy_desc_t *)desc->data);
		break;

		case DESC_TAG_REGISTRATION:
			dvb_free_registration_desc((registration_desc_t *)desc->data);
		break;

		case DESC_TAG_DATA_STREAM_ALIGN:
			dvb_free_data_stream_align_desc((data_stream_align_desc_t *)desc->data);
		break;

		case DESC_TAG_TARGET_BG_GRID:
			dvb_free_target_bg_grid_desc((target_bg_grid_desc_t *)desc->data);
		break;

		case DESC_TAG_VIDEO_WINDOW:
			dvb_free_video_window_desc((video_window_desc_t *)desc->data);
		break;

		case DESC_TAG_CA:
			dvb_free_ca_desc((ca_desc_t *)desc->data);
		break;

		case DESC_TAG_ISO_639_LANG:
			dvb_free_iso_639_lang_desc((iso_639_lang_desc_t *)desc->data);
		break;

		case DESC_TAG_SYSTEM_CLOCK:
			dvb_free_system_clock_desc((system_clock_desc_t *)desc->data);
		break;

		case DESC_TAG_MULTI_BUFFER_UTIL:
			dvb_free_multi_buffer_util_desc((multi_buffer_util_desc_t *)desc->data);
		break;

		case DESC_TAG_COPYRIGHT:
			dvb_free_copyright_desc((copyright_desc_t *)desc->data);
		break;

		case DESC_TAG_MAX_BITRATE:
			dvb_free_max_bitrate_desc((max_bitrate_desc_t *)desc->data);
		break;

		case DESC_TAG_PRIVATE_DATA_INDICATOR:
			dvb_free_private_data_indicator_desc((private_data_indicator_desc_t *)desc->data);
		break;

		case DESC_TAG_SMOOTHING_BUFFER:
			dvb_free_smoothing_buffer_desc((smoothing_buffer_desc_t *)desc->data);
		break;

		case DESC_TAG_MPEG4_AAC_PLUS:
		break;

		case DESC_TAG_AVC:
			dvb_free_avc_desc((avc_desc_t *)desc->data);
		break;

		case DESC_TAG_MPEG2_AAC:
		break;

		case DESC_TAG_NETWORK_NAME:
			dvb_free_network_name_desc((network_name_desc_t *)desc->data);
		break;

		case DESC_TAG_SERVICE_LIST:
			dvb_free_service_list_desc((service_list_desc_t *)desc->data);
		break;

		case DESC_TAG_STUFFING:
		break;

		case DESC_TAG_SAT_DELIVERY_SYSTEM:
			dvb_free_sat_delivery_system_desc((sat_delivery_system_desc_t *)desc->data);
		break;

		case DESC_TAG_CABLE_DELIVERY_SYSTEM:
			dvb_free_cable_delivery_system_desc((cable_delivery_system_desc_t *)desc->data);
		break;

		case DESC_TAG_VBI_DATA:
		break;

		case DESC_TAG_VBI_TELETEXT:
		break;

		case DESC_TAG_BOUQUET_NAME:
			dvb_free_bouquet_name_desc((bouquet_name_desc_t *)desc->data);
		break;

		case DESC_TAG_SERVICE:
			dvb_free_service_desc((service_desc_t *)desc->data);
		break;

		case DESC_TAG_COUNTRY_AVAILABILITY:
			dvb_free_country_availability_desc((country_availability_desc_t *)desc->data);
		break;

		case DESC_TAG_LINKAGE:
			dvb_free_linkage_desc((linkage_desc_t *)desc->data);
		break;

		case DESC_TAG_NVOD_REF:
			dvb_free_nvod_ref_desc((nvod_ref_desc_t *)desc->data);
		break;

		case DESC_TAG_TIME_SHIFTED_SERVICE:
			dvb_free_time_shifted_service_desc((time_shifted_service_desc_t *)desc->data);		
		break;

		case DESC_TAG_SHORT_EVENT:
			dvb_free_short_event_desc((short_event_desc_t *)desc->data);		
		break;

		case DESC_TAG_EXTENDED_EVENT:
			dvb_free_extended_event_desc((extended_event_desc_t *)desc->data);		
		break;

		case DESC_TAG_TIME_SHIFTED_EVENT:
			dvb_free_time_shifted_event_desc((time_shifted_event_desc_t *)desc->data);		
		break;

		case DESC_TAG_COMPONENT:
			dvb_free_component_desc((component_desc_t *)desc->data);		
		break;

		case DESC_TAG_MOSAIC:
			dvb_free_mosaic_desc((mosaic_desc_t *)desc->data);		
		break;

		case DESC_TAG_STREAM_IDENTIFIER:
			dvb_free_stream_identifier_desc((stream_identifier_desc_t *)desc->data);		
		break;

		case DESC_TAG_CA_IDENTIFIER:
			dvb_free_ca_identifier_desc((ca_identifier_desc_t *)desc->data);		
		break;

		case DESC_TAG_CONTENT:
			dvb_free_content_desc((content_desc_t *)desc->data);		
		break;

		case DESC_TAG_PARENTAL_RATING:
			dvb_free_parental_rating_desc((parental_rating_desc_t *)desc->data);		
		break;

		case DESC_TAG_TELETEXT:
			dvb_free_teletext_desc((teletext_desc_t *)desc->data);		
		break;

		case DESC_TAG_TELEPHONE:
			dvb_free_telephone_desc((telephone_desc_t *)desc->data);		
		break;

		case DESC_TAG_LOCAL_TIME_OFFSET:
			dvb_free_local_time_offset_desc((local_time_offset_desc_t *)desc->data);		
		break;

		case DESC_TAG_SUBTITLING:
			dvb_free_subtitling_desc((subtitling_desc_t *)desc->data);		
		break;

		case DESC_TAG_TER_DELIVERY_SYSTEM:
			dvb_free_ter_delivery_system_desc((ter_delivery_system_desc_t *)desc->data);		
		break;

		case DESC_TAG_MULTILINGUAL_NETWORK_NAME:
			dvb_free_multi_network_name_desc((multi_network_name_desc_t *)desc->data);		
		break;

		case DESC_TAG_MULTILINGUAL_BOUQUET_NAME:
			dvb_free_multi_bouquet_name_desc((multi_bouquet_name_desc_t *)desc->data);		
		break;

		case DESC_TAG_MULTILINGUAL_SERVICE_NAME:
			dvb_free_multi_service_name_desc((multi_service_name_desc_t *)desc->data);	
		break;

		case DESC_TAG_MULTILINGUAL_COMPONNT:
			dvb_free_multi_component_desc((multi_component_desc_t *)desc->data);		
		break;

		case DESC_TAG_PRIVATE_DATA_SPECIFIER:
			dvb_free_private_data_specifier_desc((private_data_specifier_desc_t *)desc->data);		
		break;

		case DESC_TAG_SERVICE_MOVE:
			dvb_free_service_move_desc((service_move_desc_t *)desc->data);		
		break;

		case DESC_TAG_SHORT_SMOOTHING_BUFFER:
			dvb_free_short_smoothing_buffer_desc((short_smoothing_buffer_desc_t *)desc->data);		
		break;

		case DESC_TAG_FREQENCY_LIST:
			dvb_free_frequency_list_desc((frequency_list_desc_t *)desc->data);		
		break;

		case DESC_TAG_DATA_BROADCAST:
			dvb_free_data_broadcast_desc((data_broadcast_desc_t *)desc->data);		
		break;

		case DESC_TAG_DATA_BROADCAST_ID:
			dvb_free_data_broadcast_id_desc((data_broadcast_id_desc_t *)desc->data);	
		break;

		case DESC_TAG_AC3:
			dvb_free_ac3_desc((ac3_desc_t *)desc->data);
		break;

		case DESC_TAG_SCRAMBLING:
			dvb_free_scrambling_desc((scrambling_desc_t *)desc->data);
		break;

		case DESC_TAG_PARTIAL_TRANSPORT_STREAM:
			dvb_free_partial_transport_stream_desc((paritial_transport_stream_desc_t *)desc->data);
		break;

		case DESC_TAG_DIGITAL_SAT_NEWS_GETHERING:
			dvb_free_digital_sat_news_gathering_desc((digital_sat_news_gathering_desc_t *)desc->data);
		break;

		case DESC_TAG_PDC:
			dvb_free_pdc_desc((pdc_desc_t *)desc->data);
		break;

		case DESC_TAG_ANCILLARY_DATA:
			dvb_free_ancillary_data_desc((ancillary_data_desc_t *)desc->data);
		break;

		case DESC_TAG_CELL_LIST:
			dvb_free_cell_list_desc((cell_list_desc_t *)desc->data);
		break;

		case DESC_TAG_CELL_FREQUENCY_LINK:
			dvb_free_cell_frequency_link_desc((cell_frequency_link_desc_t *)desc->data);
		break;

		case DESC_TAG_ANNOUNCEMENT_SUPPORT:
			dvb_free_announcement_support_desc((announcement_support_desc_t *)desc->data);
		break;

		case DESC_TAG_APPLICATION_SIGNALLING:
			dvb_free_application_signalling_desc((application_signalling_desc_t *)desc->data);
		break;

		case DESC_TAG_ADAPTATION_FIELD_DATA:
			dvb_free_adaptation_filed_data_desc((adaptation_field_data_desc_t *)desc->data);
		break;

		case DESC_TAG_SERVICE_IDENTIFIER:
		break;

		case DESC_TAG_SERVICE_AVAILABLITY:
			dvb_free_service_availability_desc((service_availability_desc_t *)desc->data);
		break;

		case DESC_TAG_DEFAULT_AUTHORITY:
		break;

		case DESC_TAG_RELATED_CONTENT:
		break;

		case DESC_TAG_TVA_ID:
		break;

		case DESC_TAG_CONTENT_IDENTIFIER:
		break;

		case DESC_TAG_TIME_SLICE_FEC_IDENTIFIER:
		break;

		case DESC_TAG_ECM_REPETITION_RATE:
		break;

		case DESC_TAG_S2_SAT_DELIVERY_SYSTEM:
		break;

		case DESC_TAG_ENHANCED_AC3:
			dvb_free_enhanced_ac3_desc((enhanced_ac3_desc_t *)desc->data);
		break;

		case DESC_TAG_DTS:
		break;

		case DESC_TAG_AAC:
			dvb_free_aac_desc((aac_desc_t *)desc->data);
		break;

		case DESC_TAG_XAIT_LOCATION:
		break;

		case DESC_TAG_FTA_CONTENT_MANAGEMENT:
			dvb_free_fta_content_management_desc((fta_content_management_desc_t *)desc->data);
		break;

		case DESC_TAG_EXTENSION:
		break;

		case DESC_TAG_DVBC_LOGICAL_CHANNEL_NUMBER:
			dvb_free_dvbc_lcn_desc((channel_desc_t *)desc->data);
		break;

		case DESC_TAG_DVB_LOGICAL_CHANNEL_NUMBER:
			dvb_free_lcn_desc((lcn_desc_t *)desc->data);	
		break;

#if 0
		// DTG_SIPSI
		case DESC_TAG_SERVICE_ATTRIBUTE:
			dvb_free_service_attribute_desc((service_attribute_desc_t *)desc->data);	
		break;
#endif

#if 1
		case DESC_TAG_SHORTNAME:
			dvb_free_short_name_desc((short_name_desc_t *)desc->data);	
		break;
#else
		case DESC_TAG_LOGICAL_CHANNEL_NUM_V2:
			dvb_free_lcn_v2_desc((lcn_v2_desc_t *)desc->data);	
		break;
#endif

		// DTG_SIPSI, HD_SIMULCAST_LCN
		case DESC_TAG_HD_SIMULCAST_LOGICAL_CHANNEL:
			dvb_free_hd_simulcast_lcn_desc((hd_simulcast_lcn_desc_t *)desc->data);	
		break;

		// BSKYB_CHANNEL_NUMBERING
		case DESC_TAG_LOGICAL_CHANNEL_NUMBER:
			dvb_free_lcn_desc((lcn_desc_t *)desc->data);	
		break;

#if 0
		/* ASTRA APS(ASTRA PLATFORM SERVICE) - SGT(SERVICE GUIDE TABLE) */
		// HD_PLUS_SCAN
		case DESC_TAG_SERVICE_LIST_NAME:
			dvb_free_service_list_name_desc((service_list_name_desc_t *)desc->data);
		break;
#endif

		case DESC_TAG_BOUQUET_LIST:
			dvb_free_bouquet_list_desc((bouquet_list_desc_t *)desc->data);
		break;

		case DESC_TAG_VIRTUAL_SERVICE_ID:
			dvb_free_virtual_service_id_desc((virtual_service_id_desc_t *)desc->data);
		break;

		// CANAL
		case DESC_TAG_CANAL_AUDIO_NAME:
			dvb_free_canal_audio_name_desc((canal_audio_name_desc_t *)desc->data);
		break;

		case DESC_TAG_CANAL_DATA:
		break;

#if 0
		// CIPLUS
		case DESC_TAG_CIPLUS_CONTENT_LABEL:
			dvb_free_ciplus_content_label_desc((ciplus_content_label_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CIPLUS_SERVICE:
			dvb_free_ciplus_service_desc((ciplus_service_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CIPLUS_PROTECTION:
			dvb_free_ci_protection_desc((ci_protection_desc_t *)desc->data);
		break;
#endif

		/* ATSC - PSIP(Program and System Information Protocol for Terrestrial and Cable */
		case DESC_TAG_ATSC_STUFFING:
		break;

		case DESC_TAG_ATSC_AC3:
			dvb_free_ac3_audio_stream_desc((ac3_audio_stream_desc_t *)desc->data);
		break;

		case DESC_TAG_ATSC_CAPTION_SERVICE:
			dvb_free_caption_service_desc((psip_caption_service_desc_t *)desc->data);
		break;

#if 0
		case DESC_TAG_ATSC_CONTENT_ADVISORY:
		break;

		case DESC_TAG_ATSC_EXTENDED_CHANNEL_NAME:
		break;

		case DESC_TAG_ATSC_SERVICE_LOCATION:
		break;

		case DESC_TAG_ATSC_TIMESHIFTED_SERVICE:
		break;

		case DESC_TAG_ATSC_COMPONENT_NAME:
		break;

		case DESC_TAG_ATSC_DCC_DEPARTING_REQUEST:
		break;

		case DESC_TAG_ATSC_DCC_ARRIVING_REQUEST:
		break;

		case DESC_TAG_ATSC_REDISTRIBUTION_CONTROL:
		break;

		case DESC_TAG_ATSC_GENRE:
		break;

		case DESC_TAG_ATSC_PRIVATE_INFORMATION:
		break;
#endif

		case DESC_TAG_ATSC_ENHANCED_AC3:
			dvb_free_enhanced_ac3_audio_stream_desc((enhanced_ac3_audio_stream_desc_t *)desc->data);
		break;	

		// ARIB_SUBTITLE
		case DESC_TAG_ARIB_SUBTITLE:
		break;

		default:
		break;
	}

	DTV_FREE(desc);

	return;
	
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_video_stream(
	unsigned char *p,
	video_stream_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 1)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;

	desc->multiple_frame_rate = (unsigned char)((temp & 0x80) >> 7);
	desc->frame_rate_code = (temp & 0x78) >> 3;
	desc->mpeg1_only = (unsigned char)((temp & 0x04) >> 2);
	desc->constrained_param = (unsigned char)((temp & 0x02) >> 1);
	desc->still_picture = (unsigned char)(temp & 0x01);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_audio_stream(
	unsigned char *p,
	audio_stream_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 1)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;

	desc->free_format = (unsigned char)((temp & 0x80) >> 7);
	desc->id = (unsigned char)((temp & 0x40) >> 6);
	desc->layer = (unsigned char)((temp & 0x30) >> 4);
	desc->variable_rate_audio_indicator = (unsigned char)((temp & 0x08) >> 3);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_hierarchy_desc(
	unsigned char *p,
	hierarchy_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	desc->hierachy_type = *p & 0x04;
	p += 1; desc_length -= 1;

	desc->hierachy_layer_index = *p & 0x3F;
	p += 1; desc_length -= 1;

	desc->hierachy_embedded_layer_index = *p & 0x3F;
	p += 1; desc_length -= 1;

	desc->hierachy_channel = *p & 0x3F;
	p += 1; desc_length -= 1;

	return DTV_OK;	
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_registration_desc(
	unsigned char *p,
	registration_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->priv_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 4)
	{
		return DTV_ERROR;
	}

	desc->format_id = get_32bit(p);
	p += 4; desc_length -= 4;

	if (desc_length > 0)
	{
		desc->priv_data = (private_data_t *)dvb_alloc_private_data(desc_length);
		
		if (desc->priv_data != NULL)
		{
			desc->priv_data->len = desc_length;
			DTV_MEMCPY(desc->priv_data->data, p, desc_length);
		}

		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_data_stream_align_desc(
	unsigned char *p,
	data_stream_align_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 1)
	{
		return DTV_ERROR;
	}

	desc->align_type = *p;
	p += 1; desc_length -= 1;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_target_bg_grid_desc(
	unsigned char *p,
	target_bg_grid_desc_t *desc
	)
{
	signed short desc_length;
	unsigned int temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	temp = get_32bit(p);
	p += 4; desc_length -= 4;

	desc->hor_size = (unsigned short)((temp & 0xFFFC0000) >> 18);
	desc->ver_size = (unsigned short)((temp & 0x0003FFF0) >> 4);
	desc->aspect_ratio_info = (unsigned char)(temp & 0x0000000F);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_video_window_desc(
	unsigned char *p,
	video_window_desc_t *desc
	)
{
	signed short desc_length;
	unsigned int temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	temp = get_32bit(p);
	p += 4; desc_length -= 4;

	desc->hor_offset = (unsigned short)((temp & 0xFFFC0000) >> 18);
	desc->ver_offset = (unsigned short)((temp & 0x0003FFF0) >> 4);
	desc->window_priority = (unsigned char)(temp & 0x0000000F);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ca_desc(
	unsigned char *p,
	ca_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	
	desc->priv_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 4)
	{
		return DTV_ERROR;
	}

	desc->ca_system_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->ca_pid = get_13bit(p);
	p += 2; desc_length -= 2;

	if (desc_length > 0)
	{
		desc->priv_data = (private_data_t *)dvb_alloc_private_data(desc_length);
		
		if (desc->priv_data != NULL)
		{
			desc->priv_data->len = desc_length;
			DTV_MEMCPY(desc->priv_data->data, p, desc_length);
		}

		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_iso_639_lang_desc(
	unsigned char *p,
	iso_639_lang_desc_t *desc
	)
{
	iso_639_lang_data_t *lang, *prev_lang = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	
	desc->lang_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 4) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		lang = (iso_639_lang_data_t *)DTV_MALLOC(sizeof(iso_639_lang_data_t));
		if (lang == NULL) return DTV_ERROR;
		
		lang->next = NULL;
		
		/* link */
		if (desc->lang_data == NULL)
		{
			desc->lang_data = lang;
		}
		else
		{
			prev_lang->next = lang;
		}
		prev_lang = lang;

		DTV_MEMCPY(lang->lang_code, p, 3);
		p += 3; desc_length -= 3;

		lang->audio_type = *p;
		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_system_clock_desc(
	unsigned char *p,
	system_clock_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 2)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;

	desc->external_clock_ref = (unsigned char)((temp & 0x80) >> 7);
	desc->clock_accuracy_int = (unsigned char)(temp & 0x3F);

	temp = *p;
	p += 1; desc_length -= 1;
	
	desc->clock_accuracy_exp = (unsigned char)((temp & 0xE0) >> 5);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_multi_buffer_util_desc(
	unsigned char *p,
	multi_buffer_util_desc_t *desc
	)
{
	signed short desc_length;
	unsigned short temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	temp = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->bound_valid = (unsigned char)((temp & 0x8000) >> 15);
	desc->ltw_offset_lower_bound = (unsigned short)(temp & 0x7FFFF);

	temp = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->ltw_offset_upper_bound = (unsigned short)(temp & 0x7FFFF);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_copyright_desc(
	unsigned char *p,
	copyright_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	
	desc->priv_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	desc->copyright_id = get_32bit(p);
	p += 4; desc_length -= 4;

	if (desc_length > 0)
	{
		desc->priv_data = (private_data_t *)dvb_alloc_private_data(desc_length);
		
		if (desc->priv_data != NULL)
		{
			desc->priv_data->len = desc_length;
			DTV_MEMCPY(desc->priv_data->data, p, desc_length);
		}

		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_max_bitrate_desc(
	unsigned char *p,
	max_bitrate_desc_t *desc
	)
{
	signed short desc_length;
	unsigned int temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 3)
	{
		return DTV_ERROR;
	}

	temp = get_24bit(p);
	p += 3; desc_length -= 3;

	desc->max_bitrate = (unsigned int)(temp & 0x3FFFFF);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_private_data_indicator_desc(
	unsigned char *p,
	private_data_indicator_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	desc->indicator = get_32bit(p);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_smoothing_buffer_desc(
	unsigned char *p,
	smoothing_buffer_desc_t *desc
	)
{
	signed short desc_length;
	unsigned int temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 6)
	{
		return DTV_ERROR;
	}

	temp = get_24bit(p);
	p += 3; desc_length -= 3;

	desc->sb_leak_rate = (unsigned int)(temp & 0x3FFFFF);
	
	temp = get_24bit(p);
	p += 3; desc_length -= 3;

	desc->sb_size = (unsigned int)(temp & 0x3FFFFF);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_avc_desc(
	unsigned char *p,
	avc_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->profile_idc = get_8bit(p);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->constraint_set0_flag = (unsigned char)((temp >> 7) & 0x1);
	desc->constraint_set1_flag = (unsigned char)((temp >> 6) & 0x1);
	desc->constraint_set2_flag = (unsigned char)((temp >> 5) & 0x1);
	desc->AVC_compatible_flags = (unsigned char)(temp& 0x1F);
	p += 1; desc_length -= 1;

	desc->level_idc = get_8bit(p);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->AVC_still_present = (unsigned char)((temp >> 7) & 0x1);
	desc->AVC_24_hour_picture_flag = (unsigned char)((temp >> 6) & 0x1);
	p += 1; desc_length -= 1;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_network_name_desc(
	unsigned char *p,
	network_name_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->name = (char *)DTV_MALLOC(desc_length+2);
	if (desc->name == NULL) return DTV_ERROR;

	DTV_MEMSET(desc->name, 0x0, desc_length + 2);
	DTV_MEMCPY(desc->name, p, desc_length);

	p += desc_length; desc_length -= desc_length;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_service_list_desc(
	unsigned char *p,
	service_list_desc_t *desc
	)
{
	service_list_data_t *svc, *prev_svc = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->service_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 3) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		svc = (service_list_data_t *)DTV_MALLOC(sizeof(service_list_data_t));
		if (svc == NULL) return DTV_ERROR;
		
		svc->next = NULL;

		/* link */
		if (desc->service_data == NULL)
		{
			desc->service_data = svc;
		}
		else
		{
			prev_svc->next = svc;
		}
		prev_svc = svc;

		svc->service_id = get_16bit(p);
		p += 2; desc_length -= 2;

		svc->service_type = *p;
		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_sat_delivery_system_desc(
	unsigned char *p,
	sat_delivery_system_desc_t *desc
	)
{
	signed short desc_length;
	unsigned int temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 11)
	{
		return DTV_ERROR;
	}

	/*
		satellite_delivery_system_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		frequency 				32 bslbf
		orbital_position 		16 bslbf
		west_east_flag 			1 bslbf // 0:west, 1:east
		polarization 			2 bslbf // 0:linear - horizontal, 1: linear - vertical, 2: Circular - left, 3:Circular - right
		If (modulation_system == "1") 
		{
			roll_off 				2 bslbf // 0:a=0,35 1:a=0,25 2:a=0,20
		} 
		else 
		{
			"00" 					2 bslbf
		}
		modulation_system 		1 bslbf // 0:DVB-S, 1:DVB-S2
		modulation_type 		2 bslbf // 0:AUTO, 1:QPSK, 2:8PSK,3:16QAM(n/a for DVB-S2)
		symbol_rate 			28 bslbf
		FEC_inner 				4 bslbf // 0:not defined, 1:1/2, 2:2/3, 3:3/4, 4:5/6, 5:7/8, 6:8/9, 7:3/5, 8:4/5, 9:9/10
		}
	*/	

	DTV_MEMCPY(desc->frequency, p, 4);
	p += 4; desc_length -= 4;

	DTV_MEMCPY(desc->orbit_pos, p, 2);
	p += 2; desc_length -= 2;

	temp = *p;
	p += 1; desc_length -= 1;

	// 0:west, 1:east
	desc->west_east = (unsigned char)((temp >> 7) & 0x1);
	// 0:linear - horizontal, 1: linear - vertical, 2: Circular - left, 3:Circular - right
	desc->polarization = (unsigned char)((temp >> 5) & 0x3);
	// 0:DVB-S, 1:DVB-S2
	desc->modulation = (unsigned char)((temp >> 2) & 0x1);
	if(desc->modulation == 0x1) // DVB-S2
	{
		// 0:a=0,35 1:a=0,25 2:a=0,20
		desc->rolloff = (unsigned char)((temp >> 3) & 0x3);
	}
	// 0:AUTO, 1:QPSK, 2:8PSK,3:16QAM(n/a for DVB-S2)
	desc->modulation_type = (unsigned char)((temp >> 0) & 0x3);

	temp = get_32bit(p);
	p += 4; desc_length -= 4;

	// 0:not defined, 1:1/2, 2:2/3, 3:3/4, 4:5/6, 5:7/8, 6:8/9, 7:3/5, 8:4/5, 9:9/10
	desc->fec_inner = (unsigned char)(temp & 0x0F);
	
	temp >>= 4;
	desc->symbol_rate[0] = (unsigned char)((temp>>24)&0xff);
	desc->symbol_rate[1] = (unsigned char)((temp>>16)&0xff);
	desc->symbol_rate[2] = (unsigned char)((temp>>8)&0xff);
	desc->symbol_rate[3] = (unsigned char)(temp&0xff);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_cable_delivery_system_desc(
	unsigned char *p,
	cable_delivery_system_desc_t *desc
	)
{
	signed short desc_length;
	unsigned int temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 11)
	{
		return DTV_ERROR;
	}

	/*
		cable_delivery_system_descriptor(){ 
			descriptor_tag		8 uimsbf
		 descriptor_length		8 uimsbf
		 frequency				32 bslbf
		 reserved_future_use	12 bslbf
		 FEC_outer				4 bslbf // 0:not defined, 1:no outer FEC coding, 2:RS(204/188)
		 modulation 			8 bslbf // 0:not defined, 1:16QAM, 2:32QAM, 3:64QAM, 4:128QAM, 5:256QAM
		 symbol_rate			28 bslbf
		 FEC_inner				4 bslbf // 0:not defined, 1:1/2, 2:2/3, 3:3/4, 4:5/6, 5:7/8, 6:8/9, 7:3/5, 8:4/5, 9:9/10
		}
	*/

	DTV_MEMCPY(desc->frequency, p, 4);
	p += 4; desc_length -= 4;

	/* reserved */
	p += 1; desc_length -= 1;

	temp = *p;
	p += 1; desc_length -= 1;

	// 0:not defined, 1:no outer FEC coding, 2:RS(204/188)
	desc->fec_outer = (unsigned char)(temp & 0x0F); 

	// 0:not defined, 1:16QAM, 2:32QAM, 3:64QAM, 4:128QAM, 5:256QAM
	desc->modulation = *p;
	p += 1; desc_length -= 1;

	temp = get_32bit(p);
	p += 4; desc_length -= 4;

	// 0:not defined, 1:1/2, 2:2/3, 3:3/4, 4:5/6, 5:7/8, 6:8/9, 7:3/5, 8:4/5, 9:9/10
	desc->fec_inner = (unsigned char)(temp & 0x0F);

	temp >>= 4;
	desc->symbol_rate[0] = (unsigned char)((temp>>24)&0xff);
	desc->symbol_rate[1] = (unsigned char)((temp>>16)&0xff);
	desc->symbol_rate[2] = (unsigned char)((temp>>8)&0xff);
	desc->symbol_rate[3] = (unsigned char)(temp&0xff);


	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_bouquet_name_desc(
	unsigned char *p,
	bouquet_name_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->name = (char *)DTV_MALLOC(desc_length + 2);
	if (desc->name == NULL) return DTV_ERROR;

	DTV_MEMSET(desc->name, 0x0, desc_length + 2);
	DTV_MEMCPY(desc->name, p, desc_length);

	p += desc_length; desc_length -= desc_length;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_service_desc(
	unsigned char *p,
	service_desc_t *desc
	)
{
	unsigned char name_length;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->provider_name = NULL;
	desc->service_name = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->service_type = *p;
	p += 1; desc_length -= 1;

	name_length = *p;
	p += 1; desc_length -= 1;

	if (desc_length >= name_length)
	{
		desc->provider_name = (char *)DTV_MALLOC(name_length + 2);
		if (desc->provider_name == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->provider_name, 0x0, name_length + 2);
		DTV_MEMCPY(desc->provider_name, p, name_length);
		desc->provider_name_len = name_length;

		p += name_length; desc_length -= name_length;
	}

	name_length = *p;
	p += 1; desc_length -= 1;

	if (desc_length >= name_length)
	{
		desc->service_name = (char *)DTV_MALLOC(name_length + 2);
		if (desc->service_name == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->service_name, 0x0, name_length + 2);
		DTV_MEMCPY(desc->service_name, p, name_length);
		desc->service_name_len = name_length;

		p += name_length; desc_length -= name_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_country_availability_desc(
	unsigned char *p,
	country_availability_desc_t *desc
	)
{
	country_code_data_t *coutry_data, *prev_code = NULL;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->code_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;

	desc->availability = (unsigned char)((temp & 0x80) >> 7);

	if ((desc_length % 3) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		coutry_data = (country_code_data_t *)DTV_MALLOC(sizeof(country_code_data_t));
		if (coutry_data == NULL) return DTV_ERROR;
		
		coutry_data->next = NULL;

		/* link */
		if (desc->code_data == NULL)
		{
			desc->code_data = coutry_data;
		}
		else
		{
			prev_code->next = coutry_data;
		}
		prev_code = coutry_data;

		DTV_MEMCPY(coutry_data->country_code, p, 3);
		
		p += 3; desc_length -= 3;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_linkage_desc(
	unsigned char *p,
	linkage_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->priv_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 7)
	{
		return DTV_ERROR;
	}

	desc->ts_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->on_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->service_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->linkage_type = *p;
	p += 1; desc_length -= 1;

	if (desc_length > 0)
	{
		desc->priv_data = (private_data_t *)dvb_alloc_private_data(desc_length);
		
		if (desc->priv_data != NULL)
		{
			desc->priv_data->len = desc_length;
			DTV_MEMCPY(desc->priv_data->data, p, desc_length);
		}
		
		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_nvod_ref_desc(
	unsigned char *p,
	nvod_ref_desc_t *desc
	)
{
	nvod_ref_data_t	*nvod, *prev_nvod = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->nvod_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 6) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		nvod = (nvod_ref_data_t *)DTV_MALLOC(sizeof(nvod_ref_data_t));
		if (nvod == NULL) return DTV_ERROR;
		
		nvod->next = NULL;

		/* link */
		if (desc->nvod_data == NULL)
		{
			desc->nvod_data = nvod;
		}
		else
		{
			prev_nvod->next = nvod;
		}
		prev_nvod = nvod;

		nvod->ts_id = get_16bit(p);
		p += 2; desc_length -= 2;
		
		nvod->on_id = get_16bit(p);
		p += 2; desc_length -= 2;

		nvod->service_id = get_16bit(p);
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_time_shifted_service_desc(
	unsigned char *p, 
	time_shifted_service_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 2)
	{
		return DTV_ERROR;
	}

	desc->ref_service_id = get_16bit(p);
	p += 2; desc_length -= 2;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_short_event_desc(
	unsigned char *p,
	short_event_desc_t *desc
	)
{
	unsigned char name_length;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name = NULL;
	desc->text = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 5)
	{
		return DTV_ERROR;
	}

	DTV_MEMCPY(desc->lang_code, p, 3);
	p += 3; desc_length -= 3;

	name_length = *p;
	p += 1; desc_length -= 1;

	if (desc_length >= name_length)
	{
		desc->name = (char *)DTV_MALLOC(name_length + 2);
		if (desc->name == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->name, 0x0, name_length + 2);

		if(p[0] < 0x20)
		{
			DTV_MEMCPY(desc->name, p + 1, name_length - 1);
		}
		else
		{
			DTV_MEMCPY(desc->name, p, name_length);
		}
			

		p += name_length; desc_length -= name_length;
	}

	if(p != NULL)
	{
		name_length = *p;
		if(name_length <= 0) return DTV_OK;
		p += 1; desc_length -= 1;

		if (desc_length >= name_length)
		{
			desc->text = (char *)DTV_MALLOC(name_length + 2);
			if (desc->text == NULL) return DTV_ERROR;

			DTV_MEMSET(desc->text, 0x0, name_length + 2);

			if(p[0] < 0x20)
			{
				DTV_MEMCPY(desc->text, p + 1, name_length - 1);
			}
			else
			{
				DTV_MEMCPY(desc->text, p, name_length);
			}

			p += name_length; desc_length -= name_length;
		}
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_extended_event_desc(
	unsigned char *p,
	extended_event_desc_t *desc
	)
{
	extended_event_data_t *ext_evt, *prev_ext_evt = NULL;
	signed short item_length;
	unsigned char items_length, text_length;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->ext_evt_data = NULL;
	desc->text = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 6)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;

	desc->desc_number = (temp & 0xF0) >> 4;
	desc->last_desc_number = (temp & 0x0F);

	DTV_MEMCPY(desc->lang_code, p, 3);
	p += 3; desc_length -= 3;

	items_length = (signed short)*p;
	p += 1; desc_length -= 1;

	while ((desc_length >= items_length) && (items_length > 0))
	{
		ext_evt = (extended_event_data_t *)DTV_MALLOC(sizeof(extended_event_data_t));
		if (ext_evt == NULL) return DTV_ERROR;
		
		ext_evt->description = NULL;
		ext_evt->name = NULL;
		ext_evt->next = NULL;

		/* link */
		if (desc->ext_evt_data == NULL)
		{
			desc->ext_evt_data = ext_evt;
		}
		else
		{
			prev_ext_evt->next = ext_evt;
		}
		prev_ext_evt = ext_evt;

		item_length = *p;
		p += 1; desc_length -= 1;
		items_length -= 1;

		ext_evt->description = (char *)DTV_MALLOC(item_length + 2);
		if (ext_evt->description == NULL) return DTV_ERROR;

		DTV_MEMSET(ext_evt->description, 0x0, item_length + 2);
		DTV_MEMCPY(ext_evt->description, p, item_length);

		p += item_length; desc_length -= item_length;
		items_length -= item_length;

		item_length = *p;
		p += 1; desc_length -= 1;
		items_length -= 1;

		ext_evt->name = (char *)DTV_MALLOC(item_length + 2);
		if (ext_evt->name == NULL) return DTV_ERROR;

		DTV_MEMSET(ext_evt->name, 0x0, item_length + 2);
		DTV_MEMCPY(ext_evt->name, p, item_length);

		p += item_length; desc_length -= item_length;
		items_length -= item_length;
	}

	if(p != NULL)
	{
		text_length = *p;
		if(text_length <= 0) return DTV_OK;
		p += 1; desc_length -= 1;
		if(p != NULL)
		{
			if (desc_length >= text_length)
			{
				desc->text = (char *)DTV_MALLOC(text_length + 2);
				if (desc->text == NULL) return DTV_ERROR;

				DTV_MEMSET(desc->text, 0x0, text_length + 2);

				if(p[0] < 0x20)
				{
					DTV_MEMCPY(desc->text, p + 1, text_length - 1);
				}
				else
				{
					DTV_MEMCPY(desc->text, p, text_length);
				}
				
				p += text_length; desc_length -= text_length;
			}
		}
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_time_shifted_event_desc(
	unsigned char *p,
	time_shifted_event_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	desc->ref_service_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->ref_event_id = get_16bit(p);
	p += 2; desc_length -= 2;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_component_desc(
	unsigned char *p,
	component_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->text = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 6)
	{
		return DTV_ERROR;
	}

	desc->stream_content = (unsigned char)(*p & 0x0F);
	p += 1; desc_length -= 1;

	desc->component_type = *p;
	p += 1; desc_length -= 1;

	desc->component_tag = *p;
	p += 1; desc_length -= 1;

	DTV_MEMCPY(desc->lang_code, p, 3);
	p += 3; desc_length -= 3;

	if (desc_length > 0)
	{
		desc->text = (char *)DTV_MALLOC(desc_length + 2);
		if (desc->text == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->text, 0x0, desc_length + 2);
		DTV_MEMCPY(desc->text, p, desc_length);

		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;	
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_mosaic_desc(
	unsigned char *p,
	mosaic_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;

	desc->entry_point = (temp & 0x80) >> 7;
	desc->nb_hor_elem_cell = (temp & 0x70) >> 4;
	desc->nb_ver_elem_cell = (temp & 0x07);

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_stream_identifier_desc(
	unsigned char *p,
	stream_identifier_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	desc->component_tag = *p;
	p += 1; desc_length -= 1;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ca_identifier_desc(
	unsigned char *p,
	ca_identifier_desc_t *desc
	)
{
	ca_identifier_data_t *ca_id, *prev_ca_id = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->ca_id_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 2) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		ca_id = (ca_identifier_data_t *)DTV_MALLOC(sizeof(ca_identifier_data_t));
		if (ca_id == NULL) return DTV_ERROR;
		
		ca_id->next = NULL;

		/* link */
		if (desc->ca_id_data == NULL)
		{
			desc->ca_id_data = ca_id;
		}
		else
		{
			prev_ca_id->next = ca_id;
		}
		prev_ca_id = ca_id;

		ca_id->ca_system_id = get_16bit(p);
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_content_desc(
	unsigned char *p,
	content_desc_t *desc
	)
{
	content_data_t *content, *prev_content = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->content_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 2) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		content = (content_data_t *)DTV_MALLOC(sizeof(content_data_t));
		if (content == NULL) return DTV_ERROR;
		
		content->next = NULL;

		/* link */
		if (desc->content_data == NULL)
		{
			desc->content_data = content;
		}
		else
		{
			prev_content->next = content;
		}
		prev_content = content;

		content->content_nibble = *p;
		p += 1; desc_length -= 1;

		content->user_nibble = *p;
		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_parental_rating_desc(
	unsigned char *p,
	parental_rating_desc_t *desc
	)
{
	parental_rating_data_t *rating, *prev_rating = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->rating_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 4) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		rating = (parental_rating_data_t *)DTV_MALLOC(sizeof(parental_rating_data_t));
		if (rating == NULL) return DTV_ERROR;
		
		rating->next = NULL;

		/* link */
		if (desc->rating_data == NULL)
		{
			desc->rating_data = rating;
		}
		else
		{
			prev_rating->next = rating;
		}
		prev_rating = rating;

		DTV_MEMCPY(rating->country_code, p, 3);
		p += 3; desc_length -= 3;

		rating->rating = *p;
		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_teletext_desc(
	unsigned char *p,
	teletext_desc_t *desc
	)
{
	teletext_data_t *txt, *prev_txt = NULL;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->txt_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 5) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		txt = (teletext_data_t *)DTV_MALLOC(sizeof(teletext_data_t));
		if (txt == NULL) return DTV_ERROR;
		
		txt->next = NULL;

		/* link */
		if (desc->txt_data == NULL)
		{
			desc->txt_data = txt;
		}
		else
		{
			prev_txt->next = txt;
		}
		prev_txt = txt;

		DTV_MEMCPY(txt->lang_code, p, 3);
		p += 3; desc_length -= 3;

		temp = *p;
		txt->teletext_type = (unsigned char)((temp >> 3) & 0x1F);
		txt->mag_number = (unsigned char)(temp & 0x07);
		p += 1; desc_length -= 1;

		txt->page_number = *p;
		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_telephone_desc(
	unsigned char *p,
	telephone_desc_t *desc
	)
{
	unsigned char country_prefix_length, international_area_code_length;
	unsigned char operator_code_length, national_area_code_length, core_number_length;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->country_prefix = NULL;
	desc->international_area_code = NULL;
	desc->operator_code = NULL;
	desc->national_area_code = NULL;
	desc->core_number = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 3)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;

	desc->foreign_avail = (unsigned char)((temp & 0x20) >> 5);
	desc->connection_type = (unsigned char)(temp & 0x1F);

	temp = *p;
	p += 1; desc_length -= 1;

	country_prefix_length = (unsigned char)((temp & 0x60) >> 5);
	international_area_code_length = (unsigned char)((temp & 0x1C) >> 2);
	operator_code_length = (unsigned char)(temp & 0x03);

	temp = *p;
	p += 1; desc_length -= 1;

	national_area_code_length = (unsigned char)((temp & 0x70) >> 4);
	core_number_length = (unsigned char)(temp & 0x0F);

	if (country_prefix_length > 0)
	{
		desc->country_prefix = (char *)DTV_MALLOC(country_prefix_length + 2);
		if (desc->country_prefix == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->country_prefix, 0x0, country_prefix_length + 2);
		DTV_MEMCPY(desc->country_prefix, p, country_prefix_length);
	}

	if (international_area_code_length > 0)
	{
		desc->international_area_code = (char *)DTV_MALLOC(international_area_code_length + 2);
		if (desc->international_area_code == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->international_area_code, 0x0, international_area_code_length + 2);
		DTV_MEMCPY(desc->international_area_code, p, international_area_code_length);
	}

	if (operator_code_length > 0)
	{
		desc->operator_code = (char *)DTV_MALLOC(operator_code_length + 2);
		if (desc->operator_code == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->operator_code, 0x0, operator_code_length + 2);
		DTV_MEMCPY(desc->operator_code, p, operator_code_length);
	}

	if (national_area_code_length > 0)
	{
		desc->national_area_code = (char *)DTV_MALLOC(national_area_code_length + 2);
		if (desc->national_area_code == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->national_area_code, 0x0, national_area_code_length + 2);
		DTV_MEMCPY(desc->national_area_code, p, national_area_code_length);
	}

	if (core_number_length > 0)
	{
		desc->core_number = (char *)DTV_MALLOC(core_number_length + 2);
		if (desc->core_number == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->core_number, 0x0, core_number_length + 2);
		DTV_MEMCPY(desc->core_number, p, core_number_length);
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_local_time_offset_desc(
	unsigned char *p,
	local_time_offset_desc_t *desc
	)
{
	local_time_offset_data_t *offset, *prev_offset = NULL;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->offset_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

//	if ((desc_length % 13) != 0)
	if (desc_length < 13)
	{
		return DTV_ERROR;
	}

	while (desc_length >= 13)
	{
		offset = (local_time_offset_data_t *)DTV_MALLOC(sizeof(local_time_offset_data_t));
		if (offset == NULL) return DTV_ERROR;
		
		offset->next = NULL;

		/* link */
		if (desc->offset_data == NULL)
		{
			desc->offset_data = offset;
		}
		else
		{
			prev_offset->next = offset;
		}
		prev_offset = offset;

		DTV_MEMCPY(offset->country_code, p, 3);
		p += 3; desc_length -= 3;

		temp = *p;
		p += 1; desc_length -= 1;
		
		offset->country_region_id = (unsigned char)((temp & 0xFC) >> 2);
		offset->local_time_offset_polarity = (unsigned char)(temp & 0x01);

		DTV_MEMCPY(offset->local_time_offset, p, 2);
		p += 2; desc_length -= 2;

		DTV_MEMCPY(offset->time_of_change, p, 5);
		p += 5; desc_length -= 5;

		DTV_MEMCPY(offset->next_time_offset, p, 2);
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_subtitling_desc(
	unsigned char *p,
	subtitling_desc_t *desc
	)
{
	subtitling_data_t *subt, *prev_subt = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->subt_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 8) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		subt = (subtitling_data_t *)DTV_MALLOC(sizeof(subtitling_data_t));
		if (subt == NULL) return DTV_ERROR;
		
		subt->next = NULL;

		/* link */
		if (desc->subt_data == NULL)
		{
			desc->subt_data = subt;
		}
		else
		{
			prev_subt->next = subt;
		}
		prev_subt = subt;

		DTV_MEMCPY(subt->lang_code, p, 3);
		p += 3; desc_length -= 3;

		subt->subtitling_type = *p;
		p += 1; desc_length -= 1;

		subt->composition_page_id = get_16bit(p);
		p += 2; desc_length -= 2;

		subt->ancillary_page_id = get_16bit(p);
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ter_delivery_system_desc(
	unsigned char *p,
	ter_delivery_system_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 11)
	{
		return DTV_ERROR;
	}

	/*
		terrestrial_delivery_system_descriptor(){ 
			descriptor_tag			8 uimsbf
		 descriptor_length			8 uimsbf
		 centre_frequency			32 uimsbf
		 bandwidth					3 bslbf // 0:8Mhz, 1:7Mhz, 2:6Mhz, 3:5Mhz
		 priority					1 bslbf // 0:LP(low priority), 1:(High priority)
		 Time_Slicing_indicator 	1 bslbf
		 MPE-FEC_indicator			1 bslbf
		 reserved_future_use		2 bslbf
		 constellation				2 bslbf // 0:QPSK, 1:16QAM, 2:64QAM
		 hierarchy_information		3 bslbf 
		 code_rate-HP_stream		3 bslbf // 0:1/2, 1:2/3, 2:3/4, 3:5/6, 4:7/8
		 code_rate-LP_stream		3 bslbf // 0:1/2, 1:2/3, 2:3/4, 3:5/6, 4:7/8
		 guard_interval 			2 bslbf // 0:1/32, 1:1/16, 2:1/8, 3:1/4
		 transmission_mode			2 bslbf // 0:2K, 1:8K, 2:4K
		 other_frequency_flag		1 bslbf
		 reserved_future_use		32 bslbf 
		 }
	*/

	DTV_MEMCPY(desc->center_frequency, p, 4);
	p += 4; desc_length -= 4;

	temp = *p;
	p += 1; desc_length -= 1;
	
	// 0:8Mhz, 1:7Mhz, 2:6Mhz, 3:5Mhz
	desc->bandwidth = (temp >> 5) & 0x07;
	// 0:LP(low priority), 1:(High priority)
	desc->priority = (temp >> 4) & 0x01;

	temp = *p;
	p += 1; desc_length -= 1;

	// 0:QPSK, 1:16QAM, 2:64QAM
	desc->constellation = (temp >> 6) & 0x03;
	desc->hierarchy = (temp >> 3) & 0x07;
	// 0:1/2, 1:2/3, 2:3/4, 3:5/6, 4:7/8
	desc->code_rate_hp = (temp >> 0) & 0x07;

	temp = *p;
	p += 1; desc_length -= 1;

	// 0:1/2, 1:2/3, 2:3/4, 3:5/6, 4:7/8
	desc->code_rate_lp = (temp >> 5) & 0x07;
	// 0:1/32, 1:1/16, 2:1/8, 3:1/4
	desc->guard_interval = (temp >> 3) & 0x03;
	// 0:2K, 1:8K, 2:4K
	desc->transmission_mode = (temp >> 1) & 0x03;
	desc->other_freq = (temp >> 0) & 0x01;

	p += 4; desc_length -= 4;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_multi_network_name_desc(
	unsigned char *p,
	multi_network_name_desc_t *desc
	)
{
	multi_network_name_data_t *name, *prev_name = NULL;
	unsigned char name_length;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	while (desc_length >= 4)
	{
		name = (multi_network_name_data_t *)DTV_MALLOC(sizeof(multi_network_name_data_t));
		if (name == NULL) return DTV_ERROR;
		
		name->name = NULL;
		name->next = NULL;

		/* link */
		if (desc->name_data == NULL)
		{
			desc->name_data = name;
		}
		else
		{
			prev_name->next = name;
		}
		prev_name = name;

		DTV_MEMCPY(name->lang_code, p, 3);
		p += 3; desc_length -= 3;

		name_length = *p;
		p += 1; desc_length -= 1;
		
		name->name = (char *)DTV_MALLOC(name_length + 2);
		if (name->name == NULL) return DTV_ERROR;

		DTV_MEMSET(name->name, 0x0, name_length + 2);
		DTV_MEMCPY(name->name, p, name_length);

		p += name_length; desc_length -= name_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_multi_bouquet_name_desc(
	unsigned char *p,
	multi_bouquet_name_desc_t *desc
	)
{
	multi_bouquet_name_data_t *name, *prev_name = NULL;
	unsigned char name_length;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	while (desc_length >= 4)
	{
		name = (multi_bouquet_name_data_t *)DTV_MALLOC(sizeof(multi_bouquet_name_data_t));
		if (name == NULL) return DTV_ERROR;
		
		name->name = NULL;
		name->next = NULL;

		/* link */
		if (desc->name_data == NULL)
		{
			desc->name_data = name;
		}
		else
		{
			prev_name->next = name;
		}
		prev_name = name;

		DTV_MEMCPY(name->lang_code, p, 3);
		p += 3; desc_length -= 3;

		name_length = *p;
		p += 1; desc_length -= 1;
		
		name->name = (char *)DTV_MALLOC(name_length + 2);
		if (name->name == NULL) return DTV_ERROR;

		DTV_MEMSET(name->name, 0x0, name_length + 2);
		DTV_MEMCPY(name->name, p, name_length);

		p += name_length; desc_length -= name_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_multi_service_name_desc(
	unsigned char *p,
	multi_service_name_desc_t *desc
	)
{
	multi_service_name_data_t *name, *prev_name = NULL;
	unsigned char name_length;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		name = (multi_service_name_data_t *)DTV_MALLOC(sizeof(multi_service_name_data_t));
		if (name == NULL) return DTV_ERROR;
		
		name->provider_name = NULL;
		name->service_name = NULL;
		name->next = NULL;

		/* link */
		if (desc->name_data == NULL)
		{
			desc->name_data = name;
		}
		else
		{
			prev_name->next = name;
		}
		prev_name = name;

		DTV_MEMCPY(name->lang_code, p, 3);
		p += 3; desc_length -= 3;

		name_length = *p;
		p += 1; desc_length -= 1;
		
		name->provider_name = (char *)DTV_MALLOC(name_length + 2);
		if (name->provider_name == NULL) return DTV_ERROR;

		DTV_MEMSET(name->provider_name, 0x0, name_length + 2);
		DTV_MEMCPY(name->provider_name, p, name_length);

		p += name_length; desc_length -= name_length;

		name_length = *p;
		p += 1; desc_length -= 1;
		
		name->service_name = (char *)DTV_MALLOC(name_length + 2);
		if (name->service_name == NULL) return DTV_ERROR;

		DTV_MEMSET(name->service_name, 0x0, name_length + 2);
		DTV_MEMCPY(name->service_name, p, name_length);
		
		p += name_length; desc_length -= name_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_multi_component_desc(
	unsigned char *p,
	multi_component_desc_t *desc
	)
{
	multi_component_data_t *comp, *prev_comp = NULL;
	unsigned char text_length;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->component_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	desc->component_tag = *p;
	p += 1; desc_length -= 1;

	while (desc_length > 0)
	{
		comp = (multi_component_data_t *)DTV_MALLOC(sizeof(multi_component_data_t));
		if (comp == NULL) return DTV_ERROR;
		
		comp->description = NULL;
		comp->next = NULL;

		/* link */
		if (desc->component_data == NULL)
		{
			desc->component_data = comp;
		}
		else
		{
			prev_comp->next = comp;
		}
		prev_comp = comp;

		DTV_MEMCPY(comp->lang_code, p, 3);
		p += 3; desc_length -= 3;

		text_length = *p;
		p += 1; desc_length -= 1;
		
		comp->description = (char *)DTV_MALLOC(text_length + 2);
		if (comp->description == NULL) return DTV_ERROR;

		DTV_MEMSET(comp->description, 0x0, text_length + 2);
		DTV_MEMCPY(comp->description, p, text_length);

		p += text_length; desc_length -= text_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_private_data_specifier_desc(
	unsigned char *p,
	private_data_specifier_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 4)
	{
		return DTV_ERROR;
	}

	desc->private_data_specifier = get_32bit(p);
	p += 4; desc_length -= 4;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_service_move_desc(
	unsigned char *p,
	service_move_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length != 6)
	{
		return DTV_ERROR;
	}

	desc->new_org_network_id = get_16bit(p);
	p += 2; desc_length -= 2;
	
	desc->new_ts_id = get_16bit(p);
	p += 2; desc_length -= 2;
	
	desc->new_service_id = get_16bit(p);
	p += 2; desc_length -= 2;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_short_smoothing_buffer_desc(
	unsigned char *p,
	short_smoothing_buffer_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;
	
	desc->sb_size = (temp & 0xC0) >> 6;
	desc->sb_leak_rate = (temp & 0x3F);

	p += desc_length; desc_length -= desc_length;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_frequency_list_desc(
	unsigned char *p,
	frequency_list_desc_t *desc
	)
{
	signed short  desc_length;
	unsigned char i;
	float scale;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->frequency = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 5)
	{
		return DTV_ERROR;
	}

	desc->coding_type = (*p & 0x03);
	p += 1; desc_length -= 1;

	switch(desc->coding_type)
	{
		case 1: scale = 10.0;  /* satellite (KHz) */
		break;
		 
		case 2: scale = 100.0; /* cable (Hz) */
		break;
		
		case 3: scale = 0.01;  /* terrestrial (KHz) */
		break;
		
		default: scale = 1.0;
		break;
	}

	desc->nfrequency = desc_length / 4;
	if ((desc->nfrequency == 0) || ((desc_length % 4) != 0))
	{
		return DTV_ERROR;
	}

	desc->frequency = (unsigned int *)DTV_MALLOC(desc->nfrequency * sizeof(unsigned int));
	if (desc->frequency == NULL) return DTV_ERROR;
	
	for (i = 0; i < desc->nfrequency; i++)
	{
		desc->frequency[i] = (unsigned int)(get_32bit(p) * scale);

		p += 4; desc_length -= 4;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_data_broadcast_desc(
	unsigned char *p,
	data_broadcast_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char selector_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->selector_byte = NULL;
	desc->text = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 8)
	{
		return DTV_ERROR;
	}

	desc->data_broadcast_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->component_tag = *p;
	p += 1; desc_length -= 1;

	selector_length = *p;
	p += 1; desc_length -= 1;

	if (selector_length > 0)
	{
		desc->selector_byte = dvb_alloc_private_data(selector_length);
		
		if (desc->selector_byte != NULL)
		{
			desc->selector_byte->len = selector_length;
			DTV_MEMCPY(desc->selector_byte->data, p, selector_length);
		}
		
		p += selector_length; desc_length -= selector_length;
	}

	DTV_MEMCPY(desc->lang_code, p, 3);
	p += 3; desc_length -= 3;

	if (desc_length > 0)
	{
		desc->text = (char *)DTV_MALLOC(desc_length + 2);
		if (desc->text == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->text, 0x0, desc_length + 2);
		DTV_MEMCPY(desc->text, p, desc_length);

		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_data_broadcast_id_desc(
	unsigned char *p,
	data_broadcast_id_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char selector_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->selector_byte = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 2)
	{
		return DTV_ERROR;
	}

	desc->data_broadcast_id = get_16bit(p);
	p += 2; desc_length -= 2;

	selector_length = desc_length;
	
	if (selector_length > 0)
	{
		desc->selector_byte = dvb_alloc_private_data(selector_length);
		
		if (desc->selector_byte != NULL)
		{
			desc->selector_byte->len = selector_length;
			DTV_MEMCPY(desc->selector_byte->data, p, selector_length);
		}		

		p += selector_length; desc_length -= selector_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ac3_desc(
	unsigned char *p,
	ac3_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->info = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;	

	if (temp & 0x80)
	{
		desc->ac3_type = *p;
		p += 1; desc_length -= 1;
	}
	
	if (temp & 0x40)
	{
		desc->bsid = *p;
		p += 1; desc_length -= 1;
	}

	if (temp & 0x20)
	{
		desc->mainid = *p;
		p += 1; desc_length -= 1;
	}

	if (temp & 0x10)
	{
		desc->asvc = *p;
		p += 1; desc_length -= 1;
	}

	if (desc_length > 0)
	{
		desc->info = (unsigned char *)DTV_MALLOC(desc_length);
		if (desc->info == NULL) return DTV_ERROR;
		
		desc->info_len = desc_length;
		DTV_MEMCPY(desc->info, p, desc_length);
		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_enhanced_ac3_desc(
	unsigned char *p,
	enhanced_ac3_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->info = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	temp = *p;
	p += 1; desc_length -= 1;	

	if (temp & 0x80)
	{
		desc->component_type = *p;
		p += 1; desc_length -= 1;
	}
	
	if (temp & 0x40)
	{
		desc->bsid = *p;
		p += 1; desc_length -= 1;
	}

	if (temp & 0x20)
	{
		desc->mainid = *p;
		p += 1; desc_length -= 1;
	}

	if (temp & 0x10)
	{
		desc->asvc = *p;
		p += 1; desc_length -= 1;
	}

	if (temp & 0x04)
	{
		desc->substream1 = *p;
		p += 1; desc_length -= 1;
	}

	if (temp & 0x02)
	{
		desc->substream2 = *p;
		p += 1; desc_length -= 1;
	}

	if (temp & 0x01)
	{
		desc->substream3 = *p;
		p += 1; desc_length -= 1;
	}

	if (desc_length > 0)
	{
		desc->info = (unsigned char *)DTV_MALLOC(desc_length);
		if (desc->info == NULL) return DTV_ERROR;
		
		desc->info_len = desc_length;
		DTV_MEMCPY(desc->info, p, desc_length);
		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_aac_desc(
	unsigned char *p,
	aac_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->info = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	desc->profile_and_level = *p;
	p += 1; desc_length -= 1;	

	temp = *p;
	p += 1; desc_length -= 1;	

	if (temp & 0x80)
	{
		desc->aac_type = *p;
		p += 1; desc_length -= 1;
	}
	
	if (desc_length > 0)
	{
		desc->info = (unsigned char *)DTV_MALLOC(desc_length + 1);
		if (desc->info == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->info, 0x0, desc_length + 1);
		
		desc->info_len = desc_length;
		DTV_MEMCPY(desc->info, p, desc_length);
		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_fta_content_management_desc(
	unsigned char *p,
	fta_content_management_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	temp = *p;
	desc->do_not_scramble = (unsigned char)((temp & 0x08) >> 3);
	desc->control_remote_access_over_internet = (unsigned char)((temp & 0x06) >> 1);
	desc->do_not_apply_revocation = (unsigned char)(temp & 0x1);
	p += 1; desc_length -= 1;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_dvbc_lcn_desc(
	unsigned char *p,
	channel_desc_t *desc
	)
{
	channel_data_t *channel_data, *prev_lcn = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->channel_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 4) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		channel_data = (channel_data_t *)DTV_MALLOC(sizeof(channel_data_t));
		if (channel_data == NULL) return DTV_ERROR;
		
		channel_data->next = NULL;

		/* link */
		if (desc->channel_data == NULL)
		{
			desc->channel_data = channel_data;
		}
		else
		{
			prev_lcn->next = channel_data;
		}
		prev_lcn = channel_data;

		channel_data->service_id = get_16bit(p);
		p += 2; desc_length -= 2;

		channel_data->channel_number = get_16bit(p);
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_lcn_desc(
	unsigned char *p,
	lcn_desc_t *desc
	)
{
	lcn_data_t *lcn, *prev_lcn = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->lcn_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 4) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		lcn = (lcn_data_t *)DTV_MALLOC(sizeof(lcn_data_t));
		if (lcn == NULL) return DTV_ERROR;
		
		lcn->next = NULL;

		/* link */
		if (desc->lcn_data == NULL)
		{
			desc->lcn_data = lcn;
		}
		else
		{
			prev_lcn->next = lcn;
		}
		prev_lcn = lcn;

		lcn->service_id = get_16bit(p);
		p += 2; desc_length -= 2;

		lcn->visiable_flag = (unsigned char)((*p & 0x80) >> 7);

	#if 0 // NORDIG /* or SKYLINK - FNT */
		lcn->lcn = get_14bit(p);
	#else
		lcn->lcn = get_10bit(p);
	#endif
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_service_attribute_desc(
	unsigned char *p,
	service_attribute_desc_t *desc
	)
{
	service_attribute_data_t *svc_attr, *prev_svc_attr = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->svc_attr_data= NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 3) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		svc_attr = (service_attribute_data_t *)DTV_MALLOC(sizeof(service_attribute_data_t));
		if (svc_attr == NULL) return DTV_ERROR;
		
		svc_attr->next = NULL;

		/* link */
		if (desc->svc_attr_data == NULL)
		{
			desc->svc_attr_data = svc_attr;
		}
		else
		{
			prev_svc_attr->next = svc_attr;
		}
		prev_svc_attr = svc_attr;

		svc_attr->service_id = get_16bit(p);
		p += 2; desc_length -= 2;

		svc_attr->numeric_flag = (unsigned char)((*p & 0x2) >> 1);
		svc_attr->visiable_flag = (unsigned char)(*p & 0x1);

		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_short_name_desc(
	unsigned char *p,
	short_name_desc_t *desc
	)
{
	signed short desc_length;
	signed short len;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length > 256)
	{
		return DTV_ERROR;
	}

	len = *p;
	p += 1; desc_length -= 1;

	desc->name = (char *)DTV_MALLOC(len + 1);
	if (desc->name == NULL) return DTV_ERROR;

	DTV_MEMSET(desc->name, 0x0, len + 1);
	DTV_MEMCPY(desc->name, p, len);
	desc->len = len;
	p += len; desc_length -= len;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_lcn_v2_desc(
	unsigned char *p,
	lcn_v2_desc_t *desc
	)
{
	lcn_v2_list_data_t *list;
	lcn_v2_data_t *lcn, *prev_lcn = NULL;
	signed short desc_length;
	unsigned char channel_list_name_len;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->lcn_list_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 4) != 0)
	{
		return DTV_ERROR;
	}

	list = (lcn_v2_list_data_t *)DTV_MALLOC(sizeof(lcn_v2_list_data_t));
	if (list == NULL) return DTV_ERROR;

	list->channel_list_id = get_8bit(p);
	p += 1; desc_length -= 1;

	channel_list_name_len = get_8bit(p);
	p += 1; desc_length -= 1;

	list->channel_list_name = (char *)DTV_MALLOC(channel_list_name_len + 1);
	if (list->channel_list_name == NULL) return DTV_ERROR;

	DTV_MEMSET(list->channel_list_name, 0x0, channel_list_name_len + 1);
	DTV_MEMCPY(list->channel_list_name, p, channel_list_name_len);
	p += channel_list_name_len; desc_length -= channel_list_name_len;
		
	DTV_MEMSET(list->country_code, 0x0, 3);
	DTV_MEMCPY(list->country_code, p, 3);
	p += 3; desc_length -= 3;

	while (desc_length > 0)
	{
		lcn = (lcn_v2_data_t *)DTV_MALLOC(sizeof(lcn_v2_data_t));
		if (lcn == NULL) return DTV_ERROR;
		
		lcn->next = NULL;

		/* link */
		if (desc->lcn_list_data->lcn_data == NULL)
		{
			desc->lcn_list_data->lcn_data = lcn;
		}
		else
		{
			prev_lcn->next = lcn;
		}
		prev_lcn = lcn;

		lcn->service_id = get_16bit(p);
		p += 2; desc_length -= 2;

	#if 0 // NORDIG /* or SKYLINK - FNT */
		lcn->lcn = get_14bit(p);
		lcn->visiable_flag = (lcn->lcn & 0x8000) ? 1 : 0;
	#else
		lcn->lcn = get_10bit(p);
		lcn->visiable_flag = 1;
	#endif

		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_hd_simulcast_lcn_desc(
	unsigned char *p,
	hd_simulcast_lcn_desc_t *desc
	)
{
	hd_simulcast_lcn_data_t *lcn, *prev_lcn = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->lcn_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 4) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		lcn = (hd_simulcast_lcn_data_t *)DTV_MALLOC(sizeof(hd_simulcast_lcn_data_t));
		if (lcn == NULL) return DTV_ERROR;
		
		lcn->next = NULL;

		/* link */
		if (desc->lcn_data == NULL)
		{
			desc->lcn_data = lcn;
		}
		else
		{
			prev_lcn->next = lcn;
		}
		prev_lcn = lcn;

		lcn->service_id = get_16bit(p);
		p += 2; desc_length -= 2;

		lcn->visiable_flag = (unsigned char)((*p & 0x80) >> 7);
		lcn->lcn = get_10bit(p);

		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_scrambling_desc(
	unsigned char *p,
	scrambling_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	desc->scrambling_mode = *p;
	p += 1; desc_length -= 1;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_partial_transport_stream_desc(
	unsigned char *p,
	paritial_transport_stream_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 4)
	{
		return DTV_ERROR;
	}

	desc->peak_rate = get_22bit(p);
	p += 3; desc_length -= 3;

	desc->minimum_overall_smoothing_rate = get_22bit(p);
	p += 3; desc_length -= 3;

	desc->maximum_overall_smoothing_buffer = get_16bit(p);
	p += 2; desc_length -= 2;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_digital_sat_news_gathering_desc(
	unsigned char *p,
	digital_sat_news_gathering_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->dsng = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	/* 0x43, 0x4F, 0x4E, 0x41 - "CONA" */

	desc->dsng = (char *)DTV_MALLOC(desc_length + 2);
	if (desc->dsng == NULL) return DTV_ERROR;

	DTV_MEMSET(desc->dsng, 0x0, desc_length + 2);
	DTV_MEMCPY(desc->dsng, p, desc_length);

	p += desc_length; desc_length -= desc_length;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_pdc_desc(
	unsigned char *p,
	pdc_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 4)
	{
		return DTV_ERROR;
	}

	desc->programme_identification_label = get_20bit(p);
	p += 3; desc_length -= 3;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ancillary_data_desc(
	unsigned char *p,
	ancillary_data_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	desc->ancillary_data_identifier = get_8bit(p);
	p += 1; desc_length -= 1;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_cell_list_desc(
	unsigned char *p,
	cell_list_desc_t *desc
	)
{
	cell_list_t *list, *prev_list = NULL;
	subcell_list_t *sublist, *prev_sublist = NULL;
	signed short desc_length, subcell_info_loop_length;
	unsigned int temp;

	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->list = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 10)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		list = (cell_list_t *)DTV_MALLOC(sizeof(cell_list_t));
		if (list == NULL) return DTV_ERROR;
		
		list->sublist = NULL;
		list->next = NULL;

		/* link */
		if (desc->list == NULL)
		{
			desc->list = list;
		}
		else
		{
			prev_list->next = list;
		}
		prev_list = list;

		list->cell_id = get_16bit(p);
		p += 2; desc_length -= 2;

		list->cell_latitude = get_16bit(p);
		p += 2; desc_length -= 2;

		list->cell_longitude = get_16bit(p);
		p += 2; desc_length -= 2;

		temp = get_24bit(p);
		p += 3; desc_length -= 3;

		list->cell_extent_of_latitude = (unsigned short)((temp >> 12) & 0x0FFF);
		list->cell_extent_of_longitude = (unsigned short)(temp & 0x0FFF);

		subcell_info_loop_length = get_8bit(p);
		p += 1; desc_length -= 1;

		while((p != NULL) && (desc_length >= subcell_info_loop_length) && (subcell_info_loop_length > 0))
		{
			sublist = (subcell_list_t *)DTV_MALLOC(sizeof(subcell_list_t));
			if (sublist == NULL) return DTV_ERROR;
			
			sublist->next = NULL;
			
			/* link */
			if (list->sublist == NULL)
			{
				list->sublist = sublist;
			}
			else
			{
				prev_sublist->next = sublist;
			}
			prev_sublist = sublist;
			
			sublist->cell_id_extension = get_8bit(p);
			p += 1; desc_length -= 1; 
			subcell_info_loop_length -= 1;
			
			sublist->subcell_latitude = get_16bit(p);
			p += 2; desc_length -= 2; 
			subcell_info_loop_length -= 2;

			sublist->subcell_longitude = get_16bit(p);
			p += 2; desc_length -= 2; 
			subcell_info_loop_length -= 2;
			
			temp = get_24bit(p);
			p += 3; desc_length -= 3; 
			subcell_info_loop_length -= 3;
			
			sublist->subcell_extent_of_latitude = (unsigned short)((temp >> 12) & 0x0FFF);
			sublist->subcell_extent_of_longitude = (unsigned short)(temp & 0x0FFF);
		}
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_cell_frequency_link_desc(
	unsigned char *p,
	cell_frequency_link_desc_t *desc
	)
{
	cell_frequency_link_t *link, *prev_link = NULL;
	subcell_frequency_link_t *sublink, *prev_sublink = NULL;
	signed short desc_length, subcell_info_loop_length;

	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->frequency_link = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 6)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		link = (cell_frequency_link_t *)DTV_MALLOC(sizeof(cell_frequency_link_t));
		if (link == NULL) return DTV_ERROR;
		
		link->subfrequency_link = NULL;
		link->next = NULL;

		/* link */
		if (desc->frequency_link == NULL)
		{
			desc->frequency_link = link;
		}
		else
		{
			prev_link->next = link;
		}
		prev_link = link;

		link->cell_id = get_16bit(p);
		p += 2; desc_length -= 2;

		link->frequency = get_32bit(p);
		p += 4; desc_length -= 4;

		subcell_info_loop_length = get_8bit(p);
		p += 1; desc_length -= 1;

		while((desc_length >= subcell_info_loop_length) && (subcell_info_loop_length > 0))
		{
			sublink = (subcell_frequency_link_t *)DTV_MALLOC(sizeof(subcell_frequency_link_t));
			if (sublink == NULL) return DTV_ERROR;
			
			sublink->next = NULL;
			
			/* link */
			if (link->subfrequency_link == NULL)
			{
				link->subfrequency_link = sublink;
			}
			else
			{
				prev_sublink->next = sublink;
			}
			prev_sublink = sublink;
			
			sublink->cell_id_extension = get_8bit(p);
			p += 1; desc_length -= 1; 
			subcell_info_loop_length -= 1;
			
			sublink->transposer_frequency = get_32bit(p);
			p += 4; desc_length -= 4; 
			subcell_info_loop_length -= 4;
		}
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_announcement_support_desc(
	unsigned char *p,
	announcement_support_desc_t *desc
	)
{
	announcement_support_t *announcement_support, *prev_announcement_support = NULL;
	signed short desc_length;
	unsigned char temp;

	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->announcement_support = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 10)
	{
		return DTV_ERROR;
	}

	desc->announcement_support_indicator = get_16bit(p);
	p += 2; desc_length -= 2;
	
	while (desc_length > 0)
	{
		announcement_support = (announcement_support_t *)DTV_MALLOC(sizeof(announcement_support_t));
		if (announcement_support == NULL) return DTV_ERROR;
		
		announcement_support->next = NULL;

		/* link */
		if (desc->announcement_support == NULL)
		{
			desc->announcement_support = announcement_support;
		}
		else
		{
			prev_announcement_support->next = announcement_support;
		}
		prev_announcement_support = announcement_support;

		temp = get_8bit(p);
		p += 1; desc_length -= 1;
		
		announcement_support->announcement_type = (temp & 0xF0) >> 4;
		announcement_support->reference_type = (temp & 0x07);

		if(announcement_support->reference_type == 0x01 ||
			announcement_support->reference_type == 0x02 ||
			announcement_support->reference_type == 0x03)
		{
			announcement_support->original_network_id = get_16bit(p);
			p += 2; desc_length -= 2;

			announcement_support->transport_stream_id = get_16bit(p);
			p += 2; desc_length -= 2;

			announcement_support->component_tag = get_8bit(p);
			p += 1; desc_length -= 1;
		}
		else
		{
			// TODO???
//			p += 2; desc_length -= 2;
//			p += 2; desc_length -= 2;
//			p += 1; desc_length -= 1;
		}
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_adaptation_field_data_desc(
	unsigned char *p,
	adaptation_field_data_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 1)
	{
		return DTV_ERROR;
	}

	desc->adaptation_field_data_identifier = get_8bit(p);
	p += 1; desc_length -= 1;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_application_signalling_desc(
	unsigned char *p,
	application_signalling_desc_t *desc
	)
{
	application_signalling_data_t *application_signalling, *prev_application_signalling = NULL;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if ((desc_length % 3) != 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		application_signalling = (application_signalling_data_t *)DTV_MALLOC(sizeof(application_signalling_data_t));
		if (application_signalling == NULL) return DTV_ERROR;
		
		application_signalling->next = NULL;

		/* link */
		if (desc->application_signalling_data == NULL)
		{
			desc->application_signalling_data = application_signalling;
		}
		else
		{
			prev_application_signalling->next = application_signalling;
		}
		prev_application_signalling = application_signalling;

		application_signalling->application_type = get_15bit(p);
		p += 2; desc_length -= 2;
		
		temp = *p;
		application_signalling->AIT_version_number = (unsigned char)(temp & 0x1F);
		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_service_availability_desc(
	unsigned char *p,
	service_availability_desc_t *desc
	)
{
	service_availability_t *service_availability, *prev_service_availability = NULL;
	signed short desc_length;
	unsigned char temp;

	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->service_availability = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 3)
	{
		return DTV_ERROR;
	}

	temp = get_8bit(p);
	p += 1; desc_length -= 1;
	
	desc->availability_flag = (unsigned char)((temp & 0x80) >> 7);

	while (desc_length > 0)
	{
		service_availability = (service_availability_t *)DTV_MALLOC(sizeof(service_availability_t));
		if (service_availability == NULL) return DTV_ERROR;
		
		service_availability->next = NULL;

		/* link */
		if (desc->service_availability == NULL)
		{
			desc->service_availability = service_availability;
		}
		else
		{
			prev_service_availability->next = service_availability;
		}
		prev_service_availability = service_availability;

		service_availability->cell_id = get_16bit(p);
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_service_list_name_desc(
	unsigned char *p,
	service_list_name_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->name = (char *)DTV_MALLOC(desc_length + 2);
	if (desc->name == NULL) return DTV_ERROR;

	DTV_MEMCPY(desc->lang_code, p, 3);
	p += 3; desc_length -= 3;

	DTV_MEMSET(desc->name, 0x0, desc_length + 2);
	DTV_MEMCPY(desc->name, p, desc_length);

	p += desc_length; desc_length -= desc_length;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_bouquet_list_desc(
	unsigned char *p,
	bouquet_list_desc_t *desc
	)
{
	bouq_list_t *list, *prev_list = NULL;
	signed short desc_length, bouquet_name_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->list = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	while (desc_length > 0)
	{
		list = (bouq_list_t *)DTV_MALLOC(sizeof(bouq_list_t));
		if (list == NULL) return DTV_ERROR;
		
		list->next = NULL;

		/* link */
		if (desc->list == NULL)
		{
			desc->list = list;
		}
		else
		{
			prev_list->next = list;
		}
		prev_list = list;

		bouquet_name_length = get_8bit(p);
		p += 1; desc_length -= 1;

		list->name = (char *)DTV_MALLOC(bouquet_name_length + 2);
		if (list->name == NULL) return DTV_ERROR;

		DTV_MEMSET(list->name, 0x0, bouquet_name_length + 2);
		DTV_MEMCPY(list->name, p, bouquet_name_length);
		
		p += desc_length; desc_length -= bouquet_name_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_virtual_service_id_desc(
	unsigned char *p,
	virtual_service_id_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->virtual_service_reserved = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->virtual_service_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->virtual_service_reserved = (char *)DTV_MALLOC(desc_length + 2);
	if (desc->virtual_service_reserved == NULL) return DTV_ERROR;
	
	DTV_MEMSET(desc->virtual_service_reserved, 0x0, desc_length + 2);
	DTV_MEMCPY(desc->virtual_service_reserved, p, desc_length);

	p += desc_length; desc_length -= desc_length;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_canal_audio_name_desc(
	unsigned char *p,
	canal_audio_name_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char name_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->audio_name = NULL;
	
	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	name_length = *p;
	if (name_length == 0)
	{
		return DTV_ERROR;
	}
	p += 1; desc_length -= 1;

#if 1
	desc->audio_name = (char *)DTV_MALLOC(ISO_LANGUAGE_CODE + 1);
	if (desc->audio_name == NULL) return DTV_ERROR;
	
	DTV_MEMSET(desc->audio_name, 0x0, ISO_LANGUAGE_CODE + 1);
	DTV_MEMCPY(desc->audio_name, p, ISO_LANGUAGE_CODE);
	desc->audio_name_len = ISO_LANGUAGE_CODE;
	
	p += ISO_LANGUAGE_CODE; desc_length -= ISO_LANGUAGE_CODE;
#else
	if (desc_length >= name_length)
	{
		desc->audio_name = (char *)DTV_MALLOC(name_length + 2);
		if (desc->audio_name == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->audio_name, 0x0, name_length + 2);
		DTV_MEMCPY(desc->audio_name, p, name_length);
		desc->audio_name_len = name_length;

		p += name_length; desc_length -= name_length;
	}
#endif

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ciplus_content_lable_desc(
	unsigned char *p,
	ciplus_content_label_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->content_byte_min = *p;
	p += 1; desc_length -= 1;

	desc->content_byte_max = *p;
	p += 1; desc_length -= 1;

	DTV_MEMCPY(desc->lang_code, p, 3);
	p += 3; desc_length -= 3;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ciplus_service_desc(
	unsigned char *p,
	ciplus_service_desc_t *desc
	)
{
	unsigned char name_length;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->provider_name = NULL;
	desc->service_name = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->service_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->service_type = *p;
	p += 1; desc_length -= 1;

	temp = *p;
	desc->visiable_flag = (unsigned char)((temp & 0x80) >> 7);
	desc->selectable = (unsigned char)((temp & 0x40) >> 6);
	desc->lcn = get_14bit(p);
	p += 2; desc_length -= 2;

	name_length = *p;
	p += 1; desc_length -= 1;

	if (desc_length >= name_length)
	{
		desc->provider_name = (char *)DTV_MALLOC(name_length + 2);
		if (desc->provider_name == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->provider_name, 0x0, name_length + 2);
		DTV_MEMCPY(desc->provider_name, p, name_length);
		desc->provider_name_len = name_length;

		p += name_length; desc_length -= name_length;
	}

	name_length = *p;
	p += 1; desc_length -= 1;

	if (desc_length >= name_length)
	{
		desc->service_name = (char *)DTV_MALLOC(name_length + 2);
		if (desc->service_name == NULL) return DTV_ERROR;

		DTV_MEMSET(desc->service_name, 0x0, name_length + 2);
		DTV_MEMCPY(desc->service_name, p, name_length);
		desc->service_name_len = name_length;

		p += name_length; desc_length -= name_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ci_protection_desc(
	unsigned char *p,
	ci_protection_desc_t *desc
	)
{
	ci_protection_brand_t *brand, *prev_brand = NULL;
	signed short desc_length;
	unsigned char temp;
	unsigned char number_of_entries;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->brand = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length < 2) // 6
	{
		return DTV_ERROR;
	}

	temp = *p;
	desc->free_ci_mode_flag = (unsigned char)((temp & 0x80) >> 7);
	desc->match_brand_flag = (unsigned char)((temp & 0x40) >> 6);
	p += 1; desc_length -= 1;

	desc->number_of_entries = 0;
	
	if(desc->match_brand_flag == 1)
	{
		desc->number_of_entries = *p;
		p += 1; desc_length -= 1;

		number_of_entries = 0;
		
		while ((desc_length > 0) && 
				(desc->number_of_entries > number_of_entries))
		{
			brand = (ci_protection_brand_t *)DTV_MALLOC(sizeof(ci_protection_brand_t));
			if (brand == NULL) return DTV_ERROR;
			
			brand->next = NULL;

			/* link */
			if (desc->brand == NULL)
			{
				desc->brand = brand;
			}
			else
			{
				prev_brand->next = brand;
			}
			prev_brand = brand;

			brand->id = get_16bit(p);
			p += 2; desc_length -= 2;
			number_of_entries++;
		}
	}

	if (desc_length > 0)
	{
		desc->priv_data = (private_data_t *)dvb_alloc_private_data(desc_length);
		
		if (desc->priv_data != NULL)
		{
			desc->priv_data->len = desc_length;
			DTV_MEMCPY(desc->priv_data->data, p, desc_length);
		}
		
		p += desc_length; desc_length -= desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_ac3_audio_stream_desc(
	unsigned char *p,
	ac3_audio_stream_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	temp = get_8bit(p);
	desc->sample_rate_code = (unsigned char)((temp >> 5) & 0x7);
	desc->bsid = (unsigned char)(temp & 0x1F);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->bit_rate_code = (unsigned char)((temp >> 2) & 0x3F);
	desc->surround_mode = (unsigned char)(temp & 0x3);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->bsmod = (unsigned char)((temp >> 5) & 0x7);
	desc->num_channels = (unsigned char)((temp >> 3) & 0x3);
	desc->full_svc = (unsigned char)(temp & 0x7);
	p += 1; desc_length -= 1;

	desc->langcod = get_8bit(p);
	p += 1; desc_length -= 1;
	
	temp = get_8bit(p);
	desc->mainid = (unsigned char)((temp >> 5) & 0x7);
	desc->priority = (unsigned char)((temp >> 3) & 0x3);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->language_flag = (unsigned char)((temp >> 7) & 0x1);
	desc->language_flag_2 = (unsigned char)((temp >> 6) & 0x1);
	p += 1; desc_length -= 1;

	DTV_MEMCPY(desc->language, p, ISO_LANGUAGE_CODE);
	p += ISO_LANGUAGE_CODE; desc_length -= ISO_LANGUAGE_CODE;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_caption_service_desc(
	unsigned char *p,
	psip_caption_service_desc_t *desc
	)
{
	psip_caption_service_t *caption, *prev_caption = NULL;
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->caption_service = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	temp = get_8bit(p);
	desc->number_of_services = (unsigned char)(temp & 0x1F);
	p += 1; desc_length -= 1;

	while (desc_length > 0)
	{
		caption = (psip_caption_service_t *)DTV_MALLOC(sizeof(psip_caption_service_t));
		if (caption == NULL) return DTV_ERROR;
		
		caption->next = NULL;

		/* link */
		if (desc->caption_service== NULL)
		{
			desc->caption_service = caption;
		}
		else
		{
			prev_caption->next = caption;
		}
		prev_caption = caption;

		DTV_MEMSET(caption->language, 0x0, ISO_LANGUAGE_CODE);
		DTV_MEMCPY(caption->language, p, ISO_LANGUAGE_CODE);
		p += ISO_LANGUAGE_CODE; desc_length -= ISO_LANGUAGE_CODE;
		
		temp = get_8bit(p);
		p += 1; desc_length -= 1;
		caption->digital_cc = (unsigned char)((temp >> 7) & 0x1);
		if(caption->digital_cc == 0)
		{
			caption->line21_field = (unsigned char)(temp & 0x1);
		}
		else
		{
			caption->line21_field = 0;
			caption->caption_service_number = (unsigned char)(temp & 0x3F);
		}

		caption->easy_reader = (unsigned char)((temp >> 7) & 0x1);
		caption->wide_aspect_ratio = (unsigned char)((temp >> 6) & 0x1);
		p += 2; desc_length -= 2;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_parse_enhanced_ac3_audio_stream_desc(
	unsigned char *p,
	enhanced_ac3_audio_stream_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	temp = get_8bit(p);
	desc->bsid_flag = (unsigned char)((temp >> 6) & 0x1);
	desc->mainid_flag = (unsigned char)((temp >> 5) & 0x1);
	desc->asvc_flag = (unsigned char)((temp >> 4) & 0x1);
	desc->mixinfoexists = (unsigned char)((temp >> 3) & 0x1);
	desc->substream1_flag = (unsigned char)((temp >> 1) & 0x1);
	desc->substream2_flag = (unsigned char)((temp >> 1) & 0x1);
	desc->substream3_flag = (unsigned char)((temp >> 0) & 0x1);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->full_service_flag = (unsigned char)((temp >> 6) & 0x1);
	desc->audio_service_type = (unsigned char)((temp >> 3) & 0x7);
	desc->number_of_channels = (unsigned char)((temp >> 0) & 0x7);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->language_flag = (unsigned char)((temp >> 7) & 0x1);
	desc->language_flag_2 = (unsigned char)((temp >> 6) & 0x1);
	desc->bsid = (unsigned char)((temp >> 0) & 0x1F);
	p += 1; desc_length -= 1;

	temp = get_8bit(p);
	desc->priority = (unsigned char)((temp >> 3) & 0x3);
	desc->mainid = (unsigned char)((temp >> 0) & 0x7);
	p += 1; desc_length -= 1;

	DTV_MEMCPY(desc->language, p, 3);
	p += ISO_LANGUAGE_CODE; desc_length -= ISO_LANGUAGE_CODE;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_desc_parse(
	unsigned char *p,
	descriptor_t *desc
	)
{
	DTV_ECODE err = DTV_ERROR;
	unsigned char tag;
	int len;

	if (p == NULL || desc == NULL)
	{
		return DTV_ERROR;
	}

	tag = *p;
	len = *(p + 1);

	desc->tag = tag;
	desc->len = len;
	desc->next = NULL;
	
	switch(tag)
	{
		case DESC_TAG_VIDEO_STREAM:
			err = dvb_parse_video_stream(p, (video_stream_desc_t *)desc->data);
		break;
		
		case DESC_TAG_AUDIO_STREAM:
			err = dvb_parse_audio_stream(p, (audio_stream_desc_t *)desc->data);
		break;
		
		case DESC_TAG_HIERARCHY:
			err = dvb_parse_hierarchy_desc(p, (hierarchy_desc_t *)desc->data);
		break;
		
		case DESC_TAG_REGISTRATION:
			err = dvb_parse_registration_desc(p, (registration_desc_t *)desc->data);
		break;
		
		case DESC_TAG_DATA_STREAM_ALIGN:
			err = dvb_parse_data_stream_align_desc(p, (data_stream_align_desc_t *)desc->data);
		break;
		
		case DESC_TAG_TARGET_BG_GRID:
			err = dvb_parse_target_bg_grid_desc(p, (target_bg_grid_desc_t *)desc->data);
		break;
		
		case DESC_TAG_VIDEO_WINDOW:
			err = dvb_parse_video_window_desc(p, (video_window_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CA:
			err = dvb_parse_ca_desc(p, (ca_desc_t *)desc->data);
		break;
		
		case DESC_TAG_ISO_639_LANG:
			err = dvb_parse_iso_639_lang_desc(p, (iso_639_lang_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SYSTEM_CLOCK:
			err = dvb_parse_system_clock_desc(p, (system_clock_desc_t *)desc->data);
		break;
		
		case DESC_TAG_MULTI_BUFFER_UTIL:
			err = dvb_parse_multi_buffer_util_desc(p, (multi_buffer_util_desc_t *)desc->data);
		break;
		
		case DESC_TAG_COPYRIGHT:
			err = dvb_parse_copyright_desc(p, (copyright_desc_t *)desc->data);
		break;
		
		case DESC_TAG_MAX_BITRATE:
			err = dvb_parse_max_bitrate_desc(p, (max_bitrate_desc_t *)desc->data);
		break;
		
		case DESC_TAG_PRIVATE_DATA_INDICATOR:
			err = dvb_parse_private_data_indicator_desc(p, (private_data_indicator_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SMOOTHING_BUFFER:
			err = dvb_parse_smoothing_buffer_desc(p, (smoothing_buffer_desc_t *)desc->data);
		break;

		case DESC_TAG_MPEG4_AAC_PLUS:
		break;

		case DESC_TAG_AVC:
			err = dvb_parse_avc_desc(p, (avc_desc_t *)desc->data);
		break;

		case DESC_TAG_MPEG2_AAC:
		break;
		
		case DESC_TAG_NETWORK_NAME:
			err = dvb_parse_network_name_desc(p, (network_name_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SERVICE_LIST:
			err = dvb_parse_service_list_desc(p, (service_list_desc_t *)desc->data);
		break;
		
		case DESC_TAG_STUFFING:
		break;
		
		case DESC_TAG_SAT_DELIVERY_SYSTEM:
			err = dvb_parse_sat_delivery_system_desc(p, (sat_delivery_system_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CABLE_DELIVERY_SYSTEM:
			err = dvb_parse_cable_delivery_system_desc(p, (cable_delivery_system_desc_t *)desc->data);
		break;
		
		case DESC_TAG_VBI_DATA:
		break;
		
		case DESC_TAG_VBI_TELETEXT:
		break;
		
		case DESC_TAG_BOUQUET_NAME:
			err = dvb_parse_bouquet_name_desc(p, (bouquet_name_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SERVICE:
			err = dvb_parse_service_desc(p, (service_desc_t *)desc->data);
		break;
		
		case DESC_TAG_COUNTRY_AVAILABILITY:
			err = dvb_parse_country_availability_desc(p, (country_availability_desc_t *)desc->data);
		break;
		
		case DESC_TAG_LINKAGE:
			err = dvb_parse_linkage_desc(p, (linkage_desc_t *)desc->data);
		break;
		
		case DESC_TAG_NVOD_REF:
			err = dvb_parse_nvod_ref_desc(p, (nvod_ref_desc_t *)desc->data);
		break;
		
		case DESC_TAG_TIME_SHIFTED_SERVICE:
			err = dvb_parse_time_shifted_service_desc(p, (time_shifted_service_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SHORT_EVENT:
			err = dvb_parse_short_event_desc(p, (short_event_desc_t *)desc->data);
		break;
		
		case DESC_TAG_EXTENDED_EVENT:
			err = dvb_parse_extended_event_desc(p, (extended_event_desc_t *)desc->data);
		break;
		
		case DESC_TAG_TIME_SHIFTED_EVENT:
			err = dvb_parse_time_shifted_event_desc(p, (time_shifted_event_desc_t *)desc->data);
		break;
		
		case DESC_TAG_COMPONENT:
			err = dvb_parse_component_desc(p, (component_desc_t *)desc->data);
		break;
		
		case DESC_TAG_MOSAIC:
			err = dvb_parse_mosaic_desc(p, (mosaic_desc_t *)desc->data);
		break;
		
		case DESC_TAG_STREAM_IDENTIFIER:
			err = dvb_parse_stream_identifier_desc(p, (stream_identifier_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CA_IDENTIFIER:
			err = dvb_parse_ca_identifier_desc(p, (ca_identifier_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CONTENT:
			err = dvb_parse_content_desc(p, (content_desc_t *)desc->data);
		break;
		
		case DESC_TAG_PARENTAL_RATING:
			err = dvb_parse_parental_rating_desc(p, (parental_rating_desc_t *)desc->data);
		break;
		
		case DESC_TAG_TELETEXT:
			err = dvb_parse_teletext_desc(p, (teletext_desc_t *)desc->data);
		break;
		
		case DESC_TAG_TELEPHONE:
			err = dvb_parse_telephone_desc(p, (telephone_desc_t *)desc->data);
		break;
		
		case DESC_TAG_LOCAL_TIME_OFFSET:
			err = dvb_parse_local_time_offset_desc(p, (local_time_offset_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SUBTITLING:
			err = dvb_parse_subtitling_desc(p, (subtitling_desc_t *)desc->data);
		break;
		
		case DESC_TAG_TER_DELIVERY_SYSTEM:
			err = dvb_parse_ter_delivery_system_desc(p, (ter_delivery_system_desc_t *)desc->data);
		break;
		
		case DESC_TAG_MULTILINGUAL_NETWORK_NAME:
			err = dvb_parse_multi_network_name_desc(p, (multi_network_name_desc_t *)desc->data);
		break;
		
		case DESC_TAG_MULTILINGUAL_BOUQUET_NAME:
			err = dvb_parse_multi_bouquet_name_desc(p, (multi_bouquet_name_desc_t *)desc->data);
		break;
		
		case DESC_TAG_MULTILINGUAL_SERVICE_NAME:
			err = dvb_parse_multi_service_name_desc(p, (multi_service_name_desc_t *)desc->data);
		break;
		
		case DESC_TAG_MULTILINGUAL_COMPONNT:
			err = dvb_parse_multi_component_desc(p, (multi_component_desc_t *)desc->data);
		break;
		
		case DESC_TAG_PRIVATE_DATA_SPECIFIER:
			err = dvb_parse_private_data_specifier_desc(p, (private_data_specifier_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SERVICE_MOVE:
			err = dvb_parse_service_move_desc(p, (service_move_desc_t *)desc->data);
		break;
		
		case DESC_TAG_SHORT_SMOOTHING_BUFFER:
			err = dvb_parse_short_smoothing_buffer_desc(p, (short_smoothing_buffer_desc_t *)desc->data);
		break;
		
		case DESC_TAG_FREQENCY_LIST:
			err = dvb_parse_frequency_list_desc(p, (frequency_list_desc_t *)desc->data);
		break;
	
		case DESC_TAG_DATA_BROADCAST:
			err = dvb_parse_data_broadcast_desc(p, (data_broadcast_desc_t *)desc->data);
		break;
	
		case DESC_TAG_DATA_BROADCAST_ID:
			err = dvb_parse_data_broadcast_id_desc(p, (data_broadcast_id_desc_t *)desc->data);
		break;
	
		case DESC_TAG_AC3:
			err = dvb_parse_ac3_desc(p, (ac3_desc_t *)desc->data);
		break;

		case DESC_TAG_SCRAMBLING:
			err = dvb_parse_scrambling_desc(p, (scrambling_desc_t *)desc->data);
		break;

		case DESC_TAG_PARTIAL_TRANSPORT_STREAM:
			err = dvb_parse_partial_transport_stream_desc(p, (paritial_transport_stream_desc_t *)desc->data);
		break;

		case DESC_TAG_DIGITAL_SAT_NEWS_GETHERING:
			err = dvb_parse_digital_sat_news_gathering_desc(p, (digital_sat_news_gathering_desc_t *)desc->data);
		break;

		case DESC_TAG_PDC:
			err = dvb_parse_pdc_desc(p, (pdc_desc_t *)desc->data);
		break;

		case DESC_TAG_ANCILLARY_DATA:
			err = dvb_parse_ancillary_data_desc(p, (ancillary_data_desc_t *)desc->data);
		break;

		case DESC_TAG_CELL_LIST:
			err = dvb_parse_cell_list_desc(p, (cell_list_desc_t *)desc->data);
		break;

		case DESC_TAG_CELL_FREQUENCY_LINK:
			err = dvb_parse_cell_frequency_link_desc(p, (cell_frequency_link_desc_t *)desc->data);
		break;

		case DESC_TAG_ANNOUNCEMENT_SUPPORT:
			err = dvb_parse_announcement_support_desc(p, (announcement_support_desc_t *)desc->data);
		break;

		case DESC_TAG_APPLICATION_SIGNALLING:
			err = dvb_parse_application_signalling_desc(p, (application_signalling_desc_t *)desc->data);
		break;

		case DESC_TAG_ADAPTATION_FIELD_DATA:
			err = dvb_parse_adaptation_field_data_desc(p, (adaptation_field_data_desc_t *)desc->data);
		break;

		case DESC_TAG_SERVICE_IDENTIFIER:
		break;

		case DESC_TAG_SERVICE_AVAILABLITY:
			err = dvb_parse_service_availability_desc(p, (service_availability_desc_t *)desc->data);
		break;

		case DESC_TAG_DEFAULT_AUTHORITY:
		break;

		case DESC_TAG_RELATED_CONTENT:
		break;

		case DESC_TAG_TVA_ID:
		break;

		case DESC_TAG_CONTENT_IDENTIFIER:
		break;

		case DESC_TAG_TIME_SLICE_FEC_IDENTIFIER:
		break;

		case DESC_TAG_ECM_REPETITION_RATE:
		break;

		case DESC_TAG_S2_SAT_DELIVERY_SYSTEM:
		break;

		case DESC_TAG_ENHANCED_AC3:
			err = dvb_parse_enhanced_ac3_desc(p, (enhanced_ac3_desc_t *)desc->data);
		break;

		case DESC_TAG_DTS:
		break;

		case DESC_TAG_AAC:
			err = dvb_parse_aac_desc(p, (aac_desc_t *)desc->data);
		break;

		case DESC_TAG_XAIT_LOCATION:
		break;

		case DESC_TAG_FTA_CONTENT_MANAGEMENT:
			err = dvb_parse_fta_content_management_desc(p, (fta_content_management_desc_t *)desc->data);
		break;

		case DESC_TAG_EXTENSION:
		break;

		case DESC_TAG_DVBC_LOGICAL_CHANNEL_NUMBER:
			err = dvb_parse_dvbc_lcn_desc(p, (channel_desc_t *)desc->data);
		break;
		
		case DESC_TAG_DVB_LOGICAL_CHANNEL_NUMBER:
			err = dvb_parse_lcn_desc(p, (lcn_desc_t *)desc->data);
		break;

#if 0
		// DTG_SIPSI
		case DESC_TAG_SERVICE_ATTRIBUTE:
			err = dvb_parse_service_attribute_desc(p, (service_attribute_desc_t *)desc->data);
		break;
#endif

#if 1
		case DESC_TAG_SHORTNAME:
			err = dvb_parse_short_name_desc(p, (short_name_desc_t *)desc->data);
		break;
#else
		case DESC_TAG_LOGICAL_CHANNEL_NUM_V2:
			err = dvb_parse_lcn_v2_desc(p, (lcn_v2_desc_t *)desc->data);
		break;
#endif

		// DTG_SIPSI, HD_SIMULCAST_LCN
		case DESC_TAG_HD_SIMULCAST_LOGICAL_CHANNEL:
			err = dvb_parse_hd_simulcast_lcn_desc(p, (hd_simulcast_lcn_desc_t *)desc->data);
		break;

		// BSKYB_CHANNEL_NUMBERING
		case DESC_TAG_LOGICAL_CHANNEL_NUMBER:
			err = dvb_parse_lcn_desc(p, (lcn_desc_t *)desc->data);
		break;

#if 0
		/* ASTRA APS(ASTRA PLATFORM SERVICE) - SGT(SERVICE GUIDE TABLE) */
		// HD_PLUS_SCAN
		case DESC_TAG_SERVICE_LIST_NAME:
			err = dvb_parse_service_list_name_desc(p, (service_list_name_desc_t *)desc->data);
		break;
#endif

		case DESC_TAG_BOUQUET_LIST:
			err = dvb_parse_bouquet_list_desc(p, (bouquet_list_desc_t *)desc->data);
		break;

		case DESC_TAG_VIRTUAL_SERVICE_ID:
			err = dvb_parse_virtual_service_id_desc(p, (virtual_service_id_desc_t *)desc->data);
		break;

		// CANAL
		case DESC_TAG_CANAL_AUDIO_NAME:
			err = dvb_parse_canal_audio_name_desc(p, (canal_audio_name_desc_t *)desc->data);
		break;

		case DESC_TAG_CANAL_DATA:
		break;

#if 0
		// CIPLUS
		case DESC_TAG_CIPLUS_CONTENT_LABEL:
			err = dvb_parse_ciplus_content_lable_desc(p, (ciplus_content_label_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CIPLUS_SERVICE:
			err = dvb_parse_ciplus_service_desc(p, (ciplus_service_desc_t *)desc->data);
		break;
		
		case DESC_TAG_CIPLUS_PROTECTION:
			err = dvb_parse_ci_protection_desc(p, (ci_protection_desc_t *)desc->data);
		break;
#endif

		/* ATSC - PSIP(Program and System Information Protocol for Terrestrial and Cable */
		case DESC_TAG_ATSC_STUFFING:
		break;

		case DESC_TAG_ATSC_AC3:
			err = dvb_parse_ac3_audio_stream_desc(p, (ac3_audio_stream_desc_t *)desc->data);
		break;

		case DESC_TAG_ATSC_CAPTION_SERVICE:
			err = dvb_parse_caption_service_desc(p, (psip_caption_service_desc_t *)desc->data);
		break;

#if 0
		case DESC_TAG_ATSC_CONTENT_ADVISORY:
		break;

		case DESC_TAG_ATSC_EXTENDED_CHANNEL_NAME:
		break;

		case DESC_TAG_ATSC_SERVICE_LOCATION:
		break;

		case DESC_TAG_ATSC_TIMESHIFTED_SERVICE:
		break;

		case DESC_TAG_ATSC_COMPONENT_NAME:
		break;

		case DESC_TAG_ATSC_DCC_DEPARTING_REQUEST:
		break;

		case DESC_TAG_ATSC_DCC_ARRIVING_REQUEST:
		break;

		case DESC_TAG_ATSC_REDISTRIBUTION_CONTROL:
		break;

		case DESC_TAG_ATSC_GENRE:
		break;

		case DESC_TAG_ATSC_PRIVATE_INFORMATION:
		break;
#endif

		case DESC_TAG_ATSC_ENHANCED_AC3:
			err = dvb_parse_enhanced_ac3_audio_stream_desc(p, (enhanced_ac3_audio_stream_desc_t *)desc->data);
		break;	

		// ARIB_SUBTITLE
		case DESC_TAG_ARIB_SUBTITLE:
		break;

		default:
		break;
	}

	return err;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_application_desc_t *
dvb_ait_alloc_application_desc(
	void
	)
{
	ait_application_desc_t *desc;
	
	desc = (ait_application_desc_t *)DTV_MALLOC(sizeof(ait_application_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_application_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_application_name_desc_t *
dvb_ait_alloc_application_name_desc(
	void
	)
{
	ait_application_name_desc_t *desc;
	
	desc = (ait_application_name_desc_t *)DTV_MALLOC(sizeof(ait_application_name_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_application_name_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_transport_protocol_desc_t *
dvb_ait_alloc_transport_protocol_desc(
	void
	)
{
	ait_transport_protocol_desc_t *desc;
	
	desc = (ait_transport_protocol_desc_t *)DTV_MALLOC(sizeof(ait_transport_protocol_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_transport_protocol_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_external_application_auth_desc_t *
dvb_ait_alloc_external_application_auth_desc(
	void
	)
{
	ait_external_application_auth_desc_t *desc;
	
	desc = (ait_external_application_auth_desc_t *)DTV_MALLOC(sizeof(ait_external_application_auth_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_external_application_auth_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_application_recording_desc_t *
dvb_ait_alloc_application_recording_desc(
	void
	)
{
	ait_application_recording_desc_t *desc;
	
	desc = (ait_application_recording_desc_t *)DTV_MALLOC(sizeof(ait_application_recording_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_application_recording_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_application_icons_desc_t *
dvb_ait_alloc_application_icons_desc(
	void
	)
{
	ait_application_icons_desc_t *desc;
	
	desc = (ait_application_icons_desc_t *)DTV_MALLOC(sizeof(ait_application_icons_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_application_storage_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_application_storage_desc_t *
dvb_ait_alloc_application_storage_desc(
	void
	)
{
	ait_application_storage_desc_t *desc;
	
	desc = (ait_application_storage_desc_t *)DTV_MALLOC(sizeof(ait_application_storage_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_application_storage_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_graphics_constraints_desc_t *
dvb_ait_alloc_graphics_constraints_desc(
	void
	)
{
	ait_graphics_constraints_desc_t *desc;
	
	desc = (ait_graphics_constraints_desc_t *)DTV_MALLOC(sizeof(ait_graphics_constraints_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_graphics_constraints_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_simple_application_location_desc_t *
dvb_ait_alloc_simple_application_location_desc(
	void
	)
{
	ait_simple_application_location_desc_t *desc;
	
	desc = (ait_simple_application_location_desc_t *)DTV_MALLOC(sizeof(ait_simple_application_location_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_simple_application_location_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_application_usage_desc_t *
dvb_ait_alloc_application_usage_desc(
	void
	)
{
	ait_application_usage_desc_t *desc;
	
	desc = (ait_application_usage_desc_t *)DTV_MALLOC(sizeof(ait_application_usage_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_application_usage_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
ait_simple_application_boundary_desc_t *
dvb_ait_alloc_simple_application_boundary_desc(
	void
	)
{
	ait_simple_application_boundary_desc_t *desc;
	
	desc = (ait_simple_application_boundary_desc_t *)DTV_MALLOC(sizeof(ait_simple_application_boundary_desc_t));
	if (desc == NULL) return NULL;

	DTV_MEMSET(desc, 0, sizeof(ait_simple_application_boundary_desc_t));

	return desc;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_application_desc(
	ait_application_desc_t *desc
	)
{
	ait_application_profile_data_t	*profile, *next;

	if (desc)
	{
		profile = desc->profile_data;
		
		while (profile != NULL)
		{
			next = profile->next;

			DTV_FREE(profile);

			profile = next;
		}

		if(desc->transport_protocol_label != NULL)
		{
			DTV_FREE(desc->transport_protocol_label);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_application_name_desc(
	ait_application_name_desc_t *desc
	)
{
	ait_application_name_data_t	*name, *next;
	
	if (desc)
	{
		name = desc->name_data;
		
		while (name != NULL)
		{
			next = name->next;

			if (name->name)
			{
				DTV_FREE(name->name);
			}

			DTV_FREE(name);

			name = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_transport_protocol_desc(
	ait_transport_protocol_desc_t *desc
	)
{
	if (desc)
	{
		if(desc->selector_byte)
		{
			DTV_FREE(desc->selector_byte);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_external_application_auth_desc(
	ait_external_application_auth_desc_t *desc
	)
{
	ait_external_application_auth_data_t	*auth, *next;

	if (desc)
	{
		auth = desc->auth_data;
		
		while (auth != NULL)
		{
			next = auth->next;

			DTV_FREE(auth);

			auth = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_application_recording_desc(
	ait_application_recording_desc_t *desc
	)
{
	ait_application_recording_lebel_data_t *lebel_data, *next;

	if (desc)
	{
		lebel_data = desc->lebel_data;
		
		while (lebel_data != NULL)
		{
			next = lebel_data->next;

			DTV_FREE(lebel_data);

			lebel_data = next;
		}

		if(desc->component_tag)
		{
			DTV_FREE(desc->component_tag);
		}

		if(desc->private_data)
		{
			DTV_FREE(desc->private_data);
		}

		if(desc->reserved_future_use)
		{
			DTV_FREE(desc->reserved_future_use);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_application_icons_desc(
	ait_application_icons_desc_t *desc
	)
{
	if (desc)
	{
		if(desc->icon_locator_byte != NULL)
		{
			DTV_FREE(desc->icon_locator_byte);
		}

		if(desc->reserved_future_use != NULL)
		{
			DTV_FREE(desc->reserved_future_use);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_application_storage_desc(
	ait_application_storage_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_graphics_constraints_desc(
	ait_graphics_constraints_desc_t *desc
	)
{
	if (desc)
	{
		if(desc->graphics_configuration_byte)
		{
			DTV_FREE(desc->graphics_configuration_byte);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_simple_application_location_desc(
	ait_simple_application_location_desc_t *desc
	)
{
	if (desc)
	{
		if(desc->initial_path_bytes)
		{
			DTV_FREE(desc->initial_path_bytes);
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_application_usage_desc(
	ait_application_usage_desc_t *desc
	)
{
	if (desc)
	{
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_free_simple_application_boundary_desc(
	ait_simple_application_boundary_desc_t *desc
	)
{
	ait_boundary_extension_data_t *boundary_extension_data, *next;
	
	if (desc)
	{
		boundary_extension_data = desc->boundary_extension_data;
		
		while (boundary_extension_data != NULL)
		{
			next = boundary_extension_data->next;

			if(boundary_extension_data->boundary_extension_byte != NULL)
			{
				DTV_FREE(boundary_extension_data->boundary_extension_byte);
			}
			
			DTV_FREE(boundary_extension_data);

			boundary_extension_data = next;
		}
		
		DTV_FREE(desc);
	}
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_application_desc(
	unsigned char *p,
	ait_application_desc_t *desc
	)
{
	ait_application_profile_data_t *profile_data, *prev_application_profile_data = NULL;
	signed short desc_length, application_profiles_loop_length;
	unsigned char temp;

	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->profile_data = NULL;
	desc->transport_protocol_label = NULL;
	desc->transport_protocol_label_length = 0;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	application_profiles_loop_length = get_8bit(p);
	p += 1; desc_length -= 1;	

	while ((desc_length > 0)
		&& (desc_length >= application_profiles_loop_length)
		&& (application_profiles_loop_length > 0))
	{
		profile_data = (ait_application_profile_data_t *)DTV_MALLOC(sizeof(ait_application_profile_data_t));
		if (profile_data == NULL) return DTV_ERROR;
		
		profile_data->next = NULL;

		/* link */
		if (desc->profile_data == NULL)
		{
			desc->profile_data = profile_data;
		}
		else
		{
			prev_application_profile_data->next = profile_data;
		}
		prev_application_profile_data = profile_data;

		profile_data->application_profile = get_16bit(p);
		profile_data->version_major = get_8bit(p + 2);
		profile_data->version_minor = get_8bit(p + 3);
		profile_data->version_micro = get_8bit(p + 4);

		p += (desc_length + 5); desc_length = (unsigned short)(desc_length - (desc_length + 5));
		application_profiles_loop_length = (unsigned short)(application_profiles_loop_length - (desc_length + 5));
	}

	temp = get_8bit(p);
	p += 1; desc_length -= 1;	
	desc->service_bound_flag = (unsigned char)((temp >> 7) & 0x1);
	desc->visibility = (unsigned char)((temp >> 5) & 0x3);
	desc->application_priority = get_8bit(p);
	p += 1; desc_length -= 1;	

	if(desc_length > 0)
	{
		desc->transport_protocol_label = (unsigned char *)DTV_MALLOC(desc_length);
		if (desc->transport_protocol_label == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->transport_protocol_label, 0x0, desc_length);
		DTV_MEMCPY(desc->transport_protocol_label, p, desc_length);
		desc->transport_protocol_label_length = desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_application_name_desc(
	unsigned char *p,
	ait_application_name_desc_t *desc
	)
{
	ait_application_name_data_t *name, *prev_name = NULL;
	unsigned char name_length;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->name_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	while (desc_length >= 4)
	{
		name = (ait_application_name_data_t *)DTV_MALLOC(sizeof(ait_application_name_data_t));
		if (name == NULL) return DTV_ERROR;
		
		name->name = NULL;
		name->next = NULL;

		/* link */
		if (desc->name_data == NULL)
		{
			desc->name_data = name;
		}
		else
		{
			prev_name->next = name;
		}
		prev_name = name;

		DTV_MEMCPY(name->lang_code, p, 3);
		p += 3; desc_length -= 3;

		name_length = *p;
		p += 1; desc_length -= 1;
		
		name->name = (char *)DTV_MALLOC(name_length + 2);
		if (name->name == NULL) return DTV_ERROR;

		DTV_MEMSET(name->name, 0x0, name_length + 2);
		DTV_MEMCPY(name->name, p, name_length);
		
		p += name_length; desc_length -= name_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_transport_protocol_desc(
	unsigned char *p,
	ait_transport_protocol_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->selector_byte = NULL;
	desc->selector_byte_length = 0;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->protocol_id = get_16bit(p);
	p += 2; desc_length -= 2;

	desc->transport_protocol_label = get_8bit(p);
	p += 1; desc_length -= 1;

	if(desc_length > 0)
	{
		desc->selector_byte = (unsigned char *)DTV_MALLOC(desc_length);
		if (desc->selector_byte == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->selector_byte, 0x0, desc_length);
		DTV_MEMCPY(desc->selector_byte, p, desc_length);
		desc->selector_byte_length = desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_external_application_auth_desc(
	unsigned char *p,
	ait_external_application_auth_desc_t *desc
	)
{
	ait_external_application_auth_data_t *auth, *prev_auth = NULL;
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->auth_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	while (desc_length >= 7)
	{
		auth = (ait_external_application_auth_data_t *)DTV_MALLOC(sizeof(ait_external_application_auth_data_t));
		if (auth == NULL) return DTV_ERROR;
		
		auth->next = NULL;

		/* link */
		if (desc->auth_data == NULL)
		{
			desc->auth_data = auth;
		}
		else
		{
			prev_auth->next = auth;
		}
		prev_auth = auth;

		auth->application_identifier.organisation_id = get_32bit(p);
		auth->application_identifier.application_id = get_16bit(p + 4);
		p += 6; desc_length -= 6;

		auth->application_priority = get_8bit(p);
		p += 1; desc_length -= 1;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_application_recording_desc(
	unsigned char *p,
	ait_application_recording_desc_t *desc
	)
{
	ait_application_recording_lebel_data_t *lebel_data, *prev_lebel_data = NULL;
	signed short desc_length, label_length, label_count, component_tag_list_length, private_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->lebel_data = NULL;
	desc->component_tag = NULL;
	desc->private_data = NULL;
	desc->reserved_future_use = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	temp = *p;
	desc->scheduled_recording_flag = (unsigned char )((temp >> 7) & 0x1);
	desc->trick_mode_aware_flag = (unsigned char )((temp >> 6) & 0x1);
	desc->time_shift_flag = (unsigned char )((temp >> 5) & 0x1);
	desc->dynamic_flag = (unsigned char )((temp >> 4) & 0x1);
	desc->av_synced_flag = (unsigned char )((temp >> 3) & 0x1);
	desc->initiating_replay_flag = (unsigned char )((temp >> 2) & 0x1);
	p += 1; desc_length -= 1;

	label_count = *p;
	desc->label_count = label_count;
	p += 1; desc_length -= 1;

	while(desc_length > 0 && label_count > 0)
	{
		lebel_data = (ait_application_recording_lebel_data_t *)DTV_MALLOC(sizeof(ait_application_recording_lebel_data_t));
		if (lebel_data == NULL) return DTV_ERROR;
		
		lebel_data->next = NULL;

		/* link */
		if (desc->lebel_data == NULL)
		{
			desc->lebel_data = lebel_data;
		}
		else
		{
			prev_lebel_data->next = lebel_data;
		}
		prev_lebel_data = lebel_data;

		label_length = *p;
		p += 1; desc_length -= 1;

		if(label_length != 0)
		{
			lebel_data->label_char = (unsigned char *)DTV_MALLOC(label_length + 1);
			if (lebel_data->label_char == NULL) return DTV_ERROR;
			DTV_MEMSET(lebel_data->label_char, 0x0, label_length + 1);
			DTV_MEMCPY(lebel_data->label_char, p, label_length);
			p += label_length; desc_length -= label_length;
		}

		temp = *p;
		lebel_data->storage_properties = (unsigned char)((temp >> 6) * 0x3);
		p += 1; desc_length -= 1;
		label_count--;
	}

	component_tag_list_length = *p;
	p += 1; desc_length -= 1;

	if(desc_length > 0)
	{
		desc->component_tag = (unsigned char *)DTV_MALLOC(component_tag_list_length + 1);
		if (desc->component_tag == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->component_tag, 0x0, component_tag_list_length + 1);
		DTV_MEMCPY(desc->component_tag, p, component_tag_list_length);
		desc->component_tag_list_length = component_tag_list_length;
		p += component_tag_list_length; desc_length -= component_tag_list_length;
	}

	private_length = *p;
	p += 1; desc_length -= 1;

	if(desc_length > 0)
	{
		desc->private_data = (unsigned char *)DTV_MALLOC(private_length + 1);
		if (desc->private_data == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->private_data, 0x0, private_length + 1);
		DTV_MEMCPY(desc->private_data, p, private_length);
		desc->private_data_length = private_length;
		p += private_length; desc_length -= private_length;
	}

	if(desc_length > 0)
	{
		desc->reserved_future_use = (unsigned char *)DTV_MALLOC(desc_length + 1);
		if (desc->reserved_future_use == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->reserved_future_use, 0x0, desc_length + 1);
		DTV_MEMCPY(desc->reserved_future_use, p, desc_length);
		desc->reserved_future_use_length = desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_application_icons_desc(
	unsigned char *p,
	ait_application_icons_desc_t *desc
	)
{
	signed short desc_length, icon_locator_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	icon_locator_length = *p;
	p += 1; desc_length -= 1;

	if(icon_locator_length > 0)
	{
		desc->icon_locator_byte = (unsigned char *)DTV_MALLOC(icon_locator_length + 1);
		if (desc->icon_locator_byte == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->icon_locator_byte, 0x0, icon_locator_length + 1);
		DTV_MEMCPY(desc->icon_locator_byte, p, icon_locator_length);
		desc->icon_locator_length = icon_locator_length;
		p += icon_locator_length; desc_length -= icon_locator_length;
	}

	desc->icon_flags = get_16bit(p);
	p += 2; desc_length -= 2;

	if(desc_length > 0)
	{
		desc->reserved_future_use = (unsigned char *)DTV_MALLOC(desc_length + 1);
		if (desc->reserved_future_use == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->reserved_future_use, 0x0, desc_length + 1);
		DTV_MEMCPY(desc->reserved_future_use, p, desc_length);
		desc->reserved_future_use_length = desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_application_storage_desc(
	unsigned char *p,
	ait_application_storage_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->storage_property = 0xff; // reserved

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->storage_property = *p;
	p += 1; desc_length -= 1;

	temp = *p;
	desc->not_launchable_from_broadcast = (unsigned char)((temp >> 7) & 0x1);
	desc->launchable_completely_from_cache = (unsigned char)((temp >> 6) & 0x1);
	desc->is_launchable_with_older_version = (unsigned char)((temp >> 5) & 0x1);
	p += 1; desc_length -= 1;

	desc->Version = get_29bit(p);
	p += 4; desc_length -= 4;

	desc->Priority = *p;
	
	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_graphics_constraints_desc(
	unsigned char *p,
	ait_graphics_constraints_desc_t *desc
	)
{
	signed short desc_length;
	unsigned char temp;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->graphics_configuration_byte = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	temp = *p;
	desc->can_run_without_visible_ui = (unsigned char)((temp >> 2) & 0x1);
	desc->handles_configuration_changed = (unsigned char)((temp >> 1) & 0x1);
	desc->handles_externally_controlled_video = (unsigned char)(temp & 0x1);
	p += 1; desc_length -= 1;

	if(desc_length > 0)
	{
		desc->graphics_configuration_byte = (unsigned char *)DTV_MALLOC(desc_length + 1);
		if (desc->graphics_configuration_byte == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->graphics_configuration_byte, 0x0, desc_length + 1);
		DTV_MEMCPY(desc->graphics_configuration_byte, p, desc_length);
		desc->graphics_configuration_byte_length = desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_simple_application_location_desc(
	unsigned char *p,
	ait_simple_application_location_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->initial_path_bytes = NULL;
	desc->initial_path_bytes_length = 0;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	if(desc_length > 0)
	{
		desc->initial_path_bytes = (unsigned char *)DTV_MALLOC(desc_length + 1);
		if (desc->initial_path_bytes == NULL) return DTV_ERROR;
		DTV_MEMSET(desc->initial_path_bytes, 0x0, desc_length + 1);
		DTV_MEMCPY(desc->initial_path_bytes, p, desc_length);
		desc->initial_path_bytes_length = desc_length;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_application_usage_desc(
	unsigned char *p,
	ait_application_usage_desc_t *desc
	)
{
	signed short desc_length;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->usage_type = 0;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	desc->usage_type = *p;

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_parse_simple_application_boundary_desc(
	unsigned char *p,
	ait_simple_application_boundary_desc_t *desc
	)
{
	ait_boundary_extension_data_t *boundary_extension_data, *prev_boundary_extension_data = NULL;
	signed short desc_length, boundary_extension_length, boundary_extension_count;
	
	if (desc == NULL)
	{
		return DTV_ERROR;
	}
	desc->boundary_extension_count = 0;
	desc->boundary_extension_data = NULL;

	/* descriptor_tag */
	p++;

	desc_length = (signed short)*p++;

	if (desc_length == 0)
	{
		return DTV_ERROR;
	}

	boundary_extension_count = *p;
	desc->boundary_extension_count = boundary_extension_count;
	p += 1; desc_length -= 1;

	while (desc_length > 0 && boundary_extension_count > 0)
	{
		boundary_extension_data = (ait_boundary_extension_data_t *)DTV_MALLOC(sizeof(ait_boundary_extension_data_t));
		if (boundary_extension_data == NULL) return DTV_ERROR;
		
		boundary_extension_data->next = NULL;

		/* link */
		if (desc->boundary_extension_data == NULL)
		{
			desc->boundary_extension_data = boundary_extension_data;
		}
		else
		{
			prev_boundary_extension_data->next = boundary_extension_data;
		}
		prev_boundary_extension_data = boundary_extension_data;

		boundary_extension_length = *p;
		p += 1; desc_length -= 1;

		if(boundary_extension_length != 0)
		{
			boundary_extension_data->boundary_extension_byte = (unsigned char *)DTV_MALLOC(boundary_extension_length + 1);
			if (boundary_extension_data->boundary_extension_byte == NULL) return DTV_ERROR;
			DTV_MEMSET(boundary_extension_data->boundary_extension_byte, 0x0, boundary_extension_length + 1);
			DTV_MEMCPY(boundary_extension_data->boundary_extension_byte, p, boundary_extension_length);
			p += boundary_extension_length; desc_length -= boundary_extension_length;
		}

		boundary_extension_count--;
	}

	return DTV_OK;
}


/*-----------------------------------------------------------------------------
 * hbbtv -AIT
 *
 *
 *---------------------------------------------------------------------------*/
descriptor_t *
dvb_ait_desc_alloc(
	unsigned char tag
	)
{
	descriptor_t *desc;

	desc = (descriptor_t *)DTV_MALLOC(sizeof(descriptor_t));

	if (desc == NULL)
	{
		return NULL;
	}

	DTV_MEMSET(desc, 0, sizeof(descriptor_t));

	switch (tag)
	{
		case APPLICATION_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_application_desc();
		break;
		
		case APPLICATION_NAME_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_application_name_desc();
		break;

		case TRANSPORT_PROTOCOL_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_transport_protocol_desc();
		break;

		case DVB_J_APPLICATION_DESCRIPTOR:
		break;

		case DVB_J_APPLICATION_LOCATION_DESCRIPTOR:
		break;

		case EXTERNAL_APPLICATION_AUTHORISATION_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_external_application_auth_desc();
		break;

		case APPLICATION_RECORDING_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_application_recording_desc();
		break;

		case APPLICATION_ICONS_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_application_icons_desc();
		break;

		case APPLICATION_STORAGE_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_application_storage_desc();
		break;

		case GRAPHICS_CONSTRAINTS_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_graphics_constraints_desc();
		break;

		case SIMPLE_APPLICATION_LOCATION_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_simple_application_location_desc();
		break;

		case APPLICATION_USAGE_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_application_usage_desc();
		break;

		case SIMPLE_APPLICATION_BOUNDARY_DESCRIPTOR:
			desc->data = (void *)dvb_ait_alloc_simple_application_boundary_desc();
		break;

		default:
		break;
	}

	return desc;
}


/*-----------------------------------------------------------------------------
 * hbbtv -AIT
 *
 *
 *---------------------------------------------------------------------------*/
void
dvb_ait_desc_free(
	descriptor_t *desc
	)
{
	if (desc == NULL)
	{
		return;
	}
	
	switch(desc->tag)
	{
		case APPLICATION_DESCRIPTOR:
			dvb_ait_free_application_desc((ait_application_desc_t *)desc->data);
		break;
		
		case APPLICATION_NAME_DESCRIPTOR:
			dvb_ait_free_application_name_desc((ait_application_name_desc_t *)desc->data);
		break;

		case TRANSPORT_PROTOCOL_DESCRIPTOR:
			dvb_ait_free_transport_protocol_desc((ait_transport_protocol_desc_t *)desc->data);
		break;

		case DVB_J_APPLICATION_DESCRIPTOR:
		break;

		case DVB_J_APPLICATION_LOCATION_DESCRIPTOR:
		break;

		case EXTERNAL_APPLICATION_AUTHORISATION_DESCRIPTOR:
			dvb_ait_free_external_application_auth_desc((ait_external_application_auth_desc_t *)desc->data);
		break;

		case APPLICATION_RECORDING_DESCRIPTOR:
			dvb_ait_free_application_recording_desc((ait_application_recording_desc_t *)desc->data);
		break;

		case APPLICATION_ICONS_DESCRIPTOR:
			dvb_ait_free_application_icons_desc((ait_application_icons_desc_t *)desc->data);
		break;

		case APPLICATION_STORAGE_DESCRIPTOR:
			dvb_ait_free_application_storage_desc((ait_application_storage_desc_t *)desc->data);
		break;

		case GRAPHICS_CONSTRAINTS_DESCRIPTOR:
			dvb_ait_free_graphics_constraints_desc((ait_graphics_constraints_desc_t *)desc->data);
		break;

		case SIMPLE_APPLICATION_LOCATION_DESCRIPTOR:
			dvb_ait_free_simple_application_location_desc((ait_simple_application_location_desc_t *)desc->data);
		break;

		case APPLICATION_USAGE_DESCRIPTOR:
			dvb_ait_free_application_usage_desc((ait_application_usage_desc_t *)desc->data);
		break;

		case SIMPLE_APPLICATION_BOUNDARY_DESCRIPTOR:
			dvb_ait_free_simple_application_boundary_desc((ait_simple_application_boundary_desc_t *)desc->data);
		break;

		default:
		break;
	}

	DTV_FREE(desc);

	return;
	
}


/*-----------------------------------------------------------------------------
 * hbbtv -AIT
 *
 *
 *---------------------------------------------------------------------------*/
DTV_ECODE
dvb_ait_desc_parse(
	unsigned char *p,
	descriptor_t *desc
	)
{
	DTV_ECODE err = DTV_ERROR;
	unsigned char tag;
	int len;

	if (p == NULL || desc == NULL)
	{
		return DTV_ERROR;
	}

	tag = *p;
	len = *(p + 1);

	desc->tag = tag;
	desc->len = len;
	desc->next = NULL;
	
	switch(tag)
	{
		case APPLICATION_DESCRIPTOR:
			err = dvb_ait_parse_application_desc(p, (ait_application_desc_t *)desc->data);
		break;
		
		case APPLICATION_NAME_DESCRIPTOR:
			err = dvb_ait_parse_application_name_desc(p, (ait_application_name_desc_t *)desc->data);
		break;

		case TRANSPORT_PROTOCOL_DESCRIPTOR:
			err = dvb_ait_parse_transport_protocol_desc(p, (ait_transport_protocol_desc_t *)desc->data);
		break;

		case DVB_J_APPLICATION_DESCRIPTOR:
		break;

		case DVB_J_APPLICATION_LOCATION_DESCRIPTOR:
		break;

		case EXTERNAL_APPLICATION_AUTHORISATION_DESCRIPTOR:
			err = dvb_ait_parse_external_application_auth_desc(p, (ait_external_application_auth_desc_t *)desc->data);
		break;

		case APPLICATION_RECORDING_DESCRIPTOR:
			err = dvb_ait_parse_application_recording_desc(p, (ait_application_recording_desc_t *)desc->data);
		break;

		case APPLICATION_ICONS_DESCRIPTOR:
			err = dvb_ait_parse_application_icons_desc(p, (ait_application_icons_desc_t *)desc->data);
		break;

		case APPLICATION_STORAGE_DESCRIPTOR:
			err = dvb_ait_parse_application_storage_desc(p, (ait_application_storage_desc_t *)desc->data);
		break;

		case GRAPHICS_CONSTRAINTS_DESCRIPTOR:
			err = dvb_ait_parse_graphics_constraints_desc(p, (ait_graphics_constraints_desc_t *)desc->data);
		break;

		case SIMPLE_APPLICATION_LOCATION_DESCRIPTOR:
			err = dvb_ait_parse_simple_application_location_desc(p, (ait_simple_application_location_desc_t *)desc->data);
		break;

		case APPLICATION_USAGE_DESCRIPTOR:
			err = dvb_ait_parse_application_usage_desc(p, (ait_application_usage_desc_t *)desc->data);
		break;

		case SIMPLE_APPLICATION_BOUNDARY_DESCRIPTOR:
			err = dvb_ait_parse_simple_application_boundary_desc(p, (ait_simple_application_boundary_desc_t *)desc->data);
		break;

		default:
		break;
	}

	return err;
}

