<?php
class ByteArray {
	var $m_activeInput = true;
	var $m_buffer = '';
	var $m_seek = 0;
	//====== PUBLIC ========
	function clear() {
		$this->m_buffer = '';
		$this->m_activeInput = true;
		$this->m_seek = 0;
	}
	
	function eof() {
		if ($this->m_seek == strlen($this->m_buffer)) {
			return true; //the pointer is at the end
		} else {
			return false; //the pointer isnt at the end
		}
	}
	
	function addBool($bool) {
		$this->addByte($bool);
	}
	
	function readBool() {
		return $this->readByte();
	}
	
	function addByte($hex) {
		$this->m_buffer .= chr($hex %256); //append
	}

	function addString($str) {
		$this->addDword(strlen($str));
		$this->m_buffer .= $str;
	}
	
	function addWord($hex) {
		$this->m_buffer .= chr($hex %256);
		$this->m_buffer .= chr(($hex / pow(256,1)) %256);
	}
	
	function addCmd($hex) {
		$this->addWord($hex);
	}
	
	function addAck($ack) {
		$this->addByte($ack);
	}
	
	function readAck() {
		return $this->readByte();
	}

	function addDword($hex) {
		$this->m_buffer .= chr($hex %256);
		$this->m_buffer .= chr(($hex / pow(256,1)) %256);
		$this->m_buffer .= chr(($hex / pow(256,2)) %256);
		$this->m_buffer .= chr(($hex / pow(256,3)) %256);
	}

	function getBuffer() {
		return $this->m_buffer;
	}
	
	function append($toAppend) {
		$this->m_buffer .= $toAppend;
	}
	
	function printHex() {
		echo $this->_strToHex($this->m_buffer);
	}
	
	function _strToHex($string) {
		for ($i=0; $i < strlen($string); $i++) {
		$temp = dechex(ord($string[$i]));
		if (strlen($temp)<2) $temp = '0'.$temp; //single line
		$hex .= $temp;
		}
		return strtoupper(substr(chunk_split($hex, 2, ' '), 0, -1));
	}
	
	function getPacket() {
		//Packet begining [Byte]
		$output = '~';
		//Packet size [DWORD]
		$bufferSize = strlen($this->m_buffer);
		$output .= chr($bufferSize %256);
		$output .= chr(($bufferSize / pow(256,1)) %256);
		$output .= chr(($bufferSize / pow(256,2)) %256);
		$output .= chr(($bufferSize / pow(256,3)) %256);
		//Packet content
		return $output.$this->m_buffer;
	}

	function modSeek($newSeek) {
		$this->m_seek += $newSeek;
	}
	
	function setSeek($newSeek) {
		$this->m_seek = $newSeek;
	}
	
	function readByte(){
		$val=0;
		for ($i=0; $i>=0; $i--) {
			$val += pow(256, $i) * ord($this->m_buffer[$this->m_seek+$i]);
		}
		$this->m_seek+=1;
		return $val;
	}
	
	function readWord(){
		$val=0;
		for($i=1; $i>=0; $i--) {
			$val += pow(256, $i) * ord($this->m_buffer[$this->m_seek+$i]);
		}
		$this->m_seek+=2;
		return $val;
	}

	function readDword(){
		$val=0;
		for($i=3; $i>=0; $i--) {
			$val += pow(256, $i) * ord($this->m_buffer[$this->m_seek+$i]);
		}
		$this->m_seek+=4;
		return $val;
	}

	function readString(){
		$length = $this->readDword();
		$output = substr($this->m_buffer, $this->m_seek, $length);
		$this->m_seek+=$length;
		return $output;
	}
	
	function size() {
		return strlen($this->m_buffer);
	}
	
	function delete($seek) { //delete from left to right
		$newbuffer = substr($this->getBuffer(), $seek); //position to 'seek' and read the rest
		$this->clear();
		$this->append($newbuffer);
	}
}
?>