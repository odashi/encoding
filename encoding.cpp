// encoding.cpp
#include "encoding.h"
#include "jis2unicode.h"

unsigned int Encoding::decode(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size,
		enum ENCODING_TYPE encoding)
{
	// dispatching
	switch (encoding) {
	case UTF16: return decode_utf16(dest, dest_size, src, src_size);
	case UTF8: return decode_utf8(dest, dest_size, src, src_size);
	case SHIFTJIS: return decode_shiftjis(dest, dest_size, src, src_size);
	case EUCJP: return decode_eucjp(dest, dest_size, src, src_size);
	}

	// unknown encoding.
	return 0;
}

unsigned int Encoding::decode_utf16(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size)
{
	int high = 1, low = 0; // for endian
	unsigned char b1, b2;
	unsigned int len = 0;

	if (src_size < 2)
		return 0;

	// recognize BOM
	b1 = src[0], b2 = src[1];
	if (b1 == 0xff && b2 == 0xfe) {
		// little endian
		src += 2, src_size -= 2;
	} else if (b1 == 0xfe && b2 == 0xff) {
		// big endian
		src += 2, src_size -= 2;
		high = 0, low = 1;
	}

	if (!dest) {
		// counting
		for (unsigned int i = 0; i < src_size - 1; i += 2, len++) {
			b1 = src[i], b2 = src[i + 1];
			if (b1 == 0x00 && b2 == 0x00)
				break; // end of text
		}
		return len;
	}

	// decoding
	for (unsigned int i = 0; i < src_size - 1 && len < dest_size; i += 2, len++) {
		b1 = src[i + high], b2 = src[i + low];
		if (b1 == 0x00 && b2 == 0x00)
			break; // end of text
		dest[len] = ((int)b1 << 8) | (int)b2;
	}
	return len;
}

static unsigned int Encoding::decode_utf8(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size)
{
	unsigned char b1, b2, b3;
	unsigned int len = 0;

	// recognize BOM
	if (src_size > 3)
		if (src[0] == 0xef && src[1] == 0xbb && src[2] == 0xbf)
			src += 3, src_size -= 3;

	if (!dest) {
		// counting
		for (unsigned int i = 0; i < src_size; i++, len++) {
			b1 = src[i];
			// end of text
			if (b1 == 0x00)
				break;
			// 1 byte sequence(ASCII compatible)
			// 4~6 bytes sequence (error)
			// other errors
			if (b1 <= 0xc1 || 0xf0 <= b1)
				continue;
			// 2 bytes sequence
			if (b1 <= 0xdf) {
				if (++i >= src_size)
					break;
				b2 = src[i];
				// correct sequence
				if (0x80 <= b2 && b2 <= 0xbf)
					continue;
				// bad sequence
				i--;
			}
			// 3 bytes sequence
			else /* b1 <= 0xef */ {
				if ((i += 2) >= src_size)
					break;
				b2 = src[i - 1], b3 = src[i];
				// correct sequence
				if (0x80 <= b2 && b2 <= 0xbf && 0x80 <= b3 && b3 <= 0xbf)
					continue;
				// bad sequence
				i -= 2;
			}
		}
		return len;
	}

	// decoding
	for (unsigned int i = 0; i < src_size && len < dest_size; i++, len++) {
		b1 = src[i];
		// end of text
		if (b1 == 0x00)
			break;
		// 1 byte sequence(ASCII compatible)
		if (b1 <= 0x7f)
			dest[len] = (int)b1;
		// 4~6 bytes sequence (error)
		// other errors
		else if (b1 <= 0xc1 || 0xf0 <= b1)
			dest[len] = UNICODE_BAD_SEQUENCE;
		// 2 bytes sequence
		else if (b1 <= 0xdf) {
			if (++i >= src_size)
				break;
			b2 = src[i];
			// correct sequence
			if (0x80 <= b2 && b2 <= 0xbf)
				dest[len] =
						(((int)b1 & 0x1f) << 6) |
						((int)b2 & 0x3f);
			// bad sequence
			else
				i--;
		}
		// 3 bytes sequence
		else /* b1 <= 0xef */ {
			if ((i += 2) >= src_size)
				break;
			b2 = src[i - 1], b3 = src[i];
			// correct sequence
			if (0x80 <= b2 && b2 <= 0xbf && 0x80 <= b3 && b3 <= 0xbf)
				dest[len] =
						(((int)b1 & 0x1f) << 12) |
						(((int)b2 & 0x3f) << 6) |
						((int)b3 & 0x3f);
			// bad sequence
			else
				i -= 2;
		}
	}
	return len;
}

static unsigned int Encoding::decode_shiftjis(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size)
{
	const int KU_SIZE = 94;
	const int offset[] = {
		        -1,  0*KU_SIZE,  2*KU_SIZE,  4*KU_SIZE,  6*KU_SIZE,  8*KU_SIZE, 10*KU_SIZE, 12*KU_SIZE,
		14*KU_SIZE, 16*KU_SIZE, 18*KU_SIZE, 20*KU_SIZE, 22*KU_SIZE, 24*KU_SIZE, 26*KU_SIZE, 28*KU_SIZE,
		30*KU_SIZE, 32*KU_SIZE, 34*KU_SIZE, 36*KU_SIZE, 38*KU_SIZE, 40*KU_SIZE, 42*KU_SIZE, 44*KU_SIZE,
		46*KU_SIZE, 48*KU_SIZE, 50*KU_SIZE, 52*KU_SIZE, 54*KU_SIZE, 56*KU_SIZE, 58*KU_SIZE, 60*KU_SIZE,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		62*KU_SIZE, 64*KU_SIZE, 66*KU_SIZE, 68*KU_SIZE, 70*KU_SIZE, 72*KU_SIZE, 74*KU_SIZE, 76*KU_SIZE,
		78*KU_SIZE, 80*KU_SIZE, 82*KU_SIZE, 84*KU_SIZE, 86*KU_SIZE, 88*KU_SIZE, 90*KU_SIZE, 92*KU_SIZE,
		(94+ 0)*KU_SIZE, (94+ 2)*KU_SIZE, (94+ 4)*KU_SIZE, (94+12)*KU_SIZE, (94+14)*KU_SIZE, (94+78)*KU_SIZE, (94+80)*KU_SIZE, (94+82)*KU_SIZE,
		(94+84)*KU_SIZE, (94+86)*KU_SIZE, (94+88)*KU_SIZE, (94+90)*KU_SIZE, (94+92)*KU_SIZE, -1, -1, -1
	};

	unsigned char b1, b2;
	int codepoint;
	unsigned int len = 0;

	if (!dest) {
		// counting
		for (unsigned int i = 0; i < src_size; i++, len++) {
			b1  = src[i];
			// 2 bytes sequence
			if (jisx0201_2_unicode[b1] == UNICODE_BAD_SEQUENCE) {
				if (++i >= src_size)
					break;
				b2 = src[i];
				// correct sequence
				if (offset[b1 - 0x80] != -1 && b2 != 0x7f && 0x40 <= b2 && b2 <= 0xfc)
					continue;
				// bad sequence
				i--;
			}
		}
		return len;
	}

	// decoding
	for (unsigned int i = 0; i < src_size && len < dest_size; i++, len++) {
		b1  = src[i];
		// 1 byte sequence (ASCII & JIS X 0201)
		if (jisx0201_2_unicode[b1] != UNICODE_BAD_SEQUENCE)
			dest[len] = jisx0201_2_unicode[b1];
		// 2 bytes sequence (JIS X 0208)
		else {
			if (++i >= src_size)
				break;
			b2 = src[i];
			// correct sequence
			if (offset[b1 - 0x80] != -1 && b2 != 0x7f && 0x40 <= b2 && b2 <= 0xfc) {
				codepoint = offset[b1 - 0x80] + (b2 < 0x80 ? b2 : b2 - 1) - 0x40;
				// jis x 0213 shifting
				if (b1 >= 0xf0) {
					if (b1 == 0xf0 && b2 >= 0x80) codepoint += 6*KU_SIZE;
					else if (b1 == 0xf2 && b2 >= 0x80) codepoint += 6*KU_SIZE;
					else if (b1 == 0xf4 && b2 >= 0x80) codepoint += 62*KU_SIZE;
				}
				dest[len] = jisx0213_2_unicode[codepoint];
			}
			// bad sequence
			else
			{
				dest[len] = UNICODE_BAD_SEQUENCE;
				i--;
			}
		}
	}
	return len;
}

static unsigned int Encoding::decode_eucjp(
		int *dest,
		unsigned int dest_size,
		const unsigned char *src,
		unsigned int src_size)
{
	unsigned char b1, b2;
	unsigned int len = 0;

	if (!dest) {
		// counting
		for (unsigned int i = 0; i < src_size; i++, len++) {
			b1 = src[i];
			// 2 bytes sequence
			if (b1 >= 0x80) {
				if (++i >= src_size)
					break;
				b2 = src[i];
				// single shift
				// JIS X 0213
				if (b1 == 0x8e || (0xa1 <= b1 && b1 <= 0xf4 && 0xa1 <= b2 && b2 <= 0xfe))
					continue;
				// bad sequence
				i--;
			}
		}
		return len;
	}

	// decoding
	for (unsigned int i = 0; i < src_size && len < dest_size; i++, len++) {
		b1 = src[i];
		// 1 byte sequence
		if (b1 <= 0x7f)
			dest[len] = (int)b1;
		// 2 bytes sequence
		else /* b1 >= 0x80 */ {
			if (++i >= src_size)
				break;
			b2 = src[i];
			// single shift
			if (b1 == 0x8e)
				dest[len] = jisx0201_2_unicode[b2];
			// JIS X 0208
			else if (0xa1 <= b1 && b1 <= 0xf4 && 0xa1 <= b2 && b2 <= 0xfe)
				dest[len] = jisx0213_2_unicode[(b1 - 0xa1) * 94 + (b2 - 0xa1)];
			// bad sequence
			else {
				dest[len] = UNICODE_BAD_SEQUENCE;
				i--;
			}
		}
	}
	return len;
}

enum Encoding::EncodingType Encoding::getEncoding(
		const unsigned char *src,
		unsigned int src_size)
{
	/* Basic idea:
	 *   UTF-16
	 *     Find UTF-16 encoded ASCII.
	 *   UTF-8, Shift_JIS, EUC_JP
	 *     Calculate the "similarity value" and choose the largest one.
	 */
	int utf8 = 0, sjis = 0, eucjp = 0;
	unsigned char b1, b2, b3, b4;
	int utf16_high = 1, utf16_low = 0; // for UTF-16 endian

	// check UTF-16 BOM
	if (src_size >= 2) {
		b1 = src[0], b2 = src[1];
		if (b1 == 0xff && b2 == 0xfe) {
			// little endian
			src += 2, src_size -= 2;
		} else if (b1 == 0xfe && b2 == 0xff) {
			// big endian
			src += 2, src_size -= 2;
			utf16_high = 0, utf16_low = 1;
		}
	}

	// find UTF-16 encoded ASCII
	for (unsigned int i = 0; i < src_size - 1; i += 2) {
		b1 = src[i + utf16_high], b2 = src[i + utf16_low];
		if (b1 == 0x00 && b2 != 0x00 && b2 != 0x7f)
				return UTF16;
	}

	// calculate UTF-8 similarity
	for (unsigned int i = 0; i < src_size; i++) {
		b1 = src[i];
		// 1 byte sequence
		if (b1 <= 0x7f)
			utf8++;
		// 2 bytes sequence
		else if (0xc2 <= b1 && b1 <= 0xdf && i < src_size - 1) {
			b2 = src[i + 1];
			if (0x80 <= b2 && b2 <= 0xbf)
				utf8 += 2, i++;
		}
		// 3 bytes sequence
		else if (0xe0 <= b1 && b1 <= 0xef && i < src_size - 2) {
			b2 = src[i + 1], b3 = src[i + 2];
			if (0x80 <= b2 && b2 <= 0xbf && 0x80 <= b3 && b3 <= 0xbf)
				utf8 += 3, i += 2;
		}
		// 4 bytes sequence
		else if (0xf0 <= b1 && b1 <= 0xf7 && i < src_size - 3) {
			b2 = src[i + 1], b3 = src[i + 2], b4 = src[i + 3];
			if (0x80 <= b2 && b2 <= 0xbf && 0x80 <= b3 && b3 <= 0xbf && 0x80 <= b4 && b4 <= 0xbf)
				utf8 += 4, i += 3;
		}
	}

	// calculate Shift_JIS similarity
	for (unsigned int i = 0; i < src_size; i++) {
		b1 = src[i];
		// 1 byte sequence
		if (b1 <= 0x7f || (0xa1 <= b1 && b1 <= 0xdf))
			sjis++;
		// 2 bytes sequence
		else if (((0x81 <= b1 && b1 <= 0x9f) || (0xe0 <= b1 && b1 <= 0xfc)) && i < src_size - 1) {
			b2 = src[i + 1];
			if (b2 != 0x7f && 0x40 <= b2 && b2 <= 0xfc)
				sjis += 2, i++;
		}
	}

	// calculate EUC_JP similarity
	for (unsigned int i = 0; i < src_size; i++) {
		b1 = src[i];
		// 1 byte sequence
		if (b1 <= 0x7f)
			eucjp++;
		// 3 bytes sequence (JIS X 0213 plane 2)
		else if (b1 == 0x8f && i < src_size - 2) {
			b2 = src[i + 1], b3 = src[i + 2];
			if (0xa1 <= b2 && b2 <= 0xfe && 0xa1 <= b3 && b3 <= 0xfe)
				eucjp += 3, i += 2;
		}
		// 2 bytes sequence
		else if (i < src_size - 1) {
			b2 = src[i + 1];
			// JIS X 0201 kana/JIS X 0213 plane 1
			if (b1 == 0x8e || (0xa1 <= b1 && b1 <= 0xfe && 0xa1 <= b2 && b2 <= 0xfe))
				eucjp += 2, i++;
		}
	}

	if (utf8 >= sjis && utf8 >= eucjp)
		return UTF8;
	if (sjis >= eucjp)
		return SHIFTJIS;

	return EUCJP;
}
