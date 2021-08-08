
all:
	g++ UDPpublish.cpp -o UDPpublish
	g++ UDPrecieve.cpp -o UDPrecieve

clean:
	rm -f UDPpublish UDPrecieve
