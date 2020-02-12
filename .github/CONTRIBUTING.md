# Contribute
- Fork the project.
- Make your feature addition or bug fix.
- Add tests for it. This is important so I don't break it in a future version
  unintentionally.
- Commit, do not mess with version
- If you add functionality, document it in the README
- Send me a pull request. Bonus points for topic branches.

## How to run tests

Since most of the tests are actually integration tests you will need to have a
working docker setup to make the full test suite pass.

1. build vendorized gtest: `./utils/build_gtest.sh`
2. build restclient-cpp: `./autogen.sh && ./configure && make check`
3. run the unit test suite: `make ci`

## Help wanted
Given that I'm not in a position to maintain compatibility with all the different
platforms, contributions around these are especially appreciated. I try to label
issues accordingly. Some examples are:
- [windows support](https://github.com/mrtazz/restclient-cpp/labels/windows)
- [debian build problems](https://github.com/mrtazz/restclient-cpp/labels/debian)
