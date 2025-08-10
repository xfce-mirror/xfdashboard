subdir('data')
subdir('icons')
subdir('metainfo')
subdir('themes')

autostart_desktop_in = configure_file(
  input: 'org.xfce.xfdashboard-autostart.desktop.in.in',
  output: 'org.xfce.xfdashboard-autostart.desktop.in',
  configuration: {
    'bindir': get_option('prefix') / get_option('bindir'),
  },
  install: false,
)

i18n.merge_file(
  input: autostart_desktop_in,
  output: 'org.xfce.xfdashboard-autostart.desktop',
  type: 'desktop',
  po_dir: '..' / 'po',
  install: true,
  install_dir: get_option('prefix') / get_option('sysconfdir') / 'xdg' / 'autostart',
)

desktop_files = [
  'org.xfce.xfdashboard-settings.desktop',
  'org.xfce.xfdashboard.desktop',
]

foreach desktop_file : desktop_files
  desktop_in = configure_file(
    input: '@0@.in.in'.format(desktop_file),
    output: '@0@.in'.format(desktop_file),
    configuration: {
      'bindir': get_option('prefix') / get_option('bindir'),
    },
    install: false,
  )

  i18n.merge_file(
    input: desktop_in,
    output: desktop_file,
    type: 'desktop',
    po_dir: '..' / 'po',
    install: true,
    install_dir: get_option('prefix') / get_option('datadir') / 'applications',
  )
endforeach
