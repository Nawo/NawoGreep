# NawoGrep

Recruitment task

# The program in C++ which behaves quite like grep -r.

The program has one obligatory parameter which is <pattern (a string)> and can have four optional parameters (if user does not specify then default values are used).

Additional parameters:

| Parameter           | Description                                                                | default value        |
| :------------------ | :------------------------------------------------------------------------- | :------------------- |
| -d or --dir         | start directory where program needs to look for files (also in subfolders) | current directory    |
| -l or --log_file    | name of the log file                                                       | \<program name>\.log |
| -r or --result_file | name of the file where result is given                                     | \<program name>\.txt |
| -t or --threads     | number of threads in the pool                                              | 4                    |

The program executed without obligatory parameter return information on console how to use it and stopped. The program use **threads pool** to process files.

When the program finished his work, it give following **output on console**:

Searched files: \<nr of files found in directory and subdirectories>

Files with pattern: \<nr of files where the patten was found>

Patterns number: \<Number of patterns found in all files>

Result file: \<path to result file>

Log file: \<path to log file>

Used threads: \<nr of threads>

Elapsed time: \<time elapsed from beginning to the end of the program>

Program produces two files; **the result file** look as below:

\<file path>\:\<line number>\: line content

\<file path>\:\<line number>\: line content

it contain list of all files (with line number and line content) where the pattern was found, sorted from the file where most patterns were found, and ending were least.

**The log file** contain list of thread ids and file names which its processed, sorted from the thread id with the most files number and ending with the least, (here are only file names, not full paths to files).

\<thread id>\:\<file name>\,\<file name>\, ...

\<thread id>\:\<file name>\,\<file name>\, ...
