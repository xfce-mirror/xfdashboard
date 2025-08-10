middle_click_window_close_sources = [
  'middle-click-window-close.c',
  'middle-click-window-close.h',
  'plugin.c',
]

shared_module(
  'middle-click-window-close',
  middle_click_window_close_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('xfdashboard-plugin-middle-click-window-close'),
    '-DPLUGIN_ID="@0@"'.format('middle-click-window-close'),
  ],
  include_directories: [
    include_directories('..' / '..'),
  ],
  dependencies: [
    glib,
    gtk,
    libxfce4util,
    libxfdashboard_dep,
  ],
  install: true,
  install_dir: get_option('prefix') / get_option('libdir') / 'xfdashboard' / 'plugins',
)
