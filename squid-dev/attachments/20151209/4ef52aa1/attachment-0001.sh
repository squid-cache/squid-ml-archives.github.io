#!/bin/sh

# Logs basic information about the Squid process.
# Handy as an on-fatal handler called by the Squid process.

# Tune these variables as needed:
Log=/tmp/squid-on-fatal.log # this script logs here (rm before starting Squid)
SquidPidFile=/usr/local/squid/var/run/squid.pid
SysLogFile=/var/log/syslog


main() {
	run ulimit -a
	run cat /proc/meminfo
	run cat /proc/$SquidPid/status
	run cat /proc/$SquidPid/limits
	run tail /proc/$SquidPid/oom_*
	run ipcs -l
	run ipcs -u
	run sysctl -a
	run tail "$SysLogFile"
}

run() {
	say "Running $* {"
	$* >> "$Log" 2>&1
	say "} ran $*"
	say ""
}

say() {
	echo "$*" >> "$Log"
}

# TODO: Teach Squid to pass its PID to the script instead.
SquidPid=""
calculateSquidPid() {
	# Squid PID file may not yet exist.
	if test -f "$SquidPidFile"
	then
		SquidPid=`cat "$SquidPidFile"`
		if test -z "$SquidPid"
		then
			say "Failed to find Squid PID in $SquidPidFile"
		fi
	else
		say "No Squid PID file (yet?): $SquidPidFile"
	fi

	if test -z "$SquidPid"
	then
		# Our parent ($PPID) is the shell interpreter. Our grandparent is Squid.
		SquidPid=`ps --no-headers --format ppid --pid $PPID | awk '{ print $1; }'`
	fi

	if test -z "$SquidPid"
	then
		say "Failed to find grandparent PID of $$ via $PPID"
		SquidPid=$PPID
	fi

	if test -z "$SquidPid"
	then
		say "Failed to find parent PID of $$"
		SquidPid=$$
	fi

	say "Assumed Squid PID: $SquidPid"
}

say "Starting $0 on `date` as `id`"
calculateSquidPid
main
say "Finished $0 on `date`"

exit 0
