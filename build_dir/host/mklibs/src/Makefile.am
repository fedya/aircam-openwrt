SUBDIRS = mklibs-readelf

noinst_SCRIPTS = \
	mklibs.py \
	mklibs-copy.py
man_MANS = \
	mklibs.1 \
	mklibs-copy.1

install-exec-local:
	test -z "$(bindir)" || $(mkdir_p) "$(DESTDIR)$(bindir)"
	@list='$(noinst_SCRIPTS)'; for p in $$list; do \
	if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  if test -f $$d$$p; then \
	    f=`echo "$$p" | sed 's|\.py$$||'`; \
	    echo " $(INSTALL_SCRIPT) '$$d$$p' '$(DESTDIR)$(bindir)/$$f'"; \
	    $(INSTALL_SCRIPT) "$$d$$p" "$(DESTDIR)$(bindir)/$$f"; \
	  else :; fi; \
	done

