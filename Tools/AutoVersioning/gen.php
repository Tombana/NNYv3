<?php
echo '-------------------------------'."\n";
echo '| AutoVersioning 1.0 script'."\n";
echo '| Made by NitriX'."\n";
echo '-------------------------------'."\n";
echo 'Retrieving old version infos...'."\n";
$infos = file_get_contents('src/version.hpp');

echo 'Cleaning special chars...'."\n";
$infos = str_replace("\t",'',$infos);
$infos = str_replace("\r",'',$infos);

echo 'Cleaning comments...'."\n";
$temp = explode("\n",$infos);
for ($i=0; $i<count($temp); $i++) {
if ($temp[$i][0] == '#') { //pickup only #defines
	$output .= $temp[$i];
}
}

echo 'Parsing infos...'."\n";
$infos = explode('#define ',$output);

echo 'Reading infos...'."\n";
for ($i=1; $i<count($infos); $i++) {
	$data = explode(' ',$infos[$i]); //Split variables/values
	$data[1] = str_replace('"','',$data[1]); //remove quotes from values
	if ($data[0] == 'BUILD_NUMBER') {
		echo 'Build number is '.$data[1]."\n";
		$NUMBER = $data[1];
	}
	if ($data[0] == 'BUILD_VERSION') {
		echo 'Build version is '.$data[1]."\n";
		$VERSION = $data[1];
	}
}

echo 'Deleting file...'."\n";
unlink('src/version.hpp');

echo 'Saving new content...'."\n";
$fp = fopen('src/version.hpp', 'w+');
fputs($fp, '#define BUILD_VERSION "'.$VERSION.'"'."\n\n");
fputs($fp, '//==== WARNING! DONT EDIT BELOW THIS LINE! ===='."\n");
fputs($fp, '//This part is dinamically generated on compilation by AutoVersioning tool'."\n");
fputs($fp, '#define BUILD_DATE "'.date('F j, Y g:ia').'"'."\n");
fputs($fp, '#define BUILD_NUMBER '.($NUMBER+1)."\n");
fclose($fp);

echo 'Done!'."\n";

//Must return 0
exit(0);
?>