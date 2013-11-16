CFLAGS += -g -O0 `pkg-config --cflags libavformat libavutil`
LDFLAGS += -lavformat `pkg-config --libs libavformat libavutil`

MODULES = \
	cx_array.o \
	cx_list.o \
	cx_obj.o \
	cx_string.o \
	fscan_posix.o \
	main.o \
	track_info.o \
	meta_ffmpeg.o \
	tags.o

cifond: Makefile $(MODULES)
	$(CC) $(MODULES) $(LDFLAGS) -o cifond
