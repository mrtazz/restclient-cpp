# Contribute
- Fork the project.
- Make your feature addition or bug fix.
- Add tests for it. This is important so I don't break it in a future version
  unintentionally.
- Commit, do not mess with version
- If you add functionality, document it in the README
- Send me a pull request. Bonus points for topic branches.

## How to run tests

1. build vendorized gtest: `./utils/build_gtest.sh`
2. build restclient-cpp: `./autogen.sh && ./configure && make check`
3. run the unit test suite: `make test`
