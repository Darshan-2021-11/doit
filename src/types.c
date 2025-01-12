#include <stdio.h>

#include "types.h"
#include "log_util.h"

doit_tasks_t tasks;

void
doit_init_tasks_t(void) {
	tasks.head = tasks.size = tasks.n = 0;
}

void
doit_init_task_t(uint16_t idx, uint16_t name, uint16_t priority, uint16_t prev, uint16_t parent) {
	time_t created_At;
	time(&created_At);
	printf("Current time (raw): %ld\n", created_At);
	tasks.task_list[idx] =
		(doit_task_t) {
			created_At,
			name,
			priority,
			prev,
			parent,
			UINT16_MAX,
			UINT16_MAX,
		};
}

uint16_t
doit_alloc_task_t(void) {
	if (tasks.size == MAXLEN)
		die(
				"Attempted to allocate more task_t than MAXLEN: %s\n",
				MAXLEN
			 );
	if (tasks.n == tasks.size) {
		++tasks.size;
		return tasks.n++;
	}
	uint16_t idx;
	idx = tasks.n;
	tasks.n = *(int *)(&(tasks.task_list[idx]));
	return idx;
}

void
doit_free_task_t(uint16_t idx) {
	uint16_t
		previdx = tasks.task_list[idx].prev,
						nextidx = tasks.task_list[idx].next,
						parentidx = tasks.task_list[idx].parent,
						childidx = tasks.task_list[idx].child;


	if (previdx != UINT16_MAX)
		tasks.task_list[previdx].next = nextidx;
	else if (parentidx != UINT16_MAX)
		tasks.task_list[parentidx].child = nextidx;
	else tasks.head = nextidx;

	if (nextidx != UINT16_MAX)
		tasks.task_list[nextidx].prev = previdx;

	// recursively delete the child doit_task_t
	while (childidx != UINT16_MAX)
		doit_free_task_t(childidx);

	*(int *)(&(tasks.task_list[idx])) = tasks.n;
	tasks.n = idx;
}

void
doit_print_tasks_t(int level) {
	(void)level;
	/**
	 * Resources:
	 * 1. https://en.wikipedia.org/wiki/Box-drawing_characters
	 * 2. https://stackoverflow.com/questions/4494306/drawing-tables-in-terminal-using-ansi-box-characters
	 */
#define BEGIN_GRAPHIC "\e(0"
#define END_GRAPHIC "\e(B"
#define CNTD "\x71"
#define NEXT BEGIN_GRAPHIC "\x74" CNTD END_GRAPHIC
#define LAST BEGIN_GRAPHIC "\x6D" CNTD END_GRAPHIC

	printf("%s", NEXT);
	printf("%s", LAST);

#undef BEGIN_GRAPHIC
#undef END_GRAPHIC
#undef LAST
#undef NEXT
#undef CNTD
}

void
doit_defrag_tasks_t(void) {
}
