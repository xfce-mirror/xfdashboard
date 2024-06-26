dnl
dnl xfdashboard - Maybe a Gnome shell like dashboard for Xfce
dnl
dnl Copyright (c) 2012-2016 Stephan Haller <nomad@froevel.de>
dnl

dnl ***************************
dnl *** Version information ***
dnl ***************************
XDT_VERSION_INIT([1.1.0], [git])

dnl **************************************
dnl *** Minimum package version needed ***
dnl **************************************
m4_define([wnck_minimum_version], [3.0])
m4_define([clutter_minimum_version], [1.12.0])
m4_define([glib_minimum_version], [2.66])
m4_define([gio_minimum_version], [2.66])
m4_define([garcon_minimum_version], [0.2.0])
m4_define([xfconf_minimum_version], [4.14.0])
m4_define([xcomposite_minimum_version], [0.2])
m4_define([gtk_minimum_version], [3.2])
m4_define([libxfce4util_minimum_version],[4.10.0])
m4_define([libxfce4ui_minimum_version],[4.10.0])

dnl ***************************
dnl *** Initialize autoconf ***
dnl ***************************
AC_COPYRIGHT([Copyright (c) 2012-2022 Stephan Haller <nomad@froevel.de>])
AC_INIT([xfdashboard], [xdt_version], [https://gitlab.xfce.org/apps/xfdashboard], [xfdashboard])
AC_PREREQ([2.69])
AC_REVISION([xdt_version_build])

dnl ***************************
dnl *** Initialize automake ***
dnl ***************************
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.8 no-dist-gzip dist-bzip2 tar-ustar foreign])
AM_MAINTAINER_MODE()
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])

dnl *******************************
dnl *** Check for UNIX variants ***
dnl *******************************
AC_USE_SYSTEM_EXTENSIONS
AC_SEARCH_LIBS([strerror],[cposix])

dnl ********************************
dnl *** Check for basic programs ***
dnl ********************************
AC_PROG_CC()
AM_PROG_CC_C_O()
AC_PROG_LD()
AC_PROG_INSTALL()
AC_CHECK_PROGS([PERL], [perl5 perl])
m4_ifdef([AC_PROG_SED], [AC_PROG_SED], [AC_CHECK_PROG(SED, sed, sed)])
XDT_PROG_PKG_CONFIG()

dnl **************************
dnl *** Initialize libtool ***
dnl **************************
LT_PREREQ([2.2.6])
LT_INIT([disable-static])
LT_LIB_M

dnl **********************************
dnl *** Substitute package version ***
dnl **********************************
XFDASHBOARD_VERSION=xdt_version()
XFDASHBOARD_VERSION_MAJOR=xdt_version_major()
XFDASHBOARD_VERSION_MINOR=xdt_version_minor()
XFDASHBOARD_VERSION_MICRO=xdt_version_micro()
AC_SUBST([XFDASHBOARD_VERSION])
AC_SUBST([XFDASHBOARD_VERSION_MAJOR])
AC_SUBST([XFDASHBOARD_VERSION_MINOR])
AC_SUBST([XFDASHBOARD_VERSION_MICRO])

dnl **********************************
dnl *** Substitute library version ***
dnl **********************************
m4_define([libxfdashboard_binary_age], [m4_eval(10000 * xdt_version_major + 100 * xdt_version_minor + xdt_version_micro)])
m4_define([libxfdashboard_interface_age], [xdt_version_micro])

m4_define([libxfdashboard_lt_current], [m4_eval(libxfdashboard_binary_age - libxfdashboard_interface_age)])
m4_define([libxfdashboard_lt_revision], [libxfdashboard_interface_age])
m4_define([libxfdashboard_lt_age], [m4_eval(libxfdashboard_binary_age - libxfdashboard_interface_age)])
m4_define([libxfdashboard_verinfo], [libxfdashboard_lt_current():libxfdashboard_lt_revision():libxfdashboard_lt_age()])

LIBXFDASHBOARD_VERINFO=libxfdashboard_verinfo()
AC_SUBST([LIBXFDASHBOARD_VERINFO])

dnl **********************************
dnl *** Check for standard headers ***
dnl **********************************
AC_CHECK_HEADERS([stdlib.h unistd.h locale.h stdio.h errno.h time.h string.h \
                  math.h sys/types.h sys/wait.h memory.h signal.h sys/prctl.h \
                  libintl.h])
AC_CHECK_FUNCS([bind_textdomain_codeset])

dnl **********************
dnl *** Check for libm ***
dnl **********************
AC_CHECK_LIBM
AC_SUBST(LIBM)

dnl ******************************
dnl *** Check for i18n support ***
dnl ******************************
GETTEXT_PACKAGE="$PACKAGE"
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Name of default gettext domain])
AC_SUBST([GETTEXT_PACKAGE])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])

dnl *******************************
dnl *** Check for X11 installed ***
dnl *******************************
XDT_CHECK_LIBX11_REQUIRE()

dnl *******************************************
dnl *** Check for X11 extension: XComposite ***
dnl *******************************************
HAVE_XCOMPOSITE=""
AC_ARG_ENABLE([xcomposite],
	[AS_HELP_STRING([--disable-xcomposite],
		[disable use of X11 extension XComposite @<:@default=enabled@:>@])],
	[enabled_x11_extension_composite="$enableval"],
	[enabled_x11_extension_composite=yes]
)

AC_MSG_CHECKING([whether to build with X11 extension XComposite])
AM_CONDITIONAL([XFDASHBOARD_BUILD_WITH_XCOMPOSITE], [test x"$enabled_x11_extension_composite" = x"yes"])
AC_MSG_RESULT([$enabled_x11_extension_composite])

if test "x$enabled_x11_extension_composite" = xyes; then
	if $PKG_CONFIG --print-errors --exists xcomposite 2>&1; then
		PKG_CHECK_MODULES(XCOMPOSITE, xcomposite >= [xcomposite_minimum_version])
		AC_DEFINE([HAVE_XCOMPOSITE], [1], [Define if XComposite extension is available])
	fi
fi
AC_SUBST(HAVE_XCOMPOSITE)

dnl ****************************************
dnl *** Check for X11 extension: XDamage ***
dnl ****************************************
HAVE_XDAMAGE=""
AC_ARG_ENABLE([xdamage],
	[AS_HELP_STRING([--disable-xdamage],
		[disable use of X11 extension XDamage @<:@default=enabled@:>@])],
	[enabled_x11_extension_damage="$enableval"],
	[enabled_x11_extension_damage=yes]
)

AC_MSG_CHECKING([whether to build with X11 extension XDamage])
AM_CONDITIONAL([XFDASHBOARD_BUILD_WITH_XDAMAGE], [test x"$enabled_x11_extension_damage" = x"yes"])
AC_MSG_RESULT([$enabled_x11_extension_damage])

if test "x$enabled_x11_extension_damage" = xyes; then
	if $PKG_CONFIG --print-errors --exists xdamage 2>&1; then
		PKG_CHECK_MODULES(XDAMAGE, xdamage)
		AC_DEFINE([HAVE_XDAMAGE], [1], [Define if XDamage extension is available])
	fi
fi
AC_SUBST(HAVE_XDAMAGE)

dnl *****************************************
dnl *** Check for X11 extension: Xinerama ***
dnl *****************************************
HAVE_XINERAMA=""
AC_ARG_ENABLE([xinerama],
	[AS_HELP_STRING([--disable-xinerama],
		[disable use of X11 extension Xinerama @<:@default=enabled@:>@])],
	[enabled_x11_extension_xinerama="$enableval"],
	[enabled_x11_extension_xinerama=yes]
)

AC_MSG_CHECKING([whether to build with X11 extension Xinerama])
AM_CONDITIONAL([XFDASHBOARD_BUILD_WITH_XINERAMA], [test x"$enabled_x11_extension_xinerama" = x"yes"])
AC_MSG_RESULT([$enabled_x11_extension_xinerama])

if test "x$enabled_x11_extension_xinerama" = xyes; then
	if $PKG_CONFIG --print-errors --exists xinerama 2>&1; then
		PKG_CHECK_MODULES(XINERAMA, xinerama)
		AC_DEFINE([HAVE_XINERAMA], [1], [Define if Xinerama extension is available])
	fi
fi
AC_SUBST(HAVE_XINERAMA)

dnl ***********************************
dnl *** Check for required packages ***
dnl ***********************************
XFDASHBOARD_REQUIRES=""

XDT_CHECK_PACKAGE([LIBWNCK], [libwnck-3.0], [wnck_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES libwnck-3.0"])
XDT_CHECK_PACKAGE([GTK], [gtk+-3.0], [gtk_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES gtk+-3.0"])
XDT_CHECK_PACKAGE([GLIB], [glib-2.0], [glib_minimum_version], [
	XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES glib-2.0"

	XDT_PROG_PKG_CONFIG()

	GLIB_MKENUMS=`$PKG_CONFIG --variable=glib_mkenums glib-2.0`
	AC_SUBST(GLIB_MKENUMS)

	GLIB_GENMARSHAL=`$PKG_CONFIG --variable=glib_genmarshal glib-2.0`
	AC_SUBST(GLIB_GENMARSHAL)])
XDT_CHECK_PACKAGE([GIO], [gio-2.0], [gio_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES gio-2.0"])
XDT_CHECK_PACKAGE([GIO_UNIX], [gio-unix-2.0], [gio_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES gio-unix-2.0"])
XDT_CHECK_PACKAGE([GARCON], [garcon-1], [garcon_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES garcon-1"])
XDT_CHECK_PACKAGE([LIBXFCONF], [libxfconf-0], [xfconf_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES libxfconf-0"])
XDT_CHECK_PACKAGE([LIBXFCE4UTIL], [libxfce4util-1.0],[libxfce4util_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES libxfce4util-1.0"])
XDT_CHECK_PACKAGE([LIBXFCE4UI], [libxfce4ui-2],[libxfce4ui_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES libxfce4ui-2"])

dnl **********************************************
dnl *** Check for Clutter and its backend: GDK ***
dnl **********************************************
BACKENDS="x11"

XDT_CHECK_PACKAGE([CLUTTER], [clutter-1.0], [clutter_minimum_version], [
	XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES clutter-1.0"
	XDT_CHECK_PACKAGE([CLUTTER_COGL], [clutter-cogl-1.0], [clutter_minimum_version], [XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES clutter-cogl-1.0"])
	XDT_CHECK_PACKAGE([CLUTTER_GDK], [clutter-gdk-1.0], [clutter_minimum_version], [ac_clutter_gdk=yes], [ac_clutter_gdk=no])]
)

HAVE_BACKEND_GDK=""
AM_CONDITIONAL([XFDASHBOARD_BUILD_WITH_BACKEND_GDK], [test x"$ac_clutter_gdk" = x"yes"])
if test "x$ac_clutter_gdk" = xyes; then
	BACKENDS="$BACKENDS gdk"
	XFDASHBOARD_REQUIRES="$XFDASHBOARD_REQUIRES clutter-gdk-1.0"
	AC_DEFINE([HAVE_BACKEND_GDK], [1], [Define if GDK backend is available])
fi
AC_SUBST(HAVE_BACKEND_GDK)

dnl ***********************************
dnl *** Check for debugging support ***
dnl ***********************************
XDT_FEATURE_DEBUG([xdt_debug_default])

AS_CASE([$enable_debug],
	[werror],
	[XFDASHBOARD_DEBUG_CFLAGS="-DXFDASHBOARD_ENABLE_DEBUG"],

	[full],
	[XFDASHBOARD_DEBUG_CFLAGS="-DXFDASHBOARD_ENABLE_DEBUG"],

	[yes],
	[XFDASHBOARD_DEBUG_CFLAGS="-DXFDASHBOARD_ENABLE_DEBUG"],

	[minimum],
	[XFDASHBOARD_DEBUG_CFLAGS="-DG_DISABLE_CAST_CHECKS"],

	[no],
	[XFDASHBOARD_DEBUG_CFLAGS="-DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"],

	[AC_MSG_ERROR([Unknown argument for --enable-debug])]
)
AC_SUBST(XFDASHBOARD_DEBUG_CFLAGS)

dnl **************************************
dnl *** Check for linker optimizations ***
dnl **************************************
XDT_FEATURE_LINKER_OPTS()

dnl *****************************************************
dnl *** Prevent _FORTIFY_SOURCE redefinition warnings ***
dnl *****************************************************
PLATFORM_CPPFLAGS="-U_FORTIFY_SOURCE $PLATFORM_CPPFLAGS"

dnl *********************************
dnl *** Substitute platform flags ***
dnl *********************************
AC_MSG_CHECKING([PLATFORM_CPPFLAGS])
AC_MSG_RESULT([$PLATFORM_CPPFLAGS])
AC_SUBST([PLATFORM_CPPFLAGS])
AC_MSG_CHECKING([PLATFORM_CFLAGS])
AC_MSG_RESULT([$PLATFORM_CFLAGS])
AC_SUBST([PLATFORM_CFLAGS])
AC_MSG_CHECKING([PLATFORM_LDFLAGS])
AC_MSG_RESULT([$PLATFORM_LDFLAGS])
AC_SUBST([PLATFORM_LDFLAGS])

dnl **************************************
dnl *** Substitue package requirements ***
dnl **************************************
AC_SUBST(XFDASHBOARD_REQUIRES)

AC_CONFIG_FILES([
Makefile
common/Makefile
data/Makefile
data/metainfo/Makefile
data/data/Makefile
data/icons/Makefile
data/icons/16x16/Makefile
data/icons/24x24/Makefile
data/icons/32x32/Makefile
data/icons/48x48/Makefile
data/icons/64x64/Makefile
data/icons/96x96/Makefile
data/icons/128x128/Makefile
data/icons/scalable/Makefile
data/themes/Makefile
data/themes/xfdashboard/Makefile
data/themes/xfdashboard-auber/Makefile
data/themes/xfdashboard-blue/Makefile
data/themes/xfdashboard-dark/Makefile
data/themes/xfdashboard-mint/Makefile
data/themes/xfdashboard-moranga/Makefile
data/themes/xfdashboard-wine/Makefile
libxfdashboard/libxfdashboard.pc
libxfdashboard/Makefile
plugins/Makefile
plugins/autopin-windows/Makefile
plugins/clock-view/Makefile
plugins/example-search-provider/Makefile
plugins/gnome-shell-search-provider/Makefile
plugins/hot-corner/Makefile
plugins/middle-click-window-close/Makefile
plugins/recently-used-search-provider/Makefile
po/Makefile.in
settings/Makefile
xfdashboard/Makefile
])
AC_OUTPUT

dnl ***************************
dnl *** Print configuration ***
dnl ***************************
echo
echo "Build configuration:"
echo "  Package:          $PACKAGE"
echo "  Version:          $XFDASHBOARD_VERSION"
echo "  Library version:  $LIBXFDASHBOARD_VERINFO"
echo
echo "Compiler options:"
echo "  Compiler flags:   $CFLAGS $MAINTAINER_CFLAGS"
echo "  Debug level:      $enable_debug"
echo "  Debug flags:      $XFDASHBOARD_DEBUG_CFLAGS"
echo
echo "Library versions used:"
echo "  libwnck:          $LIBWNCK_VERSION"
echo "  clutter:          $CLUTTER_VERSION"
echo "  cogl:             $CLUTTER_COGL_VERSION"
echo "  gtk+:             $GTK_VERSION"
echo "  glib:             $GLIB_VERSION"
echo "  gio:              $GIO_VERSION"
echo "  gio-unix:         $GIO_UNIX_VERSION"
echo "  garcon:           $GARCON_VERSION"
echo "  libxfce4ui:       $LIBXFCE4UI_VERSION"
echo "  libxfce4util:     $LIBXFCE4UTIL_VERSION"
echo "  libxconf:         $LIBXFCONF_VERSION"
echo
echo "Tools used:"
echo "  glib-genmarshal:  $GLIB_GENMARSHAL"
echo "  glib-mkenums:     $GLIB_MKENUMS"
echo
echo "Features enabled:"
echo "  Backends:         $BACKENDS"
echo "  XComposite:       $enabled_x11_extension_composite"
echo "  XDamage:          $enabled_x11_extension_damage"
echo "  Xinerama:         $enabled_x11_extension_xinerama"
echo
