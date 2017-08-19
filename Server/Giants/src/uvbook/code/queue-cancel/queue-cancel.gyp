{
  'targets': [
    {
      'target_name': 'queue-cancel',
      'type': 'executable',
      'include_dirs': [
        '../../libuv/include',
      ],
      'sources': [
        'main.cpp',
      ],
      'dependencies': [
        #'./deps/openssl/openssl.gyp:openssl',
        '../../libuv/uv.gyp:libuv',
      ],

      'defines': [
        'NODE_WANT_INTERNALS=1',
        'ARCH="<(target_arch)"',
        'PLATFORM="<(OS)"',
      ],
      'msvs_disabled_warnings': [ 4251, ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        [ 'OS=="win"', {
          'defines': [
            '_UNICODE=1',
          ],

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
