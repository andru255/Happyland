SUBDIRS=graphics c-containers

if DEBUG
AM_CFLAGS = -ggdb -O0 -D__DEBUG__
else
AM_CFLAGS = -O2
endif

bin_PROGRAMS=happyland

happyland_LDADD= \
	graphics/libgraphics.a \
	c-containers/libccontainers.a

happyland_SOURCES = \
	main.c \
	config.h \
	error-codes.h \
	string-tools.c string-tools.h \
	utils.c utils.h \
	game.c game.h
