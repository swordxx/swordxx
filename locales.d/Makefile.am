localeDATA = locales.d/abbr.conf
localeDATA += locales.d/da.conf
localeDATA += locales.d/de.conf
localeDATA += locales.d/es.conf
localeDATA += locales.d/fr.conf
localeDATA += locales.d/it.conf
localeDATA += locales.d/no.conf
localeDATA += locales.d/pt.conf
localeDATA += locales.d/pt_BR.conf

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

