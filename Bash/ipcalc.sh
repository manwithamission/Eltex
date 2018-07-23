#!/bin/bash
oct="255"
x="Введите ip-адрес:"
y="Введите маску:"
echo -e $x
read ip
echo -e $y
read mask

IFS=. read -r i1 i2 i3 i4 <<< $ip
IFS=. read -r m1 m2 m3 m4 <<< $mask

printf "______________________________________________________\n\vАдрес сети: \t\t\t%d.%d.%d.%d\n" "$((i1 & m1))" "$((i2 & m2))" "$((i3 & m3))" "$((i4 & m4))"
n1="$((i1 & m1))"
n2="$((i2 & m2))" #network
n3="$((i3 & m3))"
n4="$((i4 & m4))"
t1="$((oct ^ m1))"
t2="$((oct ^ m2))"
t3="$((oct ^ m3))"
t4="$((oct ^ m4))"

printf "Широковещательный адрес: \t%d.%d.%d.%d\n" "$((t1 | n1))" "$((t2 | n2))" "$((t3 | n3))" "$((t4 | n4))"
tmp1="$((t1 | n1))"
tmp2="$((t2 | n2))"
tmp3="$((t3 | n3))"
tmp4="$((t4 | n4))"

s1="$((tmp1 - n1))"  #broadcast
s2="$((tmp2 - n2))"
s3="$((tmp3 - n3))"
s4="$((tmp4 - n4))"

summa="1"

for (( i = 1; i < 5; i++ ));
do
	if (( "(s$i) - (n$i)" >= 0 )); then
		# printf "ЧТО ТУТ У НАС %s\n" "$(( (s$i) - (n$i) + 1))"
		summa="$(( $((summa))" * "$(( (s$i) - (n$i) + 1)) ))"
		# printf "ЧТО ТУТ У НАС %s\n" "$summa"
	fi
	#statements
done

printf "Кол-во хостов: \t\t\t%d\n" "$((summa - 2))"