AUTOMAKE_OPTIONS = 1.6

swicuSOURCES = translit_BETA_Greek.txt
swicuSOURCES += translit_BGreek_Greek.txt
swicuSOURCES += translit_CCAT_Hebrew.txt
swicuSOURCES += translit_CCAT_Syriac.txt
swicuSOURCES += translit_Latin_Armenian.txt
swicuSOURCES += translit_Latin_Coptic.txt
swicuSOURCES += translit_Latin_Ethiopic.txt
swicuSOURCES += translit_Latin_Georgian.txt
swicuSOURCES += translit_Latin_Gothic.txt
swicuSOURCES += translit_Latin_Syriac.txt
swicuSOURCES += translit_Latin_Thai.txt
swicuSOURCES += translit_Latin_Ugaritic.txt
swicuSOURCES += translit_Any_Latin1.txt
swicuSOURCES += translit_Any_Latex.txt
#swicuSOURCES += translit_HTMLEnt_Unicode.txt
#swicuSOURCES += translit_Han_Pinyin.txt
#swicuSOURCES += translit_Kanji_English.txt
#swicuSOURCES += translit_Kanji_OnRomaji.txt
swicuSOURCES += translit_Latin_Arabic.txt
swicuSOURCES += translit_Latin_Hebrew.txt
swicuSOURCES += translit_swordindex.txt


if ICU
pkglib_DATA = $(swicuSOURCES:.txt=.res)
endif
EXTRA_DIST = $(swicuSOURCES)

CLEANFILES = *.res *.lst *.mk *.mak swicu.dat

SUFFIXES = .txt .res .dat

.txt.res:
	$(GENRB) -s . -d . $<
