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
swicu_DATA = $(swicuSOURCES:.txt=.res)
else
swicu_DATA =
endif
EXTRA_DIST = $(swicuSOURCES)

CLEANFILES = *.res *.lst *.mk *.mak swicu.dat

SUFFIXES = .txt .res .dat

.txt.res:
	$(GENRB) -s . -d . $<


if ICU
swicudir = $(pkglibdir)/${VERSION}_icu_${ICU_VER}

#installswicu_DATA: $(swicu_DATA)
#	@$(NORMAL_INSTALL)
#	$(mkinstalldirs) $(DESTDIR)$(swicudir)
#	@list='$(swicu_DATA)'; for p in $$list; do \
#	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
#	  f="`echo $$p | sed -e 's|^.*/||'`"; \
#	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(swicudir)/$$f"; \
#	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(swicudir)/$$f; \
#	done

#uninstallswicu_DATA:
#	@$(NORMAL_UNINSTALL)
#	@list='$(swicu_DATA)'; for p in $$list; do \
#	  f="`echo $$p | sed -e 's|^.*/||'`"; \
#	  echo " rm -f $(DESTDIR)$(swicudir)/$$f"; \
#	  rm -f $(DESTDIR)$(swicudir)/$$f; \
#	done
else
swicudir = $(pkglibdir)

#installswicu_DATA:
#
#uninstallswicu_DATA:
endif
