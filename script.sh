for ((np=1; np <= 8; np=np*2))
do
    for ((id=0; id <= 4; id++))
	do
	    ./myprog.out $id $np --save_stat
	done
done