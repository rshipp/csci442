# Project 1: `partition_tokens`

Read the Piazza post here:

https://piazza.com/class/ii5r7hikwh0vu?cid=61

## How to use it

To run this program from the command line, type:
```
  make run
```

The program will wait for input. Type whatever you want, followed by a new
line, and then press CTRL-D to send the end-of-file (EOF) character to stop
reading. The program will then try to partition the input and print the
results.

Alternatively, you can save your command to a file so you don't have to keep
typing the same thing again and again while you work. Assuming the file is
called 'input.txt', first run 'make' and then do:
```
  ./partition_tokens < input.txt
```

There is also a suite of unit tests that you can use to help you implement
your partition_tokens function. To run them, type:
```
  make test
```

You'll see pass / fail results for all the tests. Work on fixing them one at a
time, and before you know it, you'll have an implementation in which you can
be extremely confident. =)
