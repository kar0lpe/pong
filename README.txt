1. Install SDL2 
	- Download SDL2 source code(project has been linked with SDL2-2.26.5 realese https://github.com/libsdl-org/SDL/releases)
	- Run 
		./configure; make; make install
	- Be sure you have exported LD_LIBRARY_PATH 
		export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/libraries/paths/
2. Install Boost(https://linux.how2shout.com/how-to-install-boost-c-on-ubuntu-20-04-or-22-04/)
	- Run 
		sudo apt update && sudo apt upgrade
	- Run 
		sudo apt-get install build-essential g++ python3-dev autotools-dev libicu-dev libbz2-dev libboost-all-dev
	- Download Boost and extract files (project has been linked with boost_1_82_0)
	- Get into main directory boost_1_82_0
	- Run 
		./bootstrap.sh --prefix=/usr/
	- Run 
		sudo ./b2 install
3. Build project in build directory
	- Run
		cmake -B build -S .
	- Run 
		cmake --build build
4. Run project
	- Firstly start server 
		./build/server
	- Start client
		./build/client