# magnum-web-application-template

## Basic Install Instructions


#### CMake IDE Options (Arch)
```
-DCMAKE_TOOLCHAIN_FILE="../toolchains/generic/Emscripten-wasm.cmake"
-DCMAKE_BUILD_TYPE=Debug
-DCMAKE_PREFIX_PATH=/usr/lib/emscripten/system
-DCMAKE_INSTALL_PREFIX=/usr/lib/emscripten/system
-DMAGNUM_DEPLOY_PREFIX=/srv/http/emscripten-webgl2/
```

At this stage, You need to execute 'install' step manually or by IDE.

#### Install NPM Modules
```
npm install
```

#### Run web development environment
```
npm start
```

Surf to [localhost:9000](http://localhost:9000)

Enjoy!

## TODO

* Requests are welcomed via github issues.
 I'm working on a personal project based on this template,
 I have few more modules and components people might find useful.
