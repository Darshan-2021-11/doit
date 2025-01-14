#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "log_util.h"

doit_tasks_t doit_tasks[MAXPRIORITY];
int16_t doit_task_priority = 0;

void
doit_init_task_t(char name[MAXLEN], uint16_t idx, uint16_t prev, uint16_t parent) {
	time_t createdAt;
	time(&createdAt);
	{
		doit_tasks[doit_task_priority].task_list[idx].createdAt = createdAt,
		doit_tasks[doit_task_priority].task_list[idx].prev = prev,
		doit_tasks[doit_task_priority].task_list[idx].parent = parent,
		doit_tasks[doit_task_priority].task_list[idx].next = UINT16_MAX,
		doit_tasks[doit_task_priority].task_list[idx].child = UINT16_MAX,
		doit_tasks[doit_task_priority].task_list[idx].lastChild = UINT16_MAX;
		char *dest = doit_tasks[doit_task_priority].task_list[idx].name,
				 *src  = name;
		while (*src && src - name < MAXLEN - 1) *dest++ = *src++;
		*dest = '\0';
	}
	if (prev != UINT16_MAX)
		doit_tasks[doit_task_priority].task_list[prev].next = idx;
	else if (parent != UINT16_MAX)
		doit_tasks[doit_task_priority].task_list[parent].child = idx;
#ifdef DEBUG
	printf("void doit_init_task_t(char[], uint16_t, uint16_t, uint16_t): initialized doit_tasks[%d].task_list[%d], %s.\n", doit_task_priority, idx, name);
#endif
}

void
doit_init_tasks_t(void) {
	doit_tasks[doit_task_priority].size = doit_tasks[doit_task_priority].n = 0;
	doit_tasks[doit_task_priority].head = doit_tasks[doit_task_priority].lastChild = UINT16_MAX;
#ifdef DEBUG
	printf("void doit_init_tasks_t(void): initialization of doit_tasks_t[%d] completed.\n", doit_task_priority);
#endif
}

bool
doit_check_bounds_tasks_t(uint16_t idx) {
	return 
		idx >= doit_tasks[doit_task_priority].size ||
		((doit_task_alt_t *)&doit_tasks[doit_task_priority].task_list[idx])->unused == UINT16_MAX;
}

uint16_t
doit_alloc_task_t(void) {
	if (doit_tasks[doit_task_priority].size == MAXLEN) {
		doit_warn("Stop procastinating, its already too much.\n");
		/*
			 doit_die(
			 "uint16_t doit_alloc_task_t(void): Attempted to allocate more task_t than MAXLEN: %d\n",
			 MAXLEN
			 );
			 */
		doit_die(
				"Attempted to add more task than %d.\n",
				MAXLEN
				);
	}
	if (doit_tasks[doit_task_priority].n == doit_tasks[doit_task_priority].size) {
		++doit_tasks[doit_task_priority].size;
#ifdef DEBUG
		printf("uint16_t doit_alloc_task_t(void): allocated new block doit_tasks[%d].task_list[%d].\n", doit_task_priority, doit_tasks[doit_task_priority].n);
#endif
		return doit_tasks[doit_task_priority].n++;
	}
	uint16_t idx;
	idx = doit_tasks[doit_task_priority].n;
	doit_tasks[doit_task_priority].n = ((doit_task_alt_t *)&doit_tasks[doit_task_priority].task_list[idx])->free_ptr;
#ifdef DEBUG
	printf("uint16_t doit_alloc_task_t(void): allocated new block doit_tasks[%d].task_list[%d].\n", doit_task_priority, doit_tasks[doit_task_priority].n);
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

	while (childidx != UINT16_MAX)
		doit_free_task_t(childidx), childidx = doit_tasks[doit_task_priority].task_list[idx].child;

	if (previdx != UINT16_MAX) {
#ifdef DEBUG
		printf("void doit_free_task_t(uint16_t): freed block doit_tasks[%d].task_list[%d], %s.\n", doit_task_priority, idx, doit_tasks[doit_task_priority].task_list[idx].name);
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
		printf("void doit_free_task_t(uint16_t): freed block doit_tasks[%d].task_list[%d], %s.\n", doit_task_priority, idx, doit_tasks[doit_task_priority].task_list[idx].name);
#endif
		doit_tasks[doit_task_priority].task_list[parentidx].child = nextidx;
		if (nextidx != UINT16_MAX)
			doit_tasks[doit_task_priority].task_list[nextidx].prev = previdx;
	} else {
#ifdef DEBUG
		printf("void doit_free_task_t(uint16_t): freed head block doit_tasks[%d].task_list[%d], %s.\n", doit_task_priority, idx, doit_tasks[doit_task_priority].task_list[idx].name);
#endif
		if (nextidx != UINT16_MAX)
			doit_tasks[doit_task_priority].task_list[nextidx].prev = previdx,
			doit_tasks[doit_task_priority].head = nextidx;
		else
			doit_init_tasks_t();
	}
	*(doit_task_alt_t *)&doit_tasks[doit_task_priority].task_list[idx] =
		(doit_task_alt_t) {
			UINT16_MAX,
			doit_tasks[doit_task_priority].n
		};
	doit_tasks[doit_task_priority].n = idx;
}

uint16_t
doit_add_task_t(char name[MAXLEN], uint16_t idx, bool child) {
	if (
			idx != UINT16_MAX &&
			doit_check_bounds_tasks_t(idx)
		 ) {
#ifdef DEBUG
		//printf("uint16_t doit_add_task_t(char [], uint16_t, bool): block with index: %d, does not exist in doit_tasks[%d].\n", doit_task_priority);
		printf("Task with id: %d, does not exist.\n", idx);
#endif
		return UINT16_MAX;
	}
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
	printf("Task added successfully.\n");
	return allocated;
}

void
doit_delete_task_t(uint16_t idx) {
	if (doit_check_bounds_tasks_t(idx)) {
		//printf("void doit_free_task_t(void): block with index: %d, does not exist in doit_tasks[%d].\n", doit_task_priority);
		printf("Task with id: %d, does not exist.\n", idx);
		return;
	}
	doit_free_task_t(idx);
	printf("Task(and its subtasks) removed successfully.\n");
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

/**
 * Resources:
 * 1. https://en.wikipedia.org/wiki/Box-drawing_characters
 * 2. https://stackoverflow.com/questions/4494306/drawing-tables-in-terminal-using-ansi-box-characters
 */
#define BEGIN_GRAPHIC "\e(0"
#define END_GRAPHIC "\e(B"
#define BEGIN_BOLD  "\e[1m"
#define END_BOLD    "\e[0m"
#define CNTD        "\x71"
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
doit_print_tasks_t_helper(uint16_t idx, uint16_t level) {
	list[level] = doit_tasks[doit_task_priority].task_list[idx].next != UINT16_MAX;

	for (uint16_t i = 0; i < level; ++i)
		printf("%s", list[i] ? lvl_cntd : lvl_dcntd);
	printf("%s", list[level] ? next : last);
	doit_print_task_t(idx);

	if (doit_tasks[doit_task_priority].task_list[idx].child != UINT16_MAX)
		doit_print_tasks_t_helper(doit_tasks[doit_task_priority].task_list[idx].child, level + 1);
	if (doit_tasks[doit_task_priority].task_list[idx].next != UINT16_MAX)
		doit_print_tasks_t_helper(doit_tasks[doit_task_priority].task_list[idx].next, level);
}
#undef END_GRAPHIC
#undef BEGIN_BOLD
#undef END_BOLD
#undef CNTD
#undef LEVEL_CNTD
#undef LEVEL_DCNTD
#undef NEXT
#undef LAST

void
doit_print_tasks_t(uint16_t idx) {
	if (idx == UINT16_MAX) idx = doit_tasks[doit_task_priority].head;
	if (
			doit_tasks[doit_task_priority].head == UINT16_MAX ||
			doit_check_bounds_tasks_t(idx)
		 )
		return;

	doit_print_tasks_t_helper(idx, 0);
}

void
doit_defrag_tasks_t(void) {
	if (!doit_tasks[doit_task_priority].size) return;
	uint16_t l = 0, r = doit_tasks[doit_task_priority].size - 1, t;
	while (l <= r) {
		if (((doit_task_alt_t *)&doit_tasks[doit_task_priority].task_list[r])->unused == UINT16_MAX)
			--r;
		else if (((doit_task_alt_t *)&doit_tasks[doit_task_priority].task_list[l])->unused != UINT16_MAX)
			++l;
		else {
			{
				// copy r to l
				doit_tasks[doit_task_priority].task_list[l].createdAt = doit_tasks[doit_task_priority].task_list[r].createdAt,
				doit_tasks[doit_task_priority].task_list[l].prev = doit_tasks[doit_task_priority].task_list[r].prev,
				doit_tasks[doit_task_priority].task_list[l].parent = doit_tasks[doit_task_priority].task_list[r].parent,
				doit_tasks[doit_task_priority].task_list[l].next = doit_tasks[doit_task_priority].task_list[r].next,
				doit_tasks[doit_task_priority].task_list[l].child = doit_tasks[doit_task_priority].task_list[r].child,
				doit_tasks[doit_task_priority].task_list[l].lastChild = doit_tasks[doit_task_priority].task_list[r].lastChild;
				char *dest = doit_tasks[doit_task_priority].task_list[l].name,
						 *src  = doit_tasks[doit_task_priority].task_list[r].name;
				while (*src && src - doit_tasks[doit_task_priority].task_list[r].name < MAXLEN - 1) *dest++ = *src++;
				*dest = '\0';
			}

			if ((t = doit_tasks[doit_task_priority].task_list[l].prev) != UINT16_MAX)
				doit_tasks[doit_task_priority].task_list[t].next = l;
			if ((t = doit_tasks[doit_task_priority].task_list[l].next) != UINT16_MAX)
				doit_tasks[doit_task_priority].task_list[t].prev = l;

			if (doit_tasks[doit_task_priority].head == r)
				doit_tasks[doit_task_priority].head = l;
			if (doit_tasks[doit_task_priority].lastChild == r)
				doit_tasks[doit_task_priority].lastChild = l;

			++l, --r;
		}
		doit_tasks[doit_task_priority].size = l,
			doit_tasks[doit_task_priority].n = l;
	}
}
