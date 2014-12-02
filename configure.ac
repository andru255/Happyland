#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT(Happyland, 0.1, baudouin@feildel.org)


AM_INIT_AUTOMAKE()

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lSDL2':
AC_CHECK_LIB(SDL2, SDL_Init)
# FIXME: Replace `main' with a function in `-lSDL2_image':
AC_CHECK_LIB(SDL2_image, IMG_Init)
# FIXME: Replace `main' with a function in `-lpthread':
#AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_CHECK_HEADERS(stdlib.h string.h stdio.h errno.h SDL2/SDL.h SDL2/SDL_image.h)

# Checks for typedefs, structures, and compiler characteristics.
#AC_CHECK_HEADER_STDBOOL
#AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS(atexit strtol)

AC_PROG_RANLIB

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/graphics/Makefile
	src/c-containers/Makefile
	src/characters/Makefile
])
AC_OUTPUT
