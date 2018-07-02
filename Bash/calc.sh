#!/bin/bash

opr="Выберите операцию:\n[1] Сложение\n[2] Вычитание\n[3] Умножение\n[4] Деление"
echo -e $opr

read s
case $s in
	"1" 	)
	first="Введите первое число"
	echo -e $first
	read a;
	second="Введите второе число"
	echo -e $second
	read b;
	let "c=a+b"
	echo -e "Результат:"
	echo "$a+$b=$c"
	;;
	"2"		)
	first="Введите первое число"
	echo -e $first
	read a;
	second="Введите второе число"
	echo -e $second
	read b;
	let "c=a-b"
	echo -e "Результат:"
	echo "$a-$b=$c"
	;;
	"3"		)
	first="Введите первое число"
	echo -e $first
	read a;
	second="Введите второе число"
	echo -e $second
	read b;
	let "c=a*b"
	echo -e "Результат:"
	echo "$a*$b=$c"
	;;
	"4"		)
	first="Введите первое число"
	echo -e $first
	read a;
	second="Введите второе число"
	echo -e $second
	read b;
	let "c=a/b"
	echo -e "Результат:"
	echo "$a/$b" |bc -l
	;;
esac