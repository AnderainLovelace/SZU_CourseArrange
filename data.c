/*
    by Anderain Lovelace
	2013-10-18
	anderain.devleop@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "cond.h"
#include "data.h"

#define BUFFER_SIZE 512

// return count of DAY_DATA
int analysis_date (DAY_DATA **data,const char * line) {
	DAY_DATA		d[7];
	int				used[7] = {0,0,0,0,0,0,0};
	int				count,i;
	unsigned char	buf[64],*p;
	int				index,cindex,t;
	while (*line) {

		// skip "双" && "周" && "单" 
		do {
			buf[0] = *line++;
			buf[1] = *line++;
			buf[2] = '\0';
		}
		while(strcmp(buf,"双")==0 || strcmp(buf,"周")==0 || strcmp(buf,"单")==0);

		if		(strcmp(buf,"一")==0)	index = 1;
		else if	(strcmp(buf,"二")==0)	index = 2;
		else if	(strcmp(buf,"三")==0)	index = 3;
		else if	(strcmp(buf,"四")==0)	index = 4;
		else if	(strcmp(buf,"五")==0)	index = 5;
		else if	(strcmp(buf,"六")==0)	index = 6;
		else if	(strcmp(buf,"七")==0)	index = 7;
		// get <num>
		p = buf;while(isdigit(*line)) *p++ = *line++;*p = 0;cindex = atol(buf);
		// skip <,>
		line++;
		// skip <num>
		while(isdigit(*line)) line++;
		// skip things in "((...))"
		if (*line=='(') { 
			t = 1;
			line++;
			while (t){
				if		(*line=='(') t++;
				else if	(*line==')') t--;
				line++;
			}
		}
		if (*line==';') line++; // skip ';'
		
		if (! used[index-1]){
			d[index-1].index		= index;
			d[index-1].size			= 2;
			d[index-1].start		= cindex;
			used[index-1] = 1;
		}
		else {
			d[index-1].size			+= 2;
		}

	}
	for (count = 0,i=0;i<7;++i) count += used[i];
	(*data) = (DAY_DATA*)calloc(sizeof(DAY_DATA),count);

	for (t=0,i=0;i<7;++i) {
		if (used[i]) {
			memcpy((*data)+t,d+i,sizeof(DAY_DATA));
			++t;
		}
	}

	return count;
}

const char * pline;

char * get_line_token (const char * p,char * token)
{
	unsigned char *	t = token;
	unsigned char	c;
	int				in_str = 0;

	if (p != NULL) {
		pline = p;
	}

	while(isspace(*pline)) ++pline;

	while(1) {
		c=*pline++;
		if (in_str) {
			if (c=='"')	in_str = 0;
			else		*t++ = c;
		}
		else{
			if (c=='"')	in_str = 1;
			else if (c == ',' || isspace(c) || c=='\0')
				break;
			else		*t++ = c;
		}
	}
	*t = '\0';
	return token;
}

int load_data (LIST * list,const char * file_name) {
	FILE			*data_file;
	char			buf[BUFFER_SIZE];
	char			token[BUFFER_SIZE];
	COURSE_DATA		*c_data;
	DAY_DATA		*d_data;

	// open file
	data_file = fopen(file_name,"r");
	if (data_file==NULL)
		return LOAD_CANNOTOPEN;
	
	// get lines
	while (!feof(data_file)) {
		fgets(buf,BUFFER_SIZE,data_file);
		
		c_data	= (COURSE_DATA*)calloc(sizeof(COURSE_DATA),1);
		
		c_data->id			= strdup(get_line_token(buf ,token));	/* get num id		*/
		c_data->name		= strdup(get_line_token(NULL,token));	/* get course name	*/
		c_data->school		= strdup(get_line_token(NULL,token));	/* get school		*/
		c_data->teacher		= strdup(get_line_token(NULL,token));	/* get teacher name	*/
		c_data->day_count	= analysis_date(&d_data,get_line_token(NULL,token));		/* get date data	*/
		c_data->day_data	= d_data;
		
		if (cond_check(c_data))
		{
			free(c_data->id);
			free(c_data->name);
			free(c_data->school);
			free(c_data->teacher);
			free(c_data->day_data);
			free(c_data);
			continue;
		}
		else
			list_push(list,c_data);
	}
	// print data

	/*{
		NODE * n = list->head;
		int i,c;
		while (n) {

			printf("COURSE ID=<%s>\n",((COURSE_DATA*)(n->pdata))->id);
			printf("\t name      = %s\n",((COURSE_DATA*)(n->pdata))->name);
			printf("\t school    = %s\n",((COURSE_DATA*)(n->pdata))->school);
			printf("\t teacher   = %s\n",((COURSE_DATA*)(n->pdata))->teacher);
			printf("\t day count = %d\n",(c = ((COURSE_DATA*)(n->pdata))->day_count));
			for (i=0;i<c;++i) {
				printf("\t\t\t index = %d\n",((COURSE_DATA*)(n->pdata))->day_data[i].index);
				printf("\t\t\t\t start = %d\n",((COURSE_DATA*)(n->pdata))->day_data[i].start);
				printf("\t\t\t\t size  = %d\n",((COURSE_DATA*)(n->pdata))->day_data[i].size);
			}
			n=n->next;
		}
	}*/

	fclose(data_file);

	return LOAD_SUCCESS;
}