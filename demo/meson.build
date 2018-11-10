demodeps = [ application, core, graphics, math, resource, ui, xml ]
demosrc  = [ 'main.c' ]
demoexe  = executable('demo_ui', demosrc,
                       dependencies : demodeps,
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
run_command('ln', '-rs', 'assets', '../build/demo')
