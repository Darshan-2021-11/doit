#ifndef defines_h
#define defines_h

#define LINE_MAXLEN     1023
#define MAXLEN           255

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
