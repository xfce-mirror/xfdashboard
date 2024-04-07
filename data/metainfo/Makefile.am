metainfodir = $(datadir)/metainfo
metainfo_DATA = $(metainfo_in_files:.xml.in=.xml)
metainfo_in_files = org.xfce.xfdashboard.metainfo.xml.in

org.xfce.xfdashboard.metainfo.xml: org.xfce.xfdashboard.metainfo.xml.in
	$(AM_V_GEN)$(MSGFMT) --xml --template $< -d $(top_srcdir)/po -o $@

EXTRA_DIST = \
        $(metainfo_DATA) \
        org.xfce.xfdashboard.metainfo.xml.in

DISTCLEANFILES = \
        $(metainfo_in_files:.xml.in=.xml)
