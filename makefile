all:
	rm -rf build
	mkdir build
	cmake -B build
	cmake --build build
test:
	cmake --build build
	build/main
rpm:
	sudo dnf install gcc-c++ make cmake
	sudo dnf install glfw-devel libglvnd-devel
	sudo dnf install mesa-libGL-devel mesa-libGLU-devel
apt:
	sudo apt install g++ cmake libglfw3-dev libgl1-mesa-dev
bmp:
	g++ bmp_maker.cpp -o bmp_maker
	./bmp_maker
	read
	rm bmp_maker
permission:
	chmod 777 -R bmp_images/*
	chmod 777 *.avi