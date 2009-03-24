<?php

getpost_ifset(array('id', 'status', 'Period', 'frommonth', 'fromstatsmonth', 'tomonth', 'tostatsmonth', 'fromday', 'fromstatsday_sday', 'fromstatsmonth_sday', 'today', 'tostatsday_sday', 'tostatsmonth_sday', 'stitle', 'atmenu', 'current_page', 'order', 'sens','popup_select'));

$HD_Form = new FormHandler("cc_epayment_log_agent");

$HD_Form -> FG_DEBUG = 0;
$HD_Form -> FG_TABLE_ID = "id";
$HD_Form -> FG_TABLE_DEFAULT_ORDER = "creationdate";
$HD_Form -> FG_TABLE_DEFAULT_SENS = "DESC";

$date_clause = "";

if (DB_TYPE == "postgres") {		
	 	$UNIX_TIMESTAMP = "";
}else{
		$UNIX_TIMESTAMP = "UNIX_TIMESTAMP";
}
$lastdayofmonth = date("t", strtotime($tostatsmonth.'-01'));
if ($Period=="Month"){
	if ($frommonth && isset($fromstatsmonth)) $date_clause.=" $UNIX_TIMESTAMP(creationdate) >= $UNIX_TIMESTAMP('$fromstatsmonth-01')";
	if ($tomonth && isset($tostatsmonth))
	{
		if (strlen($date_clause)>0) $date_clause.=" AND ";
		$date_clause.=" $UNIX_TIMESTAMP(creationdate) <= $UNIX_TIMESTAMP('".$tostatsmonth."-$lastdayofmonth 23:59:59')"; 
	}
}else{
	if ($fromday && isset($fromstatsday_sday) && isset($fromstatsmonth_sday)) 
	{
		if (strlen($date_clause)>0) $date_clause.=" AND ";
		$date_clause.=" $UNIX_TIMESTAMP(creationdate) >= $UNIX_TIMESTAMP('$fromstatsmonth_sday-$fromstatsday_sday')";
	}
	if ($today && isset($tostatsday_sday) && isset($tostatsmonth_sday))
	{
		if (strlen($date_clause)>0) $date_clause.=" AND ";
		$date_clause.=" $UNIX_TIMESTAMP(creationdate) <= $UNIX_TIMESTAMP('$tostatsmonth_sday-".sprintf("%02d",intval($tostatsday_sday)/*+1*/)." 23:59:59')";
	}
}

if (strlen($HD_Form -> FG_TABLE_CLAUSE)>0 && strlen($enteradmin)>0) $HD_Form -> FG_TABLE_CLAUSE.=" AND ";
if($enteradmin != "")
{
	$HD_Form -> FG_TABLE_CLAUSE.=" iduser = '$enteradmin'";
}	
if (strlen($enteradmin)>0 && strlen($loglevel)>0 && $loglevel > 0) $HD_Form -> FG_TABLE_CLAUSE.=" AND ";
if($status >= 0 && $status != '')
{
	$HD_Form -> FG_TABLE_CLAUSE.=" status = '$status'";
}
if (strlen($loglevel)>0 && strlen($date_clause)>0) $HD_Form -> FG_TABLE_CLAUSE.=" AND ";
if($date_clause != "")
{
	if (strlen($HD_Form -> FG_TABLE_CLAUSE)>0) $date_clause =" AND ".$date_clause;
	$HD_Form -> FG_TABLE_CLAUSE.=" ".$date_clause;
}


$status_list = Constants::getPaymentStateList();


$HD_Form -> AddViewElement(gettext("Id"), "ID", "10%", "center");
$HD_Form -> AddViewElement(gettext("Created At"), "creationdate", "15%", "center");
$HD_Form -> AddViewElement(gettext("Payment Method"), "paymentmethod", "15%", "center");
$HD_Form -> AddViewElement(gettext("Status"), "status", "15%", "center", "sort", "", "list", $status_list);
$HD_Form -> AddViewElement(gettext("Owner"), "cc_owner", "15%", "center");
$HD_Form -> AddViewElement(gettext("Amount"), "amount", "10%", "center");
$HD_Form -> AddViewElement(gettext("Agent ID"), "cardid", "10%", "center");
$HD_Form -> AddViewElement(gettext("Agent LOGIN"), "cardid", "25%", "center", "sort", "", "30", "", "", "", "", "linktoagent");

// added a parameter to append  FG_TABLE_ID  ( by default ) or disable 0.
$HD_Form -> FieldViewElement ('id, creationdate, paymentmethod, status, cc_owner, amount, agent_id, agent_id ');

$HD_Form -> CV_NO_FIELDS  = gettext("THERE ARE NO ".strtoupper($HD_Form->FG_INSTANCE_NAME)." CREATED!");
$HD_Form -> CV_DISPLAY_LINE_TITLE_ABOVE_TABLE = false;
$HD_Form -> CV_TEXT_TITLE_ABOVE_TABLE = '';
$HD_Form -> CV_DISPLAY_FILTER_ABOVE_TABLE = false;
$HD_Form -> FG_LIMITE_DISPLAY = 25;

$HD_Form -> FG_EDITION = true;
$HD_Form -> FG_DELETION = true;
$HD_Form -> FG_ADDITION = true;

$HD_Form -> AddEditElement (gettext("ID"),
			   "id",		
			   "$value",	
			   "INPUT",	
			   "size=10 READONLY maxlength=10",	
			   "4",	
			   gettext("Insert the Id "),
			    "" , "", "", "", "", "", "", 
				gettext("Select the ID"));


$HD_Form -> AddEditElement (gettext("DATE"),
			   "creationdate",		
			   '$value',	
			   "INPUT",	
			   "size=40 READONLY maxlength=40 ",	
			   "10",	
			   gettext("Insert the creation date"),
			   "" , "", "",	"", "", "", "", "" );


$HD_Form -> AddEditElement (gettext("PAYMENT METHOD"),
			   "paymentmethod",		
			   '$value',	
			   "INPUT",	
			   "size=30 maxlength=30",	
			   "12",	
			   gettext("Insert the payment method"),
			   "" , "", "",	"", "" , "", "", gettext("Enter the payment method ") );


$HD_Form -> AddEditElement (gettext("STATUS"),	
				"status",			
				'$value',	
				"SELECT",	
				"",	
				"",	
				"", 
				"list" , "", "",	"", $status_list, "%1", "", "" );

$HD_Form -> AddEditElement (gettext("OWNER"),
			   "cc_owner",		
			   '$value',	
			   "INPUT",	
			   "size=30 maxlength=30",	
			   "12",	
			   gettext("Insert the owner"),
			   "" , "", "",	"", "" , "", "", gettext("Enter the owner") );


$HD_Form -> AddEditElement (gettext("AMOUNT"),
			   "amount",		
			   '$value',	
			   "INPUT",	
			   "size=30 maxlength=30",	
			   "12",	
			   gettext("Insert the amount"),
			   "" , "", "",	"", "" , "", "", gettext("Enter the amount ") );



$HD_Form -> AddEditElement (gettext("ID AGENT"),
			   "agent_id",		
			   "$value",	
			   "POPUPVALUE",	
			   "size=30 maxlength=50",	
			   "4",	
			   gettext("Insert the Id CardNumber"),
			    "" , "", "", "", "", 				
				"A2B_entity_agent.php?popup_select=1&"
				, ", 'AgentIdSelection','width=550,height=330,top=20,left=100,scrollbars=1'" ,
				gettext("Select the Agent ID"));


$HD_Form -> FieldEditElement ('id, creationdate, paymentmethod, status, cc_owner, amount, id_agent ');


$HD_Form -> FG_FILTER_APPLY = true;
$HD_Form -> FG_FILTERFIELD = 'agent_id';
$HD_Form -> FG_FILTERFIELDNAME = 'agent_id';
$HD_Form -> FG_FILTER_FORM_ACTION = 'list';

if (isset($filterprefix)  &&  (strlen($filterprefix)>0)){
	if (strlen($HD_Form -> FG_TABLE_CLAUSE)>0) $HD_Form -> FG_TABLE_CLAUSE.=" AND ";
	$HD_Form -> FG_TABLE_CLAUSE.="agent_id like '$filterprefix%'";
}



$HD_Form -> FG_INTRO_TEXT_EDITION= '';
$HD_Form -> FG_BUTTON_EDITION_SRC = $HD_Form -> FG_BUTTON_ADITION_SRC  = Images_Path . "/cormfirmboton.gif";
$HD_Form -> FG_BUTTON_EDITION_BOTTOM_TEXT = $HD_Form -> FG_BUTTON_ADITION_BOTTOM_TEXT = gettext("Click 'Confirm Data' to continue");

$HD_Form -> FG_GO_LINK_AFTER_ACTION_EDIT = $_SERVER['PHP_SELF']."?atmenu=document&stitle=Document&wh=AC&id=";

