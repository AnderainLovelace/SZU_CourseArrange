/*
    by Anderain Lovelace
	2013-10-18
	anderain.devleop@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cond.h"
#include "data.h"

#define COURSE_MAX		64
#define BUFFER_SIZE		256

typedef int BOOL;
#define TRUE	1
#define FALSE	0

char			map[7][12]; //[day][course index]
char 			course_name[COURSE_MAX][BUFFER_SIZE];

COURSE_DATA	*	arranged_course[COURSE_MAX];
int				arranged_size;

LIST			course_list[COURSE_MAX];
int				course_count=0;

int				solution_count;
char			output_file_name[BUFFER_SIZE];
FILE *			file_out;

void draw_map_stdout ()
{
	int i,j;
	printf("========================\n");
	printf("  ");
	for (i=0;i<7;++i) {
		printf("%3d",i+1);
	}
	printf("\n");
	for (j=0;j<12;++j) {
		if (j==4 || j==8 || j==10)printf("------------------------\n");
		printf("%-3d",j+1);
		for (i=0;i<7;++i) {
			if (map[i][j]==0)
				printf("   ");
			else
				printf("%3d",map[i][j]);
		}
		printf("\n");
	}
	printf("========================\n");
	
	//getchar();
}
void draw_map ()
{
	int i,j;
	for(i=0;i<arranged_size;++i)
		fprintf(file_out,"±àºÅ%d,%s,%s,\t%s<br />\n",i+1,arranged_course[i]->id,arranged_course[i]->name,arranged_course[i]->teacher);

	fputs("<table border=\"1\">",file_out);
	fputs("<tr>",file_out);
	fprintf(file_out,"<td></td>\n",i+1);
	for (i=0;i<5;++i) {
		fprintf(file_out,"<td>ÐÇÆÚ%d</td>\n",i+1);
	}
	fputs("</tr>",file_out);
	for (j=0;j<12;++j) {
		fputs("<tr>",file_out);
		fprintf(file_out,"<td>µÚ%d½Ú</td>\n",j+1);
		for (i=0;i<5;++i) {
			if (map[i][j]==0)
				fprintf(file_out,"<td>&nbsp;</td>",i+1);
			else
				fprintf(file_out,"<td>%3d</td>",map[i][j]/10);
		}
		fputs("</tr>\n",file_out);
	}
	fputs("</table><br />",file_out);
}
void arrange (int deep) {
	NODE *	n = course_list[deep].head;
	int		l = 0;

	for (l=0;n != NULL;n=n->next,++l) {
		// try to fill map
		DAY_DATA		*d;
		COURSE_DATA		*c;
		int				i,j,k,day;
		BOOL			fail = FALSE;
		int				stop_d,stop_k;

		c = (COURSE_DATA*)n->pdata;

		for(i=0;i<c->day_count;++i) { // from day
			d		= &(c->day_data[i]);
			day		= d->index - 1;
			for (j=0,k=d->start-1;j<d->size;++j,++k) {	// from course
				if (map[day][k] != 0) {				// already filled
					fail = TRUE;
					break;
				}
				else {
					map[day][k] = (deep+1)*10+l;
				}
			}
			if (fail){
				stop_d = day;
				stop_k = k;
				break;
			}
		}
		if (fail) {
			// clean up 
			for (i=0;i<c->day_count;++i) {
				d		= &(c->day_data[i]);
				day		= d->index - 1;
				for (j=0,k=d->start-1;j<d->size;++j,++k) {
					if (stop_d==day && stop_k==k)
						goto FAIL_CLEAN_DONE;
					map[day][k] = 0;
				}
			}
			FAIL_CLEAN_DONE:;
		}
		else { // fill complete
			// save course
			arranged_course[arranged_size++] = c;
			//
			// <here we get a SOLUTION>
			//
			if (deep+1>=course_count) {
				solution_count ++;
				draw_map();
			}
			else {
				// try to arrange next course
				arrange(deep+1);
			}
			arranged_size--;
			// clean up
			
			for (i=0;i<c->day_count;++i) {
				d		= &(c->day_data[i]);
				day		= d->index - 1;
				for (j=0,k=d->start-1;j<d->size;++j,++k) {
					map[day][k] = 0;
				}
			}
			// try next
		}
	}
}

void arrange_course() {
	arranged_size = 0;
	// clear map
	memset(map,0,sizeof(map));
	// start
	arrange(0);

	printf("count of solution(s) : %d\n",solution_count);
}

// return TRUE to start
BOOL analysis_arguments (const int argc,const char * argv[])
{
	int		i;
	course_count = 0;
	if (argc<=0) return FALSE;
	// default output file name
	strcpy(output_file_name,"result.html");
	for (i=0;i<argc;++i) {
		// assign output file name
		if (strcmp(argv[i],"-o")==0){
			strcpy(output_file_name,argv[++i]);
		}
		// assign a ignore condition
		else if (strcmp(argv[i],"-i")==0){
			++i;
			if (strcmp(argv[i],"teacher")==0) {
				add_ignore_teacher(argv[++i]);
			}
			else if (strcmp(argv[i],"class")==0){
				add_ignore_class(atol(argv[++i]));
			}
			else if (strcmp(argv[i],"day")==0) {
				add_ignore_day(atol(argv[++i])-1);
			}
		}
		else {
			strcpy(course_name[course_count++],argv[i]);
		}
	}
	return TRUE;
}

int main (int argc,char *argv[]) {

	int				i;
	char			buf[BUFFER_SIZE];

	if (analysis_arguments(argc-1,argv+1))
	{
		for(i=0;i<course_count;++i) {
			sprintf(buf,"%s.csv",course_name[i]);
			list_init(&course_list[i]);
			load_data(&course_list[i],buf);
			
		}
		
		file_out = fopen(output_file_name,"w");
		fputs("<html><head><title>Result table of course arrangement</title></head><body>",file_out);

		arrange_course();

		fputs("</body></html>",file_out);
		fclose(file_out);
	}
	else {
		puts("usage: course_name [course_name]... [-o][-i]...");
		puts("");
		puts("\t-i\t[day <num>][class <num>][teacher <name>]");
		puts("\t-o\t[file_name]");
	}
	return 0;
}