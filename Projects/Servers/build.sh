#ACE library
if [ ! -e /usr/local/lib/libACE.so ]; then
    echo "============================="
    echo "==   Building ACE library  =="
    echo "============================="
    echo "ACE library wasn't detected on your system, it will be installed."
    if [ "$(whoami)" != 'root' ]; then
	echo "You must run this script as root to allow the ACE library to be installed."
	exit
    fi
    cd ../Dependencies/ACE_wrappers
    echo "=> Regenerating configuration..."
    autoreconf --install --force
    echo "=> Creating build tree..."
    mkdir build
    cd build
    echo "=> Generating makefiles..."
    ../configure
    echo "=> Compiling..."
    make
    echo "=> Installing..."
    make install
    cd ../../../Servers
fi

#Building Realm & World server
echo "============================="
echo "==    Building servers     =="
echo "============================="
if [ -d obj ]; then
    echo "=> Cleaning up old 'obj' folder..."
    if [ -d Realm ]; then
	rm -rf Realm
    fi
    if [ -d World ]; then
	rm -rf World
    fi
    rm -rf obj
fi
echo "=> Creating new obj folder..."
mkdir obj
mkdir obj/Realm
mkdir obj/World
echo "=> Compiling..."
make

echo "============================="
echo "==      JOB FINISHED!      =="
echo "============================="