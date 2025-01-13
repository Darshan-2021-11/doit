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
	uint16_t idx1 = doit_alloc_task_t();
	doit_init_task_t(
			idx1,
			50,
			2,
			UINT16_MAX,
			UINT16_MAX
			);
	/*
	struct tm *temp = localtime(&tasks.task_list[idx].createdAt);
	printf("idx1_time: %s", asctime(temp));
	*/
	uint16_t idx2 = doit_alloc_task_t();
	doit_init_task_t(
			idx2,
			50,
			2,
			idx1,
			UINT16_MAX
			);

	uint16_t idx3 = doit_alloc_task_t();
	doit_init_task_t(
			idx3,
			50,
			2,
			idx2,
			UINT16_MAX
			);

	printf("idx1: %hu, idx2: %hu, idx3, %hu\n", idx1, idx2, idx3);
	printf("head: %hu\n", tasks.head);
	doit_free_task_t(idx1);
	printf("head: %hu\n", tasks.head);
	printf("head: %hu\n", tasks.head);

	uint16_t idx4 = doit_alloc_task_t();
	printf("alloc: %hu\n", idx4);
	uint16_t idx5 = doit_alloc_task_t();
	printf("alloc: %hu\n", idx5);
	uint16_t idx6 = doit_alloc_task_t();
	printf("alloc: %hu\n", idx6);
}
