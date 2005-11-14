/* 
** ZABBIX
** Copyright (C) 2000-2005 SIA Zabbix
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**/

#include "config.h"

#include "common.h"
#include "sysinfo.h"

int	SYSTEM_UPTIME(const char *cmd, const char *param, unsigned flags, AGENT_RESULT *result)
{
	int	mib[2];
        size_t	len;
	struct timeval	uptime;
	int	now;
	int	ret = SYSINFO_RET_FAIL;

	assert(result);
	init_result(result);

	mib[0]=CTL_KERN;
	mib[1]=KERN_BOOTTIME;

	len=sizeof(uptime);

	if(sysctl(mib,2,&uptime,(size_t *)&len,NULL,0) == 0)
	{
		now=time(NULL);

		result->type |= AR_DOUBLE;
		result->dbl = (double)(now-uptime.tv_sec);

		ret = SYSINFO_RET_OK;
	}
	return ret;
}

