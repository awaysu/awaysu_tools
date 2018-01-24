CC=arm-linux-gcc
EXEC_CFLAG = -Wall -fPIC -lz -L../zlib-1.2.11
INCLUDE=-I./include -I../zlib-1.2.11/

LIB_SRC = \
        backed_block.c \
        output_file.c \
        sparse.c \
        sparse_crc32.c \
        sparse_err.c \
        sparse_read.c

IMG2SIMG_SRC=img2simg.c
IMG2SIMG_EXE=img2simg

SIMG2IMG_SRC=simg2img.c
SIMG2IMG_EXE=simg2img

SIMG2SIMG_SRC=simg2simg.c
SIMG2SIMG_EXE=simg2simg

all: build_img2simg build_simg2img build_simg2simg

build_img2simg:
	@echo "[build] img2simg"
	@$(CC) -o $(IMG2SIMG_EXE) $(EXEC_CFLAG) $(INCLUDE) $(LIB_SRC) $(IMG2SIMG_SRC)	

build_simg2img:
	@echo "[build] simg2img"
	@$(CC) -o $(SIMG2IMG_EXE) $(EXEC_CFLAG) $(INCLUDE) $(LIB_SRC) $(SIMG2IMG_SRC)	

build_simg2simg:
	@echo "[build] simg2simg"
	@$(CC) -o $(SIMG2SIMG_EXE) $(EXEC_CFLAG) $(INCLUDE) $(LIB_SRC) $(SIMG2SIMG_SRC)	

clean:
	rm -rf $(IMG2SIMG_EXE) $(SIMG2IMG_EXE) $(SIMG2SIMG_EXE)
