#include "config.h"

#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <signal.h>

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* No warning for bzero */
#include <string.h>
#include <strings.h>

/* For config file operations */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* For setpriority */
#include <sys/time.h>
#include <sys/resource.h>

/* Required for getpwuid */
#include <pwd.h>

#include "common.h"
#include "sysinfo.h"
#include "security.h"
#include "zabbix_agent.h"

#include "log.h"
#include "cfg.h"
#include "stats.h"

#define	LISTENQ 1024

/*
char    *interface;
int     clock[60*15];
int     sent[60*15];
int     received[60*15];
int     sent_load1;
int     sent_load5;
int     sent_load15;
int     received_total;
int     received_load1;
int     received_load5;
int     received_load15;
*/


INTERFACE interfaces[128]=
{
	{0}
};

void	init_stat()
{
	FILE	*file;
	char	*s;
	char	line[MAX_STRING_LEN+1];
	char	interface[MAX_STRING_LEN+1];
	int	i,j,j1;

	for(i=0;i<128;i++)
	{
		interfaces[i].interface=0;
		for(j=0;j<60*15;j++)
		{
			interfaces[i].clock[j]=0;
		}
	}

	file=fopen("/proc/net/dev","r");
	if(NULL == file)
	{
		fprintf(stderr, "Cannot open config file [%s] [%m]\n","/proc/net/dev");
		return;
	}
	i=0;
	while(fgets(line,MAX_STRING_LEN,file) != NULL)
	{
		if( (s=strstr(line,":")) == NULL)
			continue;
		strncpy(interface,line,s-line);
		interface[s-line]=0;
		j1=0;
		for(j=0;j<strlen(interface);j++)
		{
			if(interface[j]!=' ')
			{
				interface[j1++]=interface[j];
			}
		}
		interface[j1]=0;
		interfaces[i].interface=strdup(interface);
		i++;
	}

	fclose(file);
}

void	report_stat(int now)
{
	FILE	*file;

	int	time=0,
		time1=0,
		time5=0,
		time15=0,
		sent=0,
		sent1=0,
		sent5=0,
		sent15=0,
		received=0,
		received1=0,
		received5=0,
		received15=0;

	int	i,j;

	file=fopen("/tmp/zabbix_agentd.tmp","w");
	if(NULL == file)
	{
		fprintf(stderr, "Cannot file [%s] [%m]\n","/proc/net/dev");
		return;
	}

	for(i=0;i<128;i++)
	{
		if(interfaces[i].interface==0)
		{
			break;
		}
/*		printf("IF [%s]\n",interfaces[i].interface);*/
		sent=0;sent1=0;received1=0;
		received=0;sent5=0;received5=0;
		sent15=0;received15=0;

		time=now+1;
		time1=now+1;
		time5=now+1;
		time15=now+1;
		for(j=0;j<60*15;j++)
		{
			if(interfaces[i].clock[j]==0)
			{
				continue;
			}
			if(interfaces[i].clock[j]==now)
			{
				continue;
			}
			if((interfaces[i].clock[j] >= now-60) && (time1 > interfaces[i].clock[j]))
			{
				time1=interfaces[i].clock[j];
			}
			if((interfaces[i].clock[j] >= now-5*60) && (time5 > interfaces[i].clock[j]))
			{
				time5=interfaces[i].clock[j];
			}
			if((interfaces[i].clock[j] >= now-15*60) && (time15 > interfaces[i].clock[j]))
			{
				time15=interfaces[i].clock[j];
			}
		}
		for(j=0;j<60*15;j++)
		{
			if(interfaces[i].clock[j]==now)
			{
				sent=interfaces[i].sent[j];
				received=interfaces[i].received[j];
			}
			if(interfaces[i].clock[j]==time1)
			{
				sent1=interfaces[i].sent[j];
				received1=interfaces[i].received[j];
			}
			if(interfaces[i].clock[j]==time5)
			{
				sent5=interfaces[i].sent[j];
				received5=interfaces[i].received[j];
			}
			if(interfaces[i].clock[j]==time15)
			{
				sent15=interfaces[i].sent[j];
				received15=interfaces[i].received[j];
			}
		}
		if((sent!=0)&&(sent1!=0))
		{
			fprintf(file,"netloadout1[%s] %f\n", interfaces[i].interface, (float)((sent-sent1)/(now-time1)));
		}
		else
		{
			fprintf(file,"netloadout1[%s] 0\n", interfaces[i].interface);
		}
		if((sent!=0)&&(sent5!=0))
		{
			fprintf(file,"netloadout5[%s] %f\n", interfaces[i].interface, (float)((sent-sent5)/(now-time5)));
		}
		else
		{
			fprintf(file,"netloadout5[%s] 0\n", interfaces[i].interface);
		}
		if((sent!=0)&&(sent15!=0))
		{
			fprintf(file,"netloadout15[%s] %f\n", interfaces[i].interface, (float)((sent-sent15)/(now-time15)));
		}
		else
		{
			fprintf(file,"netloadout15[%s] 0\n", interfaces[i].interface);
		}
		if((received!=0)&&(received1!=0))
		{
			fprintf(file,"netloadin1[%s] %f\n", interfaces[i].interface, (float)((received-received1)/(now-time1)));
		}
		else
		{
			fprintf(file,"netloadin1[%s] 0\n", interfaces[i].interface);
		}
		if((received!=0)&&(received5!=0))
		{
			fprintf(file,"netloadin5[%s] %f\n", interfaces[i].interface, (float)((received-received5)/(now-time5)));
		}
		else
		{
			fprintf(file,"netloadin5[%s] 0\n", interfaces[i].interface);
		}
		if((received!=0)&&(received15!=0))
		{
			fprintf(file,"netloadin15[%s] %f\n", interfaces[i].interface, (float)((received-received15)/(now-time15)));
		}
		else
		{
			fprintf(file,"netloadin15[%s] 0\n", interfaces[i].interface);
		}
	}

	fclose(file);
}


void	add_values(int now,char *interface,int value_sent,int value_received)
{
	int i,j;

/*	printf("Add_values [%s] [%d] [%d]\n",interface,value_sent,value_received);*/

	for(i=0;i<128;i++)
	{
		if(0 == strcmp(interfaces[i].interface,interface))
		{
			for(j=0;j<15*60;j++)
			{
				if(interfaces[i].clock[j]<now-15*60)
				{
					interfaces[i].clock[j]=now;
					interfaces[i].sent[j]=value_sent;
					interfaces[i].received[j]=value_received;
					break;
				}
			}
			break;
		}
	}
}

void	collect_stat()
{
	char	*s;
	char	line[MAX_STRING_LEN+1];
	int	i,j;
	int	i1,j1;
	char	a[MAX_STRING_LEN+1];
	char	b[MAX_STRING_LEN+1];
	char	*token;
	char	interface[MAX_STRING_LEN+1];
	FILE	*file;
	int	now;
	int	received,sent;

	/* Must be static */
	static	int initialised=0;

	if( 0 == initialised)
	{
		init_stat();
		initialised=1;
	}

	now=time(NULL);

	file=fopen("/proc/net/dev","r");
	if(NULL == file)
	{
		fprintf(stderr, "Cannot open config file [%s] [%m]\n","/proc/net/dev");
		return;
	}


	i=0;
	while(fgets(line,MAX_STRING_LEN,file) != NULL)
	{
		if( (s=strstr(line,":")) == NULL)
			continue;
		strncpy(interface,line,s-line);
		interface[s-line]=0;
		j1=0;
		for(i1=0;i1<strlen(interface);i1++)
		{
			if(interface[i1]!=' ')
			{
				interface[j1++]=interface[i1];
			}
		}
		interface[j1]=0;
		s=strtok(line,":");
		j=0;
		while(s)
		{
			s = strtok(NULL," ");
			if(j==0)
			{
/*				printf("Received [%s]\n",s);*/
				received=atoi(s);
			}
			else if(j==8)
			{
/*				printf("Sent [%s]\n",s);*/
				sent=atoi(s);
				add_values(now,interface,sent,received);
			}
			j++;
		}
		i++;
	}
	fclose(file);
	report_stat(now);
}

void	collect_statistics()
{
	for(;;)
	{
		collect_stat();
		sleep(1);
	}
}
