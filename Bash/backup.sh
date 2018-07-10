#!/bin/bash

target="Укажите файл/директорию/дисковое устройство для резервного копирования: "
actions="[1]Добавить задание на резервное копирование\n[2]Удалить задание на резервное копирование\n[3]Показать существующие задания"
hour="Укажите часы в формате 0-23: "
minute="Укажите минуты в формате 0-59: "

echo -e $actions
read b
case $b in
	"1"		)
			echo -e $target
			read a

			if [ -f $a ]
			then

			echo -e "[1]Копировать файл\n[2]Архивировать файл"
			read x

			case $x in
				"1" 	)
						setup="cp $a /home/arthur/temp"
				;;
				"2" 	)
						setup="tar -cf $a.tgz $a | mv $a.tgz /home/arthur/temp"
				;;
			esac
			
			elif [ -d $a ]
			then
				setup="tar -czf $a.tgz $a | mv /home/arthur/Eltex/$a.tgz /home/arthur/temp"

			elif [ -b $a ]
			then
				setup="sudo mount $a /mnt/backup | sudo dd if=$a bs=1M conv=noerror | gzip -c > /mnt/backup/$a.gz | sudo umount /mnt/backup"
			fi

			echo -e $hour
			read h
			clear
			echo -e $minute
			read m
			clear
			echo "Задание на бэкап добавлено на $h:$m"
			crontab -u $USER -l > ~/tmp
			echo "$m $h * * * $setup #backup" >> ~/tmp
			crontab -u $USER ~/tmp
	;;
	"2" 	)
			i=1
			crontab -u $USER -l | grep -v "# " | grep "#backup" > ~/list.tmp
			while read line	
			do
			echo "[$i] Бэкап на ${line:3:2}:${line:0:2}"
			((i++))
			done < ~/list.tmp
			echo "Введите номер бэкапа"
			read c
			i=1
			echo -n > ~/tmp
			while read line
			do
			if ! [ $i -eq $c ]
			then
				echo "$line" >> ~/tmp
			else
				echo "[*] Бэкап на ${line:3:2}:${line:0:2}"
			fi
			((i++))
			done < ~/list.tmp
			crontab -u $USER -l | grep -v "#backup" | grep -v "# "> ~/list.tmp
			cat ~/tmp >> ~/list.tmp
			crontab -u $USER ~/list.tmp
	;;
	"3" 	)
			crontab -u $USER -l | grep -v "# " | grep "#backup" > ~/tmp
			while read line
			do
			echo "Бэкап на ${line:3:2}:${line:0:2}"
			done < ~/tmp
	;;
esac