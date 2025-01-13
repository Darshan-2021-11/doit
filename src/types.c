#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "log_util.h"

doit_tasks_t doit_tasks[MAXPRIORITY];
int16_t doit_task_priority = 0;

void
doit_init_tasks_t(void) {
	doit_tasks[doit_task_priority].size = doit_tasks[doit_task_priority].n = 0;
	doit_tasks[doit_task_priority].head = doit_tasks[doit_task_priority].lastChild = UINT16_MAX;
#ifdef DEBUG
	printf("Initialization of tasks completed.\n");
#endif
}

void
doit_init_task_t(char name[MAXLEN], uint16_t idx, uint16_t prev, uint16_t parent) {
	time_t created_At;
	time(&created_At);
	doit_tasks[doit_task_priority].task_list[idx] =
		(doit_task_t) {
			created_At = created_At,
			.prev = prev,
			.parent = parent,
			.next = UINT16_MAX,
			.child = UINT16_MAX,
			.lastChild = UINT16_MAX,
		};
	strncpy(doit_tasks[doit_task_priority].task_list[idx].name, name, MAXLEN - 1);
	if (prev != UINT16_MAX)
		doit_tasks[doit_task_priority].task_list[prev].next = idx;
	else if (parent != UINT16_MAX)
		doit_tasks[doit_task_priority].task_list[parent].child = idx;
#ifdef DEBUG
	printf("doit_tasks[doit_task_priority].task_list[%d] initialized with given.\n", idx);
#endif
}

uint16_t
doit_alloc_task_t(void) {
	if (doit_tasks[doit_task_priority].size == MAXLEN)
		doit_die(
				"Attempted to allocate more task_t than MAXLEN: %d\n",
				MAXLEN
			 );
	if (doit_tasks[doit_task_priority].n == doit_tasks[doit_task_priority].size) {
		++doit_tasks[doit_task_priority].size;
#ifdef DEBUG
	printf("doit_alloc_task_t(void): allocated new block.\n");
#endif
		return doit_tasks[doit_task_priority].n++;
	}
	uint16_t idx;
	idx = doit_tasks[doit_task_priority].n;
	doit_tasks[doit_task_priority].n = ((doit_task_alt_t *)&(doit_tasks[doit_task_priority].task_list[idx]))->free_ptr;
#ifdef DEBUG
	printf("doit_alloc_task_t(void): allocated previosly used block.\n");
#endif
	return idx;
}

void
doit_free_task_t(uint16_t idx) {
	uint16_t
		previdx = doit_tasks[doit_task_priority].task_list[idx].prev,
		nextidx = doit_tasks[doit_task_priority].task_list[idx].next,
		parentidx = doit_tasks[doit_task_priority].task_list[idx].parent,
		childidx = doit_tasks[doit_task_priority].task_list[idx].child;

	while (childidx != UINT16_MAX) {
		doit_free_task_t(childidx);
		childidx = doit_tasks[doit_task_priority].task_list[idx].child;
	}

	if (previdx != UINT16_MAX) {
#ifdef DEBUG
		printf("doit_free_task_t(uint16_t): Element removed with index: %d.\n", idx);
#endif
		doit_tasks[doit_task_priority].task_list[previdx].next = nextidx;
		if (nextidx != UINT16_MAX)
			doit_tasks[doit_task_priority].task_list[nextidx].prev = previdx;
		else if (parentidx != UINT16_MAX)
			doit_tasks[doit_task_priority].task_list[parentidx].lastChild = previdx;
		else
			doit_tasks[doit_task_priority].lastChild = previdx;
	} else if (parentidx != UINT16_MAX) {
#ifdef DEBUG
		printf("doit_free_task_t(uint16_t): Element removed with index: %d.\n", idx);
#endif
		doit_tasks[doit_task_priority].task_list[parentidx].child = nextidx;
		if (nextidx != UINT16_MAX)
			doit_tasks[doit_task_priority].task_list[nextidx].prev = previdx;
	} else {
#ifdef DEBUG
		printf("doit_free_task_t(uint16_t): Head element removed with index: %d.\n", idx);
#endif
		if (nextidx != UINT16_MAX)
			doit_tasks[doit_task_priority].task_list[nextidx].prev = previdx,
			doit_tasks[doit_task_priority].head = nextidx;
		else doit_init_tasks_t();
	}
	*(doit_task_alt_t *)(&(doit_tasks[doit_task_priority].task_list[idx])) =
		(doit_task_alt_t) {
			INT_MAX,
			doit_tasks[doit_task_priority].n
		};
	doit_tasks[doit_task_priority].n = idx;
}

uint16_t
doit_add_task_t(char name[MAXLEN], uint16_t idx, bool child) {
	uint16_t allocated = doit_alloc_task_t();
	if (!child) {
		if (idx != UINT16_MAX) idx = doit_tasks[doit_task_priority].task_list[idx].parent;
	}
	if (idx == UINT16_MAX) {
		doit_init_task_t(
				name,
				allocated,
				doit_tasks[doit_task_priority].lastChild,
				UINT16_MAX
				);
		doit_tasks[doit_task_priority].lastChild = allocated;
		// if no head, make current as head
		if (doit_tasks[doit_task_priority].head == UINT16_MAX)
			doit_tasks[doit_task_priority].head = allocated;
	} else {
		doit_init_task_t(
				name,
				allocated,
				doit_tasks[doit_task_priority].task_list[idx].lastChild,
				idx
				);
		doit_tasks[doit_task_priority].task_list[idx].lastChild = allocated;
	}
	return allocated;
}

void
doit_print_task_t(uint16_t idx) {
	struct tm *temp = localtime(&doit_tasks[doit_task_priority].task_list[idx].createdAt);
	char buffer[32];
	strftime(
			buffer,
			sizeof(buffer),
			"%a,%d/%m/%Y %H:%M",
			temp
			);
	printf(
			"(%hu) %s\t%s\n",
			idx,
			buffer,
			doit_tasks[doit_task_priority].task_list[idx].name
			);
}

//TODO

/**
 * Resources:
 * 1. https://en.wikipedia.org/wiki/Box-drawing_characters
 * 2. https://stackoverflow.com/questions/4494306/drawing-tables-in-terminal-using-ansi-box-characters
 */
#define BEGIN_GRAPHIC "\e(0"
#define END_GRAPHIC "\e(B"
#define BEGIN_BOLD "\e[1m"
#define END_BOLD "\e[0m"
#define CNTD "\x71"
#define LEVEL_CNTD  BEGIN_GRAPHIC /* BEGIN_BOLD */ "\x78\t"      /* END_BOLD */ END_GRAPHIC
#define LEVEL_DCNTD BEGIN_GRAPHIC /* BEGIN_BOLD */ " \t"         /* END_BOLD */ END_GRAPHIC
#define NEXT        BEGIN_GRAPHIC /* BEGIN_BOLD */ "\x74"   CNTD /* END_BOLD */ END_GRAPHIC
#define LAST        BEGIN_GRAPHIC /* BEGIN_BOLD */ "\x6D"   CNTD /* END_BOLD */ END_GRAPHIC
static const char *next = NEXT;
static const char *last = LAST;
static const char *lvl_cntd = LEVEL_CNTD;
static const char *lvl_dcntd = LEVEL_DCNTD;
static bool list[MAXLEN];

void
doit_print_tasks_t(uint16_t idx, uint16_t level) {

	list[level] = doit_tasks[doit_task_priority].task_list[idx].next != UINT16_MAX;

	for (int16_t i = 0; i < level; ++i)
		printf("%s", list[i] ? lvl_cntd : lvl_dcntd);
	printf("%s", list[level] ? next : last);
	doit_print_task_t(idx);

	if (doit_tasks[doit_task_priority].task_list[idx].child != UINT16_MAX)
		doit_print_tasks_t(doit_tasks[doit_task_priority].task_list[idx].child, level + 1);
	if (doit_tasks[doit_task_priority].task_list[idx].next != UINT16_MAX)
		doit_print_tasks_t(doit_tasks[doit_task_priority].task_list[idx].next, level);
}
#undef BEGIN_GRAPHIC
#undef END_GRAPHIC
#undef LAST
#undef NEXT
#undef CNTD

//TODO
void
doit_defrag_tasks_t(void) {
}
