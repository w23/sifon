CFLAGS += -g -O0 -std=c99 `pkg-config --cflags libavformat libavutil sqlite3 libevent` -Wall -Wextra -Werror -pedantic
LDFLAGS += -lavformat `pkg-config --libs libavformat libavutil sqlite3 libevent`

CFLAGS += -D_POSIX_C_SOURCE=200112L

MODULES = \
	cx_log.o \
	cx_array.o \
	cx_list.o \
	cx_obj.o \
	cx_string.o \
	cx_stream.o \
	instancer.o \
	keeper.o \
	server.o \
	sourcer.o \
	main.o \
	track_info.o \
	meta_ffmpeg.o \
	tags.o

cifond: Makefile $(MODULES)
	$(CC) $(MODULES) $(LDFLAGS) -o cifond

clean:
	@rm $(MODULES) cifond
