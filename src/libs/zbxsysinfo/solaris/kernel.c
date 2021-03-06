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

#include "common.h"

#include "sysinfo.h"


int	KERNEL_MAXPROC(const char *cmd, const char *param, unsigned flags, AGENT_RESULT *result)
{
    int ret = SYSINFO_RET_FAIL;
    kstat_ctl_t *kc;
    kstat_t	*kt;
    struct var	*v;
    
	assert(result);

	init_result(result);	

	kc = kstat_open();
	if(kc)
	{
		kt = kstat_lookup(kc, "unix", 0, "var");
		if(kt)
		{
			if((kt->ks_type == KSTAT_TYPE_RAW) &&
				(kstat_read(kc, kt, NULL) != -1))
			{
				v = (struct var *) kt->ks_data;

				/* int	v_proc;	    Max processes system wide */
				SET_UI64_RESULT(result, v->v_proc);
				ret = SYSINFO_RET_OK;
			}
		}
		kstat_close(kc);
	}

	return ret;
}

int	KERNEL_MAXFILES(const char *cmd, const char *param, unsigned flags, AGENT_RESULT *result)
{
	assert(result);

        init_result(result);	

	return	SYSINFO_RET_FAIL;
}

int     OLD_KERNEL(const char *cmd, const char *param, unsigned flags, AGENT_RESULT *result)
{
	char    key[MAX_STRING_LEN];
        int     ret;

        assert(result);

        init_result(result);	

        if(num_param(param) > 1)
        {
                return SYSINFO_RET_FAIL;
        }

        if(get_param(param, 1, key, MAX_STRING_LEN) != 0)
        {
                return SYSINFO_RET_FAIL;
        }

        if(strcmp(key,"maxfiles") == 0)
        {
                ret = KERNEL_MAXFILES(cmd, param, flags, result);
        }
        else if(strcmp(key,"maxproc") == 0)
        {
                ret = KERNEL_MAXPROC(cmd, param, flags, result);
        }
        else
        {
                ret = SYSINFO_RET_FAIL;
        }

        return ret;
}

