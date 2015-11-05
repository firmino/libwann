# compiler and libraries
CC=clang++
OPTIONS= -std=c++11 -g -O2

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

main:
	@echo "COMPILING MAIN: "
	$(CC) -c $(SRC)/Main.cpp -o $(BUILD)/Main.o $(OPTIONS)
	$(CC)  $(BUILD)/*.o  -o $(BUILD)/programa.exe $(OPTIONS) 
	@echo "\n\n"
	$(BUILD)/programa.exe
###########################################################################






############################# whole WiSARD #############################
all: clean init memory discriminator wisard sswisard
	$(CC) -c $(SRC)/Main.cpp -o $(BUILD)/Main.o $(OPTIONS)
	$(CC)  $(BUILD)/*.o  -o $(BUILD)/programa.exe $(OPTIONS) 
	@echo "\n\n"
	$(BUILD)/programa.exe
	
######################################################################

