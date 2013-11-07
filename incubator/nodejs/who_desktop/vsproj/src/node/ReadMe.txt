from http://nodejs.org/dist/v0.10.21/node-v0.10.21.tar.gz
 to vsproj/src/node

[의존성]
node.h
 └ uv.h, v8.h (from node-v0.10.21/deps/uv/include, node-v0.10.21/deps/v8/include)

uv.h
 └ uv-private/stdint-msvc2008.h, uv-private/uv-win.h

uv-win.h
 └ tree.h, ngx-queue.h

v8.h
 └ v8stdint.h


[불필요, 하지만 복사를 쉽게 하기 위해 둠(용량도 얼마 안되니)]
node-v0.10.21/deps/uv/include => nodejs/who_desktop/vsproj/src/node/src
 uv-bsd.h
 uv-darwin.h
 uv-linux.h
 uv-sunos.h

node-v0.10.21/deps/v8/include => nodejs/who_desktop/vsproj/src/node/src
 v8-debug.h
 v8-preparser.h
 v8-profiler.h
 v8-testing.h
