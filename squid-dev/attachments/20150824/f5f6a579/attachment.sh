#!/bin/bash

files=$(fgrep -rl HERE .| grep -e '.cc$' -e '.cci$')
for file in $files
do
	sed 's/ *HERE *<< */ /' $file >/tmp/work.file
	cat /tmp/work.file >$file
done
