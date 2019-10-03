# Repertoires du projet

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
OBJPROF_DIR = obj-prof

# Options de compilation/édition des liens

CC = clang
LD = clang
INC = -I$(INC_DIR)

CFLAGS += $(INC) -Wall -std=c99 -O2 -g -Wextra
LDFLAGS = -lm

# prof files not used
OBJPROF_FILES =
INC_PROF_FILES =

# et les votres!
OBJ_FILES = $(OBJ_DIR)/main.o $(OBJ_DIR)/extracteur.o $(OBJ_DIR)/rgb.o $(OBJ_DIR)/idct.o $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/mcu.o $(OBJ_DIR)/upsampling.o $(OBJ_DIR)/ppm.o $(OBJ_DIR)/image.o $(OBJ_DIR)/conversion.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/huffman.o
INC_FILES = $(INC_DIR)/idct.h $(INC_DIR)/bitstream.h $(INC_DIR)/jpeg_reader.h  $(INC_DIR)/huffman.h


TARGET = $(BIN_DIR)/jpeg2ppm

#######################################
#########    	 MAIN       ###########
#######################################

all: make_dir $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(LD) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $(TARGET)
	@echo -e "\n \033[1;32m [MAKEFILE FINISHED SUCCESSFULLY] \n \033[0m "


# Test if obj directory exists
make_dir:
	test -d obj || mkdir -p obj
	@echo -e "\n \033[0;36m [TESTED OBJ DIRECTORY SUCCESSFULLY] \n \033[0m "


#######################################
##########	     TESTS       ##########
#######################################

# cible test extracteur
EXTRACT = $(BIN_DIR)/extracteur
# cible test iqzz
IQZZ = $(BIN_DIR)/iqzz
# cible test idct
IDCT = $(BIN_DIR)/idct
# cible test rgb
RGB = $(BIN_DIR)/rgb
# cible test ppm
PPM = $(BIN_DIR)/ppm
# cible test mcu
MCU = $(BIN_DIR)/mcu
# cible test upsampling
UPSAMPLING = $(BIN_DIR)/upsampling
# cible test ourhuffman
HUFFMAN = $(BIN_DIR)/huffman
# cible test bitstream
BITSTREAM = $(BIN_DIR)/bitstream
# cible test jpeg_reader
JPEG_READER = $(BIN_DIR)/jpeg_reader

TESTS = $(IDCT) $(IQZZ) $(EXTRACT) $(PPM) $(RGB) $(MCU) $(UPSAMPLING) $(HUFFMAN) $(BITSTREAM) $(JPEG_READER)


tests: make_dir $(IQZZ) $(IDCT) $(EXTRACT) $(PPM) $(RGB) $(MCU) $(UPSAMPLING) $(HUFFMAN) $(BITSTREAM) $(JPEG_READER)
	@echo -e "\n \033[1;32m [TESTS FINISHED SUCCESSFULLY] \n \033[0m "

$(IQZZ): $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/test_iqzz.o $(OBJ_DIR)/idct.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/idct.o $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/test_iqzz.o -o $(IQZZ)

$(IDCT): $(OBJ_DIR)/idct.o $(OBJ_DIR)/test_idct.o $(OBJ_DIR)/iqzz.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/idct.o $(OBJ_DIR)/test_idct.o -o $(IDCT)

$(EXTRACT): $(OBJ_DIR)/extracteur.o $(OBJ_DIR)/test_extracteur.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/idct.o $(OBJ_DIR)/ppm.o $(OBJ_DIR)/huffman.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/extracteur.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/idct.o $(OBJ_DIR)/ppm.o $(OBJ_DIR)/test_extracteur.o -o $(EXTRACT)

$(PPM): $(OBJ_DIR)/ppm.o $(OBJ_DIR)/test_ppm.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/ppm.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/test_ppm.o -o $(PPM)

$(RGB): $(OBJ_DIR)/rgb.o $(OBJ_DIR)/test_rgb.o $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/idct.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/idct.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/rgb.o $(OBJ_DIR)/test_rgb.o -o $(RGB)

$(MCU): $(OBJ_DIR)/mcu.o $(OBJ_DIR)/test_mcu.o $(OBJ_DIR)/image.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/idct.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/image.o $(OBJ_DIR)/iqzz.o $(OBJ_DIR)/mcu.o $(OBJ_DIR)/test_mcu.o -o $(MCU)

$(UPSAMPLING): $(OBJ_DIR)/upsampling.o $(OBJ_DIR)/test_upsampling.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/upsampling.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/mcu.o $(OBJ_DIR)/test_upsampling.o -o $(UPSAMPLING)

$(HUFFMAN): $(OBJ_DIR)/huffman.o $(OBJ_DIR)/test_huffman.o $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/test_huffman.o -o $(HUFFMAN)

$(BITSTREAM): $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/test_bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/test_bitstream.o -o $(BITSTREAM)

$(JPEG_READER): $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/test_jpeg_reader.o $(OBJ_DIR)/huffman.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_DIR)/bitstream.o $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/huffman.o $(OBJ_DIR)/test_jpeg_reader.o -o $(JPEG_READER)


obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.PHONY: clean

doc: $(TARGET)
	doxygen doc/project_doc.doxy

clean:
	rm -rf $(TARGET) $(TESTS) $(OBJ_FILES) images/test_bitstream.txt $(wildcard $(OBJ_DIR)/*.o) $(wildcard $(SRC_DIR)/*.o) $(wildcard $(BIN_DIR)/*.ppm images/*.ppm $(BIN_DIR)/*.pgm images/*.pgm images/*.blabla bin/*.blabla doc/html doc/latex)
	@echo -e "\n \033[1;32m [REMOVED FILES SUCCESSFULLY] \n \033[0m "
