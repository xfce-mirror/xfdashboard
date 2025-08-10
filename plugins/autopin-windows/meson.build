autopin_windows_sources = [
  'autopin-windows.c',
  'autopin-windows.h',
  'plugin.c',
]

shared_module(
  'autopin-windows',
  autopin_windows_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('xfdashboard-plugin-autopin-windows'),
    '-DPLUGIN_ID="@0@"'.format('autopin-windows'),
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
