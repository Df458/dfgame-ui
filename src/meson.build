uideps = [ core, graphics, math, resource, xml ]
uisrc  = [
    'frame.c',
    'frame_data.c',
    'frame_io.c',

    'layout.c',

    'menu.c',
]
uiinc  = []
uilib  = static_library('dfgame_ui', uisrc,
                         include_directories : uiinc,
                         dependencies : uideps,
                         link_args : args,
                         install : true)

pkgconfig.generate(libraries : uilib,
                    version : '0.1.0',
                    name : 'dfgame-ui',
                    filebase : 'dfgame-ui',
                    extra_cflags : [ '-I${prefix}/include/dfgame/ui' ],
                    requires : ['libxml-2.0', 'dfgame-core', 'dfgame-graphics', 'dfgame-math', 'dfgame-resource'],
                    libraries : ['-ldfgame_ui'],
                    description : 'dfgame ui module, provides uiet/tilemap support')

install_headers([
  'frame.h',
  'frame_data.h',
  'frame_io.h',

  'layout.h',
  'layout_element.h',

  'menu.h',
], subdir : 'dfgame/ui')

ui = declare_dependency(include_directories : include_directories('.'), link_with : uilib)

if tidy.found()
    run_target(
        'tidy-ui',
        command: [
            tidy,
            '-checks=*',
            '-p', meson.build_root()
        ] + uisrc)
endif
