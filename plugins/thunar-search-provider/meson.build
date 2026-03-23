search_provider_sources = [
  'thunar-search-providers.c',
  'thunar-search-providers.h',
  'plugin.c',
]

search_provider_path = '/usr/local/share/xfdashboard/thunar-search-providers'

shared_module(
  'thunar-search-provider',
  search_provider_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('xfdashboard-plugin-thunar-search-providers'),
    '-DPLUGIN_ID="@0@"'.format('search-provider'),
    '-DSHELL_SEARCH_PROVIDERS_PATH="@0@"'.format(search_provider_path),
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

install_data(
    'org.xfce.Thunar.search-provider.ini',
    install_dir: get_option('datadir') / 'xfdashboard' / 'thunar-search-providers'
)

install_data(
    'thunar-search-bridge.py',
    install_dir: get_option('libexecdir') / 'xfdashboard',
    install_mode: 'rwxr-xr-x'
)
