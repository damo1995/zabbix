<?php
/* 
** Zabbix
** Copyright (C) 2000,2001,2002,2003,2004 Alexei Vladishev
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
	include "include/config.inc.php";
	$page["title"] = S_SCREENS;
	$page["file"] = "screenconf.php";
	show_header($page["title"],0,0);
	insert_confirm_javascript();
?>

<?php
	show_table_header(S_CONFIGURATION_OF_SCREENS_BIG);
	echo "<br>";
?>

<?php
	if(!check_right("Screen","U",0))
	{
//		show_table_header("<font color=\"AA0000\">No permissions !</font>");
//		show_footer();
//		exit;
	}
?>

<?php
	if(isset($_GET["register"]))
	{
		if($_GET["register"]=="add")
		{
			$result=add_screen($_GET["name"],$_GET["cols"],$_GET["rows"]);
			show_messages($result,S_SCREEN_ADDED,S_CANNOT_ADD_SCREEN);
		}
		if($_GET["register"]=="update")
		{
			$result=update_screen($_GET["screenid"],$_GET["name"],$_GET["cols"],$_GET["rows"]);
			show_messages($result, S_SCREEN_UPDATED, S_CANNOT_UPDATE_SCREEN);
		}
		if($_GET["register"]=="delete")
		{
			$result=delete_screen($_GET["screenid"]);
			show_messages($result, S_SCREEN_DELETED, S_CANNOT_DELETE_SCREEN);
			unset($_GET["screenid"]);
		}
	}
?>

<?php
	show_table_header("SCREENS");
	echo "<TABLE BORDER=0 COLS=4 WIDTH=100% BGCOLOR=\"#CCCCCC\" cellspacing=1 cellpadding=3>";
	echo "<TD WIDTH=5% NOSAVE><B>".S_ID."</B></TD>";
	echo "<TD><B>".S_NAME."</B></TD>";
	echo "<TD WIDTH=5% NOSAVE><B>".S_COLUMNS."</B></TD>";
	echo "<TD WIDTH=5% NOSAVE><B>".S_ROWS."</B></TD>";
	echo "<TD WIDTH=10% NOSAVE><B>".S_ACTIONS."</B></TD>";
	echo "</TR>";

	$result=DBselect("select screenid,name,cols,rows from screens order by name");
	$col=0;
	while($row=DBfetch($result))
	{
		if(!check_right("Screen","R",$row["screenid"]))
		{
			continue;
		}
		if($col++%2==0)	{ echo "<TR BGCOLOR=#EEEEEE>"; }
		else		{ echo "<TR BGCOLOR=#DDDDDD>"; }
	
		echo "<TD>".$row["screenid"]."</TD>";
		echo "<TD><a href=\"screenedit.php?screenid=".$row["screenid"]."\">".$row["name"]."</a></TD>";
		echo "<TD>".$row["cols"]."</TD>";
		echo "<TD>".$row["rows"]."</TD>";
		echo "<TD><A HREF=\"screenconf.php?screenid=".$row["screenid"]."#form\">".S_CHANGE."</A></TD>";
		echo "</TR>";
	}
	if(DBnum_rows($result)==0)
	{
			echo "<TR BGCOLOR=#EEEEEE>";
			echo "<TD COLSPAN=5 ALIGN=CENTER>".S_NO_SCREENS_DEFINED."</TD>";
			echo "<TR>";
	}
	echo "</TABLE>";
?>

<?php
	echo "<a name=\"form\"></a>";

	if(isset($_GET["screenid"]))
	{
		$result=DBselect("select screenid,name,cols,rows from screens g where screenid=".$_GET["screenid"]);
		$row=DBfetch($result);
		$name=$row["name"];
		$cols=$row["cols"];
		$rows=$row["rows"];
	}
	else
	{
		$name="";
		$cols=1;
		$rows=1;
	}

	echo "<br>";
	show_table2_header_begin();
	echo S_SCREEN;

	show_table2_v_delimiter();
	echo "<form method=\"get\" action=\"screenconf.php\">";
	if(isset($_GET["screenid"]))
	{
		echo "<input class=\"biginput\" name=\"screenid\" type=\"hidden\" value=".$_GET["screenid"].">";
	}
	echo S_NAME;
	show_table2_h_delimiter();
	echo "<input class=\"biginput\" name=\"name\" value=\"$name\" size=32>";

	show_table2_v_delimiter();
	echo S_COLUMNS;
	show_table2_h_delimiter();
	echo "<input class=\"biginput\" name=\"cols\" size=5 value=\"$cols\">";

	show_table2_v_delimiter();
	echo S_ROWS;
	show_table2_h_delimiter();
	echo "<input class=\"biginput\" name=\"rows\" size=5 value=\"$rows\">";

	show_table2_v_delimiter2();
	echo "<input class=\"button\" type=\"submit\" name=\"register\" value=\"add\">";
	if(isset($_GET["screenid"]))
	{
		echo "<input class=\"button\" type=\"submit\" name=\"register\" value=\"update\">";
		echo "<input class=\"button\" type=\"submit\" name=\"register\" value=\"delete\" onClick=\"return Confirm('".S_DELETE_SCREEN_Q."');\">";
	}

	show_table2_header_end();
?>

<?php
	show_footer();
?>
