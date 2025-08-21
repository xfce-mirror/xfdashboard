project(
  'xfdashboard',
  'c',
version : '1.1.0',
  license : 'GPL-2.0-or-later',
  meson_version : '>= 0.54.0',
  default_options : ['c_std=gnu11', 'buildtype=debugoptimized', 'warning_level=2']
)

project_namespace = 'apps'
pkgdatadir = get_option('prefix') / get_option('datadir') / meson.project_name()
copyright_year = '2025'

cc = meson.get_compiler('c')
pkgconfig = import('pkgconfig')
gnome = import('gnome')
i18n = import('i18n')

dependency_versions = {
  'libwnck': '>= 3.0',
  'glib': '>= 2.66',
  'gtk': '>= 3.22.0',
  'xfce4': '>= 4.16.0',
  'cogl': '>= 1.18.0',
  'clutter': '>= 1.24.0',
  'x11': '>= 1.6.7',
  'xcomposite': '>= 0.2',
  'xdamage': '>= 1.1.5',
  'xinerama': '>= 1.1.4',
}

libwnck = dependency('libwnck-3.0', version: dependency_versions['libwnck'])
glib = dependency('glib-2.0', version: dependency_versions['glib'])
gio = dependency('gio-2.0', version: dependency_versions['glib'])
gio_unix = dependency('gio-unix-2.0', version: dependency_versions['glib'])
gtk = dependency('gtk+-3.0', version: dependency_versions['gtk'])
garcon = dependency('garcon-1', version: dependency_versions['xfce4'])
xfconf = dependency('libxfconf-0', version: dependency_versions['xfce4'])
libxfce4ui = dependency('libxfce4ui-2', version: dependency_versions['xfce4'])
libxfce4util = dependency('libxfce4util-1.0', version: dependency_versions['xfce4'])
cogl = dependency('cogl-1.0', version: dependency_versions['cogl'])
clutter = dependency('clutter-1.0', version: dependency_versions['clutter'])
clutter_cogl = dependency('clutter-cogl-1.0', version: dependency_versions['clutter'])
clutter_gdk = dependency('clutter-gdk-1.0', version: dependency_versions['clutter'], required: get_option('clutter-gdk'))
x11 = dependency('x11', version: dependency_versions['x11'])
xcomposite = dependency('xcomposite', version: dependency_versions['xcomposite'], required: get_option('xcomposite'))
xdamage = dependency('xdamage', version: dependency_versions['xdamage'], required: get_option('xdamage'))
xinerama = dependency('xinerama', version: dependency_versions['xinerama'], required: get_option('xinerama'))

feature_cflags = []

if clutter_gdk.found()
  feature_cflags += '-DHAVE_BACKEND_GDK=1'
endif
if xcomposite.found()
  feature_cflags += '-DHAVE_XCOMPOSITE=1'
endif
if xdamage.found()
  feature_cflags += '-DHAVE_XDAMAGE=1'
endif
if xinerama.found()
  feature_cflags += '-DHAVE_XINERAMA=1'
endif

extra_cflags = []
extra_cflags_check = [
  '-Wmissing-declarations',
  '-Wmissing-noreturn',
  '-Wold-style-definition',
  '-Wredundant-decls',
  '-Wpointer-arith',
  '-Wcast-align',
  '-Winit-self',
  '-Wshadow',
  '-Wmissing-include-dirs',
  '-Wundef',
  '-Wformat',
  '-Wformat-security',
  '-Wformat-y2k',
  '-Wnested-externs',
  '-Wno-unused-parameter',
  '-Wno-declaration-after-statement',
  '-Wno-missing-field-initializers',
  '-Werror=implicit-function-declaration',
  '-Wno-error=deprecated-declarations',
]

optimization = get_option('optimization')
if get_option('debug') and optimization in ['0', 'g']
  extra_cflags_check += '-fstack-protector-strong'
  extra_cflags += [
    '-DDEBUG=1',
    '-DDEBUG_TRACE=1',
    '-DG_ENABLE_DEBUG',
    '-DXFDASHBOARD_ENABLE_DEBUG'
  ]
elif optimization in ['3', 'minsize']
  extra_cflags += [
    '-DNDEBUG',
    '-DG_DISABLE_CAST_CHECKS',
    '-DG_DISABLE_ASSERT',
  ]
endif

if dependency_versions.has_key('glib')
  glib_version_parts = dependency_versions['glib'].split(' ')
  glib_min_version_parts = glib_version_parts[1].split('.')
  glib_min_version_define = 'GLIB_VERSION_@0@_@1@'.format(glib_min_version_parts[0], glib_min_version_parts[1])
  extra_cflags += [
    '-DGLIB_VERSION_MIN_REQUIRED=@0@'.format(glib_min_version_define),
    '-DGLIB_VERSION_MAX_ALLOWED=@0@'.format(glib_min_version_define),
    '-DG_LOG_USE_STRUCTURED=1',
  ]
endif

version_parts = meson.project_version().split('-dev')[0].split('.')
version_short = '@0@.@1@'.format(version_parts[0], version_parts[1])

extra_cflags += [
  '-DPACKAGE="@0@"'.format(meson.project_name()),
  '-DPACKAGE_NAME="@0@"'.format(meson.project_name()),
  '-DPACKAGE_VERSION="@0@"'.format(meson.project_version()),
  '-DVERSION="@0@"'.format(meson.project_version()),
  '-DVERSION_SHORT="@0@"'.format(version_short),
  '-DTVM_VERSION_HELP="@0@"'.format(version_short),
  '-DPACKAGE_STRING="@0@ @1@"'.format(meson.project_name(), meson.project_version()),
  '-DCOPYRIGHT_YEAR="@0@"'.format(copyright_year),
  '-DPACKAGE_LOCALE_DIR="@0@"'.format(get_option('prefix') / get_option('localedir')),
  '-DPACKAGE_BUGREPORT="https://gitlab.xfce.org/@0@/@1@/-/issues"'.format(project_namespace, meson.project_name()),
  '-DGETTEXT_PACKAGE="@0@"'.format(meson.project_name()),
  '-DPREFIX="@0@"'.format(get_option('prefix')),
  '-DBINDIR="@0@"'.format(get_option('prefix') / get_option('bindir')),
  '-DDATADIR="@0@"'.format(get_option('prefix') / get_option('datadir')),
  '-DINCLUDEDIR="@0@"'.format(get_option('prefix') / get_option('includedir')),
  '-DLIBDIR="@0@"'.format(get_option('prefix') / get_option('libdir')),
  '-DLIBEXECDIR="@0@"'.format(get_option('prefix') / get_option('libexecdir')),
  '-DLOCALEDIR="@0@"'.format(get_option('prefix') / get_option('localedir')),
  '-DLOCALSTATEDIR="@0@"'.format(get_option('prefix') / get_option('localstatedir')),
  '-DSBINDIR="@0@"'.format(get_option('prefix') / get_option('sbindir')),
  '-DSYSCONFDIR="@0@"'.format(get_option('prefix') / get_option('sysconfdir')),
  '-DHAVE_XFCE_REVISION_H=1',

  # Note that unlike other Xfce projects, PACKAGE_DATADIR here is not pkgdatadir!
  '-DPACKAGE_DATADIR="@0@"'.format(get_option('prefix') / get_option('datadir')),
  '-DPACKAGE_LIBDIR="@0@"'.format(get_option('prefix') / get_option('libdir')),
]

add_project_arguments(cc.get_supported_arguments(extra_cflags_check), language: 'c')
add_project_arguments(extra_cflags, language: 'c')
add_project_arguments(feature_cflags, language: 'c')

xfce_revision_h = vcs_tag(
  command: ['git', 'rev-parse', '--short', 'HEAD'],
  fallback: 'UNKNOWN',
  input: 'xfce-revision.h.in',
  output: 'xfce-revision.h',
  replace_string: '@REVISION@',
)

subdir('data')
subdir('common')
subdir('libxfdashboard')
subdir('settings')
subdir('xfdashboard')
subdir('plugins')
subdir('po')
