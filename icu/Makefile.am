AUTOMAKE_OPTIONS = 1.6

if ICU
#pkglib_DATA = swicu.dat
#else
#endif

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
endif

CLEANFILES = *.res *.lst *.mk *.mak swicu.dat

pkglib_DATA = $(swicuSOURCES:.txt=.res)

SUFFIXES = .txt .res .dat

#swicu.dat: $(swicuDATA) trans.lst icupkg.mk
#	$(PKGDATA) -O icupkg.mk -p swicu -m common -T . -s . \
#		-d . -r $(VERSION) trans.lst

#trans.lst:
#	ls -1 *.res > trans.lst

#icupkg.mk:
#	@echo "GENCCODE=@GENCCODE@" > icupkg.mk
#	@echo "GENCMN=@GENCMN@" >> icupkg.mk

.txt.res:
	$(GENRB) -s . -d . $<
