CC		=	gcc
CXX		=   g++
STRIP	=	strip
RM		=	rm -f
WINDRES =	windres


CXXFLAGS = -O3  -DNDEBUG -DHAVE_STRING_H -DUSE_OWN_MD5  \
			-std=gnu++98 -I. \

CFLAGS	=	-O3  -DNDEBUG -DHAVE_STRING_H -DUSE_OWN_MD5  \
			 -I. \

LFLAGS	=

TARGET	=	huaweicalc.exe

CPPSRCS	=	main_cmd.cpp \
			calc.cpp \
			encrypt_1.cpp \
			encrypt_2.cpp \
			encrypt_3.cpp \
			encrypt_4.cpp \
			encrypt_5_v2.cpp \
			encrypt_6.cpp \
			encrypt_7.cpp \
			encrypt_v1.cpp \
			md5.cpp
	
OBJS = $(addsuffix .o,$(basename $(CPPSRCS)))
LIBS = -static -lm

ifdef WINDIR
	OBJS += w32dlg.rc.o
endif
	
.SUFFIXES: .c.o
.SUFFIXES: .cpp.o

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(LFLAGS) -g -o $@ $(OBJS) $(LIBS)
	$(STRIP) $@

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

w32dlg.rc.o: w32dlg.rc
	$(WINDRES) -i $< -o $@
	
clean:
	rm -f $(TARGET) $(OBJS) 
