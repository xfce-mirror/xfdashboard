themedir = $(datadir)/themes/xfdashboard-wine/xfdashboard-1.0

theme_in_files = \
	xfdashboard.theme.desktop.in

theme_DATA = \
	$(theme_in_files:.theme.desktop.in=.theme) \
	xfdashboard-wine.css \
	xfdashboard.css \
	xfdashboard.xml \
	xfdashboard-secondary.xml \
	animations.xml \
	effects.xml \
	screenshot-xfdashboard-wine.png

# Substitute bindir so it works when built in a non-system prefix
xfdashboard.theme.desktop.in:xfdashboard.theme.desktop.in.in
	$(AM_V_GEN) sed -e "s|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|" \
	    $< > $@ || rm $@

xfdashboard.theme: xfdashboard.theme.desktop.in
	$(AM_V_GEN)$(MSGFMT) --desktop --template $< -d $(top_srcdir)/po -o $@

CLEANFILES = \
	$(theme_in_files) \
	$(theme_in_files:.theme.desktop.in=.theme)

EXTRA_DIST = \
	$(theme_in_files:.theme.desktop.in=.theme.desktop.in.in) \
	$(theme_DATA)

DISTCLEANFILES = \
	$(theme_in_files) \
	$(theme_in_files:.theme.desktop.in=.theme)
