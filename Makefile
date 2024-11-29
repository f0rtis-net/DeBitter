init:
	mkdir build
	cd build
	cmake ..
	cd ..

build:
	cd build
	make
	cd ..