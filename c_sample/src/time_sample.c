#include "stdio.h"
#include <time.h>
#include <string.h>
#include "sample.h"

void get_time(char *pTime)
{
	memset(pTime, '\0', sizeof(pTime));
	struct tm *tm_ptr;
	time_t the_time;
	time(&the_time);
	tm_ptr = localtime(&the_time);
	sprintf(pTime, "%02d/%02d/%02d,%02d:%02d:%02d", 
		tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, 
		tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);		
}
                                                           
BOOL set_time(char *buffertime)                             
{
	struct tm tm_per;
	time_t time1; 
	int year,month,day,hour,min,sec,flag,len;
	year = month = day = hour = min = sec = flag = len = 0;

	len = strlen(buffertime);
	if(strlen(buffertime) != 19){
		return FALSE;
	}
	
	flag = sscanf(buffertime,"%d:%d:%d,%d:%d:%d", &year, &month, &day, &hour, &min, &sec);

	if(flag != 6){
		flag = -1;
	}else{
		flag = 0;
	}
	
	if(year < 1)
		flag = -1;
	if((month < 1) || (month > 12))
		flag = -1;
	if((day < 1) || (day > 31))
	 	flag = -1;
	if((hour < 0) || (hour > 23))
	 	flag = -1;
	if((min < 0) || (min > 59))
	 	flag = -1;
	if((sec < 0) || (sec > 59))
	 	flag = -1;
	
	if(flag < 0){
		return FALSE;
	}
	
	tm_per.tm_year 	= year-1900;
	tm_per.tm_mon	= month-1;
	tm_per.tm_mday	= day;
	tm_per.tm_hour	= hour;
	tm_per.tm_min	= min;
	tm_per.tm_sec	= sec;	
	
	time1=mktime(&tm_per);
	printf("mktime=%ld\n",time1);
	if(time1 > 0){
		if(stime(&time1)==0){
			return TRUE;
		}else{
			return FALSE;
		}
	}else{
		return FALSE;
	}
}      


#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	char pTime[MAX_SHORT_LENGTH];
	
	get_time(pTime);
	set_time(pTime);
	
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

