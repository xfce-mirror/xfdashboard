libxfconf_settings_sources = [
  'xfconf-settings.c',
  'xfconf-settings.h',
]

libxfconf_settings_deps = [
  clutter,
  glib,
  libxfce4ui,
  xfconf,
]

libxfconf_settings = static_library(
  'xfconf-settings',
  libxfconf_settings_sources,
  include_directories: [
    include_directories('..'),
  ],
  dependencies: libxfconf_settings_deps,
  c_args: [
    '-DLIBXFDASHBOARD_COMPILATION=1',
    '-DG_LOG_DOMAIN="@0@"'.format('xfdashboard'),
  ],
  install: false,
)

libxfconf_settings_dep = declare_dependency(
  link_with: libxfconf_settings,
  dependencies: libxfconf_settings_deps,
)
