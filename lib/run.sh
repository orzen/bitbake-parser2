#!/bin/sh

make clean
make

rm build/lib.linux-x86_64-3.7/bb_cparser.cpython-37m-x86_64-linux-gnu.so
python3 setup.py build

G_MESSAGES_DEBUG=all PYTHONPATH=$PYTHONPATH:/home/mattihn/code/poky/bitbake/lib ./pytest/foo.py
G_MESSAGES_DEBUG=all PYTHONPATH=$PYTHONPATH:/home/mattihn/code/poky/bitbake/lib ./pytest/match_test.py
