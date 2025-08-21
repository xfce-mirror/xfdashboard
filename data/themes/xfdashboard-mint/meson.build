theme_files = [
  'animations.xml',
  'effects.xml',
  'screenshot-xfdashboard-mint.png',
  'xfdashboard-mint.css',
  'xfdashboard-secondary.xml',
  'xfdashboard.css',
  'xfdashboard.xml',
]

themedir = get_option('prefix') / get_option('datadir') / 'themes' / 'xfdashboard-mint' / 'xfdashboard-1.0'

foreach theme_file : theme_files
  install_data(
    theme_file,
    install_dir: themedir,
  )
endforeach

theme_in = configure_file(
  input: 'xfdashboard.theme.desktop.in.in',
  output: 'xfdashboard.theme.desktop.in',
  configuration: {
    'PACKAGE_VERSION': meson.project_version(),
  },
  install: false,
)

i18n.merge_file(
  input: theme_in,
  output: 'xfdashboard.theme',
  type: 'desktop',
  po_dir: '..' / '..' / '..' / 'po',
  install: true,
  install_dir: themedir,
)
