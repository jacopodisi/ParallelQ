for (( i = 0; i < 30; i++ )) 
do	
	for ((np=1; np <= 8; np=np*2))
	do
	    for ((id=0; id <= 4; id++))
		do
		    ./myprog.out $id $np --save_time=true
		done
	done
done