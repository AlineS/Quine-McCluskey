#!/bin/bash

random(){
	local n_nums="5000";
	local out="out.txt";
	for i in $(seq 1 $n_nums); do
		R="$((($RANDOM % 10000)+1))";
		[[ "$R" -lt "10" ]] && R="0000$R";
		[[ "$R" -gt "9" ]] && [[ "$R" -lt "100" ]] && R="000$R";
		[[ "$R" -gt "99" ]] && [[ "$R" -lt "1000" ]] && R="00$R";
		[[ "$R" -gt "999" ]] && [[ "$R" -lt "10000" ]] && R="0$R";
		echo $R >> $out;
	done
}
random
