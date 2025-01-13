#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	snprintf(data_file, sizeof(data_file), "%s/%s/%s", home_env, ".local/share", DATA_PATH);

	doit_load_config(config_file);
	doit_load_data(data_file);
	//doit_print_tasks_t((&(doit_tasks_t){}), 0);
	doit_init_tasks_t();
	//doit_add_task_t(uint16_t idx, uint16_t name, bool child);
	(void)doit_add_task_t("task_1", UINT16_MAX, 0);
	(void)doit_add_task_t("task_2", UINT16_MAX, 0);
	(void)doit_add_task_t("task_3", UINT16_MAX, 0);
	(void)doit_add_task_t("task_4", 0, 0);
	(void)doit_add_task_t("task_4", 0, 1);
	(void)doit_add_task_t("task_5", 0, 1);
	(void)doit_add_task_t("task_6", 0, 1);
	(void)doit_add_task_t("task_7", 3, 1);
	(void)doit_add_task_t("task_8", 7, 1);
	(void)doit_add_task_t("task_9", 7, 1);
	(void)doit_add_task_t("task_10", 7, 1);
	(void)doit_add_task_t("task_11", 7, 1);
	(void)doit_add_task_t("task_12", 7, 1);
	doit_print_tasks_t(doit_tasks[doit_task_priority].head, 0);
	//printf("sizeof(time_t): %lubytes.\n", sizeof(time_t));
}
