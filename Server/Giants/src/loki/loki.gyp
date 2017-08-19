{
  'targets': [
    {
      'target_name': 'loki',
      'type': 'executable',
      'include_dirs': [
        '../../deps/uv/include',
        '../../deps/3rd/mysql-connector-c++-noinstall-1.1.3-win32/include',
        '../../deps/3rd/boost_1_47',
      ],
      'sources': [
        '../common/library.h',
        '../common/stl.h',
        '../common/config.h',
        'main.cpp',
        'DatabaseEngine.h',
        'DatabaseEngine.cpp',
        'Console.h',
        'Console.cpp',
        'NetCommandHandler.h',
        'NetCommandHandler.cpp',
        'User.h',
        'User.cpp',
        'SocketManager.h',
        'SocketManager.cpp',
        'ClassSessionManager.h',
        'ClassSessionManager.cpp',
#        'Responser.h',
#        'Responser.cpp',
        '../../../../SharedCode/library.h',
        '../../../../SharedCode/stl.h',
        '../../../../SharedCode/LokiCommand.h',
        '../../../../SharedCode/LokiCommand.cpp',
        '../../../../SharedCode/LokiBuffer.h',
        '../../../../SharedCode/LokiBuffer.cpp',
        '../../../../SharedCode/LokiReceiveBuffer.h',
        '../../../../SharedCode/LokiReceiveBuffer.cpp',
      ],
      'dependencies': [
        #'./deps/openssl/openssl.gyp:openssl',
        '../../deps/uv/uv.gyp:libuv',
        '../../deps/libzmq/libzmq.gyp:*',
        '../../../../SharedCode/Data/data.gyp:*',
      ],

      'defines': [
        'NODE_WANT_INTERNALS=1',
        'ARCH="<(target_arch)"',
        'PLATFORM="<(OS)"',
      ],
      'msvs_disabled_warnings': [ 4251, ],
      'conditions': [
        [ 'OS=="win"', {
          'defines': [
            '_UNICODE=1',
          ],
          'libraries': [ '-lmysqlcppconn.lib' ],
          "library_dirs":[
            '../../deps/3rd/mysql-connector-c++-noinstall-1.1.3-win32/lib',
          ]
        }, { # POSIX
          'defines': [ '__POSIX__' ],
        }],
        [ 'OS=="linux"', {
          'libraries': [
            '-lmysqlcppconn',
          ],
        }],
        [ 'OS=="mac"', {
          'libraries': [ '-framework Carbon' ],
          'defines!': [
            'PLATFORM="mac"',
          ],
          'defines': [
            # we need to use node's preferred "darwin" rather than gyp's preferred "mac"
            'PLATFORM="darwin"',
          ],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }],
        [ 'OS=="freebsd"', {
          'libraries': [
            '-lutil',
            '-lkvm',
          ],
        }],
        [ 'OS=="solaris"', {
          'libraries': [
            '-lkstat',
            '-lumem',
          ],
          'defines!': [
            'PLATFORM="solaris"',
          ],
          'defines': [
            # we need to use node's preferred "sunos"
            # rather than gyp's preferred "solaris"
            'PLATFORM="sunos"',
          ],
        }],
      ],
      'msvs_settings': {
        'VCLinkerTool': {
          'SubSystem': 1, # /subsystem:console
        },
      },
    },
  ] # end targets
}
