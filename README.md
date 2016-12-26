#scrun

A sample utility to run and test a shellcode on Windows, Linux, macOS (Mac OS X) or other UNIX systems.

### Installation

You must compile the C source of scrun on the target system:

```sh
$ gcc -o scrun scrun.c
```

or use your preferred framework to compile C programs for the target system.

### Usage

```sh
$ scrun shellcode_file
```
