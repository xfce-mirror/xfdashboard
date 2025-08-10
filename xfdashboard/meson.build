xfdashboard_sources = [
  'application.c',
  'application.h',
  'main.c',
  xfce_revision_h,
]

executable(
  'xfdashboard',
  xfdashboard_sources,
  include_directories: [
    include_directories('..'),
  ],
  c_args: [
    '-DLIBXFDASHBOARD_COMPILATION=1',
    '-DG_LOG_DOMAIN="@0@"'.format('xfdashboard'),
  ],
  dependencies: [
    glib,
    gtk,
    libxfce4ui,
    libxfce4util,
    libxfconf_settings_dep,
    libxfdashboard_dep,
    xfconf,
  ],
  install: true,
  install_dir: get_option('prefix') / get_option('bindir'),
)
