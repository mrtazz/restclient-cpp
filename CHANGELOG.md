# Changelog

## v0.2.0 (???)
-(HEAD, origin/master, origin/HEAD, master)  add empty m4 directory
- welp
- restructure autotools build
- add CONTRIBUTING.md
- Merge pull request #10 from eightnoteight/readme-fix
- readme fix
- call curl_global_cleanup() also when cleaning up curl
- use autogen.sh in travis runs
- add autogen.sh
- check for curl headers in configure script
- Merge remote-tracking branch 'origin/master'
- Compile library with -fPIC.
- Add missing header to Makefile.am
- cleanup .gitignore
- remove accidentally added vendor files from git
- add travis status into README
- initialize values
- free header data after use
- update to google-test 1.7.0
- learn to read Luke
- only add gtest linker dep for test target
- build gtest with autoreconf
- use vendorized gtest
- vendorize gtest, because reasons
- use random older version of autoconf as required
- install build-essentials on travis builders
- add gtest build as travis step
- adapt travis build to autotools
- make travis config officially c++
-(autotools)  add check target to Makefile.am
- minor changes to Makefile.am
- add basic autotools configuration
- build all branches
-(header-data)  small changes to the install target
- Merge branch 'master' of github.com:krujos/restclient-cpp into header-data
-(basic-auth)  Merge branch 'master' of github.com:eriksson/restclient-cpp into basic-auth
- Merge remote-tracking branch 'origin/develop'
- Be more efficent with memory
- Update README.md
- Add header's to response object
- Basic Authentication
- remove extra space
- Merge in master
- Cleanup warning for unused upload_object
- Add install target (${HOME}/lib) & build against libs & includes in ${HOME}
- Change makefile to work in my setup
- specify the libcurl package for travis
- add travis-ci config
- add cppcheck make target
- Merge branch 'release/v0.1.2' into develop

## v0.1.2 (22nd March 2011)
- return -1 for failed query instead of exit(1)
- add unit tests for failed query returncode == -1
- fix typos in README and add more sections
- include cstring for memcpy
- add cstdlib as include for proper dependencies

## v0.1.1 (27th October 2010)
- bump version to v0.1.1
- add target for static library
- add target for dynamic library
- ignore lib folder in gitignore
- include meta.h from local include folder
- update README

## v0.1.0 (15th October 2010)
- initial release
- support basic HTTP verbs (GET, POST, PUT, DELETE)
