#!/bin/bash

target="Введите имя процесса"
first="Укажите с какой даты выводить лог"
month="Выберите месяц"
dat="Укажите число 1-31"
msum="[1]Январь\n[2]Февраль\n[3]Март\n[4]Апрель\n[5]Май\n[6]Июнь\n[7]Июль\n[8]Август\n[9]Сентябрь\n[10]Октябрь\n[11]ноябрь\n[12]Декабрь"
hour="Укажите часы в формате 00-23"

echo -e $target
read a
echo -e $first
echo -e $month
echo -e $msum
read b
case $b in
	"1" )
		mtmp=Jan
	;;
	"2" )
		mtmp=Feb
	;;
	"3" )
		mtmp=Mar
	;;
	"4" )
		mtmp=Apr
	;;
	"5" )
		mtmp=May
	;;
	"6" )
		mtmp=Jun
	;;
	"7" )
		mtmp=Jul
	;;
	"8" )
		mtmp=Aug
	;;
	"9" )
		mtmp=Sep
	;;
	"10" )
		mtmp=Oct
	;;
	"11" )
		mtmp=Nov
	;;
	"12" )
		mtmp=Dec
	;;
esac
echo -e $dat
read c
# echo -e "awk '/$mtmp $c/ && /$a/' /var/log/syslog > /home/arthur/Eltex/Bash/tmplog.txt"
awk -v mtmp=$mtmp -v c=$c -v a=$a "/$mtmp $c/ && /$a/" /var/log/syslog > /home/arthur/Eltex/Bash/tmplog.txt
echo -e $hour
read d
# while read line
# do
echo "cat -v /home/arthur/Eltex/Bash/tmplog.txt | grep -e "$mtmp $dat $d:""
cat -v /home/arthur/Eltex/Bash/tmplog.txt | grep -e "$mtmp $c $d:"
# echo $line | grep -c "$mtmp $dat $d"
# done < /home/arthur/Eltex/Bash/tmplog.txt

# awk '/Jul 11/ && /gnome/' /var/log/syslog > /home/arthur/Eltex/Bash/tmplog.txt