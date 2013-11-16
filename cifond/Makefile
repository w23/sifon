CFLAGS += -g -O0
LDFLAGS += 

MODULES = \
	cx_array.o \
	cx_list.o \
	cx_obj.o \
	cx_string.o \
	fscan_posix.o \
	main.o \
	track_info.o

cifond: Makefile $(MODULES)
	$(LINK.c) $(MODULES) -o cifond
