CXX = g++
CXXFLAGS = -lX11 -O2 -s

make: dwmStatus.cpp
	$(CXX) $(CXXFLAGS) -o dwmStatus dwmStatus.cpp
install:
	sudo mv dwmStatus /usr/local/bin
clean:
	rm -f dwmStatus

