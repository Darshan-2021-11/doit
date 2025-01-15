#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "doit.h"
#include "parse.h"

char data_file[MAXLEN];
char *data_path;

int
main(int argc, char *argv[]) {
	char *program_name = argv[0];
	if (argc < 2) {
usage:
		doit_usage(program_name);
		return ERR_INVALID_ARGUMENTS;
	}

	int opt;
	uint16_t id = UINT16_MAX;
	bool child = true;

	while((opt = getopt(argc, argv, "d:n:p:a")) != -1) {
		switch (opt) {
			case 'a':
				child = false;
				break;
			case 'd':
				data_path = optarg;
				break;
			case 'n':
				if (strspn(optarg, "0123456789") == strlen(optarg)) id = atoi(optarg);
				else goto usage;
				break;
			case 'p':
				if (strspn(optarg, "0123456789") == strlen(optarg)) {
					doit_task_current_priority = atoi(optarg);
					if (doit_task_current_priority < MAXPRIORITY) break;
				}
				goto usage;

			default:
				goto usage;
		}
	}

	if (data_path != NULL)
		snprintf(data_file, sizeof(data_path), "%s", data_path);
	else
		snprintf(data_file, sizeof(data_file), "%s/%s/%s", getenv(HOME_ENV), ".local/share", DATA_PATH);
	doit_load_data(data_file);


	if (optind >= argc) goto usage;
	else {
		if (strcmp(argv[optind], "list") == 0) {
			doit_print_tasks_t(id);

		} else if (strcmp(argv[optind], "del") == 0) {
			doit_delete_task_t(id);
			doit_defrag_tasks_t();
			doit_dump_data(data_file);

		} else if (strcmp(argv[optind], "add") == 0) {
			if (optind == argc - 1) goto usage;
			for (int i = optind + 1; i < argc; ++i)
				doit_add_task_t(argv[i], id, child);
			doit_dump_data(data_file);
		} else goto usage;
	}
}

void doit_usage(char *prg) {
	printf(
			"Usage:\n"
			"%s [options]\n"
			"Options:\n"
			"add  :\t Add task(s). Specify task using quotes. Defaults to top level.\n"
			"      \t Specify id(number) with `-n` flag to add subtask or with `-a` to add it to its level.\n"
			"del  :\t Delete a task, specifying the id(number) with `-n` flag.\n"
			"list :\t List tasks in tree format, specifying id(number) with `-n` to shows subtasks.\n"
			"-p   :\t Give the priority(number [0 - %d]) of task specified. Defaults to 0.\n"
			"help :\t Show this help command.\n",
			prg, MAXPRIORITY - 1);
}
