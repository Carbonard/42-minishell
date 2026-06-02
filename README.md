_This project has been created as part of the 42 curriculum by elangari, rselva-2._

# Minishell

## Description
The goal of this proyect is to implement a simplified version of [bash](https://www.gnu.org/software/bash/) with some restrictions imposed by the 42's curriculum and norm.

### What this program does:
* Display a prompt when waiting for a new command.
* Have a working history.
* Search and launch the right executable (based on the `PATH` variable or using a relative or an absolute path).
* Handle `’` (single quote) which should prevent the shell from interpreting the meta-characters in the quoted sequence.
* Handle `"` (double quote) which should prevent the shell from interpreting the meta-characters in the quoted sequence except for `$` (dollar sign).
* Implement the following redirections:
  * `<` should redirect input.
  * `>` should redirect output.
  * `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen.
  * `>>` should redirect output in append mode.
* Implement pipes (`|` character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
* Handle environment variables (`$` followed by a sequence of characters) which
should expand to their values.
* Handle `$?` which should expand to the exit status of the most recently executed foreground pipeline.
* Handle _ctrl-C_, _ctrl-D_ and _ctrl-\\_ which should behave like in bash. In interactive mode:
  * _ctrl-C_ displays a new prompt on a new line.
  * _ctrl-D_ exits the shell.
  * _ctrl-\\_ does nothing.
* Implement the following built-in commands:
  * `echo`, `echo -n` and a simplified `echo -e`
  * `cd` with only a relative or absolute path and `cd -`
  * `pwd` with no options
  * `export` with no options
  * `unset` with no options
  * `env` with no options or arguments
  * `exit` with no options
* Interpret `&&` and `||` with parenthesis for priorities.
* Wildcards `*` (for the current working directory).

## Instrucctions

### Installation
* For Ubuntu/Debian:
	```
	sudo apt-get update
	sudo apt-get install libreadline-dev
	git clone [url] minishell
	```
### Usage
From the root of the repository:

```
make
```
Compiles the program.
#### Non-interactive
* `./minishell [file]` -> reads from the file
	```
	./minishell test_battery.sh
	```

* `./minishell -c [commands]` -> reads from the first argument after `-c`
	```
	./minishell -c "echo hello world"
	```

#### Interactive
Executes the program then reads the input from _STDIN_.
```
./minishell
``` 

## Resources

### Official documentation
* [Linux man pages](https://man7.org/linux/man-pages/)
* [Signals in bash](https://www.gnu.org/software/bash/manual/html_node/Signals.html)

### Others
* [Languagues and automatons by _Un Profe De Informática_](https://www.youtube.com/playlist?list=PLrsxkaDs20ZTCxLHHe66YHhuOxkwJnSMH)
* [Readline gitbook](https://03-jon-perez.gitbook.io/coding-library/c/readline)  
_As well as multiple Stackoverflow and Reddit articles that would be imposible to track._

### AI usage in this proyect
AI has been used to understand some of bash's behaviours which may seem extrange.