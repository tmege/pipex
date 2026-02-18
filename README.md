# pipex

A C implementation of the shell pipe mechanism, replicating the behavior of:

```sh
< infile cmd1 | cmd2 > outfile
```

## About

pipex is a 42 School project that teaches Unix process creation, file descriptors, pipes, and inter-process communication. The program forks two child processes connected by a pipe, each executing a command with its own redirected input/output.

## Usage

```sh
./pipex infile cmd1 cmd2 outfile
```

| Argument  | Description                              |
|-----------|------------------------------------------|
| `infile`  | File used as stdin for `cmd1`            |
| `cmd1`    | First command (reads from `infile`)      |
| `cmd2`    | Second command (reads from `cmd1` output)|
| `outfile` | File where `cmd2` output is written      |

This is equivalent to:

```sh
< infile cmd1 | cmd2 > outfile
```

## Examples

```sh
./pipex /etc/passwd "grep root" "cut -d: -f1" out.txt
# equivalent to: < /etc/passwd grep root | cut -d: -f1 > out.txt

./pipex input.txt "cat" "wc -l" output.txt
# equivalent to: < input.txt cat | wc -l > output.txt

./pipex input.txt "ls -la" "grep .c" output.txt
# equivalent to: < input.txt ls -la | grep .c > output.txt
```

## Compilation

```sh
make        # build the binary
make clean  # remove object files
make fclean # remove object files and binary
make re     # fclean + all
```

## Project structure

```
pipex/
├── src/
│   ├── pipex.c       # main, child processes, pipeline setup
│   ├── utils.c       # error handling, cmd splitting, free helpers
│   └── find_path.c   # PATH resolution for command lookup
├── inc/
│   └── pipex.h       # prototypes and includes
├── obj/              # compiled object files (generated)
├── libft/            # custom C library
├── Makefile
└── README.md
```

## How it works

1. A pipe is created (`pipe()`), giving two file descriptors: read end and write end.
2. **Child 1** opens `infile`, redirects it to stdin, redirects the pipe write end to stdout, then executes `cmd1`.
3. **Child 2** reads from the pipe read end (stdin), redirects stdout to `outfile`, then executes `cmd2`.
4. The parent closes both pipe ends and waits for both children to finish.

## Error handling

- Invalid argument count prints usage and exits.
- Failed `open`, `pipe`, or `fork` calls print a system error and exit.
- Commands not found in `PATH` are reported and the child exits with failure.
