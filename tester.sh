#!/bin/bash

make

cp minishell bash

touch minishell_output.log minishell_error.log bash_output.log bash_error.log

echo 'Executing minishell...'
< test_battery.sh ./bash >minishell_output.log 2>minishell_error.log
echo 'Executing bash...'
< test_battery.sh source test_battery.sh >bash_output.log 2>bash_error.log

echo -e '\e[93mOutput diff:\e[0m'
output_diff=$(diff --color=auto minishell_output.log bash_output.log)
if [ "$output_diff" = "" ]
then
	echo -e "\e[34mOK\e[0m"
else
	diff --color=auto minishell_output.log bash_output.log
fi
echo -e '\e[93mError diff:\e[0m'
error_diff=$(diff --color=auto minishell_error.log bash_error.log)
if [ "$error_diff" = "" ]
then
	echo -e "\e[34mOK\e[0m"
else
	diff --color=auto minishell_error.log bash_error.log
fi

# echo 'Executing with valgrind...'

# cat test_battery.sh | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --child-silent-after-fork=no --suppressions=readline.supp --log-file=valgrind.log ./bash >/dev/null 2>&1
# echo -e "\e[93mMemory leaks:\e[0m"
# cat valgrind.log | grep -B5 -A5 -E 'are definitely|are still'
# echo -e "\e[93mOpen file descriptors:\e[0m"
# cat valgrind.log | grep -B1 -A1 'Open file descriptor' | grep -v 'valgrind.log'
# echo -e "\e[93mValgrind issues:\e[0m"
# cat valgrind.log | grep -B5 -A5 ft

# echo 'Executing with valgrind...'

# cat test_battery.sh | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --child-silent-after-fork=no --suppressions=readline.supp ./bash >/dev/null 2>&1
echo -e "\e[93mMemory leaks:\e[0m"
# cat test_battery.sh | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --child-silent-after-fork=no --suppressions=readline.supp ./bash |& grep -B5 -A5 -E 'are definitely|are still'
cat test_battery.sh | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --child-silent-after-fork=no --suppressions=readline.supp ./bash |& grep -v ' 0 bytes' | grep -B2 -A500 'in use at exit' | cat > valgrind.log
if [ "$(cat valgrind.log)" = "" ]
then
	echo -e "\e[34mOK\e[0m"
else
	cat valgrind.log
fi
echo -e "\e[93mOpen file descriptors:\e[0m"
cat test_battery.sh | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --child-silent-after-fork=no --suppressions=readline.supp ./bash |& grep -B1 -A1 'Open file descriptor' | cat > valgrind.log
if [ "$(cat valgrind.log)" = "" ]
then
	echo -e "\e[34mOK\e[0m"
else
	cat valgrind.log
fi
echo -e "\e[93mValgrind issues:\e[0m"
cat test_battery.sh | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --child-silent-after-fork=no --suppressions=readline.supp ./bash |& grep -B1 -A1 -E '(at |by )0x' | cat > valgrind.log
if [ "$(cat valgrind.log)" = "" ]
then
	echo -e "\e[34mOK\e[0m"
else
	cat valgrind.log
fi

rm minishell_output.log bash_output.log minishell_error.log bash_error.log bash valgrind.log
