#ifndef _COND_H_
#define _COND_H_

#include "data.h"

void add_ignore_day			(int);
void add_ignore_teacher		(const char *);
void add_ignore_class		(int);

cond_check(const COURSE_DATA *);

#endif