#ifndef defines_h
#define defines_h

#include <assert.h>

#define MAXLEN                    254
#define MAXPRIORITY                 3
static_assert(MAXLEN != 0,
	"Task name needs to have some non-zero length.\n");
static_assert(MAXPRIORITY != 0,
	"Tasks needs to have some non-zero priority.\n");

#define PROGRAM_STATE_LIST   INT8_MAX
#define PROGRAM_STATE_ADD           0
#define PROGRAM_STATE_DELETE        1

#define ERR_INVALID_ARGUMENTS       1

#ifdef _WIN32
	#define HOME_ENV "HOMEPATH"
	#define CONFIG_HOME_ENV "LOCALAPPDATA"
#elif __linux__ || __unix__ || __APPLE__
	#define HOME_ENV "HOME"
	#define CONFIG_HOME_ENV "XDG_CONFIG_HOME"
#else
	#error "Unsupported OS"
#endif

#define CONFIG_PATH     "doit/doitrc"
#define DATA_PATH       "doit/data"
#endif //defines_h
