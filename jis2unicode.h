// jis2unicode.h
#ifndef INCLUDED_JIS_2_UNICODE_H_
#define INCLUDED_JIS_2_UNICODE_H_

#include <wchar.h>

namespace Encoding {
	/** "JIS X 0201" to "UTF-16" translating table
	 * 
	 * Untranslatable character is translated into U+FFFD character.
	 * USAGE:
	 *   unicode = jisx0201_2_unicode[jis];
	 *     unicode: translated UTF-16 character
	 *     jis    : target JIS X 0201 character
	*/
	extern const wchar_t jisx0201_2_unicode[];

	/** "JIS X 0208" to "UTF-16" translating table
	 * 
	 * Untranslatable character is translated into U+FFFD character.
	 * USAGE:
	 * 	 unicode = jisx0208_2_unicode[(row-1)*94 + (col-1)];
	 * 	   unicode: translated UTF-16 character
	 *     row    : JIS X 0208 "ku"
	 *     col    : JIS X 0208 "ten"
	*/
	extern const wchar_t jisx0208_2_unicode[];
}

#endif // INCLUDED_JIS_2_UNICODE_H_
