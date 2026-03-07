/******************************************************************************
*
* iso3166.h
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
#ifndef iso3166
#define iso3166

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
typedef struct
{
	const char *alpha2;
	const char *alpha3;
	int numeric;
	const char *name;

} ISO3166_Entry;

static const ISO3166_Entry iso3166_db[] =
{
	{"AF", "AFG", 4, "Afghanistan"},
	{"AL", "ALB", 8, "Albania"},
	{"DZ", "DZA", 12, "Algeria"},
	{"AR", "ARG", 32, "Argentina"},
	{"AU", "AUS", 36, "Australia"},
	{"AT", "AUT", 40, "Austria"},
	{"BD", "BGD", 50, "Bangladesh"},
	{"BE", "BEL", 56, "Belgium"},
	{"BR", "BRA", 76, "Brazil"},
	{"BG", "BGR", 100, "Bulgaria"},
	{"CA", "CAN", 124, "Canada"},
	{"CL", "CHL", 152, "Chile"},
	{"CN", "CHN", 156, "China"},
	{"CO", "COL", 170, "Colombia"},
	{"DK", "DNK", 208, "Denmark"},
	{"EG", "EGY", 818, "Egypt"},
	{"FI", "FIN", 246, "Finland"},
	{"FR", "FRA", 250, "France"},
	{"DE", "DEU", 276, "Germany"},
	{"GR", "GRC", 300, "Greece"},
	{"HK", "HKG", 344, "Hong Kong"},
	{"HU", "HUN", 348, "Hungary"},
	{"IN", "IND", 356, "India"},
	{"ID", "IDN", 360, "Indonesia"},
	{"IR", "IRN", 364, "Iran"},
	{"IQ", "IRQ", 368, "Iraq"},
	{"IE", "IRL", 372, "Ireland"},
	{"IL", "ISR", 376, "Israel"},
	{"IT", "ITA", 380, "Italy"},
	{"JP", "JPN", 392, "Japan"},
	{"KR", "KOR", 410, "Korea, Republic of"},
	{"KP", "PRK", 408, "Korea, Democratic People's Republic of"},
	{"MY", "MYS", 458, "Malaysia"},
	{"MX", "MEX", 484, "Mexico"},
	{"NL", "NLD", 528, "Netherlands"},
	{"NZ", "NZL", 554, "New Zealand"},
	{"NO", "NOR", 578, "Norway"},
	{"PK", "PAK", 586, "Pakistan"},
	{"PH", "PHL", 608, "Philippines"},
	{"PL", "POL", 616, "Poland"},
	{"PT", "PRT", 620, "Portugal"},
	{"RO", "ROU", 642, "Romania"},
	{"RU", "RUS", 643, "Russian Federation"},
	{"SA", "SAU", 682, "Saudi Arabia"},
	{"SG", "SGP", 702, "Singapore"},
	{"ZA", "ZAF", 710, "South Africa"},
	{"ES", "ESP", 724, "Spain"},
	{"SE", "SWE", 752, "Sweden"},
	{"CH", "CHE", 756, "Switzerland"},
	{"TH", "THA", 764, "Thailand"},
	{"TR", "TUR", 792, "Turkey"},
	{"AE", "ARE", 784, "United Arab Emirates"},
	{"GB", "GBR", 826, "United Kingdom"},
	{"US", "USA", 840, "United States"},
	{"VN", "VNM", 704, "Vietnam"}

};
	
#define ISO3166_DB_SIZE (sizeof(iso3166_db) / sizeof(ISO3166_Entry))

typedef enum
{
    COUNTRY_UNKNOWN = 0,
    COUNTRY_AF = 4,    // Afghanistan
    COUNTRY_AL = 8,    // Albania
    COUNTRY_DZ = 12,   // Algeria
    COUNTRY_AR = 32,   // Argentina
    COUNTRY_AU = 36,   // Australia
    COUNTRY_AT = 40,   // Austria
    COUNTRY_BD = 50,   // Bangladesh
    COUNTRY_BE = 56,   // Belgium
    COUNTRY_BR = 76,   // Brazil
    COUNTRY_BG = 100,  // Bulgaria
    COUNTRY_CA = 124,  // Canada
    COUNTRY_CL = 152,  // Chile
    COUNTRY_CN = 156,  // China
    COUNTRY_CO = 170,  // Colombia
    COUNTRY_DK = 208,  // Denmark
    COUNTRY_EG = 818,  // Egypt
    COUNTRY_FI = 246,  // Finland
    COUNTRY_FR = 250,  // France
    COUNTRY_DE = 276,  // Germany
    COUNTRY_GR = 300,  // Greece
    COUNTRY_HK = 344,  // Hong Kong
    COUNTRY_HU = 348,  // Hungary
    COUNTRY_IN = 356,  // India
    COUNTRY_ID = 360,  // Indonesia
    COUNTRY_IR = 364,  // Iran
    COUNTRY_IQ = 368,  // Iraq
    COUNTRY_IE = 372,  // Ireland
    COUNTRY_IL = 376,  // Israel
    COUNTRY_IT = 380,  // Italy
    COUNTRY_JP = 392,  // Japan
    COUNTRY_KP = 408,  // North Korea
    COUNTRY_KR = 410,  // South Korea
    COUNTRY_MY = 458,  // Malaysia
    COUNTRY_MX = 484,  // Mexico
    COUNTRY_NL = 528,  // Netherlands
    COUNTRY_NZ = 554,  // New Zealand
    COUNTRY_NO = 578,  // Norway
    COUNTRY_PK = 586,  // Pakistan
    COUNTRY_PH = 608,  // Philippines
    COUNTRY_PL = 616,  // Poland
    COUNTRY_PT = 620,  // Portugal
    COUNTRY_RO = 642,  // Romania
    COUNTRY_RU = 643,  // Russia
    COUNTRY_VN = 704,  // Vietnam
    COUNTRY_SG = 702,  // Singapore
    COUNTRY_ZA = 710,  // South Africa
    COUNTRY_ES = 724,  // Spain
    COUNTRY_SE = 752,  // Sweden
    COUNTRY_CH = 756,  // Switzerland
    COUNTRY_TH = 764,  // Thailand
    COUNTRY_TR = 792,  // Turkey
    COUNTRY_AE = 784,  // UAE
    COUNTRY_GB = 826,  // United Kingdom
    COUNTRY_US = 840,   // United States

} CountryCode;

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
extern int IsValidCountryCode(const char* alpha2);
extern const char* ISO3166_GetNameByAlpha2(const char* alpha2);
extern CountryCode ISO3166_GetCountryCodeByAlpha2(const char* alpha2);

#endif /* iso3166 */
