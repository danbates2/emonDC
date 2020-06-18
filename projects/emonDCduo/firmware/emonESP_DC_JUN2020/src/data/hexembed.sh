#!/bin/bash

# the below function was added for macOS
realpath() {
  OURPWD=$PWD
  cd "$(dirname "$1")"
  LINK=$(readlink "$(basename "$1")")
  while [ "$LINK" ]; do
    cd "$(dirname "$LINK")"
    LINK=$(readlink "$(basename "$1")")
  done
  REALPATH="$PWD/$(basename "$1")"
  cd "$OURPWD"
  echo "$REALPATH"
}

#echo $0
 
full_path=$(realpath $0)
#echo $full_path
 
dir_path=$(dirname "$full_path")
#echo $dir_path
 
working=$(dirname "$dir_path" )
#echo $working
 
data_dir="$working/web_server_files"
#echo $data_dir
#echo ""$data_dir"/web_server.config_js.h"

"$dir_path"/hexembed "$dir_path"/config.js > "$data_dir"/web_server.config_js.h
"$dir_path"/hexembed "$dir_path"/home.html > "$data_dir"/web_server.home_html.h
"$dir_path"/hexembed "$dir_path"/lib.js > "$data_dir"/web_server.lib_js.h
"$dir_path"/hexembed "$dir_path"/style.css > "$data_dir"/web_server.style_css.h
"$dir_path"/hexembed "$dir_path"/wifi.html > "$data_dir"/web_server.wifi_html.h