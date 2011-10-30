/*
 * encoding.h
 *
 *  Created on: 2010/12/24
 *      Author: Odashi
 */

#ifndef ENCODING_H_
#define ENCODING_H_

#include <wchar.h>

/*
 * enum ENCODING_TYPE
 * Encoding type
 */
enum ENCODING_TYPE
{
	ENCODING_NONE = 0,
	ENCODING_UTF16,
	ENCODING_UTF8,
	ENCODING_SHIFTJIS,
	ENCODING_EUCJP
};

/*
 * enum UNICODE_SYMBOL
 * Unicode registered symbols
 */
enum UNICODE_SYMBOL
{
	UNICODE_BAD_SEQUENCE = 0xfffd
};

/*
 * void encoding_decode()
 * Transform a specified encoding into UTF-16.
 *
 * Return:
 *   The length of the text which is actually encoded.
 *   (with invalid code(0xfffd), without L'\0')
 *   If 'dest' is NULL, this function only counts the necessary size of 'dest.'
 */
size_t encoding_decode(
		wchar_t *dest,              // output UTF-16 encoded text or NULL
		size_t dest_size,           // maximum length of 'dest' (without L'\0')
		const unsigned char *src,   // input text which is encoded by such specified encoding
		size_t src_size,            // maximum length of 'src' (without '\0')
		enum ENCODING_TYPE encoding // encoding of 'src'
);

/*
 * enum ENCODING_TYPE encoding_getEncoding()
 * Guess the encoding type of text data.
 *
 * Return:
 *   An ENCODING_TYPE value which is guessed from 'src.'
 */
enum ENCODING_TYPE encoding_getEncoding(
		const unsigned char *src, // input text
		size_t src_size           // maximum length of 'src' (without '\0')
);

#endif /* ENCODING_H_ */
