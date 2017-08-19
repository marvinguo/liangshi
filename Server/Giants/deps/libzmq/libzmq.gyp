{
  'targets': [
    {
      'target_name': 'zmq',
      'type': '<(library)',
      'include_dirs': [
        './include',
      ],
      'sources': [
        'src/address.cpp',
        'src/clock.cpp',
        'src/ctx.cpp',
        'src/curve_client.cpp',
        'src/curve_server.cpp',
        'src/dealer.cpp',
        'src/devpoll.cpp',
        'src/dist.cpp',
        'src/epoll.cpp',
        'src/err.cpp',
        'src/fq.cpp',
        'src/io_object.cpp',
        'src/io_thread.cpp',
        'src/ip.cpp',
        'src/ipc_address.cpp',
        'src/ipc_connecter.cpp',
        'src/ipc_listener.cpp',
        'src/kqueue.cpp',
        'src/lb.cpp',
        'src/mailbox.cpp',
        'src/mechanism.cpp',
        'src/metadata.cpp',
        'src/msg.cpp',
        'src/mtrie.cpp',
        'src/object.cpp',
        'src/options.cpp',
        'src/own.cpp',
        'src/null_mechanism.cpp',
        'src/pair.cpp',
        'src/pgm_receiver.cpp',
        'src/pgm_sender.cpp',
        'src/pgm_socket.cpp',
        'src/pipe.cpp',
        'src/plain_client.cpp',
        'src/plain_server.cpp',
        'src/poll.cpp',
        'src/poller_base.cpp',
        'src/precompiled.cpp',
        'src/proxy.cpp',
        'src/pub.cpp',
        'src/pull.cpp',
        'src/push.cpp',
        'src/random.cpp',
        'src/raw_encoder.cpp',
        'src/raw_decoder.cpp',
        'src/reaper.cpp',
        'src/rep.cpp',
        'src/req.cpp',
        'src/router.cpp',
        'src/select.cpp',
        'src/session_base.cpp',
        'src/socks.cpp',
        'src/socks_connecter.cpp',
        'src/signaler.cpp',
        'src/socket_base.cpp',
        'src/stream.cpp',
        'src/stream_engine.cpp',
        'src/sub.cpp',
        'src/tcp.cpp',
        'src/tcp_address.cpp',
        'src/tcp_connecter.cpp',
        'src/tcp_listener.cpp',
        'src/thread.cpp',
        'src/trie.cpp',
        'src/v1_decoder.cpp',
        'src/v1_encoder.cpp',
        'src/v2_decoder.cpp',
        'src/v2_encoder.cpp',
        'src/xpub.cpp',
        'src/xsub.cpp',
        'src/zmq.cpp',
        'src/zmq_utils.cpp',
      ],
      'dependencies': [
      ],

      'defines': [
        'ZMQ_STATIC',
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
          'libraries': [ '-lmysqlcppconn.lib' ],
          "library_dirs":[
            '../../deps/3rd/mysql-connector-c++-noinstall-1.1.3-win32/lib',
          ]
        }, { # POSIX
          'defines': [ '__POSIX__' ],
        }],
        [ 'OS=="linux"', {
          'defines': [
            'ZMQ_HAVE_UIO',
          ],
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
