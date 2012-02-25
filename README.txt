各種文字コードをUnicodeコードポイント(UTF-32)へ変換するためのライブラリです．

----------------------------------------
Encoding::getEncoding()
	文字コードを判定します．判定できるコードは以下の通り．
		UTF-16LE/BE (with/without BOM)
		UTF-8 (with/without BOM)
		Shift_JIS/CP932 (JIS X 2013:2004)
		EUC-JP (JIS X 2013:2004)
Encoding::decode()
	unsigned char配列データをUnicodeコードポイント(UTF-32: int)配列に変換します．

----------------------------------------
JIS X 0213 to Unicode変換テーブル
	x2013.orgの「JIS X 0213:2004 漢字8ビット符号とUnicodeの対応表」を元に
	機械変換を行って作成しています．
	http://x0213.org/codetable/jisx0213-2004-8bit-std.txt
	元のテーブルでUnicode側が2文字シーケンス(「か゜」など)になっているものは
	U+FFFD(判定不能文字)へ割り当てています．

----------------------------------------
・文字コード判定
	Jcode.pmの方法を参考にしています．
