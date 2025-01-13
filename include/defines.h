#ifndef defines_h
#define defines_h

#define MAXLEN           255
#define MAXPRIORITY        1

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
