#!/bin/bash
opr="Выберите действие:\n[1] Добавить будильник\n[2] Удалить будильник?\n[3] Список установленных будильников "
audio="Выберите аудиофайл:\n[1] 1.mp3\n[2] 2.mp3\n[3] 3.mp3"
echo -e $opr

read s
case $s in
	"1" 	)
			echo "Введите часы сигнала будильника в формате 0-23"
			read h
			clear
			echo "Введите минуты сигнала будильника в формате 0-59"
			read m
			clear
			echo -e $audio 
			read song
			clear
			case $song in
				"1" 	)
						song="~/1.mp3"	
				;;
				"2"		)
						song="~/2.mp3"
				;;
				"3"		)
						song="~/3.mp3"
				;;
			esac
			echo "Будильник добавлен на $h:$m"
			crontab -u $USER -l > ~/tmp
			echo "$m $h * * * mplayer $song #mark" >> ~/tmp
			crontab -u $USER ~/tmp
	;;
	"2"		)
			i=1
			crontab -u $USER -l | grep -v "# " | grep "#mark" > ~/list.tmp
			while read line	
			do
			echo "[$i] Будильник на ${line:3:2}:${line:0:2}"
			((i++))
			done < ~/list.tmp
			echo "Введите номер будильника"
			read c
			i=1
			echo -n > ~/tmp
			while read line
			do
			if ! [ $i -eq $c ]
			then
				# echo "[$i] Будильник на ${line:3:2}:${line:0:2}"
				echo "$line" >> ~/tmp
			else
				echo "[*] Будильник на ${line:3:2}:${line:0:2}"
			fi
			((i++))
			done < ~/list.tmp
			crontab -u $USER -l | grep -v "#mark" | grep -v "# "> ~/list.tmp
			cat ~/tmp >> ~/list.tmp
			crontab -u $USER ~/list.tmp
	;;
	"3"		)
			crontab -u $USER -l | grep -v "# " | grep "#mark" > ~/tmp
			while read line
			do
			echo "Будильник на ${line:3:2}:${line:0:2}"
			done < ~/tmp
	;;
esac