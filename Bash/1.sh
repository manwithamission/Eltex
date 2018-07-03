#!/bin/bash

echo "Укажите место расположения: "
read u
cd $u
echo "Назовите каталоги: "
read w
echo "Укажите количество каталогов: "
read q
echo "Назовите подкаталоги: "
read r
echo "Укажите количество подкаталогов: "
read e
echo "Назовите файлы: "
read y
echo "Укажите количество файлов: "
read t


for ((a=0; a < $q ; a++)) 
do mkdir "$w$a";
	cd "$w$a";
	for ((b=0; b < $e ; b++)) 
		do mkdir "$r$b"
			cd "$r$b"	
			for ((c=0; c < $t ; c++)) 
				do touch "$y$c"
				done
				cd ../
		done
		cd ../

done ;
exit 0