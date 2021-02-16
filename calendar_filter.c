#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

struct event_details {												//structure represents event details
    char e_type[2];
    char title[11];
    char date[11];
    char time[6];
    char location[50];
};



struct event{													//structure for linked list of events on one day 
	struct event_details event_element;
	struct event* next_event;	
};
struct calendar_date{												//structure for linked list of calendar dates
	char date[11];
	int no_of_events;
	struct event * ptr_first_event;
	struct calendar_date* next_date;	
};

extern struct calendar_date * calendar_first;									
extern int total_dates;
void print_structure(struct event_details event);
struct event_details parsing(char * event_str,struct event_details event);
void calendar_update();
struct calendar_date* check_date_available(char* date);
struct calendar_date* insert_date(char* date);
int compare_dates(char* date1, char* date2);
int insert_event(struct calendar_date* go_to_date, struct event_details event_temp);
int reorder_event(struct calendar_date* go_to_date);
void print_calendar();

struct calendar_date * calendar_first=NULL;
int total_dates=0;
int main(){
	int n=0;	
	calendar_update();
	//print_calendar();
	return 0;
}


void calendar_update(){
   int i=0;
    while(1){
        char event_str[100];
        struct event_details event_temp;
        
        if(fgets(event_str,100,stdin)!=NULL){
        	event_temp=parsing(event_str,event_temp);								//parsing a given event string
        	//printf("%s\n",event_temp.title);
        	if(event_temp.e_type[0]=='C'){									//create event
     			struct calendar_date* go_to_date=check_date_available(event_temp.date);
     			if(go_to_date!=NULL){
     				insert_event(go_to_date,event_temp);
     			}
     			else{
     				struct calendar_date* new_date=insert_date(event_temp.date);
     				insert_event(new_date,event_temp);
     			}
     		}
     		
     		
     		
        	if(event_temp.e_type[0]=='X'){									//update event
       		struct calendar_date* go_to_date=check_date_available(event_temp.date);
       		if(go_to_date!=NULL){
       			struct event* event_short=go_to_date->ptr_first_event;
       			int k=0;
       			char earliest_time[6];
       			char earliest_location[50];
       			while(event_short!=NULL){
       				if(strcmp(event_short->event_element.title,event_temp.title)==0){
       					strcpy(earliest_time,go_to_date->ptr_first_event->event_element.time);
       					strcpy(earliest_location,go_to_date->ptr_first_event->event_element.location);
       					strcpy(event_short->event_element.time,event_temp.time);
       					strcpy(event_short->event_element.location,event_temp.location);
       					reorder_event(go_to_date);
       					k=1;
       					break;
       				}
       				else{
       					event_short=event_short->next_event;
       				}
       			}
       			if(k==0){
       				printf("no such event found\n");
       			}
       			else{
       				int y=strcmp(earliest_time,go_to_date->ptr_first_event->event_element.time);
       				int a=strcmp(earliest_location,go_to_date->ptr_first_event->event_element.location);
       				if(y!=0 || a!=0){
       					print_structure(event_temp);
       				}
       			}
       		}
       		else{
       			printf("no such event\n");
       		}
       	}
       	
       	
       	
        	if(event_temp.e_type[0]=='D'){									//delete event
        		struct calendar_date* go_to_date=check_date_available(event_temp.date);
       		if(go_to_date!=NULL){
       			struct event* event_short=go_to_date->ptr_first_event;
       			struct event* event_short_copy=go_to_date->ptr_first_event;
       			int k=0;
       			int count=0;
       			while(event_short!=NULL){
       				if(strcmp(event_short->event_element.title,event_temp.title)==0 && strcmp(event_short->event_element.time,event_temp.time)==0 && strcmp(event_short->event_element.location,event_temp.location)==0){
       					if(count==0){
       						go_to_date->ptr_first_event=event_short->next_event;
       						free(event_short);
       						go_to_date->no_of_events--;
       						if(go_to_date->no_of_events!=0){
       							print_structure(go_to_date->ptr_first_event->event_element);
       						}
       						else{
       							strcpy(event_temp.time,"--:--");
       							strcpy(event_temp.location,"NA\n");
       							print_structure(event_temp);
       						}
       					}
       					
       					else{
       						event_short_copy->next_event=event_short->next_event;
       						free(event_short);
       						go_to_date->no_of_events--;
       					}
       					k=1;
       					break;
       				}
       				else{
       					if(count!=0){
       						event_short_copy=event_short_copy->next_event;
       					}
       					event_short=event_short->next_event;
       					count++;
       				}
       			}
       			if(k==0){
       				printf("no such event found\n");
       			}
       		}
       		else{
       			printf("no such event\n");
       		}
        	}
        }
        else
        	break;
    }   
    return;
}

struct event_details parsing(char * event_str,struct event_details event){
	char *token;
	char delim[]=",";
	token=strtok(event_str,delim);
	strcpy(event.e_type,token);
	int i=0;
	while(token!=NULL){
		token=strtok(NULL,delim);
		i++;
		if(i==1){
			strcpy(event.title,token);
		}
		if(i==2){
			strcpy(event.date,token);
		}
		if(i==3){
			strcpy(event.time,token);
		}
		if(i==4){
			strcpy(event.location,token);
		}		
	}
	return event;	
}


struct calendar_date* check_date_available(char* date){
	if(calendar_first==NULL){
		//printf("1\n");
		return NULL;
	}
	else{
		struct calendar_date* calendar_first_copy=calendar_first;
		while(calendar_first_copy!=NULL){
			if(strcmp(calendar_first_copy->date,date)==0){
				return calendar_first_copy;
			}
			else{
				calendar_first_copy=calendar_first_copy->next_date;
			}
		}
	
	}
	return NULL;	
}

struct calendar_date* insert_date(char* date){
	struct calendar_date* copy1=calendar_first;
	int p=0;
	struct calendar_date* copy2=calendar_first;
	//printf("2\n");
	if(calendar_first==NULL){
		struct calendar_date* new_date=(struct calendar_date*)malloc(sizeof(struct calendar_date));
		strcpy(new_date->date,date);
		new_date->no_of_events=0;
		new_date->ptr_first_event=NULL;
		new_date->next_date=calendar_first;
		calendar_first=new_date;
		total_dates++;
		return new_date;
	}
	while(copy1!=NULL){
		char date_copy[11];
		strcpy(date_copy,date);
		char date_copy_other[11];
		strcpy(date_copy_other,copy1->date);
		int y=compare_dates(date_copy_other,date_copy);
		//printf("y=%d\n",y);
		if(y==2){
			if(p!=0){
				copy2=copy2->next_date;
			}
			copy1=copy1->next_date;
			p++;
			if(p==total_dates){
				struct calendar_date* new_date=(struct calendar_date*)malloc(sizeof(struct calendar_date));
				strcpy(new_date->date,date);
				new_date->no_of_events=0;
				new_date->ptr_first_event=NULL;
				new_date->next_date=copy2->next_date;
				copy2->next_date=new_date;
				total_dates++;
				return new_date;
			}
		}
		if(y==1){
			if(p==0){
				struct calendar_date* new_date=(struct calendar_date*)malloc(sizeof(struct calendar_date));
				strcpy(new_date->date,date);
				new_date->no_of_events=0;
				new_date->ptr_first_event=NULL;
				new_date->next_date=calendar_first;
				calendar_first=new_date;
				total_dates++;
				return new_date;
			}
			else{
				struct calendar_date* new_date=(struct calendar_date*)malloc(sizeof(struct calendar_date));
				strcpy(new_date->date,date);
				new_date->no_of_events=0;
				new_date->ptr_first_event=NULL;
				new_date->next_date=copy2->next_date;
				copy2->next_date=new_date;
				total_dates++;
				return new_date;
			}
		}
	}
	return NULL;
}

int compare_dates(char* date1, char* date2){// if date 1 is larger output is 1 if date 2 is larger 2 is the output
	int date1_arr[3];
	int date2_arr[3];
	char *p1;
	char *p2;
	p1=strtok(date1,"/");
	char pcopy[11];
	int x=0;
	while(p1!=NULL){
		strcpy(pcopy,p1);
		date1_arr[x]=atoi(pcopy);
		x++;
		p1=strtok(NULL,"/");
	}
	//printf("aaaaaaaa %d\n",date1_arr[2]);
	p2=strtok(date2,"/");
	x=0;
	while(p2!=NULL){
		strcpy(pcopy,p2);
		date2_arr[x]=atoi(pcopy);
		x++;
		p2=strtok(NULL,"/");
	}
	if(date1_arr[2]>date2_arr[2]){
		return 1;
	}
	if(date1_arr[2]<date2_arr[2]){
		return 2;
	}
	else{
		if(date1_arr[0]>date2_arr[0]){
			return 1;
		}
		if(date1_arr[0]<date2_arr[0]){
			return 2;
		}
		else{
			if(date1_arr[1]>date2_arr[1]){
				return 1;
			}
			if(date1_arr[1]<date2_arr[1]){
				return 2;
			}
			else{
				return -20;
			}
		}
	}
}

int insert_event(struct calendar_date* go_to_date, struct event_details event_temp){
	struct event* new_event=(struct event*)malloc(sizeof(struct event));
	strcpy(new_event->event_element.title,event_temp.title);
	strcpy(new_event->event_element.date,event_temp.date);
	strcpy(new_event->event_element.time,event_temp.time);
	strcpy(new_event->event_element.location,event_temp.location);
	strcpy(new_event->event_element.e_type,event_temp.e_type);
	if(go_to_date->ptr_first_event!=NULL){
		int y=strcmp(new_event->event_element.time,go_to_date->ptr_first_event->event_element.time);
		if(y<0){
			print_structure(new_event->event_element);//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	else{
		print_structure(new_event->event_element);
	}
	new_event->next_event=go_to_date->ptr_first_event;
	go_to_date->ptr_first_event=new_event;
	go_to_date->no_of_events++;
	reorder_event(go_to_date);
	return 1;
}

int reorder_event(struct calendar_date* go_to_date){
	struct event* copy1=go_to_date->ptr_first_event;
	struct event* copy2=go_to_date->ptr_first_event;
	while(copy1!=NULL){
		copy2=go_to_date->ptr_first_event;
		while(copy2!=NULL){
			int x=strcmp(copy1->event_element.time,copy2->event_element.time);
			if(x<0){
				struct event_details* event_short=(struct event_details*)malloc(sizeof(struct event_details));
				strcpy(event_short->title,copy2->event_element.title);
				strcpy(event_short->date,copy2->event_element.date);
				strcpy(event_short->time,copy2->event_element.time);
				strcpy(event_short->location,copy2->event_element.location);
				
				strcpy(copy2->event_element.title,copy1->event_element.title);
				strcpy(copy2->event_element.date,copy1->event_element.date);
				strcpy(copy2->event_element.time,copy1->event_element.time);
				strcpy(copy2->event_element.location,copy1->event_element.location);
				
				strcpy(copy1->event_element.title,event_short->title);
				strcpy(copy1->event_element.date,event_short->date);
				strcpy(copy1->event_element.time,event_short->time);
				strcpy(copy1->event_element.location,event_short->location);
			}
			copy2=copy2->next_event;
		}
		copy1=copy1->next_event;
	}
	return 1;
}

void print_structure(struct event_details event){
	char calendar_event[100]="";
	char com[]=",";
	strcat(calendar_event,event.date);
	strcat(calendar_event,com);
	strcat(calendar_event,event.time);
	strcat(calendar_event,com);
	strcat(calendar_event,event.location);
	printf("%s", calendar_event);
}

void print_calendar(){
	if(calendar_first==NULL){
		printf("no events in the calendar");
	}
	else{
		struct calendar_date* copy=calendar_first;
		while(copy!=NULL){
			printf("%s :\n",copy->date);
			struct event* copy2=copy->ptr_first_event;
			while(copy2!=NULL){
				printf("\t %s \n",copy2->event_element.time);
				copy2=copy2->next_event;
			}
			copy=copy->next_date;
		}
	}
	return;
}
