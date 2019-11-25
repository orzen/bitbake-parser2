
all: build

build: configure
	ninja -C build

configure:
	meson build

clean:
	rm -r build

check:
	G_MESSAGES_DEBUG=all PYTHONPATH=$(PYTHONPATH):$(PWD)/build/ext:/home/mattihn/code/poky/bitbake/lib python3 -m pytest -s

