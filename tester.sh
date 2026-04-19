#!/bin/bash

make bash

echo 'Executing minishell...'
< test_battery.sh ./bash >minishell_output.log 2>minishell_error.log
echo 'Executing bash...'
< test_battery.sh source test_battery.sh >bash_output.log 2>bash_error.log

echo 'Output diff:'
diff --color=auto minishell_output.log bash_output.log
echo 'Error diff:'
diff --color=auto minishell_error.log bash_error.log

echo 'Executing with valgrind...'

cat test_battery.sh | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --child-silent-after-fork=no --suppressions=readline.supp --log-file=valgrind.log ./bash >/dev/null 2>&1
cat valgrind.log | grep -v '4 open' | grep -B5 -A5 -E 'are definitely|are still|FILE DESCRIPTORS'
cat valgrind.log | grep -B5 -A5 ft