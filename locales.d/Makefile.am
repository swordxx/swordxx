swlocaledir = $(top_srcdir)/locales.d

localeDATA = $(swlocaledir)/abbr.conf
localeDATA += $(swlocaledir)/bg_BG-cp1251.conf
localeDATA += $(swlocaledir)/cs.conf
localeDATA += $(swlocaledir)/da.conf
localeDATA += $(swlocaledir)/de.conf
localeDATA += $(swlocaledir)/de_abbrev.conf
localeDATA += $(swlocaledir)/en_GB.conf
localeDATA += $(swlocaledir)/es.conf
localeDATA += $(swlocaledir)/fi.conf
localeDATA += $(swlocaledir)/fr.conf
localeDATA += $(swlocaledir)/it.conf
localeDATA += $(swlocaledir)/la.conf
localeDATA += $(swlocaledir)/nl.conf
localeDATA += $(swlocaledir)/no.conf
localeDATA += $(swlocaledir)/pl.conf
localeDATA += $(swlocaledir)/pt.conf
localeDATA += $(swlocaledir)/id.conf
localeDATA += $(swlocaledir)/pt_BR.conf
localeDATA += $(swlocaledir)/sk.conf
localeDATA += $(swlocaledir)/et.conf
localeDATA += $(swlocaledir)/et_abbr.conf
localeDATA += $(swlocaledir)/ru_RU-cp1251.conf
localeDATA += $(swlocaledir)/ru_RU-koi8-r.conf
localeDATA += $(swlocaledir)/uk_UA-koi8-u.conf
localeDATA += $(swlocaledir)/uk_UA-cp1251.conf
localeDATA += $(swlocaledir)/ro.conf

EXTRA_DIST += $(localeDATA)

localedatadir = $(pkgdatadir)/locales.d

installlocaleDATA: $(localeDATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(localedatadir)
	@list='$(localeDATA)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(localedatadir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(localedatadir)/$$f; \
	done

uninstalllocaleDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(localeDATA)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(localedatadir)/$$f"; \
	  rm -f $(DESTDIR)$(localedatadir)/$$f; \
	done

