UnixTools
=========
These tools were built up during a class called "Advanced Programming in the UNIX Environment". You can [view the website](http://www.cs.stevens.edu/~jschauma/631A/) of this class. 

Instructor of this class, [Jan Schaumann](https://github.com/jschauma) also put the [materials of this class](https://github.com/jschauma/cs631apue) on github(very cool), you should check it out.

The tools I built here is more like learning tools for UNIX environment programming. Of course, you can use the tools as a real tool as their demonstration, however, you can always find similar but more powerful, much more powerful, tools on you Linux or Mac or other UNIX-like system.

So this may be more interesting for other learners, students of this class, or UNIX fans, but not the ones only seeking for powerful tools. 

However, you are all welcome to use the code and fork this project, and I'm appreciate for any comments related to this project.


## Usage:

#### Fetch the code
You can clone the tools' directories into your computer or download the whole package as a zip file then unzip it.

#### Compile it
Open a terminal, `cd` to the very directory of the tool.

###### Use makefile
* Simply use `make` command. Then, it's done compile and ready to use.

###### Manually
* Please use gcc as the compiler. Use `man gcc` for more details.

Just for the record, my gcc version is 4.2.1.

#### To be more specific
Each of the tools gets their own README file, you can check them out for the usage of each tool.


## Set PATH Variable

In order to use these tools more "Unix-like" in a terminal, you can add the directory path of the tools into the PATH variable. 

##### Set the PATH variable by running the command below:

  1. `export PATH=/tool_path/tool_directory:$PATH`

    * Replace `/tool_path/tool_directory` with the path and the directory of the very tool(the executable file).

  2. Use command `echo $PATH` to check if it is added correctly.

This only changes the PATH variable temperately. So, if you open a new terminal
you need to do this again. 

###### You can also change the PATH variable permanently by modifying `.bashrc` (on Linux) or `/etc/paths` (on Mac OS).


## LICENSE

The code were written by Weiyu Xue < [Email:T-Wind@xiakelite.com](mailto:T-Wind@xiakelite.com) >, base on Jan Schaumann's class at Stevens Institue of Technonlogy and the well-known book by W. Richard Stevens.

> Copyright (c) 2013, Weiyu Xue
All rights reserved.

> Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  > * Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  > * Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  > * Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
