########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g  

MATH_LIBS = -lm

EXEC_DIR=.


.cpp.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 


#First Program (ListTest)

Cpp_OBJ=Image.o Line.o Pgm.o DisjSets.o HoughDatabase.o Database.o s3.o

PROGRAM_NAME=s3

$(PROGRAM_NAME): $(Cpp_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ) $(INCLUDES) $(LIBS_ALL)


all: 
	make $(PROGRAM_NAME) 


clean:
	(rm -f *.o;)

(:
