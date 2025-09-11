#!/bin/bash

files=$(grep -rl '\<HERE\>' .| grep -e '.cc$' -e '.cci$' -e '.h$')
for file in $files
do
	sed 's/ *HERE *<< */ /;s/\(debugs *( *[0-9]\+ *, *[0-9]\+ *, *\)HERE\( *) *;\)/\1MYNAME\2/' $file >/tmp/work.file
	cat /tmp/work.file >$file
done
