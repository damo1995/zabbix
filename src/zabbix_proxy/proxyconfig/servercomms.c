/* 
** ZABBIX
** Copyright (C) 2000-2006 SIA Zabbix
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

#include "cfg.h"
#include "db.h"
#include "log.h"
#include "zbxjson.h"

#include "comms.h"
#include "servercomms.h"

int	connect_to_server(zbx_sock_t *sock)
{
	int	res;

	zabbix_log(LOG_LEVEL_DEBUG, "In connect_to_server() [%s]:%d",
		CONFIG_SERVER,
		CONFIG_SERVER_PORT);

	if (FAIL == (res = zbx_tcp_connect(sock, CONFIG_SERVER, CONFIG_SERVER_PORT, 0)))
		zabbix_log(LOG_LEVEL_ERR, "Unable connect to the server [%s]:%d [%s]",
			CONFIG_SERVER,
			CONFIG_SERVER_PORT,
			zbx_tcp_strerror());

	return res;
}

static int	send_data_to_server(zbx_sock_t *sock, const char *data)
{
	int	res;

	zabbix_log(LOG_LEVEL_DEBUG, "In send_data_to_server() [%s]",
		data);

	if (FAIL == (res = zbx_tcp_send(sock, data)))
		zabbix_log(LOG_LEVEL_ERR, "Error while sending data to the server [%s]",
			zbx_tcp_strerror());

	return res;
}

static int	recv_data_from_server(zbx_sock_t *sock, char **data)
{
	int	res;

	zabbix_log(LOG_LEVEL_DEBUG, "In recv_data_from_server()");
	if (FAIL == (res = zbx_tcp_recv_ext(sock, data, 0)))
		zabbix_log(LOG_LEVEL_ERR, "Error while receiving answer from server [%s]",
			zbx_tcp_strerror());
	else
		zabbix_log(LOG_LEVEL_DEBUG, "Received [%s] from server",
			*data);

	return res;
}

void	disconnect_server(zbx_sock_t *sock)
{
	zbx_tcp_close(sock);
}

/******************************************************************************
 *                                                                            *
 * Function: get_server_data                                                  *
 *                                                                            *
 * Purpose: get configuration and othed data from server                      *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value: SUCCESS - processed succesfully                              * 
 *               FAIL - an error occured                                      *
 *                                                                            *
 * Author: Alksander Vladishev                                                *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
int	get_data_from_server(zbx_sock_t *sock, const char *name, const char *request, char **data)
{
	int		ret = FAIL;
	struct zbx_json	j;

	zabbix_log (LOG_LEVEL_DEBUG, "In get_server_data() [name:%s] [request:%s]",
		name,
		request);

	zbx_json_init(&j, 128);
	zbx_json_addstring(&j, "request", request, ZBX_JSON_TYPE_STRING);
	zbx_json_addstring(&j, "host", CONFIG_HOSTNAME, ZBX_JSON_TYPE_STRING);

	if (FAIL == send_data_to_server(sock, j.buffer))
		goto exit;

	if (FAIL == recv_data_from_server(sock, data))
		goto exit;

	zabbix_log (LOG_LEVEL_WARNING, "Received %s from server",
		name);

	ret = SUCCEED;
exit:
	zbx_json_free(&j);

	return ret;
}

