<?php
$protocol = file_get_contents($PROTOCOL_HPP_FILE);
$protocol = strtoupper($protocol);
$lines = explode("\n",str_replace("\r",'',str_replace("\t",'',$protocol)));
	  
foreach ($lines as $key=>$value) {
	if (substr($value, 0, 7) == '#DEFINE') {
		$params = substr($value, 8);
		$params = preg_replace('/\s\s+/', ' ', $params); //remove extra whitespaces
		$params = explode(' ',$params);
		define($params[0], hexdec(str_replace('0X','',$params[1])));
	}
}

//Here hexdec() fix a bug i forgot and doesn't worth fixing
echo 'Protocol loaded: v'.hexdec(NNY_PROTOCOL_VERSION)."\n";
?>