/******************************************************************************
*
* iso639.h
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
#ifndef ISO639_H
#define ISO639_H

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
	const char* name;	 // 언어 이름
	const char* alpha2;  // ISO 639-1
	const char* alpha3;  // ISO 639-2

} ISO639_Entry;

static const ISO639_Entry iso639_db[] =
{
	{"Afar", "aa", "aar"}, {"Abkhazian", "ab", "abk"}, {"Afrikaans", "af", "afr"},
	{"Amharic", "am", "amh"}, {"Arabic", "ar", "ara"}, {"Assamese", "as", "asm"},
	{"Aymara", "ay", "aym"}, {"Azerbaijani", "az", "aze"}, {"Bashkir", "ba", "bak"},
	{"Belarusian", "be", "bel"}, {"Bulgarian", "bg", "bul"}, {"Bengali", "bn", "ben"},
	{"Tibetan", "bo", "bod"}, {"Breton", "br", "bre"}, {"Bosnian", "bs", "bos"},
	{"Catalan", "ca", "cat"}, {"Czech", "cs", "ces"}, {"Danish", "da", "dan"},
	{"German", "de", "deu"}, {"Greek", "el", "ell"}, {"English", "en", "eng"},
	{"Spanish", "es", "spa"}, {"Estonian", "et", "est"}, {"Basque", "eu", "eus"},
	{"Persian", "fa", "fas"}, {"Finnish", "fi", "fin"}, {"French", "fr", "fra"},
	{"Irish", "ga", "gle"}, {"Hebrew", "he", "heb"}, {"Hindi", "hi", "hin"},
	{"Croatian", "hr", "hrv"}, {"Hungarian", "hu", "hun"}, {"Indonesian", "id", "ind"},
	{"Icelandic", "is", "isl"}, {"Italian", "it", "ita"}, {"Japanese", "ja", "jpn"},
	{"Georgian", "ka", "kat"}, {"Kazakh", "kk", "kaz"}, {"Korean", "ko", "kor"},
	{"Latin", "la", "lat"}, {"Lithuanian", "lt", "lit"}, {"Latvian", "lv", "lav"},
	{"Macedonian", "mk", "mkd"}, {"Malay", "ms", "msa"}, {"Maltese", "mt", "mlt"},
	{"Dutch", "nl", "nld"}, {"Norwegian", "no", "nor"}, {"Polish", "pl", "pol"},
	{"Portuguese", "pt", "por"}, {"Romanian", "ro", "ron"}, {"Russian", "ru", "rus"},
	{"Slovak", "sk", "slk"}, {"Slovenian", "sl", "slv"}, {"Albanian", "sq", "sqi"},
	{"Serbian", "sr", "srp"}, {"Swedish", "sv", "swe"}, {"Thai", "th", "tha"},
	{"Turkish", "tr", "tur"}, {"Ukrainian", "uk", "ukr"}, {"Vietnamese", "vi", "vie"},
	{"Chinese", "zh", "zho"}
};
		
#define ISO639_DB_SIZE (sizeof(iso639_db) / sizeof(ISO639_Entry))

typedef enum
{
	LANG_AFAR, LANG_ABKHAZIAN, LANG_AFRIKAANS, LANG_AMHARIC, LANG_ARABIC,
	LANG_ASSAMESE, LANG_AYMARA, LANG_AZERBAIJANI, LANG_BASHKIR, LANG_BELARUSIAN,
	LANG_BULGARIAN, LANG_BENGALI, LANG_TIBETAN, LANG_BRETON, LANG_BOSNIAN,
	LANG_CATALAN, LANG_CZECH, LANG_DANISH, LANG_GERMAN, LANG_GREEK,
	LANG_ENGLISH, LANG_SPANISH, LANG_ESTONIAN, LANG_BASQUE, LANG_PERSIAN,
	LANG_FINNISH, LANG_FRENCH, LANG_IRISH, LANG_HEBREW, LANG_HINDI,
	LANG_CROATIAN, LANG_HUNGARIAN, LANG_INDONESIAN, LANG_ICELANDIC, LANG_ITALIAN,
	LANG_JAPANESE, LANG_GEORGIAN, LANG_KAZAKH, LANG_KOREAN, LANG_LATIN,
	LANG_LITHUANIAN, LANG_LATVIAN, LANG_MACEDONIAN, LANG_MALAY, LANG_MALTESE,
	LANG_DUTCH, LANG_NORWEGIAN, LANG_POLISH, LANG_PORTUGUESE, LANG_ROMANIAN,
	LANG_RUSSIAN, LANG_SLOVAK, LANG_SLOVENIAN, LANG_ALBANIAN, LANG_SERBIAN,
	LANG_SWEDISH, LANG_THAI, LANG_TURKISH, LANG_UKRAINIAN, LANG_VIETNAMESE,
	LANG_CHINESE, LANG_UNKNOWN

} LanguageCode;

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
extern int IsValidLanguage(const char* langCode);
extern const char* ISO639_LangToAlpha2(LanguageCode lang);
extern LanguageCode ISO639_Alph2ToLang(const char* langCode);
extern LanguageCode ISO639_Alph3ToLang(const char* langCode);

#endif /* ISO639_H */
