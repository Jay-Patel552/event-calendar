#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

struct email_details {
    char e_type[2];
    char title[11];
    char date[11];
    char time[6];
    char location[50];
    int validity; // -1 = unchecked state, 0= invalid, 1= valid email
};

char** input_email(char** all_emails,int* n);
int email_title_valid(char* token,int a);
int email_date_valid(char *token,int a);
int email_time_valid(char *token, int a);
int email_location_valid(char* token,int a);
struct email_details parsing(char * email_str,struct email_details email);
void remove_substring(char* string1, char* substring);

char** input_email(char** all_emails,int* n){
    int i=0;
    while(1){
        //printf("Subject:\n");
        char email_temp[100];
        if(fgets(email_temp,100,stdin)!=NULL){
        	*n=(*n)+1;
        	char sub[]="Subject: ";
        	remove_substring(email_temp,sub);
        	all_emails=realloc(all_emails,sizeof(char*) * (*n));
        	all_emails[i]=(char*) malloc(sizeof(email_temp)*sizeof(char));
        	strcpy(all_emails[i],email_temp);
        	i++;
        }
        else
        	break;
    }   
    return all_emails;
}

struct email_details parsing(char * email_str,struct email_details email){
	char str2[100];
	strcpy(str2,email_str);
	char delim[] = ",";
	char *ptr = strtok(email_str, delim);
    	int c=0;
	while(ptr != NULL){
	ptr = strtok(NULL, delim);
	c++;
	}
	if(c!=5){
	        email.validity=0;
	        return email;
	}
	char *arr[c];
	char *ptr2=strtok(str2,delim);
	arr[0]=ptr2;
	int i=1;
	while(ptr2 != NULL){
	ptr2 = strtok(NULL, delim);
	arr[i]=ptr2;
	i++;	
	}
    	if(arr[0][1]=='\0' && (arr[0][0]=='C'|| arr[0][0]=='D' || arr[0][0]=='X')){
    	    strcpy(email.e_type,arr[0]);
    	}
    	else{
    	    email.validity=0;
    	    return email;
    	}
    	for(int x=1;x<c;x++){
    	    //printf("%s %d\n",arr[x],x);
    	    if(x==1){
    	        int a=0;
    	        a=email_title_valid(arr[x],a);
    	        if(a==1){
    	            strcpy(email.title,arr[x]);
    	        }
    	        else{
    	            email.validity=0;
    	            return email;
    	        }
    	    }
    	    if(x==2){
    	        int a=0;
    	        char temp[50];
    	        strcpy(temp,arr[x]);
    	        a=email_date_valid(temp,a);
    	        if(a==1){
    	            strcpy(email.date,arr[x]);
    	        }
    	        else{
    	            email.validity=0;
    	            return email;
    	        }
    	    }
    	    if(x==3){
    	        int a=0;
    	        char token_temp[20];
    	        strcpy(token_temp,arr[x]);
    	        a=email_time_valid(token_temp,a);
    	        if(a==1){
    	            strcpy(email.time,arr[x]);
    	        }
    	        else{
    	            email.validity=0;
    	            return email;
    	        }
    	    }
    	    if(x==4){
    	    	int a=0;
    	        a=email_location_valid(arr[x],a);
    	        if(a==1){
    	            strcpy(email.location,arr[x]);
    	        }
    	        else{
    	            email.validity=0;
    	            return email;
    	        }
    	    }
    	}
    	email.validity=1;
    	return email;
}

void remove_substring(char* string1, char* substring){
	int len=strlen(substring);
	if(len>0){
		char *p = string1;
        	while ((p = strstr(p, substring)) != NULL){
            	memmove(p, p + len, strlen(p)-len + 1);
        	}
        }
        return;
}


int email_title_valid(char* token,int a){
    if(strlen(token)>10)
        return 0;
    else
        return 1;
}

int email_date_valid(char *token,int a){
    if(token[2]!='/' && token[5]!='/' && strlen(token)>10 && strlen(token)<7){
        return 0;
    }
    for(int i=0;i<strlen(token);i++){
        if(isdigit(token[i])>0 || token[i]=='/'){
            if(token[i]=='/' && !(i==2 || i==5)){
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    char* token2;
    char delim3[2]="/";
    char MM[3];
    char DD[3];
    char yyyy[5];
    token2=strtok(token,delim3);
    strcpy(MM,token2);
    int month=atoi(MM);
    int day,year;
    int c=1;
    while(token2!=NULL && c<3){
        token2=strtok(NULL,delim3);
        if(c==1){
            strcpy(DD,token2);
            day=atoi(DD);
        }
        else{
            strcpy(yyyy,token2);
            year=atoi(yyyy);
        }
        c++;
    }

    if(month>12){
        return 0;
    }
    int leap=0;
    if(year%4==0){
        leap=1;
        if(year%200==0){
            leap=0;
        }
        if(year%400==0){
            leap=1;
        }
    }
    if(leap==1){
        if(month==1 && (day>31 || day<1))
            return 0;
        if(month==2 && (day>29 || day<1))
            return 0;
        if(month==3 && (day>31 || day<1))
            return 0;
        if(month==4 && (day>30 || day<1))
            return 0;
        if(month==5 && (day>31 || day<1))
            return 0;
        if(month==6 && (day>30 || day<1))
            return 0;
        if(month==7 && (day>31 || day<1))
            return 0;
        if(month==8 && (day>31 || day<1))
            return 0;
        if(month==9 && (day>30 || day<1))
            return 0;
        if(month==10 && (day>31 || day<1))
            return 0;
        if(month==11 && (day>30 || day<1))
            return 0;
        if(month==12 && (day>31 || day<1))
            return 0;
    }
    if(leap==0){
        if(month==1 && (day>31 || day<1))
            return 0;
        if(month==2 && (day>28 || day<1))
            return 0;
        if(month==3 && (day>31 || day<1))
            return 0;
        if(month==4 && (day>30 || day<1))
            return 0;
        if(month==5 && (day>31 || day<1))
            return 0;
        if(month==6 && (day>30 || day<1))
            return 0;
        if(month==7 && (day>31 || day<1))
            return 0;
        if(month==8 && (day>31 || day<1))
            return 0;
        if(month==9 && (day>30 || day<1))
            return 0;
        if(month==10 && (day>31 || day<1))
            return 0;
        if(month==11 && (day>30 || day<1))
            return 0;
        if(month==12 && (day>31 || day<1))
            return 0;
    }
    return 1;
}

int email_time_valid(char *token, int a){
     if(strlen(token)!=5)
        return 0;
     for(int i=0;i<strlen(token);i++){
        if(i==2){
            if(token[i]!=':')
                return 0;
        }
        else{
            if(!isdigit(token[i]))
                return 0;
        }
     }
     char HH[3];
     char* tko;
     char delimx[2]=":";
     tko=strtok(token,delimx);
     strcpy(HH,tko);
     int hour=atoi(HH);
     tko=strtok(NULL,delimx);
     int min=atoi(tko);
     //printf("%d %d",hour,min);
     if(hour<0 || hour>23 || min<0 || min>59)
        return 0;
     return 1;
}

int email_location_valid(char* token,int a){
    if(strlen(token)>12){
        return 0;
    }
    else{
        return 1;
    }
}


int main(){
    int n=0;
    char** all_emails=(char**) malloc(sizeof(char*));
    all_emails=input_email(all_emails,&n);
    for(int i=0;i<n;i++){
        struct email_details email;
        email.validity=-1;
        char str_temp[100];
        strcpy(str_temp,all_emails[i]);
        email=parsing(str_temp,email);
        if(email.validity==1){
            printf("%s",all_emails[i]);
        }
    }
    return 0;
}
