localeDATA = locales.d/abbr.conf
localeDATA += locales.d/cs.conf
localeDATA += locales.d/da.conf
localeDATA += locales.d/de.conf
localeDATA += locales.d/de_abbrev.conf
localeDATA += locales.d/en_GB.conf
localeDATA += locales.d/es.conf
localeDATA += locales.d/fi.conf
localeDATA += locales.d/fr.conf
localeDATA += locales.d/it.conf
localeDATA += locales.d/la.conf
localeDATA += locales.d/nl.conf
localeDATA += locales.d/no.conf
localeDATA += locales.d/pl.conf
localeDATA += locales.d/pt.conf
localeDATA += locales.d/id.conf
localeDATA += locales.d/pt_BR.conf
localeDATA += locales.d/sk.conf
localeDATA += locales.d/et.conf
localeDATA += locales.d/et_abbr.conf
localeDATA += locales.d/ru_RU-cp1251.conf
localeDATA += locales.d/ru_RU-koi8-r.conf
localeDATA += locales.d/uk_UA-koi8-u.conf
localeDATA += locales.d/uk_UA-cp1251.conf
localeDATA += locales.d/ro.conf

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

