# Changelog

## v0.2.0 (19th April 2015)
- support basic auth
- Add header's to response object
- fix memory leaks related to curl cleanup
- add autotools build
- add CONTRIBUTING.md
- add vendorized gtest

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
