project('harmony', 'c',
  version : '0.0.1',
  default_options : ['warning_level=3', 'c_std=gnu23'])

conf_data = configuration_data()
conf_data.set('usecurl', true)
conf_data.set('usearia', false)

#doxygen = find_program('doxygen')

#run_target('./docs', command: [doxygen, 'Doxyfile'])
#lua = dependency('luajit')
#subdir('src/harmony/')
subdir('tests')
