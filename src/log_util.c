#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void
die(const char *errstr, ...)
{
#define RED "\e[0;31m"
#define NC "\e[0m"

	fprintf(stderr, RED "[ERROR]: " NC);

	va_list ap;
	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);

#undef RED
#undef NC
	exit(1);
}
