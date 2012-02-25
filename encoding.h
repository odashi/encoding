// encoding.h

#ifndef INCLUDED_ENCODING_H_
#define INCLUDED_ENCODING_H_

namespace Encoding {

	/** Encoding type
	 */
	enum EncodingType {
		/// Unknown encoding
		NONE = 0,
		/// UTF-16 LE/BE encoding
		UTF16,
		/// UTF-8 encoding
		UTF8,
		/// Shift JIS/CP932 encoding
		SHIFTJIS,
		/// EUC-JP encoding
		EUCJP
	};

	/** Unicode registered symbols
	 */
	enum UNICODE_SYMBOL {
		/// Unrecognized character (Geta character)
		UNICODE_BAD_SEQUENCE = 0xfffd
	};

	/** Transform UTF-16 BE/LE byte stream into UTF-16 characters.
	 *
	 * @param dest Destination pointer for UTF-16 encoded text or NULL.
	 * @param dest_size Maximum length of dest (without L'\0').
	 * @param src Source text encoded UTF-16 LE/BE.
	 * @param src_size Maximum length of src (without L'\0').
	 * 
	 * @retval The length of the text which is actually encoded.
	 * (with invalid code (0xfffd), without L'\0')
	 * If dest is NULL, this function only counts the necessary size of dest.
	 */
	unsigned int decode_utf16(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size
	);

	/** Transform UTF-8 byte stream into UTF-16 characters.
	 *
	 * @param dest Destination pointer for UTF-16 encoded text or NULL.
	 * @param dest_size Maximum length of dest (without L'\0').
	 * @param src Source text encoded UTF-8.
	 * @param src_size Maximum length of src (without L'\0').
	 * 
	 * @retval The length of the text which is actually encoded.
	 * (with invalid code (0xfffd), without L'\0')
	 * If dest is NULL, this function only counts the necessary size of dest.
	 */
	unsigned int decode_utf8(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size
	);

	/** Transform Shift JIS/CP932 byte stream into UTF-16 characters.
	 *
	 * @param dest Destination pointer for UTF-16 encoded text or NULL.
	 * @param dest_size Maximum length of dest (without L'\0').
	 * @param src Source text encoded Shift JIS.
	 * @param src_size Maximum length of src (without L'\0').
	 * 
	 * @retval The length of the text which is actually encoded.
	 * (with invalid code (0xfffd), without L'\0')
	 * If dest is NULL, this function only counts the necessary size of dest.
	 */
	unsigned int decode_shiftjis(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size
	);

	/** Transform EUC-JP byte stream into UTF-16 characters.
	 *
	 * @param dest Destination pointer for UTF-16 encoded text or NULL.
	 * @param dest_size Maximum length of dest (without L'\0').
	 * @param src Source text encoded EUC-JP.
	 * @param src_size Maximum length of src (without L'\0').
	 * 
	 * @retval The length of the text which is actually encoded.
	 * (with invalid code (0xfffd), without L'\0')
	 * If dest is NULL, this function only counts the necessary size of dest.
	 */
	unsigned int decode_eucjp(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size
	);

	/** Transform a specified encoding into UTF-16.
	 *
	 * @param dest Destination pointer for UTF-16 encoded text or NULL.
	 * @param dest_size Maximum length of dest (without L'\0').
	 * @param src Source text.
	 * @param src_size Maximum length of src (without L'\0').
	 * @param encoding Encoding of src.
	 * 
	 * @retval The length of the text which is actually encoded.
	 * (with invalid code (0xfffd), without L'\0')
	 * If dest is NULL, this function only counts the necessary size of dest.
	 */
	unsigned int decode(
			int *dest,
			unsigned int dest_size,
			const unsigned char *src,
			unsigned int src_size,
			enum ENCODING_TYPE encoding
	);

	/** Guess the encoding type of text data.
	 *
	 * @param src Source text.
	 * @param src_size Maximum length of src (without '\0').
	 * 
	 * @retval An EncodingType value which is guessed from src.
	 */
	 EncodingType getEncoding(
			const unsigned char *src,
			unsigned int src_size
	);

} // namespace Encoding

#endif // INCLUDED_ENCODING_H_
