#!/bin/bash

if ! [ -z $1]; then
echo "current directory"$1
if ! [ -d $1 ]; then
mkdir $1
fi
cd $1
fi

for ((a=0; a <= 4 ; a++)) 
do mkdir "Каталог"$a;
	cd "Каталог"$a;
	for ((b=0; b <= 9 ; b++)) 
		do mkdir "Подкаталог"$b
			cd "Подкаталог"$b	
			for ((c=0; c <= 19 ; c++)) 
				do touch "Файл"$c
				done
				cd ../
		done
		cd ../

done ;
echo $a
exit 0
