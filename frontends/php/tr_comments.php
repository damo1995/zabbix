<?php
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
?>
<?php
	require_once "include/config.inc.php";
	require_once "include/triggers.inc.php";
	require_once "include/forms.inc.php";

	$page["title"] = "S_TRIGGER_COMMENTS";
	$page["file"] = "tr_comments.php";

include_once "include/page_header.php";

?>
<?php

//		VAR			TYPE	OPTIONAL FLAGS	VALIDATION	EXCEPTION
	$fields=array(
		"triggerid"=>	array(T_ZBX_INT, O_MAND, P_SYS,	DB_ID, null),
		"comments"=>	array(T_ZBX_STR, O_OPT,  null,	null, 'isset({save})'),

/* actions */
		"save"=>		array(T_ZBX_STR, O_OPT, P_SYS|P_ACT,	null,	null),
		"cancel"=>		array(T_ZBX_STR, O_OPT, P_SYS|P_ACT,	null,	null),
/* other */
/*
		"form"=>		array(T_ZBX_STR, O_OPT, P_SYS,	null,	null),
		"form_copy_to"=>	array(T_ZBX_STR, O_OPT, P_SYS,	null,	null),
		"form_refresh"=>	array(T_ZBX_INT, O_OPT,	null,	null,	null)
*/
	);

	check_fields($fields);
?>
<?php

	$sql = 'SELECT * '.
			' FROM items i, functions f '.
			' WHERE i.itemid=f.itemid '.
				' AND f.triggerid='.$_REQUEST['triggerid'].
				' AND '.DBin_node('f.triggerid');				
	if(!$db_data = DBfetch(DBselect($sql))){
		fatal_error(S_NO_TRIGGER_DEFINED);
	}
	
	$available_triggers = get_accessible_triggers(PERM_READ_ONLY, array($db_data['hostid']));
	
	if(!isset($available_triggers[$_REQUEST['triggerid']])){
		access_deny();
	}
	
	$trigger_hostid = $db_data['hostid'];
	
	if(isset($_REQUEST["save"])){
	
		$result = update_trigger_comments($_REQUEST["triggerid"],$_REQUEST["comments"]);
		show_messages($result, S_COMMENT_UPDATED, S_CANNOT_UPDATE_COMMENT);

		if($result){
			add_audit(AUDIT_ACTION_UPDATE,AUDIT_RESOURCE_TRIGGER,
				S_TRIGGER." [".$_REQUEST["triggerid"]."] [".expand_trigger_description($_REQUEST["triggerid"])."] ".
				S_COMMENTS." [".$_REQUEST["comments"]."]");
		}
	}
	else if(isset($_REQUEST["cancel"]))
	{
		redirect('tr_status.php?hostid='.$trigger_hostid);
		exit;
		
	}
?>
<?php
	show_table_header(S_TRIGGER_COMMENTS_BIG);
	echo SBR;
	insert_trigger_comment_form($_REQUEST["triggerid"]);
?>
<?php

include_once "include/page_footer.php";

?>
