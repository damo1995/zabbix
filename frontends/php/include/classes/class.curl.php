<?php
/* 
** ZABBIX
** Copyright (C) 2000-2009 SIA Zabbix
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
// Title: url manipulation class
// Author: Aly

class Curl{
 private $url;
 protected $port;
 protected $host;
 protected $protocol;
 protected $username;
 protected $password;
 protected $file;
 protected $reference;
 protected $path;
 protected $query;
 protected $arguments;

	public function __construct($url=null){
		$this->url = 		null;
		$this->port =		null;
		$this->host = 		null;
		$this->protocol = 	null;
		$this->username =	null;
		$this->password =	null;
		$this->file =		null;
		$this->reference =	null;
		$this->path =		null;
		$this->query =		null;
		$this->arguments = 	array();
	
		if(empty($url)){
			$this->formatArguments();
			$this->url = $url = 'http://'.$_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME'].'?'.$this->getQuery();
		}
		else{
			$this->url=urldecode($url);
	
			$url = parse_url($this->url);
			
			$this->protocol = isset($url['scheme']) ? $url['scheme'] : null;
			$this->host = isset($url['host']) ? $url['host'] : null;
			$this->port = isset($url['port']) ? $url['port'] : null;
			$this->username = isset($url['user']) ? $url['user'] : null;
			$this->password = isset($url['pass']) ? $url['pass'] : null;
			$this->path = isset($url['path']) ? $url['path'] : null;
			$this->query = isset($url['query']) ? $url['query'] : '';
			$this->reference = isset($url['fragment']) ? $url['fragment'] : null;
			
			$this->formatArguments($this->query);
		
		}
			if(isset($_COOKIE['zbx_sessionid']))
				$this->setArgument('sid', substr($_COOKIE['zbx_sessionid'],16,16));
		
	}
	
	public function formatQuery(){
		$query = '';
		foreach($this->arguments as $key => $value){
			$query.= $key.'='.$value.'&';
		}
		$this->query = rtrim($query,'&');
	}
	
	public function formatArguments($query=null){
		if(is_null($query)){
			$this->arguments = $_REQUEST;
		}
		else{
			//$query=ltrim($query,'?');
			$args = explode('&',$query);
			foreach($args as $id => $arg){
				if(empty($arg)) continue;
				$tmp = explode('=',$arg);
				$this->arguments[$tmp[0]] = isset($tmp[1])?$tmp[1]:'';
			}
		}
		$this->formatQuery();
	}
	
	public function getUrl(){
			$url = $this->protocol ? $this->protocol.'://' : '';
			$url .= $this->username ? $this->username : '';
			$url .= $this->password ? ':'.$this->password : '';
			$url .= $this->host ? $this->host : '';
			$url .= $this->port ? ':'.$this->port : '';
			$url .= $this->path ? $this->path : '';
			$url .= $this->query ? '?'.$this->query : '';
			$url .= $this->reference ? '#'.urlencode($this->reference) : '';
	//SDI($this->getProtocol().' : '.$this->getHost().' : '.$this->getPort().' : '.$this->getPath().' : '.$this->getQuery());
	return $url;
	}
	
	public function setPort($port){
		$this->port = $port;
	}
	
	public function getPort(){ 
		return $this->port;
	}
	
	public function setArgument($key,$value=''){
		$this->arguments[$key] = $value;
		$this->formatQuery();
	}
	
	public function getArgument($key){
		if(isset($this->arguments[$key])) return $this->arguments[$key];
		else return NULL;
	}
	
	public function setQuery($query){ 
		$this->query = $query;
		$this->formatArguments();
		$this->formatQuery();
	}
	
	public function getQuery(){ 
		return $this->query;
	}
	
	public function setProtocol($protocol){
		$this->protocol = $protocol;
	}
	
	/* Returns the protocol of $this URL, i.e. 'http' in the url 'http://server/' */
	public function getProtocol(){
		return $this->protocol;
	}
	
	public function setHost($host){
		$this->host = $host;
	}
	
	/* Returns the host name of $this URL, i.e. 'server.com' in the url 'http://server.com/' */
	public function getHost(){
		return $this->host;
	}
	
	public function setUserName($username){
		$this->username = $username;
	}
	
	/* Returns the user name part of $this URL, i.e. 'joe' in the url 'http://joe@server.com/' */
	public function getUserName(){
		return $this->username;
	}
	
	public function setPassword($password){
		$this->password = $password;
	}
	
	/* Returns the password part of $this url, i.e. 'secret' in the url 'http://joe:secret@server.com/' */
	public function getPassword(){
		return $this->password;
	}
	
	/* Returns the file part of $this url, i.e. everything after the host name. */
	public function getFile(){
		$url .= $this->path ? $this->path : '';
		$url .= $this->query ? '?'.$this->query : '';
		$url .= $this->reference ? '#'.urlencode($this->reference) : '';
		return $url;	
	}
	
	public function setReference($reference){
		$this->reference = $reference;
	}
	
	/* Returns the reference of $this url, i.e. 'bookmark' in the url 'http://server/file.html#bookmark' */
	public function getReference(){
		return $this->reference;
	}
	
	public function setPath($path){
		$this->path = $path;
	}
	
	/* Returns the file path of $this url, i.e. '/dir/file.html' in the url 'http://server/dir/file.html' */
	public function getPath(){
		return $this->path;
	}
	
	public function toString(){
		return $this->getUrl();
	}
}
?>