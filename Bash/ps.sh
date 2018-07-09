#!/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/home/arthur/Eltex/Bash

if [[ $1 == "shell" ]]; 
then
	choose="Назовите имя процесса: "
	prd="Установите периодичность в минутах: "
	echo "Скрипт запущен с терминала"
	echo $choose
	read a
	# echo $a >> ~/Eltex/Bash/pidtmp
	echo $prd
	# echo $(whoami)
	read b

	crontab -u $USER -l > ~/Eltex/Bash/tmp
	echo "*/$b * * * * bash /home/arthur/Eltex/Bash/ps.sh cron $a >> /home/arthur/Eltex/Bash/logps  #ps" >> ~/Eltex/Bash/tmp
	crontab -u $USER ~/Eltex/Bash/tmp
	echo "Проверка процесса $a установлена в расписание cron с перодичностью $b минут" >> logps
fi

if [[ $1 == "cron" ]]; 
then
	echo "Скрипт запущен с крона"
	echo $2

	# for line in $(cat pidtmp)
	# do 
	# echo "$line" 

	if (( $(pgrep $2) )); 
	then
		echo "Пид под именем $2 активен"
	else
		$2
		echo "$2 перезапущен"
	fi
	# done; 
fi