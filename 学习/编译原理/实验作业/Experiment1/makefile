# 不要删除或修改此文件

DIR_INC = .
DIR_SRC = .
DIR_OBJ = .
DIR_BIN = .

SRC = $(wildcard ${DIR_SRC}/*.c)  
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC})) 

TARGET = app.exe

BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = gcc
CFLAGS = -g -w -fmax-errors=10 -std=c99 -fsigned-char -I${DIR_INC}

${BIN_TARGET}:${OBJ}
	@echo --- build executable file: $@
	$(CC) $(OBJ) -o $@ -lm
    
${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	@echo --- build object file: $@
	$(CC) $(CFLAGS) -c  $< -o $@

RM =  rm -f

ifdef OS
	RM = del /Q
else
	ifeq ($(shell uname), Linux)
		RM = rm -f
	endif	
endif

clean:
	$(RM) $(TARGET) *.o result_comparation.html user_*