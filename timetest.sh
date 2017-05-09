let "n=10*4-1"
for ((np=1; np <= 8; np=np*2))
do
    for ((id=1; id <= 10; id++))
	do
	    ./myprog.out $id $np --save_time --eps -1 --name_option eps
	    echo $n
	    let "n--"
	done
done