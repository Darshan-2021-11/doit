#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void
doit_die(const char *errstr, ...)
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

void
doit_warn(const char *errstr, ...)
{
#define YELLOW "\e[0;33m"
#define NC "\e[0m"

	fprintf(stderr, YELLOW "[WARN]: " NC);

	va_list ap;
	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);

#undef RED
#undef NC
}
