#include "log_util.h"
#include "dir_util.h"

#ifdef _WIN32
#include <direct.h>
#include <sys/stat.h>

#define DIR_EXISTS(dir) (_stat(dir, &st) == 0 && (st.st_mode & _S_IFDIR))
#define MKDIR(dir) (!_mkdir(dir))
struct _stat st;

#elif __linux__ || __unix__ || __APPLE__
#include <sys/stat.h>
#include <unistd.h>

#define DIR_EXISTS(dir) (stat(dir, &st) == 0 && S_ISDIR(st.st_mode))
#define MKDIR(dir) (!mkdir(dir, 0755))
struct stat st;

#else
#error "Unsupported OS"
#endif

int MKDIR_P(char *dir) {
#define RED "\e[0;31m"
#define NC "\e[0m"
	char *p = dir, t;
	while (*(++p)) if (*p == '/' || *p == '\\') {
		t = *p;
		*p = '\0';
		if (!DIR_EXISTS(dir)) {
			if (!MKDIR(dir)) {
				die("Failed to create directory, '%s'\n", dir);
			}
		}
		*p = t;
	}
	return 1;
#undef RED
#undef NC
}
