#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "doit.h"
#include "parse.h"

char config_file[MAXLEN], data_file[MAXLEN];
char *config_path, *home_env;

int
main(int argc, char *argv[]) {
	char *program_name = argv[0];
	if (argc < 2) {
usage:
		doit_usage(program_name);
		return ERR_INVALID_ARGUMENTS;
	}
	home_env = getenv(HOME_ENV);
	snprintf(data_file, sizeof(data_file), "%s/%s/%s", home_env, ".local/share", DATA_PATH);
	doit_load_data(data_file);

	int opt;
	uint16_t id = UINT16_MAX;
	bool child = true;

	while((opt = getopt(argc, argv, "n:a")) != -1) {
		switch (opt) {
			case 'n':
				if (strspn(optarg, "0123456789") == strlen(optarg)) id = atoi(optarg);
				else goto usage;
				break;
			case 'a':
				child = false;
				break;
			default:
				goto usage;
		}
	}
	if (optind >= argc) goto usage;
	else {
		if (strcmp(argv[optind], "list") == 0) {
		 doit_print_tasks_t(id);

		} else if (strcmp(argv[optind], "add") == 0) {
		 doit_add_task_t(argv[optind + 1], id, child);
		 doit_dump_data(data_file);

		} else if (strcmp(argv[optind], "del") == 0) {
		 doit_delete_task_t(id);
		 doit_defrag_tasks_t();
		 doit_dump_data(data_file);

		} else goto usage;
	}



	/*
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
		 (void)doit_add_task_t("task_1", UINT16_MAX, 0);
		 (void)doit_add_task_t("task_2", UINT16_MAX, 0);
		 (void)doit_add_task_t("task_3", UINT16_MAX, 0);
		 (void)doit_add_task_t("task_4", 0, 0);
		 (void)doit_add_task_t("task_13", 0, 1);
		 (void)doit_add_task_t("task_5", 0, 1);
		 (void)doit_add_task_t("task_6", 0, 1);
		 (void)doit_add_task_t("task_7", 3, 1);
		 (void)doit_add_task_t("task_8", 7, 1);
		 (void)doit_add_task_t("task_9", 7, 1);
		 (void)doit_add_task_t("task_10", 7, 1);
		 (void)doit_add_task_t("task_11", 7, 1);
		 (void)doit_add_task_t("task_12", 7, 1);
		 doit_delete_task_t(0);
		 doit_defrag_tasks_t();
		 doit_print_tasks_t(doit_tasks[doit_task_priority].head);
		 doit_dump_data(data_file);
		 */
}

void doit_usage(char *prg) {
	printf(
			"Usage:\n"
			"%s [options]\n"
			"Options:\n"
			"add  :\t Add a task. Specify task using quotes.\n"
			"      \t Specify id with `-n` flag to add subtask or with `-a` to add it to its level.\n"
			"del  :\t Delete a task. specifying the id with `-n` flag of the task to delete.\n"
			"list :\t List all tasks in tree format. Specify id with `-n` to view its subtasks.\n"
			"help :\t Show this help command.\n",
			prg);
}
