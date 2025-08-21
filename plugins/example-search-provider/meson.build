example_search_provider_sources = [
  'example-search-provider.c',
  'example-search-provider.h',
  'plugin.c',
]

shared_module(
  'example-search-provider',
  example_search_provider_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('xfdashboard-plugin-example-search-provider'),
    '-DPLUGIN_ID="@0@"'.format('example-search-provider'),
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
