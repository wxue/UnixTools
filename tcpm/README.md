Trivially Copy File (mmap & memcpy)
=========

This program is written during the class CS631, Advanced Programming 
in the UNIX Environment, Fall 2013, SIT.

HW#2: trivially copy a file - extra

According to Jan Schaumann(instructor): "The objective of this assignment 
is for you to write your first program and get comfortable developing a tool 
from a given specification."

[Original Assignment](http://www.cs.stevens.edu/~jschauma/631A/f13-hw2.html)

[Github](https://github.com/jschauma/cs631apue)

Difference To tcp
---------

This program uses mmap and memcpy instead of read and write to implement the copy.

Modify .bash
---------

For convenient and to makes it more like a command tool, you can add
the current path to .bash.
Run the command below in the terminal: (replace "/currentpath/directory"
(without the quote) with the directory path where this program located.)

`export PATH=/currentpath/directory:$PATH`

You can use command:
`echo $PATH`
to check if it is added correctly.

Don't worry, this changes the PATH temperately. So if you open a new terminal
you need to do this again or write it into .bash permanently(not recommended).

SYNOPSIS
---------

`tcpm source_file target_file`

`tcpm source_file ... target_directory`

EXAMPLES
---------

`tcpm file1 file2`

`tcpm file1 dir`

`tcpm file1 dir/file2`

`tcpm file1 dir/subdir/subsubdir/file2`
