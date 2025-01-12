#include <stdio.h>
#include <stdlib.h>

#include "parse.h"

char config_file[MAXLEN], data_file[MAXLEN];
char *config_path, *home_env;

int
main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	home_env = getenv(HOME_ENV);
	if (config_path == NULL) {
		char *config_home = getenv(CONFIG_HOME_ENV);
		if (config_home != NULL)
			snprintf(config_file, sizeof(config_file), "%s/%s", config_home, CONFIG_PATH);
		else
			snprintf(config_file, sizeof(config_file), "%s/%s/%s", home_env, ".config", CONFIG_PATH);
	} else {
		snprintf(config_file, sizeof(config_file), "%s", config_path);
	}
	snprintf(data_file, sizeof(data_file), "%s/%s/%s", home_env, ".local", "data");

	load_config(config_file);
	load_data(data_file);
	//doit_print_tasks_t((&(doit_tasks_t){}), 0);
	doit_init_tasks_t();
	uint16_t idx = doit_alloc_task_t();
	doit_init_task_t(
			idx,
			50,
			2,
			UINT16_MAX,
			0
			);
}
