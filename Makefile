# compiler and libraries
CC=clang++
OPTIONS= -std=c++11 -g -O2 -fpic

# folders of the project
INCLUDE = ./include
SRC=./src
BUILD= ./build
RESOURCES= ./resources

  
############################# the WiSARD modules #############################
init:
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)

memory:
	@echo "COMPILING MEMORY: "
	$(CC) -c $(SRC)/Memory.cpp  -o $(BUILD)/Memory.o $(OPTIONS) 
	@echo "\n"
	
discriminator:
	@echo "COMPILING DISCRIMINATOR: "
	$(CC) -c $(SRC)/Discriminator.cpp -o $(BUILD)/Discriminator.o $(OPTIONS) 
	@echo "\n\n"

wisard: 
	@echo "COMPILING WISARD: "
	$(CC) -c $(SRC)/WiSARD.cpp  -o $(BUILD)/WiSARD.o $(OPTIONS)
	@echo "\n\n"

sswisard:
	@echo "COMPILING SS_WISARD: "
	$(CC) -c $(SRC)/SS_WiSARD.cpp  -o $(BUILD)/SS_WiSARD.o $(OPTIONS)
	@echo "\n\n"

create_library: 
	@echo "GENERATING DYNAMIC LIBRARY: "
	$(CC) -shared $(BUILD)/*.o  -o $(BUILD)/libwann.so 
	@echo "\n\n"

###########################################################################

############################# whole libwisard #############################
all: clean init memory discriminator wisard sswisard create_library

############################## moving libwisard for /usr/lob/lib###########
install:
	rm /usr/local/lib/libwann.so
	rm -rf /usr/local/include/wann
	mkdir /usr/local/include/wann

	cp $(BUILD)/libwann.so /usr/local/lib/
	cp $(INCLUDE)/*.hpp /usr/local/include/wann/
	ldconfig

############################# running tests ###############################
run_test:
	@echo "COMPILING MAIN: "
	rm -rf ./test/*.o
	rm -rf ./test/*.exe
	$(CC) -c ./test/Main.cpp -o ./test/Main.o $(OPTIONS) -lwann
	$(CC)  ./test/*.o  -o ./test/programa.exe $(OPTIONS) -lwann
	@echo "\n\n"
	./test/programa.exe