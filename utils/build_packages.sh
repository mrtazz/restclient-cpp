#!/bin/sh

# steps to build different packages for restclient-cpp

# some meta data
GEM=$(which gem)
NAME="restclient-cpp"
VERSION=$(git describe --abbrev=0)
RELEASE=1
URL="http://code.mrtazz.com/restclient-cpp"
DEPS="curl"

# helper function to echo what's being run
runthis(){
  ## print the command to the logfile
  echo "$@"
  ## run the command and redirect it's error output
  ## to the logfile
  eval "$@"
}

# we need fpm for building packages
FPM=$(which fpm)
if [ $? != 0 ]; then
  echo "Installing fpm..."
  runthis "${GEM} install fpm --no-ri --no-rdoc"
fi

# function to build packages
function build_package() {

  local package_type=${1}

  echo "Building ${package_type} for ${NAME} ${VERSION}..."
  ${FPM} -t ${package_type} -s dir \
    --name ${NAME} \
    --version $(echo ${VERSION} | grep -o "[0-9\.].*") \
    --iteration ${RELEASE} \
    --epoch 1 \
    --license MIT \
    --maintainer "Daniel Schauenberg <d@unwiredcouch.com>" \
    --url ${URL} \
    $(for dep in `echo ${DEPS}`; do echo "--depends ${dep} " ; done) \
    --vendor mrtazz \
    usr

}

# Let's do this
# build the software
runthis "./autogen.sh"
runthis "./configure --prefix=/usr"
runthis "make install DESTDIR=$(pwd)"

# build the packages
build_package rpm
build_package deb

