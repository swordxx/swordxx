modsDATA = samples/mods.d/globals.conf

modsdatadir = $(pkgdatadir)/mods.d

installmodsDATA: $(modsDATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(modsdatadir)
	@list='$(modsDATA)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(modsdatadir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(modsdatadir)/$$f; \
	done

uninstallmodsDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(modsDATA)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(modsdatadir)/$$f"; \
	  rm -f $(DESTDIR)$(modsdatadir)/$$f; \
	done

