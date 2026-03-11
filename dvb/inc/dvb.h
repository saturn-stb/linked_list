/******************************************************************************
*
* dvb.h
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
#ifndef DVB_H
#define DVB_H

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
// MPEG-TS(Transport Stream) 의 TS 패킷의 전체 길이
#define DTV_TS_PACKET_LEN 				188
	

#define DTV_INVALID_PID					0x1FFF
#define DTV_INVALID_NETWORK_ID			0xFFFF
#define DTV_INVALID_ORG_NETWORK_ID		0xFFFF
#define DTV_INVALID_ID					0xFF
#define DTV_INVALID_TYPE				0xFF
#define DTV_INVALID_VERSION				0xFF
#define DTV_INVALID_TABLE_ID			0xFF

#define DTV_INVALID_TS_ID				0xFFFF
#define DTV_INVALID_SERVICE_ID			0xFFFF // 0x0
#define DTV_INVALID_SVC_ID				0xFFFF
#define DTV_INVALID_CA_SYSTEM_ID		0xFFFF
#define DTV_INVALID_EVENT_ID			0xFFFF

#define DTV_SERVICE_ID_MASK				0xFFFF
#define DTV_PID_MASK					0x1FFF

#define LCN_UNDEFINED					0

////////////////////////////////////////////////////////////////////////
// Section Table PID
////////////////////////////////////////////////////////////////////////
#define PAT_PID			0x0000
#define CAT_PID			0x0001
	
#define NIT_PID			0x0010
#define SDT_PID			0x0011
#define BAT_PID			0x0011
#define EIT_PID			0x0012
#define RST_PID			0x0013
#define TDT_PID			0x0014
#define TOT_PID			0x0014
#define STF_PID			0x0014	/* Stuffing Table */
/* 0x15 - 0x1F */		/* Reserved for future use */

#define CANAL_CH_PID	0x00d3
#define CANAL_EPG_PID	0x00d2

/* ATSC -PSIP (Program and System Information Protocol) for Terrestrial and Cable */
#define PSIP_MGT_PID 		0x1FFB // Master Guide Table
#define PSIP_TVCT_PID		0x1FFB // Terrestrial Virtual Channel Table
#define PSIP_CVCT_PID		0x1FFB // Cable Virtual Channel Table
#define PSIP_RRT_PID		0x1FFB // Rating Region Table
#define PSIP_EIT_PID		0x1FFB // Event Information Table
#define PSIP_ETT_PID		0x1FFB // Extended Text Table
#define PSIP_STT_PID		0x1FFB // System Time Table
#define PSIP_DCCT_PID		0x1FFB // Directed Channel Change Table
#define PSIP_DCCSCT_PID		0x1FFB // Directed Channel Change Selection Code Table

////////////////////////////////////////////////////////////////////////
// Section Table ID
////////////////////////////////////////////////////////////////////////
#define PAT_TID			0x00
#define PMT_TID			0x02
#define CAT_TID			0x01
/* 0x03 - 0x3F */	/* Reserved for future use */
#define DSI_TID			0x3B
#define DII_TID			0x3B
#define DDB_TID			0x3C
#define NIT_A_TID		0x40
#define NIT_O_TID		0x41
#define SDT_A_TID		0x42
#define SDT_O_TID		0x46
#define BAT_TID			0x4A
#define EIT_PF_A_TID	0x4E
#define EIT_PF_O_TID	0x4F
#define EIT_SCH_A_0_TID	0x50
#define EIT_SCH_A_1_TID	0x51
#define EIT_SCH_A_2_TID	0x52
#define EIT_SCH_A_3_TID	0x53
#define EIT_SCH_A_4_TID	0x54
#define EIT_SCH_A_5_TID	0x55
#define EIT_SCH_A_6_TID	0x56
#define EIT_SCH_A_7_TID	0x57
#define EIT_SCH_A_8_TID	0x58
#define EIT_SCH_A_9_TID	0x59
#define EIT_SCH_A_A_TID	0x5A
#define EIT_SCH_A_B_TID	0x5B
#define EIT_SCH_A_C_TID	0x5C
#define EIT_SCH_A_D_TID	0x5D
#define EIT_SCH_A_E_TID	0x5E
#define EIT_SCH_A_F_TID	0x5F
#define EIT_SCH_O_0_TID	0x60
#define EIT_SCH_O_1_TID	0x61
#define EIT_SCH_O_2_TID	0x62
#define EIT_SCH_O_3_TID	0x63
#define EIT_SCH_O_4_TID	0x64
#define EIT_SCH_O_5_TID	0x65
#define EIT_SCH_O_6_TID	0x66
#define EIT_SCH_O_7_TID	0x67
#define EIT_SCH_O_8_TID	0x68
#define EIT_SCH_O_9_TID	0x69
#define EIT_SCH_O_A_TID	0x6A
#define EIT_SCH_O_B_TID	0x6B
#define EIT_SCH_O_C_TID	0x6C
#define EIT_SCH_O_D_TID	0x6D
#define EIT_SCH_O_E_TID	0x6E
#define EIT_SCH_O_F_TID	0x6F
#define TDT_TID			0x70
#define RST_TID			0x71
#define TOT_TID			0x73
#define	AIT_TID			0x74		/* HBBTV - Application Information Sectio */

#define CA_ECM0_TID 	0x80
#define CA_ECM1_TID 	0x81

#define CA_EMM0_TID 	0x82
#define CA_EMM1_TID 	0x83

#define SGT_TID 		0x91 // HD plus

#define DCII_TID		0xC6

#define DIT_TID 		0xFE
#define DCS_TID 		0xFE

#define TID_ACTUAL 		0
#define TID_OTHER 		1

////////////////////////////////////////////////////////////////////////
/* ASTRA APS(ASTRA PLATFORM SERVICE) - SGT(SERVICE GUIDE TABLE) */
// SGT_PID - pid of es data type 0x05(ES_PRIVATE_SECTIONS) in PMT
#define SGT_TID			0x91

/* SKYLINK (CSLINK) - FNT(FASTSCAN NETWORK TABLE) */
#define FNT_TID			0xBC

/* SKYLINK (CSLINK) - FNT(FASTSCAN SERVICE TABLE) */
#define FST_TID			0xBD

////////////////////////////////////////////////////////////////////////
#define PSIP_MGT_TID  	0xC7 // Master Guide Table
#define PSIP_TVCT_TID  	0xC8 // Terrestrial Virtual Channel Table
#define PSIP_CVCT_TID  	0xC9 // Cable Virtual Channel Table
#define PSIP_RRT_TID  	0xCA // Rating Region Table
#define PSIP_EIT_TID  	0xCB // Event Information Table
#define PSIP_ETT_TID  	0xCC // Extended Text Table
#define PSIP_STT_TID  	0xCD // System Time Table
#define PSIP_DCCT_TID  	0xD3 // Directed Channel Change Table
#define PSIP_DCCSCT_TID 0xD4 // Directed Channel Change Selection Code Table

/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
#define ISO_LANGUAGE_CODE 	3
#define ISO_COUNTRY_CODE 	3

/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
/* Descriptor size */
#define	SIZE_DESCRIPTOR_TAG_FIELD					1
#define	SIZE_DESCRIPTOR_LENGTH_FIELD				1

#define DESCRIPTOR_REGISTRATION						0x05
#define DESCRIPTOR_DATA_STREAM_ALIGNMENT			0x06
#define DESCRIPTOR_ISO_LAN							0x0a
#define DESCRIPTOR_CA			 					0x09
#define DESCRIPTOR_MULTI_BUFFER_UTIL 				0x0C
#define DESCRIPTOR_MAXIMUM_BITRATE 					0x0E
#define DESCRIPTOR_CAROUSEL_ID						0x13

#define DESCRIPTOR_MPEG4_AAC_PLUS					0x1C
#define DESCRIPTOR_EMM_ADDRESS						0x23
#define DESCRIPTOR_AVC								0x28 // Advanced Video Coding
#define DESCRIPTOR_MPEG2_AAC						0x2B

#define DESCRIPTOR_NW_NAME			 				0x40
#define DESCRIPTOR_SERVICE_LIST				   		0x41
#define DESCRIPTOR_STUFFING						 	0x42
#define DESCRIPTOR_SAT_DEL_SYS	   					0x43
#define DESCRIPTOR_CABLE_DEL_SYS					0x44
#define DESCRIPTOR_BOUQUET_NAME						0x47
#define DESCRIPTOR_SERVICE		    				0x48
#define DESCRIPTOR_COUNTRY_AVAIL	  			  	0x49
#define DESCRIPTOR_LINKAGE		    				0x4A
#define DESCRIPTOR_NVOD_REF		    				0x4B
#define DESCRIPTOR_TIME_SHIFTED_SERVICE				0x4C
#define DESCRIPTOR_SHORT_EVENT						0x4D
#define DESCRIPTOR_EXTENDED_EVENT					0x4E
#define DESCRIPTOR_TIME_SHIFTED_EVENT				0x4F
#define DESCRIPTOR_COMPONENT		   			 	0x50
#define DESCRIPTOR_MOSAIC							0x51
#define DESCRIPTOR_STREAM_ID						0x52
#define DESCRIPTOR_CA_IDENT		    				0x53
#define DESCRIPTOR_CONTENT		    				0x54
#define DESCRIPTOR_PARENTAL_RATING	    			0x55
#define DESCRIPTOR_TELETEXT		    				0x56
#define DESCRIPTOR_TELEPHONE						0x57
#define DESCRIPTOR_LOCAL_TIME_OFF					0x58
#define DESCRIPTOR_SUBTITLING						0x59
#define DESCRIPTOR_TERR_DEL_SYS						0x5A
#define DESCRIPTOR_ML_NW_NAME						0x5B
#define DESCRIPTOR_ML_BQ_NAME						0x5C
#define DESCRIPTOR_ML_SERVICE_NAME					0x5D
#define DESCRIPTOR_ML_COMPONENT						0x5E
#define DESCRIPTOR_PRIV_DATA_SPEC					0x5F
#define DESCRIPTOR_SERVICE_MOVE						0x60
#define DESCRIPTOR_SHORT_SMOOTH_BUF					0x61
#define DESCRIPTOR_FREQUENCY_LIST					0x62
#define DESCRIPTOR_PARTIAL_TP_STREAM				0x63
#define DESCRIPTOR_DATA_BROADCAST					0x64
#define DESCRIPTOR_CA_SYSTEM						0x65
#define DESCRIPTOR_DATA_BROADCAST_ID				0x66
#define DESCRIPTOR_AC3								0x6A
#define DESCRIPTOR_APPLICATION_SIGNALLING			0x6F

#define DESCRIPTOR_CONTENT_IDENTIFIER				0x76
#define DESCRIPTOR_S2_SAT_DEL_SYS					0x79
#define DESCRIPTOR_ENHANCED_AC3						0x7A
#define DESCRIPTOR_DVB_AAC							0x7C
#define DESCRIPTOR_CONTENT_MANAGEMENT				0x7E

// User define
// DTG_SIPSI
#define DESCRIPTOR_SERVICE_ATTRIBUTE				0x86
#define DESCRIPTOR_SHORTNAME						0x87

// DTG_SIPSI, HD_SIMULCAST_LCN
#define DESCRIPTOR_HD_SIMULCAST_LOGICAL_CHANNEL		0x88

// HD_PLUS_SCAN
#define DESCRIPTOR_SERVICE_LIST_NAME				0x88
#define DESCRIPTOR_BOUQUET_LIST_NAME				0x93
#define DESCRIPTOR_VIRTUAL_SERVICE_ID				0xD1

// CLOSED_CAPTION
#define DESCRIPTOR_ATSC_CLOSED_CAPTION				0x86

#define DESCRIPTOR_DVBC_LOGICAL_CHANNEL_NUM			0x82
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM				0x83
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2			0x87

#define DESCRIPTOR_CIT_REFERENCE					0x90

// CLARO_CHANNEL_NUMBERING
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_CHL			0x90 // Chile
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_ECU			0x91 // Ecuador
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_PER			0x92 // Peru
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_COL			0x93 // Colombia
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_PRY			0x94 // Paraguay
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_URY			0x95 // Uruguay
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_ARG			0x97 // Argentina

#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V1_CHL			0xA0 // Chile
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V1_ECU			0xA1 // Ecuador
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V1_PER			0xA2 // Peru
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V1_COL			0xA3 // Colombia
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V1_PRY			0xA4 // Paraguay
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V1_URY			0xA5 // Uruguay
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V1_ARG			0xA7 // Argentina

#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2_CHL			0xB0 // Chile
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2_ECU			0xB1 // Ecuador
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2_PER			0xB2 // Peru
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2_COL			0xB3 // Colombia
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2_PRY			0xB4 // Paraguay
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2_URY			0xB5 // Uruguay
#define DESCRIPTOR_LOGICAL_CHANNEL_NUM_V2_ARG			0xB7 // Argentina

#define DESCRIPTOR_LESRADIOS						0xC5

// CANAL_SUPPORT
#define DESCRIPTOR_CANAL_AUDIO_NAME					0xC5
#define DESCRIPTOR_CANAL_DATA						0xC6

#define DESCRIPTOR_MHW_DATA							0xC2

// CIPLUS_SUPPORT
#define DESCRIPTOR_CIPLUS_CONTENT_LABEL				0xCB
#define DESCRIPTOR_CIPLUS_SERVICE					0xCC
#define DESCRIPTOR_CIPLUS_PROTECTION				0xCE


// BAT_LOGICAL_CHNUM
// BSKYB_CHANNEL_NUMBERING
#define DESCRIPTOR_LOGICAL_CHANNEL					0xB1 // BSKYB (British Sky Broadcast)
//#define DESCRIPTOR_LOGICAL_CHANNEL					0xD1

#define DESCRIPTOR_CONTENTS_ORDER					0xF0
#define DESCRIPTOR_PARENTAL_INFORMATION				0xF1
#define DESCRIPTOR_CONTENTS_TRANSMISSION			0xF2

#define DESCRIPTOR_ARIB_SUBTITLE					0xFD

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
typedef enum STREAM_TYPE {
	/* ITU-T | ISO/IEC Reserved */
	ES_RESERVED         = 0x00,
	/* ISO/IEC 11172 Video */
	ES_MPEG1_VIDEO      = 0x01,
	/* ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream */
	ES_MPEG2_VIDEO      = 0x02,
	/* ISO/IEC 11172 Audio */
	ES_MPEG1_AUDIO      = 0x03,
	/* ISO/IEC 13818-3 Audio */
	ES_MPEG2_AUDIO		= 0x04,
	/* ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections */
	ES_PRIVATE_SECTIONS = 0x05,
	/* ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data */
	ES_PRIVATE_PES      = 0x06,
	/* ISO/IEC 13522 MHEG */
	ES_MHEG				= 0x07,
	/* ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM CC */
	ES_DSM_CC           = 0x08,
	/* ITU-T Rec. H.222.0 | ISO/IEC 13818-1/11172-1 auxiliary */
	ES_11171_1_AUXILIARY= 0x09,
	/* ISO/IEC 13818-6 Multiprotocol encapsulation */
	ES_13818_6_A        = 0x0A,
	/* ISO/IEC 13818-6 DSM-CC U-N Messages */
	ES_13818_6_B        = 0x0B,
	/* ISO/IEC 13818-6 Stream Descriptors */
	ES_13818_6_C        = 0x0C,
	/* ISO/IEC 13818-6 Sections (any type, including private data) */
	ES_13818_6_D        = 0x0D,
	/* ISO/IEC 13818-1 auxiliary */
	ES_13818_1_AUXILIARY= 0x0E,
	/* ISO/IEC 13818-7 Audio with ADTS transport sytax (MPEG4_AAC) */
	ES_AAC_AUDIO		= 0x0F,
	/* ISO/IEC 14496-2 Visual */
	ES_14496_2_VISUAL   = 0x10,
	/* MPEG-4 Part 2 Video */
	ES_MPEG4_PART2_VIDEO = ES_14496_2_VISUAL,
	/* ISO/IEC 14496-3 Audio with LATM transport syntax as def. in ISO/IEC 14496-3/AMD1 (AAC_PLUS) */
	ES_14496_3_AUDIO_LATM = 0x11,
	/* MPEG-4 HE-AAC AUDIO LATM/LOAS */
	ES_AAC_PLUS_AUDIO = ES_14496_3_AUDIO_LATM,
	/* ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets */
	ES_14496_1_PES        = 0x12,
	/* MPEG-4 HE-AAC AUDIO ADTS */
	ES_AAC_PLUS_AUDIO_ADTS = ES_14496_1_PES,
	/* ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC 14496 sections */
	ES_14496_1_14496_SECTION = 0x13,
	/* ISO/IEC 13818-6 DSM-CC synchronized download protocol */
	ES_13818_6_DSMCC_DOWNLOAD = 0x14,
	/* Metadata carried in PES packets using the Metadata Access Unit Wrapper */
	ES_METADATA_PES = 0x15,
	/* Metadata carried in metadata_sections */
	ES_METADATA_SECTION = 0x16,
	/* Metadata carried in ISO/IEC 13818-6 (DSM-CC) Data Carousel */
	ES_METADATA_13818_6_DATA_CAROUSEL = 0x17,
	/* Metadata carried in ISO/IEC 13818-6 (DSM-CC) Object Carousel */
	ES_METADATA_13818_6_OBJECT_CAROUSEL = 0x18,
	/* Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol using the Metadata Access Unit Wrapper */
	ES_MEATADATA_13818_6_SDP = 0x19,
	/* IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP) */
	ES_IPMP = 0x1A,
	/* AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video */
	ES_H264_VIDEO		= 0x1B,
	/* HEVC video stream as defined in ITU-T Rec. H.265 */
	ES_HEVC_VIDEO_TIVUSAT		= 0x1F,
	/* HEVC ITU-T Recommendation H.265 | ISO/IEC 23008-2:2013, Rec. ITU-R BT. 2100 For the HDR part */
	ES_H265_VIDEO		= 0x21,
	/* HEVC ITU-T Recommendation H.265 | ISO/IEC 23008-2:2013, Rec. ITU-R BT. 2100 For the HDR part */
	ES_HEVC_VIDEO		= 0x24,
	/* ITU-T Rec. H.222.0 | ISO/IEC 13818-1 reserved (0x1C ~ 0xFE) */
	ES_AUDIO_AC3 = 0x71,
	ES_AUDIO_AC3_PLUS = 0x72,
	/* IPMP stream */
	ES_MPEG2_IPMP0x7F	= 0x7F,
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	#if 1 /* DCII subtitle (Digicipher) - SCTE-27 */
	ES_DCII_SUBT_SCTE27	= 0x82,
	#endif
	/* PSIP STREAM TYPE */
	/* Audio per ATSC A/53 Part 5 [4], AC3 */
	/* Sections conveying A/90 [17] Data Service Table, Network Resources Table */
	ES_ATSC_PRIVATE_SECTION = 0x95,
	/* PES packets containing A/90 [17] streaming, synchronous data */
	ES_ATSC_PRIVATE_PES = 0xC2,
} STREAM_TYPE;

/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
typedef enum TELETEXT_TYPE {
	TELETEXT_INITIAL_PAGE                   = 0x01,
	TELETEXT_SUBTITLE_PAGE                  = 0x02,
	TELETEXT_ADDITIONAL_INFO_PAGE           = 0x03,
	TELETEXT_PROGRAM_SCHEDULE_PAGE          = 0x04,
	TELETEXT_SUBTITLE_PAGE_FOR_HARD_HEARING = 0x05,
} TELETEXT_TYPE;


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
typedef enum SUBTITLING_TYPE {
	SUBTITLING_DCII_SCTE27       			= 0x04,
	SUBTITLING_NORMAL_NO_ASPECT_RATIO       = 0x10,
	SUBTITLING_NORMAL_4x3                   = 0x11,
	SUBTITLING_NORMAL_16x9                  = 0x12,
	SUBTITLING_NORMAL_221x1                 = 0x13,
	SUBTITLING_HARD_HEARING_NO_ASPECT_RATIO = 0x20,
	SUBTITLING_HARD_HEARING_4x3             = 0x21,
	SUBTITLING_HARD_HEARING_16x9            = 0x22,
	SUBTITLING_HARD_HEARING_221x1           = 0x23,
} SUBTITLING_TYPE;


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
typedef enum LINKAGE_TYPE {
	LINKAGE_INFORMATION_SERVICE				= 0x01,
	LINKAGE_EPG_SERVICE						= 0x02,
	LINKAGE_CA_REPLACEMENT_SERVICE			= 0x03,
	LINKAGE_TS_CONTAINING_NETWORK_BOUQUET_SI= 0x04,
	LINKAGE_SERVICE_REPLACEMENT_SERVICE		= 0x05,
	LINKAGE_DATA_BROADCAST_SERVICE			= 0x06,
	LINKAGE_SYSTEM_SOFTWARE_UPDATE_SERVICE	= 0x09,
} LINKAGE_TYPE;


/*-----------------------------------------------------------------------------
 * PSIP - TVCT(Terrestrial Virtual Channel Table) Modulation Modes
 *
 *
 *---------------------------------------------------------------------------*/
typedef enum TVCT_MODULATION_MODE {
	TVCT_MOD_RESERVED 	= 0x00,
	TVCT_MOD_ANALOG 	= 0x01, // Analog . The virtual channel is modulated using standard analog methods for analog television.
	TVCT_MOD_SCTE_MODE1 = 0x02, // SCTE_mode_1 . The virtual channel has a symbol rate of 5.057 Msps, transmitted in accordance with ANSI/SCTE 07 [21] (Mode 1). Typically, mode 1 will be used for 64-QAM.
	TVCT_MOD_SCTE_MODE2 = 0x03, // SCTE_mode_2 . The virtual channel has a symbol rate of 5.361 Msps, transmitted in 	accordance with ANSI/SCTE 07 [21] (Mode 2). Typically, mode 2 will be used for 256-QAM.
	TVCT_MOD_ATSC_8VSB 	= 0x04, // ATSC (8 VSB) . The virtual channel uses the 8-VSB modulation method conforming to A/53 Part 2 [2].
	TVCT_MOD_ATSC_16VSB = 0x05, // ATSC (16 VSB) . The virtual channel uses the 16-VSB modulation method conforming to A/53 Part 2 [2].
} TVCT_MODULATION_MODE;


/*-----------------------------------------------------------------------------
 *
 *
 *
 *---------------------------------------------------------------------------*/
typedef enum
{
	CONT_UNDEFINED_00,   /* 0x00  undefined content                   */
	CONT_MOVIE,          /* 0x01  movies                              */
	CONT_NEWS,           /* 0x02  news/current affairs                */
	CONT_SHOW,           /* 0x03  show/game show                      */
	CONT_SPORT,          /* 0x04  sports                              */
	CONT_CHILDREN,       /* 0x05  children's/youth programmes         */
	CONT_MUSIC,          /* 0x06  music/ballet/dance                  */
	CONT_ART,            /* 0x07  arts/culture (without music)        */
	CONT_SOCIAL,         /* 0x08  social/political issues/economics   */
	CONT_EDUCATION,      /* 0x09  education/science/factual topics    */
	CONT_LEISURE,        /* 0x0A  leisure hobbies                     */
	CONT_UNDEFINED_0B,   /* 0x0B  undefined content                   */
	CONT_UNDEFINED_0C,   /* 0x0C  undefined content                   */
	CONT_UNDEFINED_0D,   /* 0x0D  undefined content                   */
	CONT_UNDEFINED_0E,   /* 0x0E  undefined content                   */
	CONT_USER_DEFINED    /* 0x00  user defined                        */

}  CONTENT_GENRE;

/* MOVIE/DRAMA */
typedef enum
{
	GENRE1_MOVIE_DRAMA,
	GENRE1_DETECTIVE_THRILLER,
	GENRE1_ADVENTURE_WESTERN_WAR,
	GENRE1_SCIENCE_FICTION_FANTASY_HORROR,
	GENRE1_COMEDY,
	GENRE1_SOAP_MELODRAMA_FOLKLORIC,
	GENRE1_ROMANCE,
	GENRE1_SERIOUS_CLASSICAL_RELIGIOUS_HISTORICAL_MOVE_DRAMA,
	GENRE1_ADULT_MOVIE_DRAMA,
	GENRE1_RESERVED = 0x09, /*0x09 ~ 0x0E */
	GENRE1_USER_DEFINED = 0x0F

}  GENRE_1;

/* NEWS/CURRENT AFFAIRS */
typedef enum
{
	GENRE2_NEWS_CURRENT_AFFAIRS_GENERAL,
	GENRE2_NEWS_WEATHER_REPORT,
	GENRE2_NEWS_MAGAZINE,
	GENRE2_DOCUMENTARY,
	GENRE2_DISCUSSION_INTERVIEW_DEBATE,
	GENRE2_RESERVED = 0x05, /*0x05 ~ 0x0E */
	GENRE2_USER_DEFINED = 0x0F

}  GENRE_2;

/* SHOW/GAME SHOW */
typedef enum
{
	GENRE3_SHOW_GAME_SHOW_GENERAL,
	GENRE3_GAME_SHOW_QUIZ_CONTEST,
	GENRE3_VARIETY_SHOW,
	GENRE3_TALK_SHOW,
	GENRE3_RESERVED = 0x04, /*0x04 ~ 0x0E */
	GENRE3_USER_DEFINED = 0x0F

}  GENRE_3;

/* SPORT */
typedef enum
{
	GENRE4_SPORTS_GENERAL,
	GENRE4_SPECIAL_EVENTS_OLYMPIC_GAME_WORLD_CUP_ETC,
	GENRE4_SPORTS_MAGAZINES,
	GENRE4_FOOBALL_SOCCER,
	GENRE4_TENNIS_SQUASH,
	GENRE4_TEAM_SPORTS_EXCLUDING_FOOBAL,
	GENRE4_ATHLETICS,
	GENRE4_MOTOR_SPORT,
	GENRE4_WATER_SPORT,
	GENRE4_WINTER_SPORTS,
	GENRE4_EQUSTRIAN,
	GENRE4_MARTIAL_SPORTS,
	GENRE4_RESERVED = 0x0C, /*0x0C ~ 0x0E */
	GENRE4_USER_DEFINED = 0x0F

}  GENRE_4;

/* CHILDREN'S/YOUTH PROGRAMMES */
typedef enum
{
	GENRE5_CHILDREN_YOUTH_PROGRAMMES_GENERAL,
	GENRE5_PRESCHOOL_CHILDREN_PROGRAMMES,
	GENRE5_ENTERTAINMENT_PROGRAMMES_FOR_6_TO_14,
	GENRE5_ENTERTAINMENT_PROGRAMMES_FOR_10_TO_16,
	GENRE5_INFORMATIONAL_EDUCATION_SCHOOL_PROGRAMMES,
	GENRE5_CARTOONS_PUPPETS,
	GENRE5_RESERVED = 0x06, /*0x06 ~ 0x0E */
	GENRE5_USER_DEFINED = 0x0F

}  GENRE_5;

/* MUSIC/BALLET/DANCE */
typedef enum
{
	GENRE6_MUSIC_BALLET_DANCE_GENERAL,
	GENRE6_ROCK_POP,
	GENRE6_SERIOUS_MUSIC_CLASSICAL_MUSIC,
	GENRE6_FOLK_TRADITIONAL_MUSIC,
	GENRE6_JAZZ,
	GENRE6_MUSICAL_OPERA,
	GENRE6_BALLET,
	GENRE6_RESERVED = 0x07, /*0x07 ~ 0x0E */
	GENRE6_USER_DEFINED = 0x0F

}  GENRE_6;

/* ARTS/CULTURE/ (without music) */
typedef enum
{
	GENRE7_ARTS_CULTURE_GENERAL,
	GENRE7_PERFOMING_ARTS,
	GENRE7_FINE_ARTS,
	GENRE7_RELIGION,
	GENRE7_POPULAR_CULTURE_TRADITIONAL_ARTS,
	GENRE7_LITERATURE,
	GENRE7_FILM_CINEMA,
	GENRE7_EXPERIMENTAL_FILM_VIDEO,
	GENRE7_BROADCASTING_PRESS,
	GENRE7_NEW_MEDIA,
	GENRE7_ARTS_CULTURE_MAGAZINES,
	GENRE7_FASHION,
	GENRE7_RESERVED = 0x0C, /*0x0C ~ 0x0E */
	GENRE7_USER_DEFINED = 0x0F

}  GENRE_7;

/* SOCIAL/POLITICAL ISSUES/ECONOMICS */
typedef enum
{
	GENRE8_SOCIAL_POLITICAL_ISSUES_ECONOMICS_GENERAL,
	GENRE8_MAGAZINES_REPORTS_DOCUMENTARY,
	GENRE8_ECONOMICS_SOCIAL_ADVISORY,
	GENRE8_REMARKABLE_PEOPLE,
	GENRE8_RESERVED = 0x04, /*0x04 ~ 0x0E */
	GENRE8_USER_DEFINED = 0x0F

}  GENRE_8;

/* EDUCATION/SCIENCE/FACTURAL TOPICS */
typedef enum
{
	GENRE9_EDUCATION_SCIENCE_FACTURAL_TOPICS_GENERAL,
	GENRE9_NATURE_ANIMALS_ENVIROMENT,
	GENRE9_TECHNOLGY_NATUAL_SCIENCES,
	GENRE9_MEDICINE_PHYSIOLOGY_PSYCHOLOGY,
	GENRE9_FOREIGN_COUNTRIES_EXPEDITIONS,
	GENRE9_SOCIAL_SPIRITUAL_SCIENCES,
	GENRE9_FURTHER_EDUCATION,
	GENRE9_LANGUAGES,
	GENRE9_RESERVED = 0x08, /*0x08 ~ 0x0E */
	GENRE9_USER_DEFINED = 0x0F

}  GENRE_9;

/* LEISURE HOBBIES */
typedef enum
{
	GENRE10_LEISURE_HOBBIES_GENERAL,
	GENRE10_TOURISM_TRAVEL,
	GENRE10_HANDCRAFT,
	GENRE10_MOTORING,
	GENRE10_FITNESS_AND_HEALTH,
	GENRE10_COOKING,
	GENRE10_ADVERTISEMENT_SHOPPING,
	GENRE10_GARDENING,
	GENRE10_RESERVED = 0x08, /*0x08 ~ 0x0E */
	GENRE10_USER_DEFINED = 0x0F

}  GENRE_10;

/* << data strea alignment descript >>
	data_stream_alignment_descriptor() {		
		descriptor_tag			8	uimsbf
		descriptor_length		8	uimsbf
		alignment_type			8	uimsbf
	}		

	valid video stream alignment_type values

	alignment_type description 
	1 / 0x01 Slice, or video access unit 
	2 / 0x02 video access unit 
	3 / 0x03 Group of Pictures (GOP) or SEQ 
	4 / 0x04 SEQ 

	valid audio stream alignment_type values

	alignment_type description 
	1 / 0x01 syncword 
*/

typedef enum
{
	VideoStreamAlignment_Unknown = 0x0,
	VideoStreamAlignment_Slice = 0x1, 
	VideoStreamAlignment_videoAccessUnit = 0x2,
	VideoStreamAlignment_GroupOfPictures = 0x3,
	VideoStreamAlignment_SEQ = 0x4,
	
} TYPE_VideoStreamAlignment;

typedef enum
{
	AudioStreamAlignment_Unknown = 0x0,
	AudioStreamAlignment_Syncword = 0x1,
} TYPE_AudioStreamAlignment;

/* In the linkage descriptor */
typedef enum _LINKAGE_TYPE_DESC_T
{
	LINKAGE_TYPE_RESERVED = 0,
	INFORMATION_SERVICE,
	EPG_SERVICE,
	CA_REPLACEMENT_SERVICE,
	TS_CONTAINING_COMPLETE_NETWORK,
	SERVICE_REPLACEMENT_SERVICE,
	DATA_BROCAST_SERVICE,
	RCS_MAP,
	MOBILE_HAND_OVER,
	SYSTEM_SOFTWARE_UPDATE_SERVICE,
	TS_CONTAINING_SSU_BAT_NIT,
	IP_MAC_NOTIFICATION_SERVICE,
	TS_CONTAINING_INT_BAT_NIT,

	// HD_PLUS_SCAN
	ASTRA_SERIVCE_GUIDE = 0x90, // HD+ LCN
	ASTRA_SERIVCE_AUSTRIA_LCN = 0x91, // Austria LCN
	ASTRA_SERIVCE_INTERNATIONAL_LCN = 0x93, // International LCN

	// BAT_LOGICAL_CHNUM
	BOUQUET_ID_LIST = 0x9B, // bouquet ID LIST

	// HD_PLUS_SCAN
	ASTRA_SERIVCE_WHITE_LABEL_PLATFORM_LCN = 0xA2, // White Label Platform LCN

	MAX_LINKAGE_TYPE_DESC
} LINKAGE_TYPE_DESC_T;

/******************************************************************************
 *
 *
 *
 *****************************************************************************/
/*-----------------------------------------------------------------------------
 *
 * descriptor tag
 *
 *---------------------------------------------------------------------------*/
#define DESC_TAG_VIDEO_STREAM					0x02
#define DESC_TAG_AUDIO_STREAM					0x03
#define DESC_TAG_HIERARCHY						0x04
#define DESC_TAG_REGISTRATION					0x05
#define DESC_TAG_DATA_STREAM_ALIGN				0x06
#define DESC_TAG_TARGET_BG_GRID					0x07
#define DESC_TAG_VIDEO_WINDOW					0x08
#define DESC_TAG_CA								0x09
#define DESC_TAG_ISO_639_LANG					0x0A
#define DESC_TAG_SYSTEM_CLOCK					0x0B
#define DESC_TAG_MULTI_BUFFER_UTIL				0x0C
#define DESC_TAG_COPYRIGHT						0x0D
#define DESC_TAG_MAX_BITRATE					0x0E
#define DESC_TAG_PRIVATE_DATA_INDICATOR			0x0F
#define DESC_TAG_SMOOTHING_BUFFER				0x10
#define DESC_TAG_CAROUSEL_ID					0x13
#define DESC_TAG_ASSOCIATION					0x14
#define DESC_TAG_MPEG4_AAC_PLUS					0x1C
#define DESC_TAG_AVC							0x28
#define DESC_TAG_MPEG2_AAC						0x2B

#define DESC_TAG_NETWORK_NAME					0x40
#define DESC_TAG_SERVICE_LIST					0x41
#define DESC_TAG_STUFFING						0x42
#define DESC_TAG_SAT_DELIVERY_SYSTEM			0x43
#define DESC_TAG_CABLE_DELIVERY_SYSTEM			0x44
#define DESC_TAG_VBI_DATA						0x45
#define DESC_TAG_VBI_TELETEXT					0x46
#define DESC_TAG_BOUQUET_NAME					0x47
#define DESC_TAG_SERVICE						0x48
#define DESC_TAG_COUNTRY_AVAILABILITY			0x49
#define DESC_TAG_LINKAGE						0x4A
#define DESC_TAG_NVOD_REF						0x4B
#define DESC_TAG_TIME_SHIFTED_SERVICE			0x4C
#define DESC_TAG_SHORT_EVENT					0x4D
#define DESC_TAG_EXTENDED_EVENT					0x4E
#define DESC_TAG_TIME_SHIFTED_EVENT				0x4F
#define DESC_TAG_COMPONENT						0x50
#define DESC_TAG_MOSAIC							0x51
#define DESC_TAG_STREAM_IDENTIFIER				0x52
#define DESC_TAG_CA_IDENTIFIER					0x53
#define DESC_TAG_CONTENT						0x54
#define DESC_TAG_PARENTAL_RATING				0x55
#define DESC_TAG_TELETEXT						0x56
#define DESC_TAG_TELEPHONE						0x57
#define DESC_TAG_LOCAL_TIME_OFFSET				0x58
#define DESC_TAG_SUBTITLING						0x59
#define DESC_TAG_TER_DELIVERY_SYSTEM			0x5A
#define DESC_TAG_MULTILINGUAL_NETWORK_NAME		0x5B
#define DESC_TAG_MULTILINGUAL_BOUQUET_NAME		0x5C
#define DESC_TAG_MULTILINGUAL_SERVICE_NAME		0x5D
#define DESC_TAG_MULTILINGUAL_COMPONNT			0x5E
#define DESC_TAG_PRIVATE_DATA_SPECIFIER			0x5F
#define DESC_TAG_SERVICE_MOVE					0x60
#define DESC_TAG_SHORT_SMOOTHING_BUFFER			0x61
#define DESC_TAG_FREQENCY_LIST					0x62
#define DESC_TAG_DATA_BROADCAST					0x64
#define DESC_TAG_SCRAMBLING 					0x65
#define DESC_TAG_DATA_BROADCAST_ID				0x66
#define DESC_TAG_PARTIAL_TRANSPORT_STREAM		0x67
#define DESC_TAG_DIGITAL_SAT_NEWS_GETHERING		0x68 // DSNG
#define DESC_TAG_PDC							0x69
#define DESC_TAG_AC3							0x6A
#define DESC_TAG_ANCILLARY_DATA					0x6B
#define DESC_TAG_CELL_LIST						0x6C
#define DESC_TAG_CELL_FREQUENCY_LINK			0x6D
#define DESC_TAG_ANNOUNCEMENT_SUPPORT			0x6E
#define DESC_TAG_APPLICATION_SIGNALLING			0x6F
#define DESC_TAG_ADAPTATION_FIELD_DATA			0x70
#define DESC_TAG_SERVICE_IDENTIFIER				0x71
#define DESC_TAG_SERVICE_AVAILABLITY			0x72
#define DESC_TAG_DEFAULT_AUTHORITY				0x73
#define DESC_TAG_RELATED_CONTENT				0x74
#define DESC_TAG_TVA_ID							0x75

/**/
#define DESC_TAG_CONTENT_IDENTIFIER				0x76
#define DESC_TAG_TIME_SLICE_FEC_IDENTIFIER 		0x77
#define DESC_TAG_ECM_REPETITION_RATE			0x78
#define DESC_TAG_S2_SAT_DELIVERY_SYSTEM			0x79
#define DESC_TAG_ENHANCED_AC3					0x7A
#define DESC_TAG_DTS							0x7B
#define DESC_TAG_AAC							0x7C
#define DESC_TAG_XAIT_LOCATION					0x7D
#define DESC_TAG_FTA_CONTENT_MANAGEMENT			0x7E
#define DESC_TAG_EXTENSION						0x7F

// User define
#define DESC_TAG_DVBC_LOGICAL_CHANNEL_NUMBER	0x82
#define DESC_TAG_DVB_LOGICAL_CHANNEL_NUMBER		0x83
// DTG_SIPSI
#define DESC_TAG_SERVICE_ATTRIBUTE 				0x86
#define DESC_TAG_SHORTNAME						0x87

#define DESC_TAG_LOGICAL_CHANNEL_NUM_V2			0x87

// DTG_SIPSI, HD_SIMULCAST_LCN
#define DESC_TAG_HD_SIMULCAST_LOGICAL_CHANNEL 	0x88


// BAT_LOGICAL_CHNUM
// BSKYB_CHANNEL_NUMBERING
#define DESC_TAG_LOGICAL_CHANNEL_NUMBER 		0xB1
//#define DESC_TAG_LOGICAL_CHANNEL_NUMBER 		0xD1

/* ASTRA APS(ASTRA PLATFORM SERVICE) - SGT(SERVICE GUIDE TABLE) */
// HD_PLUS_SCAN
#define DESC_TAG_SERVICE_LIST_NAME				0x88
#define DESC_TAG_BOUQUET_LIST					0x93
#define DESC_TAG_VIRTUAL_SERVICE_ID				0xD1

#define DESC_TAG_LESRADIOS						0xC5

// CANAL
#define DESC_TAG_CANAL_AUDIO_NAME				0xC5
#define DESC_TAG_CANAL_DATA						0xC6

#define DESC_TAG_MHW_DATA						0xC2

// CIPLUS
#define DESC_TAG_CIPLUS_CONTENT_LABEL			0xCB
#define DESC_TAG_CIPLUS_SERVICE					0xCC
#define DESC_TAG_CIPLUS_PROTECTION				0xCE

/* ATSC - PSIP(Program and System Information Protocol for Terrestrial and Cable */
// PSIP
#define DESC_TAG_ATSC_STUFFING					0x80
#define DESC_TAG_ATSC_AC3						0x81
#define DESC_TAG_ATSC_CAPTION_SERVICE 			0x86
#define DESC_TAG_ATSC_CONTENT_ADVISORY 			0x87
#define DESC_TAG_ATSC_EXTENDED_CHANNEL_NAME 	0xA0
#define DESC_TAG_ATSC_SERVICE_LOCATION 			0xA1
#define DESC_TAG_ATSC_TIMESHIFTED_SERVICE 		0xA2
#define DESC_TAG_ATSC_COMPONENT_NAME 			0xA3
#define DESC_TAG_ATSC_DCC_DEPARTING_REQUEST 	0xA8
#define DESC_TAG_ATSC_DCC_ARRIVING_REQUEST 		0xA9
#define DESC_TAG_ATSC_REDISTRIBUTION_CONTROL 	0xAA
#define DESC_TAG_ATSC_GENRE 					0xAB
#define DESC_TAG_ATSC_PRIVATE_INFORMATION 		0xAD
#define DESC_TAG_ATSC_ENHANCED_AC3		 		0xCC

// ARIB_SUBTITLE
#define DESC_TAG_ARIB_SUBTITLE 					0xFD

// HbbTV - AIT
#define APPLICATION_DESCRIPTOR							0x00
#define APPLICATION_NAME_DESCRIPTOR						0x01
#define TRANSPORT_PROTOCOL_DESCRIPTOR					0x02
#define DVB_J_APPLICATION_DESCRIPTOR					0x03
#define DVB_J_APPLICATION_LOCATION_DESCRIPTOR			0x04
#define EXTERNAL_APPLICATION_AUTHORISATION_DESCRIPTOR	0x05
#define APPLICATION_RECORDING_DESCRIPTOR				0x06
#define APPLICATION_ICONS_DESCRIPTOR					0x0B
#define APPLICATION_STORAGE_DESCRIPTOR					0x10
#define GRAPHICS_CONSTRAINTS_DESCRIPTOR					0x14
#define SIMPLE_APPLICATION_LOCATION_DESCRIPTOR			0x15
#define APPLICATION_USAGE_DESCRIPTOR					0x16
#define SIMPLE_APPLICATION_BOUNDARY_DESCRIPTOR			0x17

/******************************************************************************
 *
 *
 *
 *****************************************************************************/
/*-----------------------------------------------------------------------------
 *
 * common descriptor structure
 *
 *---------------------------------------------------------------------------*/
typedef struct descriptor_s {
	unsigned char			tag;	/* descriptor tag */
	int			 			len;	/* descriptor length */
	void					*data;	/* descriptor data */
	
	struct descriptor_s	*next;
} descriptor_t;

/*-----------------------------------------------------------------------------
 *
 * common private data structure
 *
 *---------------------------------------------------------------------------*/
typedef struct private_data_s {
	int				len;
	unsigned char			*data;
} private_data_t;


/*-----------------------------------------------------------------------------
	video_stream_descriptor(){		
		descriptor_tag						8	uimsbf
		descriptor_length					8	uimsbf
		multiple_frame_rate_flag			1	bslbf
		frame_rate_code						4	uimsbf
		MPEG_1_only_flag					1	bslbf
		constrained_parameter_flag			1	bslbf
		still_picture_flag					1	bslbf
		if (MPEG_1_only_flag == 1){		
			profile_and_level_indication	8	uimsbf
			chroma_format					2	uimsbf
			frame_rate_extension_flag		1	bslbf
			reserved						5	bslbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct video_stream_desc_s {
	unsigned char			multiple_frame_rate;
	unsigned char			frame_rate_code;
	unsigned char			mpeg1_only;
	unsigned char			constrained_param;
	unsigned char			still_picture;
} video_stream_desc_t;

/*-----------------------------------------------------------------------------
	audio_stream_descriptor(){		
		descriptor_tag 					8	uimsbf
		descriptor_length				8	uimsbf
		free_format_flag				1	bslbf
		ID								1	bslbf
		layer							2	bslbf
		variable_rate_audio_indicator	1	bslbf
		reserved						3	bslbf
	}
-----------------------------------------------------------------------------*/
typedef struct audio_stream_desc_s {
	unsigned char			free_format;
	unsigned char			id;
	unsigned char			layer;
	unsigned char			variable_rate_audio_indicator;
} audio_stream_desc_t;

/*-----------------------------------------------------------------------------
	hierarchy_descriptor() {		
		descriptor_tag					8	uimsbf
		descriptor_length				8	uimsbf
		reserved						4	bslbf
		hierarchy_type					4	uimsbf
		reserved						2	bslbf
		hierarchy_layer_index			6	uimsbf
		reserved						2	bslbf
		hierarchy_embedded_layer_index	6	uimsbf
		reserved						2	bslbf
		hierarchy_channel				6	uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct hierarchy_desc_s {
	unsigned char			hierachy_type;
	unsigned char			hierachy_layer_index;
	unsigned char			hierachy_embedded_layer_index;
	unsigned char			hierachy_channel;
} hierarchy_desc_t;

/*-----------------------------------------------------------------------------
	registration_descriptor() {		
		descriptor_tag						8	uimsbf
		descriptor_length					8	uimsbf
		format_identifier					32	uimsbf
		for (i = 0; i < N ; i++ ){		
			additional_identification_info	8	bslbf
		}		
	}		
-----------------------------------------------------------------------------*/
typedef struct registration_desc_s {
	unsigned int			format_id;
	private_data_t	*priv_data;
} registration_desc_t;

/*-----------------------------------------------------------------------------
	data_stream_alignment_descriptor() {		
		descriptor_tag			8	uimsbf
		descriptor_length		8	uimsbf
		alignment_type			8	uimsbf
	}		
-----------------------------------------------------------------------------*/
typedef struct data_stream_align_desc_s {
	unsigned char			align_type;
} data_stream_align_desc_t;

/*-----------------------------------------------------------------------------
	target_background_grid_descriptor() {		
		descriptor_tag				8	uimsbf
		descriptor_length			8	uimsbf
		horizontal_size				14	uimsbf
		vertical_size				14	uimsbf
		aspect_ratio_information	4	uimsbf
	}		
-----------------------------------------------------------------------------*/
typedef struct target_bg_grid_desc_s {
	unsigned short			hor_size;
	unsigned short			ver_size;
	unsigned char			aspect_ratio_info;
} target_bg_grid_desc_t;

/*-----------------------------------------------------------------------------
	video_window_descriptor() {		
		descriptor_tag				8	uimsbf
		descriptor_length			8	uimsbf
		horizontal_offset			14	uimsbf
		vertical_offset				14	uimsbf
		window_priority				4	uimsbf
	}		
-----------------------------------------------------------------------------*/
typedef struct video_window_desc_s {
	unsigned short			hor_offset;
	unsigned short			ver_offset;
	unsigned char			window_priority;
} video_window_desc_t;

/*-----------------------------------------------------------------------------
	CA_descriptor() {		
		descriptor_tag				8	uimsbf
		descriptor_length			8	uimsbf
		CA_system_ID				16	uimsbf
		reserved					3	bslbf
		CA_PID						13	uimsbf
		for ( i=0; i<N; i++) {		
			private_data_byte		8	uimsbf
		}		
	}		
-----------------------------------------------------------------------------*/
typedef struct ca_desc_s {
	unsigned short			ca_system_id;
	unsigned short			ca_pid;
	private_data_t	*priv_data;
} ca_desc_t;

/*-----------------------------------------------------------------------------
	system_clock_descriptor() {		
		descriptor_tag						8	uimsbf
		descriptor_length					8	uimsbf
		external_clock_reference_indicator	1	bslbf
		reserved							1	bslbf
		clock_accuracy_integer				6	uimsbf
		clock_accuracy_exponent				3	uimsbf
		reserved							5	bslbf
	}		
-----------------------------------------------------------------------------*/
typedef struct system_clock_desc_s {
	unsigned char			external_clock_ref;
	unsigned char			clock_accuracy_int;
	unsigned char			clock_accuracy_exp;
} system_clock_desc_t;

/*-----------------------------------------------------------------------------
	multiplex_buffer_utilization_descriptor() {		
		descriptor_tag				8	uimsbf
		descriptor_length			8	uimsbf
		bound_valid_flag			1	bslbf
		LTW_offset_lower_bound		15	uimsbf
		reserved					1	bslbf
		LTW_offset_upper_bound		15	uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct multi_buffer_util_desc_s {
	unsigned char			bound_valid;
	unsigned short			ltw_offset_lower_bound;
	unsigned short			ltw_offset_upper_bound;
} multi_buffer_util_desc_t;

/*-----------------------------------------------------------------------------
	copyright_descriptor() {		
		descriptor_tag					8	uimsbf
		descriptor_length				8	uimsbf
		copyright_identifier			32	uimsbf
		for (i = 0; i < N ; i++ ){		
			additional_copyright_info	8	bslbf
		}		
	}		
-----------------------------------------------------------------------------*/
typedef struct copyright_desc_s {
	unsigned int			copyright_id;
	private_data_t	*priv_data;
} copyright_desc_t;

/*-----------------------------------------------------------------------------
	maximum_bitrate_descriptor() {		
		descriptor_tag			8	uimsbf
		descriptor_length		8	uimsbf
		reserved				2	bslbf
		maximum_bitrate			22	uimsbf
	}		
-----------------------------------------------------------------------------*/
typedef struct max_bitrate_desc_s {
	unsigned int			max_bitrate;
} max_bitrate_desc_t;

/*-----------------------------------------------------------------------------
	private_data_indicator_descriptor() {		
		descriptor_tag				8	uimsbf
		descriptor_length			8	uimsbf
		private_data_indicator		32	uimsbf
	}		
-----------------------------------------------------------------------------*/
typedef struct private_data_indicator_desc_s {
	unsigned int			indicator;
} private_data_indicator_desc_t;

/*-----------------------------------------------------------------------------
	smoothing_buffer_descriptor () {		
		descriptor_tag			8	uimsbf
		descriptor_length		8	uimsbf
		reserved				2	bslbf
		sb_leak_rate			22	uimsbf
		reserved				2	bslbf
		sb_size					22	uimsbf
	}		
-----------------------------------------------------------------------------*/
typedef struct smoothing_buffer_desc_s {
	unsigned int			sb_leak_rate;
	unsigned int			sb_size;
} smoothing_buffer_desc_t;

/*-----------------------------------------------------------------------------
	association_tag_descriptor(){
		descriptor_tag 		8 uimsbf
		descriptor_length 		8 uimsbf
		association_tag 		16 uimsbf
		use 					16 uimsbf
		selector_length		8 uimsbf
		transaction_id			32 uimsbf
		timeout				32 uimsbf
		for (i=0;i<N;i++){
			private_data
		}
	}
-----------------------------------------------------------------------------*/
typedef struct association_tag_desc_s {
	unsigned short 			association_tag;
	unsigned short 			use;
	unsigned char 			selector_length;
	unsigned int 			transaction_id;
	unsigned int 			timeout;
	private_data_t	*priv_data;
} association_tag_desc_t;

/*-----------------------------------------------------------------------------
	ISO_639_language_descriptor() {		
		descriptor_tag				8	uimsbf
		descriptor_length			8	uimsbf
		for (i=0;i<N;i++) {		
			ISO_639_language_code	24	bslbf
			audio_type				8	bslbf
		}		
	}		
-----------------------------------------------------------------------------*/
typedef struct iso_639_lang_data_s {
	char lang_code[ISO_LANGUAGE_CODE];
	/* 0x0:Undefined, 0x1:Clean effects, 0x2:Hearing Imparied, 0x3:Visual impaired commentary, 0x4~0xff:Reserved*/
	unsigned char audio_type;

	struct iso_639_lang_data_s	*next;
} iso_639_lang_data_t;

typedef struct iso_639_lang_desc_s {
	iso_639_lang_data_t		*lang_data;
} iso_639_lang_desc_t;

/*-----------------------------------------------------------------------------
	program_association_section() {		
		table_id						8	uimsbf
		section_syntax_indicator		1	bslbf
		'0'								1	bslbf
		reserved						2	bslbf
		section_length					12	uimsbf
		transport_stream_id				16	uimsbf
		reserved						2	bslbf
		version_number					5	uimsbf
		current_next_indicator			1	bslbf
		section_number					8	uimsbf
		last_section_number				8	uimsbf
		for (i=0; i<N;i++) {		
			program_number				16	uimsbf
			reserved					3	bslbf
			if(program_number == '0') {		
				network_PID				13	uimsbf
			}		
			else {		
				program_map_PID			13	uimsbf
			}		
		}		
		CRC_32							32	rpchof
	}		
-----------------------------------------------------------------------------*/
typedef struct pat_program_data_s {
	unsigned short program_number;
	unsigned short pmt_pid;
	struct pat_program_data_s *next;
} pat_program_data_t;

typedef struct pat_section_s {
	unsigned char table_id;
	unsigned short ts_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	pat_program_data_t *prog_data;
} pat_section_t;


/*-----------------------------------------------------------------------------
	TS_program_map_section() {		
		table_id						8	uimsbf
		section_syntax_indicator		1	bslbf
		'0'								1	bslbf
		reserved						2	bslbf
		section_length					12	uimsbf
		program_number					16	uimsbf
		reserved						2	bslbf
		version_number					5	uimsbf
		current_next_indicator			1	bslbf
		section_number					8	uimsbf
		last_section_number				8	uimsbf
		reserved						3	bslbf
		PCR_PID							13	uimsbf
		reserved						4	bslbf
		program_info_length				12	uimsbf
		for (i=0; i<N; i++) {		
			descriptor()		
		}		
		for (i=0;i<N1;i++) {		
			stream_type					8	uimsbf
			reserved					3	bslbf
			elementary_PID				13	uimsnf
			reserved					4	bslbf
			ES_info_length				12	uimsbf
			for (i=0; i<N2; i++) {		
				descriptor()		
			}		
		}		
		CRC_32							32	rpchof
	}		
-----------------------------------------------------------------------------*/
typedef struct pmt_es_data_s {
	unsigned char stream_type;
	unsigned short elem_pid;
	descriptor_t *desc;
	struct pmt_es_data_s *next;
} pmt_es_data_t;

typedef struct pmt_section_s {
	unsigned char table_id;
	unsigned short program_number;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned short pcr_pid;
	descriptor_t *desc;
	pmt_es_data_t *es_data;
} pmt_section_t;


/*-----------------------------------------------------------------------------
	CA_section() {		
		table_id					8	uimsbf
		section_syntax_indicator	1	bslbf
		'0'							1	bslbf
		reserved					2	bslbf
		section_length				12	uimsbf
		reserved					18	bslbf
		version_number				5	uimsbf
		current_next_indicator		1	bslbf
		section_number				8	uimsbf
		last_section_number			8	uimsbf
		for (i=0; i<N;i++) {		
			descriptor()		
		}		
		CRC_32						32	rpchof
	}		
-----------------------------------------------------------------------------*/
typedef struct cat_section_s {
	unsigned char table_id;
	unsigned short reserved;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	descriptor_t *desc;
} cat_section_t;

/*-----------------------------------------------------------------------------
	avc_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		profile_idc 				8 uimsbf
		constraint_set0_flag 		1 bslbf
		constraint_set1_flag 		1 bslbf
		constraint_set2_flag 		1 bslbf
		AVC_compatible_flags 		5 bslbf
		level_idc 					8 uimsbf
		AVC_still_present 			1 bslbf
		AVC_24_hour_picture_flag	1 bslbf
		reserved 					6 bslbf
	}
-----------------------------------------------------------------------------*/
typedef struct avc_desc_s {
	unsigned char			profile_idc;
	unsigned char			constraint_set0_flag;
	unsigned char			constraint_set1_flag;
	unsigned char			constraint_set2_flag;
	unsigned char			AVC_compatible_flags;
	unsigned char			level_idc;
	unsigned char			AVC_still_present;
	unsigned char			AVC_24_hour_picture_flag;
} avc_desc_t;

/*-----------------------------------------------------------------------------
	network_name_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		for (i=0;i<N;i++){
			char 				8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct network_name_desc_s {
	char			*name;
} network_name_desc_t;

/*-----------------------------------------------------------------------------
	service_list_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		for (i=0;i<N;I++){
			service_id 			16 uimsbf
			service_type 		8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct service_list_data_s {
	unsigned short			service_id;
	unsigned char			service_type;

	struct service_list_data_s	*next;
} service_list_data_t;

typedef struct service_list_desc_s {
	service_list_data_t		*service_data;
} service_list_desc_t;

/*-----------------------------------------------------------------------------
	satellite_delivery_system_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		frequency 				32 bslbf
		orbital_position 		16 bslbf
		west_east_flag 			1 bslbf
		polarization 			2 bslbf
		modulation 				5 bslbf
		symbol_rate 			28 bslbf
		FEC_inner 				4 bslbf
	}
-----------------------------------------------------------------------------*/
typedef struct sat_delivery_system_desc_s {
	unsigned char			frequency[4];	/* bcd */
	unsigned char			orbit_pos[2];	/* bcd */
	unsigned char			west_east;
	unsigned char			polarization;
	unsigned char			modulation;
	unsigned char 			rolloff;
	unsigned char			modulation_type;
	unsigned char			symbol_rate[4];	/* bcd */
	unsigned char			fec_inner;
} sat_delivery_system_desc_t;

/*-----------------------------------------------------------------------------
	cable_delivery_system_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		frequency 				32 bslbf
		reserved_future_use 	12 bslbf
		FEC_outer 				4 bslbf
		modulation 				8 bslbf
		symbol_rate 			28 bslbf
		FEC_inner 				4 bslbf
	}
-----------------------------------------------------------------------------*/
typedef struct cable_delivery_system_desc_s {
	unsigned char			frequency[4];	/* bcd */
	unsigned char			fec_outer;
	unsigned char			modulation;
	unsigned char			symbol_rate[4];	/* bcd */
	unsigned char			fec_inner;
} cable_delivery_system_desc_t;

/*-----------------------------------------------------------------------------
	S2_satellite_delivery_system_decriptor(){
		descriptor_tag					8 uimsbf
		descriptor_length				8 uimsbf
		scrambling_sequence_selector 	1 bslbf
		multiple_input_stream_flag 		1 bslbf
		backwards_compatibility_indicator	1 bslbf
		reserved_future_use 				5 bslbf
		if (scrambling_sequence_selector == 1){
			Reserved						6 bslbf
			scrambling_sequence_index 		18 uimsbf
		}
		if (multiple_input_stream_flag == 1){
			input_stream_identifier 			8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct s2_sat_delivery_system_desc_s {
	unsigned char			scrambling_sequence_selector;
	unsigned char			multiple_input_stream_flag;
	unsigned char			backwards_compatibility_indicator;
	unsigned int			scrambling_sequence_index;
	unsigned char			input_stream_identifier;
} s2_sat_delivery_system_desc_t;

/*-----------------------------------------------------------------------------
	bouquet_name_descriptor(){
		descriptor_tag			8 uimsbf
		descriptor_length		8 uimsbf
		for(i=0;i<N;i++){
			char				8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct bouquet_name_desc_s {
	char			*name;
} bouquet_name_desc_t;

/*-----------------------------------------------------------------------------
	service_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		service_type 					8 uimsbf
		service_provider_name_length 	8 uimsbf
		for (i=0;i<N;I++){
			char 						8 uimsbf
		}
		service_name_length 			8 uimsbf
		for (i=0;i<N;I++){
			char 						8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct service_desc_s {
	unsigned char	service_type;
	char			*provider_name;
	int				provider_name_len;
	char			*service_name;
	int				service_name_len;
} service_desc_t;

/*-----------------------------------------------------------------------------
	country_availability_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		country_availability_flag 	1 bslbf
		reserved_future_use 		7 bslbf
		for (i=0;i<N;i++){
			country_code 			24 bslbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct country_code_data_s {
	unsigned char			country_code[ISO_COUNTRY_CODE];

	struct country_code_data_s	*next;
} country_code_data_t;

typedef struct country_availability_desc_s {
	unsigned char			availability;
	country_code_data_t			*code_data;	
} country_availability_desc_t;

/*-----------------------------------------------------------------------------
	linkage_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		transport_stream_id 			16 uimsbf
		original_network_id 			16 uimsbf
		service_id 						16 uimsbf
		linkage_type 					8 uimsbf
		if (linkage_type !=0x08){
			for (i=0;i<N;i++){
				private_data_byte 		8 bslbf
			}
		}
		if (linkage_type ==0x08){
			hand-over_type 				4 bslbf
			reserved_future_use 		3 bslbf
			origin_type 				1 bslbf
			if (hand-over_type ==0x01
				|| hand-over_type ==0x02
				|| hand-over_type ==0x03){
				network_id 				16 uimsbf
			}
			if (origin_type ==0x00){
				initial_service_id 		16 uimsbf
			}
			for (i=0;i<N;i++){
				private_data_byte 		8 bslbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct linkage_desc_s {
	unsigned short			ts_id;
	unsigned short			on_id;
	unsigned short			service_id;
	unsigned char			linkage_type;
	private_data_t			*priv_data;
} linkage_desc_t;

/*-----------------------------------------------------------------------------
	NVOD_reference_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		for (i=0;i<N;i++) {
			transport_stream_id 	16 uimsbf
			original_network_id 	16 uimsbf
			service_id 				16 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct nvod_ref_data_s {
	unsigned short			ts_id;
	unsigned short			on_id;
	unsigned short			service_id;

	struct nvod_ref_data_s		*next;
} nvod_ref_data_t;

typedef struct nvod_ref_desc_s {
	nvod_ref_data_t		*nvod_data;
} nvod_ref_desc_t;

/*-----------------------------------------------------------------------------
	time_shifted_service_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		reference_service_id 	16 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct time_shifted_service_desc_s {
	unsigned short			ref_service_id;
} time_shifted_service_desc_t;

/*-----------------------------------------------------------------------------
	short_event_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		ISO 639-2 [3] _language_code 	24 bslbf
		event_name_length 				8 uimsbf
		for (i=0;i<event_name_length;i++){
			event_name_char 			8 uimsbf
		}
		text_length 					8 uimsbf
		for (i=0;i<text_length;i++){
			text_char 					8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct short_event_desc_s {
	char			lang_code[ISO_LANGUAGE_CODE];
	char			*name;
	char			*text;
} short_event_desc_t;

/*-----------------------------------------------------------------------------
	extended_event_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		descriptor_number 				4 uimsbf
		last_descriptor_number 			4 uimsbf
		ISO 639-2 [3] _language_code 	24 bslbf
		length_of_items 				8 uimsbf
		for ( i=0;i<N;i++){
			item_description_length 	8 uimsbf
			for (j=0;j<N;j++){
				item_description_char 	8 uimsbf
			}
			item_length 				8 uimsbf
			for (j=0;j<N;j++){
				item_char 				8 uimsbf
			}
		}
		text_length 					8 uimsbf
		for (i=0;i<N;i++){
			text_char 					8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct extended_event_data_s {
	char			*description;
	char			*name;

	struct extended_event_data_s	*next;
} extended_event_data_t;
typedef struct extended_event_desc_s {
	unsigned char			desc_number;
	unsigned char			last_desc_number;
	char			lang_code[ISO_LANGUAGE_CODE];
	extended_event_data_t	*ext_evt_data;
	char			*text;	
} extended_event_desc_t;

/*-----------------------------------------------------------------------------
	Time_shifted_event_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		reference_service_id 	16 uimsbf
		reference_event_id 		16 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct time_shifted_event_desc_s {
	unsigned short			ref_service_id;
	unsigned short			ref_event_id;
} time_shifted_event_desc_t;

/*-----------------------------------------------------------------------------
	component_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		reserved_future_use 			4 bslbf
		stream_content 					4 uimsbf
		component_type 					8 uimsbf
		component_tag 					8 uimsbf
		ISO 639-2 [3] _language_code 	24 bslbf
		for (i=0;i<N;i++){
			text_char 					8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct component_desc_s {
	unsigned char			stream_content;
	unsigned char			component_type;
	unsigned char			component_tag;
	char			lang_code[ISO_LANGUAGE_CODE];
	char			*text;
} component_desc_t;

/*-----------------------------------------------------------------------------
	mosaic_descriptor(){
		descriptor_tag 							8 uimsbf
		descriptor_length 						8 uimsbf
		mosaic_entry_point 						1 bslbf
		number_of_horizontal_elementary_cells 	3 uimsbf
		reserved_future_use 					1 bslbf
		number_of_vertical_elementary_cells 	3 uimsbf
		for (i=0;i<N; i++) {
			logical_cell_id 					6 uimsbf
			reserved_future_use 				7 bslbf
			logical_cell_presentation_info 		3 uimsbf
			elementary_cell_field_length 		8 uimsbf
			for (i=0;i<elementary_cell_field_length;i++) {
				reserved_future_use 			2 bslbf
				elementary_cell_id 				6 uimsbf
			}
			cell_linkage_info 					8 uimsbf
			If (cell_linkage_info ==0x01){
				bouquet_id 						16 uimsbf
			}
			If (cell_linkage_info ==0x02){
				original_network_id 			16 uimsbf
				transport_stream_id 			16 uimsbf
				service_id 						16 uimsbf
			}
			If (cell_linkage_info ==0x03){
				original_network_id 			16 uimsbf
				transport_stream_id 			16 uimsbf
				service_id 						16 uimsbf
			}
			If (cell_linkage_info ==0x04){
				original_network_id 			16 uimsbf
				transport_stream_id 			16 uimsbf
				service_id 						16 uimsbf
				event_id 						16 uimsbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct mosaic_elem_cell_data_s {
	unsigned char			elem_cell_id;

	struct mosaic_elem_cell_data_s	*next;
} mosaic_elem_cell_data_t;

typedef struct bouquet_cell_data_s {
	unsigned short			bouquet_id;
} bouquet_cell_data_t;

typedef struct service_cell_data_s {
	unsigned short			on_id;
	unsigned short			ts_id;
	unsigned short			service_id;
} service_cell_data_t;

typedef struct other_mosaic_cell_data_s {
	unsigned short			on_id;
	unsigned short			ts_id;
	unsigned short			service_id;
} other_mosaic_cell_data_t;

typedef struct event_cell_data_s {
	unsigned short			on_id;
	unsigned short			ts_id;
	unsigned short			service_id;
	unsigned short			event_id;
} event_cell_data_t;

typedef union mosaic_cell_linkage_u {
	bouquet_cell_data_t			bouquet;
	service_cell_data_t			service;
	other_mosaic_cell_data_t	other_mosaic;
	event_cell_data_t			event;
} mosaic_cell_linkage_t;

typedef struct mosaic_data_s {
	unsigned char			logical_cell_id;
	unsigned char			logical_presentation_info;
	mosaic_elem_cell_data_t		*elem_cell;
	unsigned char			cell_linkage_info;
	mosaic_cell_linkage_t		cell_linkage;

	struct mosaic_data_s	*next;
} mosaic_data_t;

typedef struct mosaic_desc_s {
	unsigned char			entry_point;
	unsigned char			nb_hor_elem_cell;
	unsigned char			nb_ver_elem_cell;
#if 0/*not supported */
	mosaic_data_t	*mosaic;
#endif
} mosaic_desc_t;

/*-----------------------------------------------------------------------------
	stream_identifier_descriptor(){
		descriptor_tag 		8 uimsbf
		descriptor_length 	8 uimsbf
		component_tag 		8 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct stream_identifier_desc_s {
	unsigned char			component_tag;
} stream_identifier_desc_t;

/*-----------------------------------------------------------------------------
	CA_identifier_descriptor(){
		descriptor_tag 		8 uimsbf
		descriptor_length 	8 uimsbf
		for (i=0;i<N;i++){
			CA_system_id 	16 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ca_identifier_data_s {
	unsigned short			ca_system_id;

	struct ca_identifier_data_s	*next;
} ca_identifier_data_t;

typedef struct ca_identifier_desc_s {
	ca_identifier_data_t	*ca_id_data;
} ca_identifier_desc_t;

/*-----------------------------------------------------------------------------
	content_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		for (i=0;i<N;i++) {
			content_nibble_level_1 	4 uimsbf
			content_nibble_level_2 	4 uimsbf
			user_nibble 			4 uimsbf
			user_nibble 			4 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct content_data_s {
	unsigned char			content_nibble;
	unsigned char			user_nibble;
	
	struct content_data_s	*next;
} content_data_t;

typedef struct content_desc_s {
	content_data_t	*content_data;
} content_desc_t;

/*-----------------------------------------------------------------------------
	parental_rating_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		for (i=0;i<N;i++){
			country_code 		24 bslbf
			rating 				8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct parental_rating_data_s {
	unsigned char			country_code[ISO_COUNTRY_CODE];
	unsigned char			rating;

	struct parental_rating_data_s	*next;
} parental_rating_data_t;

typedef struct parental_rating_desc_s {
	parental_rating_data_t		*rating_data;
} parental_rating_desc_t;

/*-----------------------------------------------------------------------------
	teletext_descriptor(){
		descriptor_tag 						8 uimsbf
		descriptor_length 					8 uimsbf
		for (i=0;i<N;i++){
			ISO 639-2 [3] _language_code 	24 bslbf
			teletext_type 					5 uimsbf
			teletext_magazine_number 		3 uimsbf
			teletext_page_number 			8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct teletext_data_s {
	char			lang_code[ISO_LANGUAGE_CODE];
	unsigned char			teletext_type;
	unsigned char			mag_number;
	unsigned char			page_number;
	
	struct teletext_data_s		*next;
} teletext_data_t;

typedef struct teletext_desc_s {
	teletext_data_t		*txt_data;
} teletext_desc_t;

/*-----------------------------------------------------------------------------
	telephone_descriptor(){
		descriptor_tag 						8 uimsbf
		descriptor_length 					8 uimsbf
		reserved_future_use 				2 bslbf
		foreign_availability 				1 bslbf
		connection_type 					5 uimsbf
		reserved_future_use 				1 bslbf
		country_prefix_length 				2 uimsbf
		international_area_code_length 		3 uimsbf
		operator_code_length 				2 uimsbf
		reserved_future_use 				1 bslbf
		national_area_code_length 			3 uimsbf
		core_number_length 					4 uimsbf
		for (i=0;i<N;i++){
			country_prefix_char 			8 uimsbf
		}
		for (i=0;i<N;i++){
			international_area_code_char 	8 uimsbf
		}
		for (i=0;i<N;i++){
			operator_code_char 				8 uimsbf
		}
		for (i=0;i<N;i++){
			national_area_code_char 		8 uimsbf
		}
		for (i=0;i<N;i++){
			core_number_char 				8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct telephone_desc_s {
	unsigned char			foreign_avail;
	unsigned char			connection_type;
	char			*country_prefix;
	char			*international_area_code;
	char			*operator_code;
	char			*national_area_code;
	char			*core_number;
} telephone_desc_t;

/*-----------------------------------------------------------------------------
	local_time_offset_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		for(i=0;i<N;i++){
			country_code 				24 bslbf
			country_region_id 			6 bslbf
			reserved 					1 bslbf
			local_time_offset_polarity 	1 bslbf
			local_time_offset 			16 bslbf
			time_of _change 			40 bslbf
			next_time_offset 			16 bslbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct local_time_offset_data_s {
	unsigned char			country_code[ISO_COUNTRY_CODE];
	unsigned char			country_region_id;
	unsigned char			local_time_offset_polarity;
	unsigned char			local_time_offset[2];	/* bcd */
	unsigned char			time_of_change[5];		/* bcd */
	unsigned char			next_time_offset[2];	/* bcd */

	struct local_time_offset_data_s	*next;	
} local_time_offset_data_t;
typedef struct local_time_offset_desc_s {
	local_time_offset_data_t	*offset_data;
} local_time_offset_desc_t;

/*-----------------------------------------------------------------------------
	subtitling_descriptor(){
		descriptor_tag 						8 uimsbf
		descriptor_length 					8 uimsbf
		for (i= 0;i<N;I++){
			ISO 639-2 [3] _language_code 	24 bslbf
			subtitling_type 				8 bslbf
			composition_page_id 			16 bslbf
			ancillary_page_id 				16 bslbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct subtitling_data_s {
	char			lang_code[ISO_LANGUAGE_CODE];
	unsigned char			subtitling_type;
	unsigned short			composition_page_id;
	unsigned short			ancillary_page_id;
	
	struct subtitling_data_s		*next;
} subtitling_data_t;

typedef struct subtitling_desc_s {
	subtitling_data_t	*subt_data;	
} subtitling_desc_t;

/*-----------------------------------------------------------------------------
	terrestrial_delivery_system_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		centre_frequency 			32 bslbf
		bandwidth 					3 bslbf
		reserved_future_use 		5 bslbf
		constellation 				2 bslbf
		hierarchy_information 		3 bslbf
		code_rate-HP_stream 		3 bslbf
		code_rate-LP_stream 		3 bslbf
		guard_interval 				2 bslbf
		transmission_mode 			2 bslbf
		other_frequency_flag 		1 bslbf
		reserved_future_use 		32 bslbf
	}
-----------------------------------------------------------------------------*/
typedef struct ter_delivery_system_desc_s {
	unsigned char			center_frequency[4];	/* bcd */
	unsigned char			bandwidth;
	unsigned char 			priority;
	unsigned char			constellation;
	unsigned char			hierarchy;
	unsigned char			code_rate_hp;
	unsigned char			code_rate_lp;
	unsigned char			guard_interval;
	unsigned char			transmission_mode;
	unsigned char			other_freq;
} ter_delivery_system_desc_t;

/*-----------------------------------------------------------------------------
	T2_delivery_system_descriptor() { 
		descriptor_tag 					8 uimsbf 
		descriptor_length 					8 uimsbf 
		descriptor_tag_extension 			8 uimsbf 
		plp_id 							8 uimsbf 
		T2_system_id 						16 uimsbf 
		if (descriptor_length > 4){
			SISO/MISO 					2 bslbf 
			bandwidth 					4 bslbf 
			reserved_future_use 			2 bslbf 
			guard_interval 				3 bslbf 
			transmission_mode 			3 bslbf 
			other_frequency_flag 			1 bslbf 
			tfs_flag 						1 bslbf 
			for (i=0;i<N,i++){
				cell_id 					16 uimsbf 
				if (tfs_flag == 1){
					frequency_loop_length 	8 uimsbf 
					for (j=0;j<N;j++){ 
						centre_frequency 	32 uimsbf 
					}
				} 
				else{ 
					centre_frequency 		32 uimsbf 
				} 
				subcell_info_loop_length 	8 uimsbf 
				for (k=0;k<N;k++){ 
					cell_id_extension 		8 uimsbf 
					transposer_frequency 	32 uimsbf 
				}
			}
		}
-----------------------------------------------------------------------------*/
typedef struct t2_fransponder_frequency_desc_s {
	unsigned short			sub_cell_id;
	unsigned int			frequency;
	struct t2_fransponder_frequency_desc_s	*next;
} t2_fransponder_frequency_desc_t;

typedef struct t2_centre_frequency_desc_s {
	unsigned short			cell_id;
	unsigned int			frequency;
	t2_fransponder_frequency_desc_t *transponder;

	struct t2_centre_frequency_desc_s	*next;
} t2_centre_frequency_desc_t;

typedef struct t2_ter_delivery_system_desc_t {
	unsigned char			plp_id;
	unsigned short			T2_system_id;
	unsigned char			SISO_MISO;
	unsigned char			bandwidth;
	unsigned char			guard_interval;
	unsigned char			transmission_mode;
	unsigned char			other_frequency_flag;
	unsigned char			tfs_flag;
	t2_centre_frequency_desc_t *centre;
} t2_ter_delivery_system_desc_t;

/*-----------------------------------------------------------------------------
	multilingual_network_name_descriptor(){
		descriptor_tag 						8 uimsbf
		descriptor_length 					8 uimsbf
		for (i=0;i<N;i++) {
			ISO 639-2 [3] _language_code 	24 bslbf
			network_name_length 			8 uimsbf
			for (j=0;j<N;j++){
				char 						8 uimsbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct multi_network_name_data_s {
	char			lang_code[ISO_LANGUAGE_CODE];
	char			*name;
	
	struct multi_network_name_data_s	*next;
} multi_network_name_data_t;

typedef struct multi_network_name_desc_s {
	multi_network_name_data_t	*name_data;
} multi_network_name_desc_t;

/*-----------------------------------------------------------------------------
	multilingual_bouquet_name_descriptor(){
		descriptor_tag 						8 uimsbf
		descriptor_length 					8 uimsbf
		for (i=0;i<N;i++) {
			ISO 639-2 [3] _language_code 	24 bslbf
			bouquet_name_length 			8 uimsbf
			for (j=0;j<N;j++){
				char 						8 uimsbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct multi_bouquet_name_data_s {
	char			lang_code[ISO_LANGUAGE_CODE];
	char			*name;

	struct multi_bouquet_name_data_s	*next;
} multi_bouquet_name_data_t;

typedef struct multi_bouquet_name_desc_s {
	multi_bouquet_name_data_t	*name_data;
} multi_bouquet_name_desc_t;

/*-----------------------------------------------------------------------------
	multilingual_service_name_descriptor(){
		descriptor_tag 						8 uimsbf
		descriptor_length 					8 uimsbf
		for (i=0;i<N;i++) {
			ISO 639-2 [3] _language_code 	24 bslbf
			service_provider_name_length 	8 uimsbf
			for (j=0;j<N;j++){
				char 						8 uimsbf
			}
			service_name_length 			8 uimsbf
			for (j=0;j<N;j++){
				char 						8 uimsbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct multi_service_name_data_s {
	char						lang_code[ISO_LANGUAGE_CODE];
	char						*provider_name;
	char						*service_name;
	
	struct multi_service_name_data_s	*next;
} multi_service_name_data_t;

typedef struct multi_service_name_desc_s {
	multi_service_name_data_t	*name_data;
} multi_service_name_desc_t;

/*-----------------------------------------------------------------------------
	multilingual_component_descriptor(){
		descriptor_tag 						8 uimsbf
		descriptor_length 					8 uimsbf
		component_tag 						8 uimsbf
		for (i=0;i<N;i++) {
			ISO 639-2 [3] _language_code 	24 bslbf
			text_description_length 		8 uimsbf
			for (j=0;j<N;j++){
				text_char 					8 uimsbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct multi_component_data_s {
	char			lang_code[ISO_LANGUAGE_CODE];
	char			*description;
	
	struct multi_component_data_s		*next;
} multi_component_data_t;

typedef struct multi_component_desc_s {
	unsigned char			component_tag;
	multi_component_data_t		*component_data;
} multi_component_desc_t;

/*-----------------------------------------------------------------------------
	private_data_specifier_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		private_data_specifier 		32 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct private_data_specifier_desc_s {
	unsigned int			private_data_specifier;
} private_data_specifier_desc_t;

/*-----------------------------------------------------------------------------
	service_move_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		new_original_network_id 	16 uimsbf
		new_transport_stream_id 	16 uimsbf
		new_service_id 				16 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct service_move_desc_s {
	unsigned short			new_org_network_id;
	unsigned short			new_ts_id;
	unsigned short			new_service_id;
} service_move_desc_t;

/*-----------------------------------------------------------------------------
	short_smoothing_buffer_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		sb_size 				2 uimsbf
		sb_leak_rate 			6 uimsbf
		for (i=0;i<N;i++){
			DVB_reserved 		8 bslbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct short_smoothing_buffer_desc_s {
	unsigned char			sb_size;
	unsigned char			sb_leak_rate;
} short_smoothing_buffer_desc_t;

/*-----------------------------------------------------------------------------
	frequency_list_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		reserved_future_use 		6 bslbf
		coding_type 				2 bslbf
		for (i=0;I<N;i++){
			centre_frequency 		32 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct frequency_list_desc_s {
	unsigned char			coding_type;
	unsigned char			nfrequency;
	unsigned int			*frequency;
} frequency_list_desc_t;

/*-----------------------------------------------------------------------------
	data_broadcast_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		data_broadcast_id 				16 uimsbf
		component_tag 					8 uimsbf
		selector_length 				8 uimsbf
		for (i=0; i<selector_length; i++){
			selector_byte 				8 uimsbf
		}
		ISO 639-2 [3] _language_code 	24 bslbf
		text_length 					8 uimsbf
		for (i=0; i<text_length; i++){
			text_char 					8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct data_broadcast_desc_s {
	unsigned short			data_broadcast_id;
	unsigned char			component_tag;
	private_data_t	*selector_byte;
	char			lang_code[ISO_LANGUAGE_CODE];
	char			*text;
} data_broadcast_desc_t;

/*-----------------------------------------------------------------------------
	data_broadcast_id_descriptor(){
		descriptor_tag 			8 uimsbf
		descriptor_length 		8 uimsbf
		data_broadcast_id 		16 uimsbf
		for(i=0;I < N;i++ ){
			id_selector_byte 	8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct data_broadcast_id_desc_s {
	unsigned short			data_broadcast_id;
	private_data_t	*selector_byte;
} data_broadcast_id_desc_t;

/*-----------------------------------------------------------------------------
	AC-3_ descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			N x 8 uimsbf
		AC-3_type_flag 				1 bslbf
		bsid_flag 					1 bslbf
		mainid_flag 				1 bslbf
		asvc_flag 					1 bslbf
		reserved 					4 bslbf
		If (AC-3_type_flag)==1{
			AC-3_type 				8 uimsbf
		}
		If (bsid_flag)==1{
			bsid 					8 uimsbf
		}
		If (mainid_flag)==1{
			mainid 					8 uimsbf
		}
		If (asvc_flag)==1{
			asvc 					8 bslbf
		}
		For(I=0;I<N;I++){
			additional_info[I] 		N x 8 uimsbf
		}
		
	}
-----------------------------------------------------------------------------*/
typedef struct ac3_desc_s {
	unsigned char			ac3_type;
	unsigned char			bsid;
	unsigned char			mainid;
	unsigned char			asvc;
	unsigned char			info_len;
	unsigned char			*info;
} ac3_desc_t;

/*-----------------------------------------------------------------------------
	Enhanced AC-3_ descriptor()descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 					8 uimsbf
		component_type_flag 				1 bslbf
		bsid_flag 							1 bslbf
		mainid_flag 						1 bslbf
		asvc_flag 						1 bslbf
		mixinfoexists 						1 bslbf
		substream1_flag 					1 bslbf
		substream2_flag 					1 bslbf
		substream3_flag 					1 bslbf
		if (component_type_flag == 1){ 		
			component_type				8 uimsbf
		}
		if (bsid_flag == 1){ 
			bsid							8 uimsbf
		}
		if (mainid_flag == 1){ 
			mainid						8 uimsbf
		}
		if (asvc_flag == 1){ 
			asvc							8 bslbf
		}
		if (substream1_flag == 1){ 
			substream1					8 uimsbf
		}
		if (substream2_flag == 1){ 
			substream2					8 uimsbf
		}
		if (substream3_flag == 1){ 
			substream3					8 uimsbf
		}
		for (i=0;i<N;i++){ 
			additional_info_byte			8 bslbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct enhanced_ac3_desc_s {
	unsigned char component_type;
	unsigned char bsid;
	unsigned char mainid;
	unsigned char asvc;
	unsigned char substream1;
	unsigned char substream2;
	unsigned char substream3;
	unsigned char info_len;
	unsigned char *info;

} enhanced_ac3_desc_t;

/*-----------------------------------------------------------------------------
	AAC_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 				8 uimsbf
		profile_and_level 				8 uimsbf
		if (descriptor_length > 1){
			AAC_type_flag 			1 bslbf
			reserved 7 bslbf
			if (AAC_type_flag == 1){
				AAC_type 			8 uimsbf
			}
			for(i=0;i<N;i++){
				additional_info_byte 	8 uimsbf
			}
		}
	}

-----------------------------------------------------------------------------*/
typedef struct aac_desc_s {
	unsigned char profile_and_level;
	unsigned char aac_type;
	unsigned char info_len;
	unsigned char *info;

} aac_desc_t;

/*-----------------------------------------------------------------------------
	FTA_content_management_descriptor() { 
		descriptor_tag 							8 uimsbf 
	 	descriptor_length 						8 uimsbf 
	 	reserved_future_use 					4 
	 	do_not_scramble 						1 uimsbf 
	 	control_remote_access_over_internet 	2 uimsbf 
	 	do_not_apply_revocation 				1 uimsbf 
	} 
-----------------------------------------------------------------------------*/
typedef struct fta_content_management_desc_s {
	unsigned char do_not_scramble;
	unsigned char control_remote_access_over_internet;
	unsigned char do_not_apply_revocation;

} fta_content_management_desc_t;

/*-----------------------------------------------------------------------------
	channel_descriptor{
		descriptor_tag				8 uimsbf
		descriptor_length				8 uimsbf
		for (i=o; i<N; i++){
			service_id				16 uimsbf
			channel_number			16 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct channel_data_s {
	unsigned short			service_id;
	unsigned short			channel_number;
	struct channel_data_s	*next;
} channel_data_t;

typedef struct channel_desc_s {
	channel_data_t		*channel_data;
} channel_desc_t;

/*-----------------------------------------------------------------------------
	logical_channel_descriptor(){
	descriptor_tag					8 uimsbf
	descriptor_length				8 uimsbf
		 for (i=0; i<N;i++){
		 service_id 			   16 uimsbf
		 visible_service_flag		1 bslbf
		 reserved					5 bslbf
		 logical_channel_number    10 uimsbf
		 }
	}
-----------------------------------------------------------------------------*/
typedef struct lcn_data_s {
	unsigned short			service_id;
	unsigned char			visiable_flag;
	unsigned short			lcn;
	struct lcn_data_s	*next;
} lcn_data_t;

typedef struct lcn_desc_s {
	lcn_data_t		*lcn_data;
} lcn_desc_t;

/*-----------------------------------------------------------------------------
	logical_channel_v2_descriptor{
		descriptor_tag				8 uimsbf
		descriptor_length			8 uimsbf
		channel_list_id				8 uimsbf
		channel_list_name_len		8 uimsbf
		for (i=o; i<N; i++){
			channel_list_name		8 uimsbf
		}
		country_code 				24 bslbf
		logical_channel_len			8 uimsbf
		for (i=o; i<N; i++){
			service_id				16 uimsbf
			reserved					6 bslbf
			logical_channel_number		10 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct lcn_v2_data_s {
	unsigned short			service_id;
	unsigned char			visiable_flag;
	unsigned short			lcn;
	
	struct lcn_v2_data_s	*next;
} lcn_v2_data_t;

typedef struct lcn_v2_list_data_s {
	unsigned char		channel_list_id;
	char				*channel_list_name;
	unsigned char		country_code[ISO_COUNTRY_CODE];
	lcn_v2_data_t  		*lcn_data;
	
} lcn_v2_list_data_t;

typedef struct lcn_v2_desc_s {
	lcn_v2_list_data_t		*lcn_list_data;
} lcn_v2_desc_t;

/*-----------------------------------------------------------------------------
	service_attribute_descriptor{
		descriptor_tag				8 uimsbf
		descriptor_length			8 uimsbf
		for (i=o; i<N; i++){
			service_id				16 uimsbf
			reserved				6 bslbf
			numeric_flag			1 uimsbf
			visiable_flag			1 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct service_attribute_data_s {
	unsigned short			service_id;
	unsigned char			numeric_flag;
	unsigned char			visiable_flag;
	
	struct service_attribute_data_s	*next;
} service_attribute_data_t;

typedef struct service_attribute_desc_s {
	service_attribute_data_t *svc_attr_data;
} service_attribute_desc_t;

typedef struct short_name_desc_s {
	char 				*name;
	int 				len;
} short_name_desc_t;

/*-----------------------------------------------------------------------------
	hd_simulcast_logical_channel_descriptor{
		descriptor_tag				8 uimsbf
		descriptor_length			8 uimsbf
		for (i=o; i<N; i++){
			service_id				16 uimsbf
			visiable_flag			1 bslbf
			reserved				5 bslbf
			logical_channel_number	10 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct hd_simulcast_lcn_data_s {
	unsigned short			service_id;
	unsigned char			visiable_flag;
	unsigned short			lcn;
	
	struct hd_simulcast_lcn_data_s	*next;
} hd_simulcast_lcn_data_t;

typedef struct hd_simulcast_lcn_desc_s {
	hd_simulcast_lcn_data_t *lcn_data;
} hd_simulcast_lcn_desc_t;

/*-----------------------------------------------------------------------------
	scrambling_descriptor(){ 
		descriptor_tag 			8 uimsbf 
		descriptor_length			8 uimsbf 
		scrambling_mode 			8 uimsbf 
} 
-----------------------------------------------------------------------------*/
typedef struct scrambling_desc_s {
	unsigned char scrambling_mode;
} scrambling_desc_t;

/*-----------------------------------------------------------------------------
	partial_transport_stream_descriptor() {
		descriptor_tag 					8 bslbf
		descriptor_length 					8 uimsbf
		DVB_reserved_future_use 			2 bslbf
		peak_rate 						22 uimsbf
		DVB_reserved_future_use 			2 bslbf
		minimum_overall_smoothing_rate 	22 uimsbf
		DVB_reserved_future_use 			2 bslbf
		maximum_overall_smoothing_buffer 	14 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct paritial_transport_stream_desc_s {
	unsigned int peak_rate;
	unsigned int minimum_overall_smoothing_rate;
	unsigned short maximum_overall_smoothing_buffer;
} paritial_transport_stream_desc_t;

/*-----------------------------------------------------------------------------
	DSNG_descriptor (){ 
		descriptor_tag 		8 uimsbf 
		descriptor_length 		8 uimsbf 
		for (i=0;i<N;i++) { 
			byte 			8 uimsbf 
		} 
	} 
-----------------------------------------------------------------------------*/
typedef struct digital_sat_news_gathering_desc_s {
	char *dsng;
} digital_sat_news_gathering_desc_t;

/*-----------------------------------------------------------------------------
	PDC_descriptor(){ 
		descriptor_tag 					8 uimsbf 
		descriptor_length 					8 uimsbf 
		reserved_future_use 				4 bslbf 
		programme_identification_label 		20 bslbf 
	} 
-----------------------------------------------------------------------------*/
typedef struct pdc_desc_s {
	unsigned int programme_identification_label;
} pdc_desc_t;

/*-----------------------------------------------------------------------------
	ancillary_data_descriptor(){ 
		descriptor_tag 			8 uimsbf 
		descriptor_length 			8 uimsbf 
		ancillary_data_identifier 	8 bslbf 
	} 
-----------------------------------------------------------------------------*/
typedef struct ancillary_data_desc_s {
	unsigned char ancillary_data_identifier;
} ancillary_data_desc_t;

/*-----------------------------------------------------------------------------
	cell_list_descriptor(){ 
		descriptor_tag 						8 uimsbf 
		descriptor_length 					8 uimsbf 
		for (i=0;i<N;i++){ 
			cell_id 						16 uimsbf 
			cell_latitude 					16 uimsbf 
			cell_longitude 					16 uimsbf 
			cell_extent_of_latitude 		12 uimsbf 
			cell_extent_of_longitude 		12 uimsbf 
			subcell_info_loop_length 		8 uimsbf 
			for (j=0;j<N;j++){ 
				cell_id_extension 			8 uimsbf 
				subcell_latitude 			16 uimsbf 
				subcell_longitude 			16 uimsbf 
				subcell_extent_of_latitude 	12 uimsbf 
				subcell_extent_of_longitude 12 uimsbf 
			}
		} 
	}
-----------------------------------------------------------------------------*/
typedef struct subcell_list_s {
	unsigned char cell_id_extension;
	unsigned short subcell_latitude;
	unsigned short subcell_longitude;
	unsigned short subcell_extent_of_latitude;
	unsigned short subcell_extent_of_longitude;

	struct subcell_list_s *next;
} subcell_list_t;

typedef struct cell_list_s {
	unsigned short cell_id;
	unsigned short cell_latitude;
	unsigned short cell_longitude;
	unsigned short cell_extent_of_latitude;
	unsigned short cell_extent_of_longitude;
	subcell_list_t 	*sublist;

	struct cell_list_s *next;
} cell_list_t;

typedef struct cell_list_desc_s {
	cell_list_t 	*list;
} cell_list_desc_t;

/*-----------------------------------------------------------------------------
	cell_frequency_link_descriptor(){ 
		descriptor_tag 					8 uimsbf 
		descriptor_length 					8 uimsbf 
		for (i=0;i<N;i++){ 
			cell_id 						16 uimsbf 
			frequency 					32 uimsbf 
			subcell_info_loop_length 		8 uimsbf 
			for (j=0;j<N;j++){ 
				cell_id_extension 			8 uimsbf 
				transposer_frequency 		32 uimsbf 
			}
		} 
	}
-----------------------------------------------------------------------------*/
typedef struct subcell_frequency_link_s {
	unsigned short cell_id_extension;
	unsigned int transposer_frequency;

	struct subcell_frequency_link_s *next;
} subcell_frequency_link_t;

typedef struct cell_frequency_link_s {
	unsigned short cell_id;
	unsigned int frequency;
	subcell_frequency_link_t 	*subfrequency_link;

	struct cell_frequency_link_s *next;
} cell_frequency_link_t;

typedef struct cell_frequency_link_desc_s {
	cell_frequency_link_t 	*frequency_link;
} cell_frequency_link_desc_t;

/*-----------------------------------------------------------------------------
	announcement_support_descriptor(){ 
		descriptor_tag 						8 uimsbf 
		descriptor_length 						8 uimsbf 
		announcement_support_indicator 			16 bslbf 
		for (i=0; i<N; i++){ 
			announcement_type 				4 uimsbf 
			reserved_future_use 				1 bslbf 
			reference_type 					3 uimsbf 
			if (reference_type == 0x01 
				|| reference_type == 0x02 
				|| reference_type == 0x03){ 
					original_network_id 		16 uimsbf 
					transport_stream_id 		16 uimsbf 
					service_id 				16 uimsbf 
					component_tag__ 			8 uimsbf 
			} 
		} 
	}
-----------------------------------------------------------------------------*/
typedef struct announcement_support_s {
	unsigned short announcement_type;
	unsigned char reference_type;
	unsigned short original_network_id;
	unsigned short transport_stream_id;
	unsigned short service_id;
	unsigned char component_tag;

	struct announcement_support_s *next;
} announcement_support_t;

typedef struct announcement_support_desc_s {
	unsigned short 					announcement_support_indicator;
	announcement_support_t 	*announcement_support;
} announcement_support_desc_t;

/*-----------------------------------------------------------------------------
	application_signalling_descriptor(){ 
		descriptor_tag 						8 uimsbf 
		descriptor_length 					8 uimsbf 
		for (i=0; i<N; i++){ 
			reserved_future_use 			1 bslbf 
			application_type 				15 uimsbf 
			reserved_future_use 			3 bslbf 
			AIT_version_number				5 uimsbf 
		} 
	} 
-----------------------------------------------------------------------------*/
typedef struct application_signalling_data_s {
	unsigned short application_type;
	unsigned char AIT_version_number;
	
	struct application_signalling_data_s *next;
} application_signalling_data_t;

typedef struct application_signalling_desc_s {
	application_signalling_data_t *application_signalling_data;
} application_signalling_desc_t;

/*-----------------------------------------------------------------------------
	adaptation_field_data_descriptor(){ 
		descriptor_tag 				8 uimsbf 
		descriptor_length 				8 uimsbf 
		adaptation_field_data_identifier 	8 bslbf 
	} 
-----------------------------------------------------------------------------*/
typedef struct adaptation_field_data_desc_s {
	unsigned char adaptation_field_data_identifier;
} adaptation_field_data_desc_t;

/*-----------------------------------------------------------------------------
	service_availbility_descriptor(){ 
		descriptor_tag 				8 uimsbf 
		descriptor_length 				8 uimsbf 
		availability_flag 				1 bslbf 
		reserved 						7 bslbf 
		for (i=0;i<N;i++) { 
			cell_id 					16 uimsbf 
		}
	} 
-----------------------------------------------------------------------------*/
typedef struct service_availability_s {
	unsigned short cell_id;

	struct service_availability_s *next;
} service_availability_t;

typedef struct service_availability_desc_s {
	unsigned char 					availability_flag;
	service_availability_t 	*service_availability;
} service_availability_desc_t;

/*-----------------------------------------------------------------------------
	service_list_name_descriptor(){ 
		descriptor_tag 				8 uimsbf 
		descriptor_length 				8 uimsbf 
		ISO_639_2_language_code		24 bslbf 
		for (i=0;i<N;i++) { 
			char 					8 uimsbf 
		}
	} 
-----------------------------------------------------------------------------*/
typedef struct service_list_name_desc_s {
	char	lang_code[ISO_LANGUAGE_CODE];
	char 	*name;
} service_list_name_desc_t;

/*-----------------------------------------------------------------------------
	bouquet_list_descriptor(){ 
		descriptor_tag 				8 uimsbf 
		descriptor_length 				8 uimsbf 
		for (i=0;i<M;i++) { 
			bouquet_name_length 		8 uimsbf 
			for (i=0;i<N;i++) { 
				char 				8 uimsbf 
			}
		}
	} 
-----------------------------------------------------------------------------*/
typedef struct bouq_list_s {
	char 	*name;

	struct bouq_list_s *next;
} bouq_list_t;

typedef struct bouquet_list_desc_s {
	bouq_list_t 	*list;
} bouquet_list_desc_t;

/*-----------------------------------------------------------------------------
	virtual_service_id_descriptor(){ 
		descriptor_tag 				8 uimsbf 
		descriptor_length 				8 uimsbf 
		virtual_service_id 				16 uimsbf
		for (i=0;i<M;i++) { 
			reserved_future_use 		8 uimsbf
		}
	} 
-----------------------------------------------------------------------------*/
typedef struct virtual_service_id_desc_s {
	unsigned short 		virtual_service_id;
	char 		*virtual_service_reserved;
} virtual_service_id_desc_t;


// CANAL
/*-----------------------------------------------------------------------------
	canal_audio_name_descriptor(){ 
		descriptor_tag 				8 uimsbf 
		descriptor_length 			8 uimsbf 
		audio_name_len	 			8 uimsbf
		ISO_639_language_code 		24 bslbf
		for (i=0; i<N; i++) {
			label_char 				8 uimsbf
		}
	} 
-----------------------------------------------------------------------------*/
typedef struct canal_audio_name_desc_s {
	unsigned char 		audio_name_len;
	char 				*audio_name;
} canal_audio_name_desc_t;

/*-----------------------------------------------------------------------------
	ci_protection_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		free_ci_mode_flag 				1 bslbf
		match_brand_flag 				1 bslbf
		reserved_future_use 			6 bslbf
		if(match_brand_flag == 1) {
			number_of_entries			8 uimsbf
			for(i=0; i<n; i++) {
				cicam_brand_identifier	16 uimsbf
			}
		}
		for(i=0; i<n; i++) {
			private_data_byte 			8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ci_protection_brand_s {
	unsigned short 		id;

	struct ci_protection_brand_s *next;
} ci_protection_brand_t;

typedef struct ci_protection_desc_s {
	unsigned char 				free_ci_mode_flag;
	unsigned char 				match_brand_flag;
	unsigned char 				number_of_entries;
	ci_protection_brand_t 		*brand;
	private_data_t				*priv_data;
} ci_protection_desc_t;

/*-----------------------------------------------------------------------------
	ciplus_service_descriptor(){
		descriptor_tag 					8 uimsbf
		descriptor_length 				8 uimsbf
		service_id 						16 uimsbf
		service_type 					8 uimsbf
		visible_flag 					1 bslbf
		selectable						1 bslbf
		lcn 							14 uimsbf
		service_provider_name_length 	8 uimsbf
		for (i=0;i<N;I++){
			char 						8 uimsbf
		}
		service_name_length 			8 uimsbf
		for (i=0;i<N;I++){
			char 						8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ciplus_service_desc_s {
	unsigned short	service_id;
	unsigned char	service_type;
	unsigned char	visiable_flag;
	unsigned char	selectable;
	unsigned short	lcn;
	char			*provider_name;
	int				provider_name_len;
	char			*service_name;
	int				service_name_len;
} ciplus_service_desc_t;

/*-----------------------------------------------------------------------------
	ciplus_content_label_descriptor(){
		descriptor_tag 				8 uimsbf
		descriptor_length 			8 uimsbf
		content_byte_min 			8 uimsbf
		content_byte_max 			8 uimsbf
		ISO_639_language_code 		24 bslbf
		for (i=0; i<N; i++) {
			label_char 8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ciplus_content_label_desc_s {
	unsigned char	content_byte_min;
	unsigned char	content_byte_max;
	char 			lang_code[ISO_LANGUAGE_CODE];
} ciplus_content_label_desc_t;


/* PSIP - ATSC */
/*-----------------------------------------------------------------------------
	atsc ac3_audio_stream_descriptor (){ 
		descriptor_tag 					8 uimsbf // Assigned value for AC-3 descriptor
		descriptor_length 				8 uimsbf // 9 bytes
		sample_rate_code 				3 bslbf // 48 kHz
		bsid 							5 bslbf // bsid 6 
		bit_rate_code					6 bslbf // 384 kbit/s
		surround_mode					2 bslbf // Not indicated 
		bsmod							3 bslbf // Complete Main
		num_channels					4 bslbf // acmod 3/2 
		full_svc						1 bslbf // Full Service
		langcod							8 uimsbf // No longer used
		mainid							3 bslbf // Main audio service ID 0
		priority						2 bslbf // Priority not specified
		reserved						3 bslbf 
		language_flag					1 bslbf // Language field present
		language_flag_2					1 bslbf // Language2 field not present
		reserved						6 bslbf 
		language 						24 bslbf 
	} 
-----------------------------------------------------------------------------*/
typedef struct ac3_audio_stream_desc_s {
	unsigned char sample_rate_code;
	unsigned char bsid;
	unsigned char bit_rate_code;
	unsigned char surround_mode;
	unsigned char bsmod;
	unsigned char num_channels;
	unsigned char full_svc;
	unsigned char langcod;
	unsigned char mainid;
	unsigned char priority;
	unsigned char language_flag;
	unsigned char language_flag_2;
	unsigned char language[ISO_LANGUAGE_CODE];
} ac3_audio_stream_desc_t;

/*-----------------------------------------------------------------------------
	atsc enhanced_ac3_audio_stream_descriptor (){ 
		descriptor_tag 					8 uimsbf // Assigned value for E-AC-3 descriptor
		descriptor_length 				8 uimsbf // 9 bytes
		reserved						1 bslbf
		bsid_flag						1 bslbf // bsid field present
		mainid_flag						1 bslbf // mainid field present 
		asvc_flag						1 bslbf // asvc field not present 
		mixinfoexists					1 bslbf // mixing metadata not present in independent substream 0 
		substream1_flag					1 bslbf // substream1 field not present
		substream2_flag					1 bslbf // substream2 field not present
		substream3_flag					1 bslbf // substream3 field not present 
		
		reserved						1 bslbf
		full_service_flag				1 bslbf // Substream I0 contains a full service
		audio_service_type				3 bslbf // Complete Main
		number_of_channels				3 bslbf // Multichannel Audio (> 2-channel <= 3/2 + LFE) 
		
		language_flag					1 bslbf // Language field present
		language_flag_2					1 bslbf // Language2 field not present
		reserved						1 bslbf
		bsid 							5 bslbf 

		reserved						3 bslbf
		priority						2 bslbf // Priority not specified 
		mainid							3 bslbf // Main audio service ID
		
		language 						24 bslbf 
	} 
-----------------------------------------------------------------------------*/
typedef struct enhanced_ac3_audio_stream_desc_s {
	unsigned char bsid_flag;
	unsigned char mainid_flag;
	unsigned char asvc_flag;
	unsigned char mixinfoexists;
	unsigned char substream1_flag;
	unsigned char substream2_flag;
	unsigned char substream3_flag;
	
	unsigned char full_service_flag;
	unsigned char audio_service_type;
	unsigned char number_of_channels;
	
	unsigned char language_flag;
	unsigned char language_flag_2;
	unsigned char bsid;
	
	unsigned char priority;
	unsigned char mainid;
	
	unsigned char language[ISO_LANGUAGE_CODE];
} enhanced_ac3_audio_stream_desc_t;

/*-----------------------------------------------------------------------------
	caption_service_descriptor() {
		descriptor_tag 						8 0x86
		descriptor_length 						8 uimsbf
		reserved 								3 ‘111’
		number_of_services 					5 uimsbf
		for (i=0; i<number_of_services; i++) {
			language 						8*3 uimsbf
			digital_cc 						1 bslbf
			reserved 							1 ‘1’
			if (digital_cc == line21) {
				reserved 						5 ‘11111’
				line21_field 					1 bslbf
			}
			else
			caption_service_number 			6 uimsbf
			
			easy_reader 						1 bslbf
			wide_aspect_ratio 					1 bslbf
			reserved 							14 ‘11111111111111’
		}
	}
-----------------------------------------------------------------------------*/
typedef struct caption_service_data_s {
	unsigned char language[ISO_LANGUAGE_CODE];
	unsigned char digital_cc;
	unsigned char line21_field;
	unsigned char caption_service_number;
	unsigned char easy_reader;
	unsigned char wide_aspect_ratio;

	struct caption_service_data_s *next;
} caption_service_data_t;

typedef struct caption_service_desc_s {
	unsigned char 				number_of_services;
	caption_service_data_t 		*caption_service;
} caption_service_desc_t;

/******************************************************************************
 *
 *
 *
 *****************************************************************************/
/*-----------------------------------------------------------------------------
	network_information_section(){
		table_id 							8 uimsbf
		section_syntax_indicator 			1 bslbf
		reserved_future_use 				1 bslbf
		reserved 							2 bslbf
		section_length 						12 uimsbf
		network_id 							16 uimsbf
		reserved 							2 bslbf
		version_number 						5 uimsbf
		current_next_indicator 				1 bslbf
		section_number 						8 uimsbf
		last_section_number 				8 uimsbf
		reserved_future_use 				4 bslbf
		network_descriptors_length 			12 uimsbf
		for(i=0;i<N;i++){
			descriptor()
		}
		reserved_future_use 				4 bslbf
		transport_stream_loop_length 		12 uimsbf
		for(i=0;i<N;i++){
			transport_stream_id 			16 uimsbf
			original_network_id 			16 uimsbf
			reserved_future_use 			4 bslbf
			transport_descriptors_length 	12 uimsbf
			for(j=0;j<N;j++){
				descriptor()
			}
		}
		CRC_32 								32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct nit_ts_data_s {
	unsigned short ts_id;
	unsigned short on_id;
	descriptor_t *desc;
	struct nit_ts_data_s *next;
} nit_ts_data_t;

typedef struct nit_section_s {
	unsigned char table_id;
	unsigned short network_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	descriptor_t *desc;
	nit_ts_data_t *ts_data;
} nit_section_t;


/*-----------------------------------------------------------------------------
	service_description_section(){
		table_id 						8 uimsbf
		section_syntax_indicator 		1 bslbf
		reserved_future_use 			1 bslbf
		reserved 						2 bslbf
		section_length 					12 uimsbf
		transport_stream_id 			16 uimsbf
		reserved 						2 bslbf
		version_number 					5 uimsbf
		current_next_indicator 			1 bslbf
		section_number 					8 uimsbf
		last_section_number 			8 uimsbf
		original_network_id 			16 uimsbf
		reserved_future_use 			8 bslbf
		for (i=0;i<N;i++){
			service_id 					16 uimsbf
			reserved_future_use 		6 bslbf
			EIT_schedule_flag 			1 bslbf
			EIT_present_following_flag 	1 bslbf
			running_status 				3 uimsbf
			free_CA_mode 				1 bslbf
			descriptors_loop_length 	12 uimsbf
			for (j=0;j<N;j++){
				descriptor()
			}
		}
		CRC_32 							32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct sdt_service_data_s {
	unsigned short service_id;
	unsigned char eit_sch;
	unsigned char eit_pf;
	unsigned char running_status;
	unsigned char ca_mode;
	int			 len; // descriptor loop length
	descriptor_t *desc;
	struct sdt_service_data_s *next;
} sdt_service_data_t;

typedef struct sdt_section_s {
	unsigned char table_id;
	unsigned short ts_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned short on_id;
	sdt_service_data_t	*svc_data;
} sdt_section_t;


/*-----------------------------------------------------------------------------
	event_information_section(){
		table_id 						8 uimsbf
		section_syntax_indicator 		1 bslbf
		reserved_future_use 			1 bslbf
		reserved 						2 bslbf
		section_length 					12 uimsbf
		service_id 						16 uimsbf
		reserved 						2 bslbf
		version_number 					5 uimsbf
		current_next_indicator 			1 bslbf
		section_number 					8 uimsbf
		last_section_number 			8 uimsbf
		transport_stream_id 			16 uimsbf
		original_network_id 			16 uimsbf
		segment_last_section_number 	8 uimsbf
		last_table_id 					8 uimsbf
		for(i=0;i<N;i++){
			event_id 					16 uimsbf
			start_time 					40 bslbf
			duration 					24 uimsbf
			running_status 				3 uimsbf
			free_CA_mode 				1 bslbf
			descriptors_loop_length 	12 uimsbf
			for(i=0;i<N;i++){
				descriptor()
			}
		}
		CRC_32 							32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct eit_event_data_s {
	unsigned short event_id;
	unsigned char start_time[5]; /* bcd */
	unsigned char duration[3]; /* bcd */
	unsigned char running_status;
	unsigned char ca_mode;
	descriptor_t *desc;
	struct eit_event_data_s *next;
} eit_event_data_t;

typedef struct eit_section_s {
	unsigned char table_id;
	unsigned short service_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned short ts_id;
	unsigned short on_id;
	unsigned char segment_last_section;
	unsigned char last_table_id;
	eit_event_data_t *event_data;
} eit_section_t;


/*-----------------------------------------------------------------------------
	bouquet_association_section(){
		table_id						8 uimsbf
		section_syntax_indicator		1 bslbf
		reserved_future_use				1 bslbf
		reserved						2 bslbf
		section_length					12 uimsbf
		bouquet_id						16 uimsbf
		reserved						2 bslbf
		version_number					5 uimsbf
		current_next_indicator			1 bslbf
		section_number					8 uimsbf
		last_section_number				8 uimsbf
		reserved_future_use				4 bslbf
		bouquet_descriptors_length		12 uimsbf
		for(i=0;i<N;i++){
			descriptor()
		}
		reserved_future_use				4 bslbf
		transport_stream_loop_length	12 uimsbf
		for(i=0;i<N;i++){
			transport_stream_id			16 uimsbf
			original_network_id			16 uimsbf
			reserved_future_use			4 bslbf
			transport_descriptors_length	12 uimsbf
			for(j=0;j<N;j++){
				descriptor()
			}
		}
		CRC_32							32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct bat_ts_data_s {
	unsigned short ts_id;
	unsigned short on_id;
	descriptor_t *desc;

	struct bat_ts_data_s *next;
} bat_ts_data_t;

typedef struct bat_section_s {
	unsigned char table_id;
	unsigned short bouquet_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	descriptor_t *desc;
	bat_ts_data_t *ts_data;
} bat_section_t;


/*-----------------------------------------------------------------------------
	time_date_section(){
		table_id 					8 uimsbf
		section_syntax_indicator 	1 bslbf '0'
		reserved_future_use 		1 bslbf
		reserved 					2 bslbf
		section_length 				12 uimsbf
		UTC_time 					40 bslbf
	}
-----------------------------------------------------------------------------*/
typedef struct tdt_section_s {
	unsigned char table_id;
	unsigned char time[5];	/* bcd */
} tdt_section_t;


/*-----------------------------------------------------------------------------
	time_offset_section(){
		table_id 						8 uimsbf
		section_syntax_indicator	 	1 bslbf '0'
		reserved_future_use 			1 bslbf
		reserved 						2 bslbf
		section_length 					12 uimsbf
		UTC_time 						40 bslbf
		reserved 						4 bslbf
		descriptors_loop_length 		12 uimsbf
		for(i=0;i<N;i++){
			descriptor()
		}
		CRC_32 							32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct tot_section_s {
	unsigned char table_id;
	unsigned char time[5];	/* bcd */
	descriptor_t *desc;
} tot_section_t;


/*-----------------------------------------------------------------------------
	running_status_section(){
		table_id 						8 uimsbf
		section_syntax_indicator 		1 bslbf
		reserved_future_use 			1 bslbf
		reserved 						2 bslbf
		section_length 				12 uimsbf
		for (i=0;i<N;i++){
			transport_stream_id 		16 uimsbf
			original_network_id 		16 uimsbf
			service_id 				16 uimsbf
			event_id 					16 uimsbf
			reserved_future_use 		5 bslbf
			running_status 			3 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct rst_service_data_s {
	unsigned short ts_id;
	unsigned short on_id;
	unsigned short service_id;
	unsigned short event_id;
	unsigned char running_status;

	struct rst_service_data_s *next;
} rst_service_data_t;

typedef struct rst_section_s {
	unsigned char table_id;
	rst_service_data_t *svc_data;

} rst_section_t;


/* ASTRA APS(ASTRA PLATFORM SERVICE) - SGT(SERVICE GUIDE TABLE) */
/*-----------------------------------------------------------------------------
	service_guide_section(){
		table_id						8 uimsbf
		section_syntax_indicator		1 bslbf
		reserved_future_use			1 bslbf
		reserved						2 bslbf
		section_length					12 uimsbf
		service_list_id					16 uimsbf
		reserved						2 bslbf
		version_number				5 uimsbf
		current_next_indicator			1 bslbf
		section_number				8 uimsbf
		last_section_number			8 uimsbf
		reserved						16 bslbf
		reserved_future_use			4 bslbf
		service_list_descriptors_length	12 uimsbf
		for(i=0;i<N;i++){
			descriptor()
		}
		reserved_future_use			4 bslbf
		service_loop_length			12 uimsbf
		for(i=0;i<N;i++){
			service_id				16 uimsbf
			transport_stream_id		16 uimsbf
			original_network_id		16 uimsbf
			logical_channel_number 	14 uimsbf
			visible_service_flag	 	1 bslbf
			new_service_flag		 	1 bslbf
			genre_code				16 uimsbf
			reserved_future_use		4 bslbf
			service_descriptors_length	12 uimsbf
			for(j=0;j<N;j++){
				descriptor()
			}
		}
		CRC_32						32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct sgt_service_list_s {
	unsigned short service_id;
	unsigned short ts_id;
	unsigned short on_id;
	unsigned short logical_channel_number;
	unsigned char visible_service_flag;
	unsigned char new_service_flag;
	unsigned short genre_code;	
	int			 len;	/* descriptor loop length */
	descriptor_t *desc;

	struct sgt_service_list_s *next;
} sgt_service_list_t;

typedef struct sgt_section_s {
	unsigned char table_id;
	unsigned short service_list_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	descriptor_t *desc;
	sgt_service_list_t *service_list;
} sgt_section_t;

/* SKYLINK (CSLINK) - FST(FASTSCAN SERVICE TABLE) */
/*-----------------------------------------------------------------------------
	FST_section() {
		Table_id 						8 Uimsbf 0xBD
		section_syntax_indicator 		1 Bslbf 1b
		Reserved 					1 Bslbf 1b
		ISO reserved 					2 Bslbf 11b
		section_length 				12 uimsbf
		operator_network_id 			16 uimsbf
		Reserved 					2 bslbf 11b
		version_number 				5 uimsbf
		current_next_indicator 			1 bslbf 1b
		section_number 				8 uimsbf 0
		last_section_number 			8 uimsbf 0
		for (i=0, I<N, i++) {
			Original_network_id 		16 uimsbf
			Transport_stream_id 		16 uimsbf
			Service_id 				16 uimsbf
			Default_video_PID 			16 uimsbf
			Default_audio_PID 			16 uimsbf (*)
			Default_video_ECM_PID 	16 uimsbf
			Default_audio_ECM_PID 	16 uimsbf
			Default_PCR_PID 			16 uimsbf
			Reserved 				4 bslbf
			Descriptor_loop_length 		12 uimsbf
			Service_descriptor()
		}
		CRC_32 						32 Rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct fst_service_data_s {
	unsigned short on_id;
	unsigned short ts_id;
	unsigned short service_id;
	unsigned short video_pid;
	unsigned short audio_pid;
	unsigned short video_ecm_pid;
	unsigned short audio_ecm_pid;
	unsigned short pcr_pid;	
	descriptor_t *desc;

	struct fst_service_data_s *next;
} fst_service_data_t;

typedef struct fst_section_s {
	unsigned char table_id;
	unsigned short network_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	fst_service_data_t *svc_data;
} fst_section_t;

/* SKYLINK (CSLINK) - FNT(FASTSCAN NETWORK TABLE) */
/*-----------------------------------------------------------------------------
	FNT_section() {
		table_id 							8 uimsbf 0 0xBC
		section_syntax_indicator 			1 bslbf 1 1b
		reserved_future_use 				1 Bslbf 1 1b
		ISO reserved 						2 Bslbf 1 11b
		section_length 					12 Uimsbf 1, 2
		operator_network_id 				16 Uimsbf 3, 4
		Reserved 						2 Bslbf 5 11b
		version_number 					5 uimsbf 5
		current_next_indicator 				1 Bslbf 5 1b
		section_number 					8 Uimsbf 6
		last_section_number 				8 Uimsbf 6 7
		reserved_future_use 				4 bslbf 7 8 1111b
		network_descriptors_loop_length 		12 Uimsbf 7, 8 8, 9 0x000
		network_name_descriptor() 					9, 41 10, 	41 2 + str_len bytes assumption:	str_len=29
		reserved_future_use 				4 Bslbf 42 1111b
		transport_stream_loop_length 		Uimsbf 42, 43
			For (i=0, i<N, i++) {
			Transport_stream_id 			16 Uimsbf 44, 45
			Original_network_id 			16 Uimsbf 46, 47
			Reserved_future_use 			4 Bslbf 48 1111b
			Transport _descriptors_length 	12 Uimsbf 48, 49 ([1])
			Delivery_system descriptor() 			50, 63 14 bytes
			Service_list_descriptor() 				64, 89 Restricted to operators’only services 2+3*nb_svc	assumption: 8	service/TS
			logical_channel_descriptor() 				90, 123 2+4*nb_svc, same nb per TS as above
		}
		CRC_32 32 Rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct fst_ts_data_s {
	unsigned short ts_id;
	unsigned short on_id;
	descriptor_t *desc;

	struct fst_ts_data_s *next;
} fst_ts_data_t;

typedef struct fnt_section_s {
	unsigned char table_id;
	unsigned short network_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	descriptor_t *desc;
	fst_ts_data_t *ts_data;
} fnt_section_t;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/* ATSC - PSIP(Program and System Information Protocol for Terrestrial and Cable */
/*-----------------------------------------------------------------------------
	caption_service_descriptor() {
		descriptor_tag 						8 0x86
		descriptor_length 						8 uimsbf
		reserved 								3 ‘111’
		number_of_services 					5 uimsbf
		for (i=0; i<number_of_services; i++) {
			language 						8*3 uimsbf
			digital_cc 						1 bslbf
			reserved 							1 ‘1’
			if (digital_cc == line21) {
				reserved 						5 ‘11111’
				line21_field 					1 bslbf
			}
			else
			caption_service_number 			6 uimsbf
			easy_reader 						1 bslbf
			wide_aspect_ratio 					1 bslbf
			reserved 							14 ‘11111111111111’
		}
	}
-----------------------------------------------------------------------------*/
typedef struct psip_caption_service_s {
	unsigned char language[ISO_LANGUAGE_CODE];
	unsigned char digital_cc;
	unsigned char line21_field;
	unsigned char caption_service_number;
	unsigned char easy_reader;
	unsigned char wide_aspect_ratio;

	struct psip_caption_service_s *next;
} psip_caption_service_t;

typedef struct psip_caption_service_desc_s {
	unsigned char 			number_of_services;
	psip_caption_service_t *caption_service;
} psip_caption_service_desc_t;


/*-----------------------------------------------------------------------------
	content_advisory_descriptor() {
		descriptor_tag 						8 0x87
		descriptor_length 						8 uimsbf
		reserved 								2 ‘11’
		rating_region_count 					6
		for (i=0; i< rating_region_count; i++) {
			rating_region 						8 uimsbf
			rated_dimensions 					8 uimsbf
			for (j=0; j< rated_dimensions; j++) {
				rating_dimension_j 			8 uimsbf
				reserved 						4 ‘1111’
				rating_value 					4 uimsbf
			}
			rating_description_length 			8 uimsbf
			rating_description_text() 			var
		}
	}
-----------------------------------------------------------------------------*/
typedef struct psip_rated_dimensions_s {
	unsigned char rating_dimension;
	unsigned char rating_value;

	struct psip_rated_dimensions_s *next;
} psip_rated_dimensions_t;

typedef struct psip_rating_region_s {
	unsigned char rating_region;
	psip_rated_dimensions_t *rated_dimension;
	char *rating_description_text;

	struct psip_rating_region_s *next;
} psip_rating_region_t;

typedef struct psip_content_advisory_desc_s {
	psip_rating_region_t *rating_region;
} psip_content_advisory_desc_t;


/*-----------------------------------------------------------------------------
	extended_channel_name_descriptor() {
		descriptor_tag 				8 0xA0
		descriptor_length 				8 uimsbf
		long_channel_name_text() 		var
	}
-----------------------------------------------------------------------------*/
typedef struct psip_ext_channel_name_desc_s {
	char *long_channel_name;
} psip_ext_channel_name_desc_t;


/*-----------------------------------------------------------------------------
	service_location_descriptor() {
		descriptor_tag 				8 0xA1
		descriptor_length 				8 uimsbf
		reserved 						3 ‘111’
		PCR_PID 					13 uimsbf
		number_elements 				8 uimsbf
		for (i=0; i< number_elements; i++) {
			stream_type 				8 uimsbf
			reserved 					3 ‘111’
			elementary_PID 			13 uimsbf
			ISO_639_language_code 	8*3 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct psip_service_location_elements_s {
	unsigned char stream_type;
	unsigned short pid;
	char lang_code[ISO_LANGUAGE_CODE];
	
	struct psip_service_location_elements_s *next;
} psip_service_location_elements_t;

typedef struct psip_service_location_desc_s {
	unsigned short pcr_pid;
	psip_service_location_elements_t elements;
} psip_service_location_desc_t;


/*-----------------------------------------------------------------------------
	time_shifted_service_descriptor() {
		descriptor_tag 						8 0xA2
		descriptor_length 						8 uimsbf
		reserved 								3 ‘111’
		number_of_services 					5 uimsbf
		for (i=0; i< number_of_services; i++) {
			reserved 							6 ‘111111’
			time_shift 						10 uimsbf
			reserved 							4 ‘1111’
			major_channel_number 			10 uimsbf
			minor_channel_number 				10 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct psip_time_shifted_services_s {
	unsigned short time_shift;
	unsigned short major_channel_number;
	unsigned short minor_channel_number;

	struct psip_time_shifted_services_s *next;
} psip_time_shifted_services_t;

typedef struct psip_time_shifted_service_desc_s {
	psip_time_shifted_services_t *services;
} psip_time_shifted_service_desc_t;


/*-----------------------------------------------------------------------------
	component_name_descriptor() {
		descriptor_tag 				8 0xA3
		descriptor_length 				8 uimsbf
		component_name_string() 		var
	}
-----------------------------------------------------------------------------*/
typedef struct psip_component_name_desc_s {
	char *name;
} psip_component_name_desc_t;

/*-----------------------------------------------------------------------------
	dcc_departing_request_descriptor() {
		descriptor_tag 					8 0xA8
		descriptor_length 					8 uimsbf
		dcc_departing_request_type 			8 uimsbf
		dcc_departing_request_text_length 	8 uimsbf
		dcc_departing_request_text() 		var
	}
-----------------------------------------------------------------------------*/
typedef struct psip_dcc_departing_request_desc_s {
	unsigned char type;
	unsigned char *text;
} psip_dcc_departing_request_desc_t;


/*-----------------------------------------------------------------------------
	dcc_arriving_request_descriptor() {
		descriptor_tag 				8 0xA9
		descriptor_length 				8 uimsbf
		dcc_arriving_request_type 		8 uimsbf
		dcc_arriving_request_text_length 	8 uimsbf
		dcc_arriving_request_text() 		var
	}
-----------------------------------------------------------------------------*/
typedef struct psip_dcc_arriving_request_desc_s {
	unsigned char type;
	unsigned char *text;
} psip_dcc_arriving_request_desc_t;


/*-----------------------------------------------------------------------------
	redistribution_control_descriptor() {
		descriptor_tag 					8 0xAA
		descriptor_length 					8 uimsbf
		for (i=0; i< descriptor_length; i++) {
			rc_information() 				8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct psip_rc_information_s {
	unsigned char information;
	
	struct psip_rc_information_s *next;
} psip_rc_information_t;

typedef struct psip_rc_desc_s {
	psip_rc_information_t *rc;
} psip_rc_desc_t;


/*-----------------------------------------------------------------------------
	genre_descriptor() {
		descriptor_tag 					8 0xAB
		descriptor_length 					8 uimsbf
		reserved 							3 ‘111’
		attribute_count 					5 uimsbf
		for (i=0; i< attribute_count; i++) {
			attribute 						8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct psip_genre_attribute_s {
	unsigned char attribute;
	
	struct psip_genre_attribute_s *next;
} psip_genre_attribute_t;

typedef struct psip_genre_desc_s {
	psip_genre_attribute_t *genre;
} psip_genre_desc_t;


/*-----------------------------------------------------------------------------
	multiple_string_structure() {
		number_strings 							8 uimsbf
		for (i=0; i< number_strings; i++) {
			ISO_639_language_code 				24 uimsbf
			number_segments 						8 uimsbf
			for (j=0; j< number_segments; j++) {
				compression_type 					8 uimsbf
				mode 							8 uimsbf
				number_bytes 					8 uimsbf
				for (k=0; k< number_bytes; k++) {
					compressed_string_byte [k] 		8 bslbf
				}
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct psip_multiple_segments_s {
	unsigned char compression_type;
	unsigned char mode;
	unsigned char *compressed_string;
	
	struct psip_multiple_segments_s *next;
} psip_multiple_segments_t;

typedef struct psip_multiple_string_s {
	char lang_code[ISO_LANGUAGE_CODE];
	psip_multiple_segments_t *segments;
	
	struct psip_multiple_string_s *next;
} psip_multiple_string_t;

typedef struct psip_multiple_string_desc_s {
	psip_multiple_string_t *multiple_string;
} psip_multiple_string_desc_t;


/*-----------------------------------------------------------------------------
	daylight_saving() {
		DS_status 			1 bslbf
		reserved 				2 ‘11’
		DS_day_of_month 		5 uimsbf
		DS_hour 				8 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct psip_daylight_saving_desc_s {
	unsigned char status;
	unsigned char day; // day_of_month
	unsigned char hour;
} psip_daylight_saving_desc_t;


/*-----------------------------------------------------------------------------
	system_time_table_section() {
		table_id 					8 0xCD
		section_syntax_indicator 	1 ‘1’
		private_indicator 			1 ‘1’
		reserved 					2 ‘11’
		section_length 			12 uimsbf
		table_id_extension 			16 0x0000
		reserved 					2 ‘11’
		version_number 			5 ‘00000’
		current_next_indicator 		1 ‘1’
		section_number 			8 0x00
		last_section_number 		8 0x00
		protocol_version 			8 uimsbf
		system_time 				32 uimsbf
		GPS_UTC_offset 			8 uimsbf
		daylight_saving 			16 uimsbf
		for (i=0; i<N;i++) {
			descriptor()
		}
		CRC_32 					32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_stt_section_s {
	unsigned char table_id;
	unsigned short table_id_ext;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	unsigned int system_time;
	unsigned char gps_utc_offset;
	unsigned short daylight_saving;
	descriptor_t *desc;
} psip_stt_section_t;


/*-----------------------------------------------------------------------------
	master_guide_table_section() {
		table_id 							8 0xC7
		section_syntax_indicator 			1 ‘1’
		private_indicator 					1 ‘1’
		reserved 							2 ‘11’
		section_length 					12 uimsbf
		table_id_extension 					16 0x0000
		reserved 							2 ‘11’
		version_number 					5 uimsbf
		current_next_indicator 				1 ‘1’
		section_number 					8 0x00
		last_section_number 				8 0x00
		protocol_version 					8 uimsbf
		tables_defined 					16 uimsbf
		for (i=0; i< tables_defined;i++) {
			table_type 					16 uimsbf
			reserved 						3 ‘111’
			table_type_PID 				13 uimsbf
			reserved 						3 ‘111’
			table_type_version_number 		5 uimsbf
			number_bytes 				32 uimsbf
			reserved 						4 ‘1111’
			table_type_descriptors_length 	12 uimsbf
			for (k=0; k<N;k++) {
				descriptor()
			}
		}
		reserved 							4 ‘1111’
		descriptors_length 					12 uimsbf
		for (i=0; i<N;I++) {
			descriptor()
		}
		CRC_32 							32 rpchof
	}
-----------------------------------------------------------------------------*/
/*
Table 6.3 Table Types
table_type 			Meaning
0x0000 				Terrestrial VCT with current_next_indicator=’1’
0x0001 				Terrestrial VCT with current_next_indicator=’0’
0x0002 				Cable VCT with current_next_indicator=’1’
0x0003 				Cable VCT with current_next_indicator=’0’
0x0004 				Channel ETT
0x0005 				DCCSCT
0x0006-0x00FF 		[Reserved for future ATSC use]
0x0100-0x017F 		EIT-0 to EIT-127
0x0180-0x01FF 		[Reserved for future ATSC use]
0x0200-0x027F 		Event ETT-0 to event ETT-127
0x0280-0x0300 		[Reserved for future ATSC use]
0x0301-0x03FF 		RRT with rating_region 1-255
0x0400-0x0FFF 		[User private]
0x1000-0x13FF 		[Reserved for future ATSC use]
0x1400-0x14FF 		DCCT with dcc_id 0x00 . 0xFF
0x1500-0xFFFF 		[Reserved for future ATSC use]
*/
typedef struct psip_mgt_tables_s {
	unsigned short type;
	unsigned short pid;
	unsigned char version;
	unsigned int number_bytes;
	descriptor_t *desc;

	struct psip_mgt_tables_s *next;
} psip_mgt_tables_t;

typedef struct psip_mgt_section_s {
	unsigned char table_id;
	unsigned short table_id_ext;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	unsigned short tables_defined;
	psip_mgt_tables_t *tables;
	descriptor_t *desc;

} psip_mgt_section_t;


/*-----------------------------------------------------------------------------
	terrestrial_virtual_channel_table_section() {
		table_id 									8 0xC8
		section_syntax_indicator 					1 ‘1’
		private_indicator 							1 ‘1’
		reserved 									2 ‘11’
		section_length 							12 uimsbf
		transport_stream_id 						16 uimsbf
		reserved 									2 ‘11’
		version_number 							5 uimsbf
		current_next_indicator 						1 bslbf
		section_number 							8 uimsbf
		last_section_number 						8 uimsbf
		protocol_version 							8 uimsbf
		num_channels_in_section 					8 uimsbf
		for (i=0; i< num_channels_in_section; i++) {
			short_name 							7*16 uimsbf // UTF-16
			reserved 								4 ‘1111’
			major_channel_number 				10 uimsbf
			minor_channel_number 					10 uimsbf
			modulation_mode 						8 uimsbf
			carrier_frequency 						32 uimsbf
			channel_TSID 						16 uimsbf
			program_number 						16 uimsbf
			ETM_location 							2 uimsbf
			access_controlled 						1 bslbf
			hidden 								1 bslbf
			reserved 								2 ‘11’
			hide_guide 							1 bslbf
			reserved 								3 ‘111’
			service_type 							6 uimsbf
			source_id 							16 uimsbf
			reserved 								6 ‘111111’
			descriptors_length 						10 uimsbf
			for (i=0; i<N; i++) {
				descriptor()
			}
		}
		reserved 									6 ‘111111’
		additional_descriptors_length 				10 uimsbf
		for (j=0; j<N; j++) {
			additional_descriptor()
		}
		CRC_32 									32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_tvct_channel_s {
	unsigned short name[16]; // short_name
	unsigned short major_channel_number; // 0 ~ 999
	unsigned char minor_channel_number; // 1 ~ 99
	unsigned char modulation_mode;
	unsigned int frequency;
	unsigned short ts_id; // channel_TSID
	unsigned short service_id; // program_number
	unsigned char location; // ETM_location
	unsigned char access_controlled;
	unsigned char hidden;
	unsigned char hide_guide;
	unsigned char service_type;
	unsigned short source_id;
	descriptor_t *desc;

	struct psip_tvct_channel_s *next;
} psip_tvct_channel_t;

typedef struct psip_tvct_section_s {
	unsigned char table_id;
	unsigned short ts_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	psip_tvct_channel_t *channels;
	descriptor_t *desc;
} psip_tvct_section_t;


/*-----------------------------------------------------------------------------
	cable_virtual_channel_table_section() {
		table_id 									8 0xC9
		section_syntax_indicator 					1 ‘1’
		private_indicator 							1 ‘1’
		reserved 									2 ‘11’
		section_length 							12 uimsbf
		transport_stream_id 						16 uimsbf
		reserved 									2 ‘11’
		version_number 							5 uimsbf
		current_next_indicator 						1 bslbf
		section_number 							8 uimsbf
		last_section_number 						8 uimsbf
		protocol_version 							8 uimsbf
		num_channels_in_section 					8 uimsbf
		for (i=0; i< num_channels_in_section; i++) {
			short_name 							7*16 uimsbf
			reserved 								4 ‘1111’
			major_channel_number 				10 uimsbf
			minor_channel_number 					10 uimsbf
			modulation mode 						8 uimsbf
			carrier_frequency 						32 uimsbf
			channel_TSID 						16 uimsbf
			program_number 						16 uimsbf
			ETM_location 							2 uimsbf
			access_controlled 						1 bslbf
			hidden 								1 bslbf
			path_select 							1 bslbf
			out_of_band 							1 bslbf
			hide_guide 							1 bslbf
			reserved 								3 ‘111’
			service_type 							6 uimsbf
			source_id 							16 uimsbf
			reserved 								6 ‘111111’
			descriptors_length 						10 uimsbf
			for (i=0; i<N; i++) {
				descriptor()
			}
		}
		reserved 									6 ‘111111’
		additional_descriptors_length 				10 uimsbf
		for (j=0; j<N; j++) {
			additional_descriptor()
		}
		CRC_32 									32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_cvct_channel_s {
	unsigned short name[16]; // short_name
	unsigned short major_channel_number; // 0 ~ 999
	unsigned char minor_channel_number; // 1 ~ 99
	unsigned char modulation_mode;
	unsigned int frequency;
	unsigned short ts_id; // channel_TSID
	unsigned short service_id; // program_number
	unsigned char location; // ETM_location
	unsigned char access_controlled;
	unsigned char path_select; // 0 : path1, 1 : path2
	unsigned char out_of_band;
	unsigned char hidden;
	unsigned char hide_guide;
	unsigned char service_type;
	unsigned short source_id;
	descriptor_t *desc;

	struct psip_cvct_channel_s *next;
} psip_cvct_channel_t;

typedef struct psip_cvct_section_s {
	unsigned char table_id;
	unsigned short ts_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	psip_cvct_channel_t *channels;
	descriptor_t *desc;
} psip_cvct_section_t;


/*-----------------------------------------------------------------------------
	rating_region_table_section() {
		table_id 								8 0xCA
		section_syntax_indicator 				1 ‘1’
		private_indicator 						1 ‘1’
		reserved 								2 ‘11’
		section_length 						12 uimsbf
		table_id_extension {
			reserved 							8 0xFF
			rating_region 						8 uimsbf
		}
		reserved 								2 ‘11’
		version_number 						5 uimsbf
		current_next_indicator 					1 ‘1’
		section_number 						8 uimsbf
		last_section_number 					8 uimsbf
		protocol_version 						8 uimsbf
		rating_region_name_length 				8 uimsbf
		rating_region_name_text() 				var
		dimensions_defined 					8 uimsbf
		for (i=0; i< dimensions_defined; i++) {
			dimension_name_length 			8 uimsbf
			dimension_name_text() 			var
			reserved 							3 ‘111’
			graduated_scale 					1 bslbf
			values_defined 					4 uimsbf
			for (j=0; j< values_defined; j ++) {
				abbrev_rating_value_length 		8 uimsbf
				abbrev_rating_value_ text() 		var
				rating_value_length 			8 uimsbf
				rating_value_text() 			var
			}
		}
		reserved 								6 ‘111111’
		descriptors_length 						10 uimsbf
		for (i=0; i<N; i++) {
			descriptor()
		}
		CRC_32 								32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_rpt_values_defined_s {
	char *abbrev_rating_value;
	char *rating_value;

	struct psip_rpt_values_defined_s *next;
} psip_rpt_values_defined_t;

typedef struct psip_rpt_dimensions_defined_s {
	char *dimension_name;
	unsigned char graduated_scale;
	psip_rpt_values_defined_t *values;
	
	struct psip_rpt_dimensions_defined_s *next;
} psip_rpt_dimensions_defined_t;

typedef struct psip_rpt_section_s {
	unsigned char table_id;
	unsigned short rating_region; // table_id_extension - ((reserved 8B8T 0xFF) << 8) | (rating_region 8BIT) = 16BIT
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	char *rating_region_name;
	psip_rpt_dimensions_defined_t *dimensions_defined;
	descriptor_t *desc;
} psip_rpt_section_t;


/*-----------------------------------------------------------------------------
	event_information_table_section() {
		table_id 							8 0xCB
		section_syntax_indicator 			1 ‘1’
		private_indicator 					1 ‘1’
		reserved 							2 ‘11’
		section_length 					12 uimsbf
		source_id 						16 uimsbf
		reserved 							2 ’11’
		version_number 					5 uimsbf
		current_next_indicator 				1 ‘1’
		section_number 					8 uimsbf
		last_section_number 				8 uimsbf
		protocol_version 					8 uimsbf
		num_events_in_section 				8 uimsbf
		for (j=0; j< num_events_in_section; j++) {
			reserved 						2 ‘11’
			event_id 						14 uimsbf
			start_time 					32 uimsbf
			reserved 						2 ‘11’
			ETM_location 					2 uimsbf
			length_in_seconds 				20 uimsbf
			title_length 					8 uimsbf
			title_text() 					var
			reserved 						4 ‘1111’
			descriptors_length 				12
			for (i=0; i<N; i++) {
				descriptor()
			}
		}
		CRC_32 							32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_eit_data_s {
	unsigned short event_id;
	unsigned int start_time;
	unsigned char location; // ETM_location
	unsigned int length_in_seconds; // duration
	char *title;
	
	descriptor_t *desc;

	struct psip_eit_data_s *next;
} psip_eit_data_t;

typedef struct psip_eit_section_s {
	unsigned char table_id;
	unsigned short source_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	psip_eit_data_t *event_data;
	
} psip_eit_section_t;


/*-----------------------------------------------------------------------------
	extended_text_table_section() {
		table_id 						8 0xCC
		section_syntax_indicator 		1 ‘1’
		private_indicator 				1 ‘1’
		reserved 						2 ‘11’
		section_length 				12 uimsbf
		ETT_table_id_extension 			16 uimsbf
		reserved 						2 ‘11’
		version_number 				5 uimsbf
		current_next_indicator 			1 ‘1’
		section_number 				8 0x00
		last_section_number 			8 0x00
		protocol_version 				8 uimsbf
		ETM_id 						32 uimsbf
		extended_text_message() 		var
		CRC_32 						32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_ett_section_s {
	unsigned char table_id;
	unsigned short table_id_ext; // ETT_table_id_extension
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	unsigned int etm_id; // extended text message id
	char *etm;
	
} psip_ett_section_t;


/*-----------------------------------------------------------------------------
	directed_channel_change_table_section() {
		table_id 								8 0xD3
		section_syntax_indicator 				1 ‘1’
		private_indicator 						1 ‘1’
		reserved 								2 ‘11’
		section_length 						12 uimsbf
		dcc_subtype 							8 0x00
		dcc_id 								8 uimsbf
		reserved 								2 ‘11’
		version_number 						5 uimsbf
		current_next_indicator 					1 ‘1’
		section_number 						8 0x00
		last_section_number 					8 0x00
		protocol_version 						8 uimsbf
		dcc_test_count 						8 uimsbf
		for (i=0; i< dcc_test_count; i++) {
			dcc_context 						1 uimsbf
			reserved 							3 ‘111’
			dcc_from_major_channel_number 	10 uimsbf
			dcc_from_minor_channel_number 	10 uimsbf
			reserved 							4 ‘1111’
			dcc_to_major_channel_number 		10 uimsbf
			dcc_to_minor_channel_number 		10 uimsbf
			dcc_start_time 					32 uimsbf
			dcc_end_time 						32 uimsbf
			dcc_term_count 					8 uimsbf
			for (j=0; j< dcc_term_count; j++) {
				dcc_selection_type 			8 uimsbf
				dcc_selection_id 				64 uimsbf
				reserved 						6 ‘111111’
				dcc_term_descriptors_length 	10 uimsbf
				for (k=0; k<N; k++) {
					dcc_term_descriptor()
				}
			}
			reserved 							6 ‘111111’
			dcc_test_descriptors_length 			10 uimsbf
			for (j=0; j<N; j++) {
				dcc_test_descriptor()
			}
		}
		reserved 								6 ‘111111’
		dcc_additional_descriptors_length 		10 uimsbf
		for (i=0; i<N; i++) {
			dcc_additional_descriptor()
		}
		CRC_32 								32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_dcc_term_s {
	unsigned char dcc_selection_type;
	unsigned int dcc_selection_id[2];
	descriptor_t *desc; // dcc_term_descriptor

	struct psip_dcc_term_s *next;
} psip_dcc_term_t;

typedef struct psip_dcc_test_s {
	unsigned char dcc_context;
	unsigned short dcc_from_major_channel_number;
	unsigned short dcc_from_minor_channel_number;
	unsigned short dcc_to_major_channel_number;
	unsigned short dcc_to_minor_channel_number;
	unsigned int dcc_start_time;
	unsigned int dcc_end_time;
	psip_dcc_term_t *dcc_term;
	descriptor_t *desc; // dcc_test_descriptor

	struct psip_dcc_test_s *next;
} psip_dcc_test_t;

typedef struct psip_dcct_section_s {
	unsigned char table_id;
	unsigned char dcc_subtype;
	unsigned char dcc_id;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	psip_dcc_test_t *dcc_test;
	descriptor_t *desc; // dcc_additional_descriptor
	
} psip_dcct_section_t;

/*-----------------------------------------------------------------------------
	dcc_selection_code_table_section() {
		table_id 								8 0xD4
		section_syntax_indicator 				1 ‘1’
		private_indicator 						1 ‘1’
		reserved 								2 ‘11’
		section_length 						12 uimsbf
		dccsct_type 							16 uimsbf
		reserved 								2 ‘11’
		version_number 						5 uimsbf
		current_next_indicator 					1 ‘1’
		section_number 						8 0x00
		last_section_number 					8 0x00
		protocol_version 						8 uimsbf
		updates_defined 						8 uimsbf
		for (i=0; i< updates_defined; i++) {
			update_type 						8 uimsbf
			update_data_length 				8 uimsbf
			if (update_type == new_genre_category) {
				genre_category_code 			8 uimsbf
				genre_category_name_text() 	var
			}
			if (update_type == new_state) {
				dcc_state_location_code 		8 uimsbf
				dcc_state_location_code_text() 	var
			}
			if (update_type == new_county) {
			state_code 						8 uimsbf
			reserved 							6 ‘111111’
				dcc_county_location_code 		10 uimsbf
				dcc_county_location_code_text() 	var
			}
			reserved 							6 ‘111111’
			dccsct_descriptors_length 			10 uimsbf
			for (j=0; j<N; j++) {
				dccsct_descriptors()
			}
		}
		reserved 								6 ‘111111’
		dccsct_additional_descriptors_length 		10 uimsbf
		for (i=0; i<N; i++) {
			dccsct_additional_descriptors()
		}
		CRC_32 								32 rpchof
	}
-----------------------------------------------------------------------------*/
typedef struct psip_dccsc_updated_defined_s {
	unsigned char update_type;
	unsigned char genre_category_code;
	char *genre_category_name;
	unsigned char dcc_state_location_code;
	char *dcc_state_location_code_text;
	unsigned char state_code;
	unsigned short dcc_county_location_code;
	char *dcc_county_location_code_text;
	descriptor_t *desc; // dccsct_descriptors

	struct psip_dccsc_updated_defined_s *next;
} psip_dccsc_updated_defined_t;

typedef struct psip_dccsct_section_s {
	unsigned char table_id;
	unsigned short dccsct_type;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	unsigned char protocol_version;
	psip_dccsc_updated_defined_t *updated_defined;
	descriptor_t *desc; // dccsct_additional_descriptors
	
} psip_dccsct_section_t;

/*-----------------------------------------------------------------------------
compressed_bitmap(){
	steering1 				1 				bslbf
	if (steering1 == 0)
	{
		steering2 			1 				bslbf
		If (steering2 == 0)
		{
			steering3 		1 				bslbf
			if (steering3 == 0)
			{
				operator 		2 				enum {noop, eol, reserved1..2}
			} 
			else 
			{
				run_length_16_on 	4 			uimsbf pixels set on
			}
		} 
		else 
		{ 
			run_length_64_off 		6 			uimsbf pixels set off
		}
	} 
	else 
	{
		run_length_8_on 			3 			uimsbf pixels set on
		run_length_32_off 			5 			uimsbf pixels set off
	}
}
-----------------------------------------------------------------------------*/
typedef struct compressed_bitmap_s {
	unsigned char Y_component;
	unsigned char opaque_enable;
	unsigned char Cr_component;
	unsigned char Cb_component;
	
}compressed_bitmap_t;

/*-----------------------------------------------------------------------------
Color Vector Formatting
color(){
	Y_component 				5 			2 			uimsbf range 0-31
	opaque_enable 			1 						bslbf {no, yes}
	Cr_component 			5 						uimsbf range 0-31
	Cb_component 			5 						uimsbf range 0-31
	}
-----------------------------------------------------------------------------*/
typedef struct color_s {
	unsigned char Y_component;
	unsigned char opaque_enable;
	unsigned char Cr_component;
	unsigned char Cb_component;
	
}color_t;

/*-----------------------------------------------------------------------------
simple_bitmap(){
	reserved 					5 		1 			bslbf
	background_style 			1 					bslbf {transparent, framed}
	outline_style 				2 					uimsbf {none, outline, drop_shadow, reserved}
	character_color() 			16 		2 			Y, Cr, Cb
	bitmap_top_H_coordinate 	12 		3 			uimsbf Horizontal Coordinate
	bitmap_top_V_Coordinate 	12 					uimsbf Vertical Coordinate
	bitmap_bottom_H_coordinate 12 		3 			uimsbf Horizontal Coordinate
	bitmap_bottom_V_coordinate 12 					uimsbf Vertical Coordinate
	if (background_style==framed)
	{ 
		frame_top_H_coordinate 12 		(3) 			uimsbf Vertical Coordinate
		frame_top_V_coordinate 12 					uimsbf Vertical Coordinate
		frame_bottom_H_coordinate 12 	(3) 			uimsbf Horizontal Coordinate
		frame_bottom_V_coordinate 12 				uimsbf Vertical Coordinate
		frame_color() 				16 	(2) 			Y, Cr, Cb
	}
	if (outline_style==outlined)
	{
		reserved 					4 	(1) 			bslbf
		outline_thickness 			4 				uimsbf range 0-15
		outline_color() 			16  	(2) 			Y, Cr, Cb
	} 
	else if (outline_style==drop_shadow)
	{
		shadow_right 				4 	(1) 			uimsbf range 0-15
		shadow_bottom 			4 				uimsbf range 0-15
		shadow_color() 			16 	(2) 			Y, Cr, Cb
	}
	else if (outline_style==reserved)
	{
		reserved 					24 	(3) 			bslbf
	}
	bitmap_length 				16 	2 			uimsbf (N)
	compressed_bitmap() 			8*N 	N 			bslbf bitstream of tokens
}
-----------------------------------------------------------------------------*/
typedef struct coordinate_s {
	color_t color;
	unsigned short top_H_coordinate;
	unsigned short top_V_Coordinate;
	unsigned short bottom_H_coordinate;
	unsigned short bottom_V_coordinate;
	
}coordinate_t;

typedef struct frame_s {
	unsigned short top_H_coordinate;
	unsigned short top_V_Coordinate;
	unsigned short bottom_H_coordinate;
	unsigned short bottom_V_coordinate;
	color_t color;
	
}frame_t;

typedef struct outline_s {
	unsigned char thickness;
	color_t color;
	
}outline_t;

typedef struct shadow_s {
	unsigned char right;
	unsigned char bottom;
	color_t color;
	
}shadow_t;

typedef struct simple_bitmap_s {
	unsigned char background_style;
	unsigned char outline_style;
	coordinate_t bitmap; 
	frame_t frame; 
	outline_t outlined;
	shadow_t shadow;

	compressed_bitmap_t compressed_bitmap;
} simple_bitmap_t;

typedef struct message_body_s {
	char language[ISO_LANGUAGE_CODE];
	unsigned char pre_clear_display;
	unsigned char immediate;
	unsigned char display_standard;
	unsigned int display_in_pts;
	unsigned char subtitle_type;
	unsigned short display_duration;

//	simple_bitmap_t simple_bitmap;
}message_body_t;

/*-----------------------------------------------------------------------------
subtitle_message(){
	table_ID							8 			1 			uimsbf (value 0xC6)
	zero								2			2			bslbf
	ISO reserved						2						bslbf
	section_length						12						uimsbf
	zero								1			1			bslbf {false}
	segmentation_overlay_included		1						bslbf {false, true}
	protocol_version					6						uimsbf
	if (segmentation_overlay_included) 
	{
		table_extension				16			(2)			uimsbf
		last_segment_number			12			(3)			uimsbf range 0-4095
		segment_number				12						uimsbf range 0-4095
	}
	// message body
	ISO_639_language_code			24			3			uimsbf per Reference [2]
	pre_clear_display					1			1			bslbf {no, yes}
	immediate						1						bslbf {no, yes}
	reserved							1						bslbf
	display_standard					5						uimsbf {_720_480_30,
																	_720_576_25, _1280_720_60,
																	_1920_1080_60, reserved1..N}
	display_in_PTS					32			4			uimsbf
	subtitle_type						4			2			uimsbf {reserved1,simple_bitmap, reserved2..N}
	reserved							1						bslbf
	display_duration					11						uimsbf units: frames;maximum: 2000
	block_length						16			2			uimsbf (B)
	if (subtitle_type==simple_bitmap) 
	{
		simple_bitmap()				B*8			(B)			
	}
	else 
	{
		reserved()					B*8			(B)
	}
	for (i=0; i<N; i++) 
	{
		descriptor()					*			(*)
		optional descriptors
	}
	CRC_32							32			4			rpchof
}
-----------------------------------------------------------------------------*/
typedef struct dcii_section_s {
	unsigned char table_id;
	unsigned char segmentation_overlay_included;
	unsigned char protocol_version;
	unsigned short table_extension;
	signed short last_segment_number;
	signed short segment_number;	
	message_body_t message_body;
	
	descriptor_t *desc;
} dcii_section_t;

/*-----------------------------------------------------------------------------
	application_identifier {
		organisation_id 	32
		application_id 		16
	} 
-----------------------------------------------------------------------------*/
typedef struct ait_application_identifier_s {
	unsigned int organisation_id;
	unsigned short application_id;
} ait_application_identifier_t;

typedef struct ait_application_data_s {
	ait_application_identifier_t application_identifier;
	unsigned char application_control_code;
	descriptor_t *desc;
	
	struct ait_application_data_s *next;
} ait_application_data_t;

/*-----------------------------------------------------------------------------
	application_information_section() {		
		table_id					8	uimsbf
		section_syntax_indicator	1	bslbf
		reserved_future_use			1	bslbf
		reserved					2	bslbf
		section_length				12	uimsbf
		test_application_flag		1 	bslbf
		application_type 			15 	uimsbf 
		reserved					2	bslbf
		version_number				5	uimsbf
		current_next_indicator		1	bslbf
		section_number				8	uimsbf
		last_section_number			8	uimsbf
		reserved_future_use 		4 	bslbf
		common_descriptors_length 	12 	uimsbf 
		for (i=0; i<N;i++) {		
			descriptor()		
		}
		reserved_future_use 						4 	bslbf 
		application_loop_length 					12 	uimsbf 
		for(i=0;i<N;i++){
			application_identifier() 				48
			application_control_code 				8 	uimsbf
			reserved_future_use 					4 	bslbf
			application_descriptors_loop_length 	12 	uimsbf
			for(j=0;j<N;j++){
				descriptor()
			}
		}		
 		CRC_32						32	rpchof
	}		
-----------------------------------------------------------------------------*/
typedef struct ait_section_s {
	unsigned char table_id;
	unsigned char reserved;
	unsigned char test_application_flag;
	unsigned short application_type;
	unsigned char version;
	unsigned char section;
	unsigned char last_section;
	descriptor_t *desc;

	ait_application_data_t *application_data;
} ait_section_t;

/*-----------------------------------------------------------------------------
	application_descriptor() {
		descriptor_tag 					8 uimsbf 0x00
		descriptor_length 				8 uimsbf
		application_profiles_length 	8 uimsbf
		for( i=0; i<N; i++ ) {
			application_profile 		16 uimsbf
			version.major 				8 uimsbf
			version.minor 				8 uimsbf
			version.micro 				8 uimsbf
		}
		service_bound_flag 				1 bslbf
		visibility 						2 bslbf
		reserved_future_use 			5 bslbf
		application_priority 			8 uimsbf
		for( i=0; i<N; i++ ) {
			transport_protocol_label 	8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_application_profile_data_s {
	unsigned short application_profile;
	unsigned char version_major;
	unsigned char version_minor;
	unsigned char version_micro;
	
	struct ait_application_profile_data_s	*next;
} ait_application_profile_data_t;

typedef struct ait_application_desc_s {
	ait_application_profile_data_t *profile_data;
	
	unsigned char service_bound_flag;
	unsigned char visibility;
	unsigned char application_priority;
	
	unsigned char *transport_protocol_label;
	int transport_protocol_label_length;
} ait_application_desc_t;

/*-----------------------------------------------------------------------------
	application_name_descriptor() {
		descriptor_tag 						8 uimsbf 0x01
		descriptor_length 					8 uimsbf
		for (i=0; i<N; i++) {
			ISO_639_language_code 			24 bslbf
			application_name_length 		8 uimsbf
			for (i=0; i<N; i++) {
				application_name_char 		8 uimsbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_application_name_data_s {
	char			lang_code[ISO_LANGUAGE_CODE];
	char			*name;
	
	struct ait_application_name_data_s	*next;
} ait_application_name_data_t;

typedef struct ait_application_name_desc_s {
	ait_application_name_data_t	*name_data;
} ait_application_name_desc_t;

/*-----------------------------------------------------------------------------
	for( i=0; i<N; i++){
		URL_base_length 						8 uimsbf
		for( j=0; j<N; j++){
			URL_base_byte 						8 uimsbf
		}
		URL_extension_count 					8 uimsbf
		for( j=0; j<URL_extension_count; j++){
			URL_extension_length 				8 uimsbf
			for(k=0; k<URL_length; k++){
				URL_extension_byte 				8 uimsbf
			}
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_url_extension_data_s {
	unsigned char	URL_extension_length;
	unsigned char	*URL_extension_byte;

	struct ait_url_extension_data_s	*next;
} ait_url_extension_data_t;

typedef struct ait_select_byte_data_s {
	unsigned char	URL_base_length;
	unsigned char	*URL_base_byte;
	unsigned char	URL_extension_count;
	ait_url_extension_data_t *url_extension_data;

	struct ait_select_byte_data_s	*next;
} ait_select_byte_data_t;

typedef struct ait_select_byte_S {
	ait_select_byte_data_t *select_byte_data;
} ait_select_byte_t;

/*-----------------------------------------------------------------------------
	transport_protocol_descriptor() {
		descriptor_tag 					8 uimsbf 0x02
		descriptor_length 				8 uimsbf
		protocol_id 					16 uimsbf
		transport_protocol_label 		8 uimsbf
		for(i=0; i<N; i++) {
			selector_byte 				8 uimsbf N1
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_transport_protocol_desc_s {
	unsigned short	protocol_id;
	unsigned char	transport_protocol_label;
	unsigned char	*selector_byte;
	int 			selector_byte_length;
} ait_transport_protocol_desc_t;

/*-----------------------------------------------------------------------------
	external_application_authorisation_descriptor() {
		descriptor_tag 						8 uimsbf 0x05
		descriptor_length 					8 uimsbf
		for(i=0; i<N; i++) {
			application_identifier()		48
			application_priority 			8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_external_application_auth_data_s {
	ait_application_identifier_t application_identifier;
	unsigned char application_priority;
	
	struct ait_external_application_auth_data_s	*next;
} ait_external_application_auth_data_t;

typedef struct ait_external_application_auth_desc_s {
	ait_external_application_auth_data_t *auth_data;
} ait_external_application_auth_desc_t;

/*-----------------------------------------------------------------------------
	application_recording_descriptor (){
		descriptor_tag 						8 uimsbf 0x06
		descriptor_length 					8 uimsbf
		scheduled_recording_flag 			1 bslbf
		trick_mode_aware_flag 				1 bslbf
		time_shift_flag 					1 bslbf
		dynamic_flag 						1 bslbf
		av_synced_flag 						1 bslbf
		initiating_replay_flag 				1 bslbf
		reserved 							2 bslbf
		label_count 						8 uimsbf N0
		for(i=0;i<N0;i++){
			label_length 					8 uimsbf N1
			for(j=0; j<N1; j++) {
				label_char 					8 uimsbf
			}
			storage_properties 				2 uimsbf
			reserved 						6
		}
		component_tag_list_length 			8 uimsbf N2
		for(i=0;i<N2;i++){
			component_tag 					8 uimsbf
		}
		private_length 						8 uimsbf N3
		for(i=0;i<N3;i++){
			private 						8 uimsbf
		}
		for(i=0;i<N4;i++){
			reserved_future_use 			8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_application_recording_lebel_data_s {
	unsigned char	*label_char;
	unsigned char	label_length;
	unsigned char	storage_properties;

	struct ait_application_recording_lebel_data_s *next; 
} ait_application_recording_lebel_data_t;

typedef struct ait_application_recording_desc_s {
	unsigned char	scheduled_recording_flag;
	unsigned char	trick_mode_aware_flag;
	unsigned char	time_shift_flag;
	unsigned char	dynamic_flag;
	unsigned char	av_synced_flag;
	unsigned char	initiating_replay_flag;
	unsigned char	label_count;
	ait_application_recording_lebel_data_t *lebel_data;
	unsigned char	component_tag_list_length;
	unsigned char	*component_tag;
	unsigned char	private_data_length;
	unsigned char 	*private_data;
	unsigned char	*reserved_future_use;
	unsigned char	reserved_future_use_length;

} ait_application_recording_desc_t;

/*-----------------------------------------------------------------------------
	application_storage_descriptor() {
		descriptor_tag 						8 uimsbf 0x10
		descriptor_length 					8 uimsbf
		storage_property 					8 uimsbf
		not_launchable_from_broadcast 		1 bslbf
		launchable_completely_from_cache 	1 bslbf
		is_launchable_with_older_version 	1 bslbf
		Reserved 							5 bslbf
		Reserved 							1 bslbf
		Version 							31 uimsbf
		Priority 							8 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct ait_application_icons_desc_s {
	int	icon_locator_length;
	unsigned char	*icon_locator_byte;
	unsigned short	icon_flags;
	unsigned char	*reserved_future_use;
	int	reserved_future_use_length;
} ait_application_icons_desc_t;

/*-----------------------------------------------------------------------------
	application_storage_descriptor() {
		descriptor_tag 						8 uimsbf 0x10
		descriptor_length 					8 uimsbf
		storage_property 					8 uimsbf
		not_launchable_from_broadcast 		1 bslbf
		launchable_completely_from_cache 	1 bslbf
		is_launchable_with_older_version 	1 bslbf
		Reserved 							5 bslbf
		Reserved 							1 bslbf
		Version 							31 uimsbf
		Priority 							8 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct ait_application_storage_desc_s {
	unsigned char	storage_property;
	unsigned char	not_launchable_from_broadcast;
	unsigned char	launchable_completely_from_cache;
	unsigned char	is_launchable_with_older_version;
	unsigned int	Version; // 31bit
	unsigned char	Priority;
} ait_application_storage_desc_t;

/*-----------------------------------------------------------------------------
	graphics_constraints_descriptor() {
		descriptor_tag 						8 uimsbf 0x14
		descriptor_length 					8 uimsbf
		reserved_future_use 				5 bslbf
		can_run_without_visible_ui 			1 bslbf
		handles_configuration_changed 		1 bslbf
		handles_externally_controlled_video 1 bslbf
		for(i=0;i<N;i++) {
			graphics_configuration_byte 	8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_graphics_constraints_desc_s {
	unsigned char	can_run_without_visible_ui;
	unsigned char	handles_configuration_changed;
	unsigned char	handles_externally_controlled_video;
	unsigned char	*graphics_configuration_byte;
	int	graphics_configuration_byte_length;
} ait_graphics_constraints_desc_t;

/*-----------------------------------------------------------------------------
	simple_application_location_descriptor () {
		descriptor_tag 					8 uimsbf 0x15
		descriptor_length 				8 uimsbf
		for(i=0; i<N; i++) {
			initial_path_bytes 			8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/
typedef struct ait_simple_application_location_desc_s {
	unsigned char	*initial_path_bytes;
	int	initial_path_bytes_length;
} ait_simple_application_location_desc_t;

/*-----------------------------------------------------------------------------
	application_usage_descriptor() {
		descriptor_tag 			8 uimsbf 0x16
		descriptor_length 		8 uimsbf
		usage_type 				8 uimsbf
	}
-----------------------------------------------------------------------------*/
typedef struct ait_application_usage_desc_s {
	unsigned char	usage_type;
} ait_application_usage_desc_t;

/*-----------------------------------------------------------------------------
	simple_application_boundary_descriptor {
		descriptor_tag 								8 uimsbf 0x17
		descriptor_length 							8 uimsbf
		boundary_extension_count 					8 uimsbf
		for( j=0; j<boundary_extension_count; j++){
			boundary_extension_length 				8 uimsbf
			for(k=0; k<boundary_extension_length; k++){
				boundary_extension_byte 			8 uimsbf
		}
	}
-----------------------------------------------------------------------------*/

typedef struct ait_boundary_extension_data_s {
	unsigned char	*boundary_extension_byte;
	unsigned char	boundary_extension_length;

	struct ait_boundary_extension_data_s	*next;
} ait_boundary_extension_data_t;

typedef struct ait_simple_application_boundary_desc_s {
	unsigned char	boundary_extension_count;
	ait_boundary_extension_data_t *boundary_extension_data;
} ait_simple_application_boundary_desc_t;

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
extern int detect_packet_len(unsigned char *p);
extern void init_crc32_table(void);
extern unsigned int get_mpeg_crc32(unsigned char *data, int len);

#endif /* DVB_H */
