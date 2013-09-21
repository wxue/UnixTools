# Trivially Copy File


This program is written during the class CS631, Advanced Programming 
in the UNIX Environment, Fall 2013, SIT.

HW#2: trivially copy a file

According to Jan Schaumann(instructor): "The objective of this assignment 
is for you to write your first program and get comfortable developing a tool 
from a given specification."

[Original Assignment](http://www.cs.stevens.edu/~jschauma/631A/f13-hw2.html)

[Github](https://github.com/jschauma/cs631apue)

<<<<<<< HEAD
## Synopsis
=======
Modify .bash
---------

For convenience and to makes it more like a command tool, you can add
the current path to .bash.
Run the command below in the terminal: (replace `/currentpath/directory`
 with the directory path where this program located.)

`export PATH=/currentpath/directory:$PATH`

You can use command:
`echo $PATH`
to check if it is added correctly.

Don't worry, this changes the PATH temperately. So if you open a new terminal
you need to do this again or write it into .bash permanently(not recommended in this case).

SYNOPSIS
---------
>>>>>>> 17b5f5d336ac51223bb0f8b3a4901bec2bc3aa56

`tcp source_file target_file`

`tcp source_file ... target_directory`

## Examples

`tcp file1 file2`

`tcp file1 dir`

`tcp file1 dir/file2`

`tcp file1 dir/subdir/subsubdir/file2`
