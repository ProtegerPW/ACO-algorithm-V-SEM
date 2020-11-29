
#/bin/bash
for i in $(seq "$1"); do
	for j in $(seq "$2"); do
		for k in {1..10}; do
			if [[ "$i" -eq "$j" ]]; then
				continue
			fi
		./ACO.o test100 $i $j >> acoResults
		done
	echo "#################" >> acoResults
	done
done
