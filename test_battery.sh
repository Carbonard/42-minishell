

echo $?

echo hola

ls asdfas
echo $?

ls libft
echo $?

false_command 1 2 3
echo $?
echo $?

echo $PATH

cd abcd

export A=abcd

echo $A, "$A", '$A', $AA

export $A=5
echo $?

export BB='$asdfg$'

echo $A$BB $BB

unset BB=
echo $?
unset B
echo $?

echo $BB

export '$a'=4
echo $?

export N5=1 N6=2 N7=3 N7=4 N8=$N6 && echo $N5 $N6,$N7"$N8"

unset N1 N5 N6 N7 N8

echo $N1,$N5.$N6-$N7+$N8

unset '$N2'
echo $?

mkdir no_perm_dir

chmod 000 no_perm_dir

cd no_perm_dir
echo $?

ls no_perm_dir
echo $?

chmod 777 no_perm_dir

rmdir no_perm_dir

cd Makefile
echo $?

cd dafljak
echo $?

./libft
echo $?

./Makefile
echo $?

echo 'hola que tal' > testfile1
echo 'muy bien y tu' >> testfile1
echo 'yo tambien' >> testfile1

cat testfile1

echo "jaja que     bueno" > testfile1

cat testfile1

< test_battery.sh cat -e | grep a | grep e > testfile2

cat -e testfile2

echo aaa | ls fasf && echo bbb
echo $?

echo ccc | echo ddd && echo eee
echo $?

ls fdsafa | echo fff && echo ggg
echo $?

ls sdas | echo hhh
echo $?

echo hola | ls asdfa
echo $?

echo adios | ls
echo $?

<< eof cat | cat > testfile3 | cat -e
aaa
bbb
eof
echo $?

cat testfile3
echo $?

rm testfile3
rm testfile3
echo $?

(cd fdsa || cd src) && echo jjj

((echo qwerty && ls sdfa) || echo zxcv && echo jkl)
((echo pqwerty && ls psdfa) || echo gggg && echo hhh)

echo -n aaa

echo -nn bbb

echo --n ccc

echo ddd -n

<<eof1<<eof2 cat|cat|grep -i 'a'|grep i|grep u | grep a | cat -e|grep u
qwe
asd
zaxd
qed
qeda
a
qeera
eof1
uio
uioa
uio
auio
uiauj
eof2

echo *

echo m*

echo *l

echo tes*

echo *.sh

mkdir test_dir_ast
cd test_dir_ast
touch '$HOME' _1 1 1_a 1a 1_b 1b 2a 2b a_a aa aA a_ab aab aAb _AAB A_AB 'a b' %abc '\-abc' a_bc ab_c abc aBc %Abc '\-Abc' Abc ' c' con %dAbc %DAbc ' f' f 'f ' f_ 'f 1' 'f\\1' 'f\1' f_1 'f|1' f1 'f 2' 'f\\2' 'f\2' f_2 'f|2' f2 'f\\3' 'f\3' f_3 'f|3' f3 '"filename"' "'filename'" __g _g __h _h ho__la ho_la hola 'hola"nop' __ij _ij __ik _ik %zAbc %ZAbc
echo *
mkdir -p cosa/ls cosa/zzz/a cosa/zzz/b cosa/zzz/c
cd cosa
pwd
ls
*
cd ../..
rm -r test_dir_ast

ls

(cd src) && pwd
cd src && cd .. && pwd


((cd src && (cd ..) && cd .. && cat < Makefile) && ls sdf || echo ag) | cat -e > testfile1

echo $?

cat testfile1

cd src && cd .. cat < Makefile || cd ..

touch testfile4
chmod 000 testfile4

echo asdfgh > testfile4
echo $?

ls > testfile4
echo $?

chmod 777 testfile4
rm testfile4

touch "':('"
echo '*' '*l'
rm "':('"

touch abcdefghijklmnopqrstuvwxyz
echo abcd*hij*no*st*xyz
echo abcd'efg'h*ij'klmn'opq*tu*yz
echo *efghijklmnop*qrstuvw*
echo a*b**hij*rs*
rm abcdefghijklmnopqrstuvwxyz


echo hola

cat << eof1 && cat << eof2 && cat << eof3
test1
eof1
test2
eof2
test3
eof3


cat << eof1 << eof2 && cat << eof3
-test1
eof1
-test2
eof2
-test3
eof3

mkdir -p test_dir_a/test_dir_b/test_dir_c
cd test_dir_a/test_dir_b/test_dir_c
rm -rf ../../../test_dir_a
echo $PWD
cd ..
echo $PWD
cd .
echo $PWD
cd -
cd ../..
echo $PWD

echo ..................

ls

rm testfile1 testfile2

touch t3st_f1l3
cd t3st_f1l3
chmod 000 t3st_f1l3
cd t3st_f1l3
chmod 777 t3st_f1l3
rm t3st_f1l3

mkdir 7357_d1r
chmod 000 7357_d1r
cd 7357_d1r
chmod 777 7357_d1r
rmdir 7357_d1r

cp minishell badshell
chmod 000 badshell
cd badshell
./badshell
chmod 777 badshell
rm badshell

echo -n "testing -n flag"
echo -nn "testing -n flag"
echo testing -n flag

echo -e "testing -e flag\ntested"
echo -eee "testing -e flag\ttested"
echo testing -e flag

echo -ne "testing -ne flag"
echo -en "testing -en flag"
echo -ene "testing -ene flag"
echo -ne "testing -ne flag"
echo -ne -en "testing -ne -en flag"
echo -e
# kkkkk -e "hola\n\tque tal\abien\\, \', \"\""

echo testing" "quotes    and " things"