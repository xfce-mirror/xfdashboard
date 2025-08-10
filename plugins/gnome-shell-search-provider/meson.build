gnome_shell_search_provider_sources = [
  'gnome-shell-search-provider.c',
  'gnome-shell-search-provider.h',
  'plugin.c',
]

gnome_shell_search_provider_path = get_option('gnome_shell_search_provider_path')
if gnome_shell_search_provider_path == ''
  gnome_shell_search_provider_path = get_option('prefix') / get_option('datadir') / 'gnome-shell' / 'search-providers'
endif

shared_module(
  'gnome-shell-search-provider',
  gnome_shell_search_provider_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('xfdashboard-plugin-gnome-shell-search-provider'),
    '-DPLUGIN_ID="@0@"'.format('gnome-shell-search-provider'),
    '-DGNOME_SHELL_PROVIDERS_PATH="@0@"'.format(gnome_shell_search_provider_path),
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
