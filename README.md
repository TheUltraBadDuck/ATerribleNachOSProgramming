
# A Terrible NachOS 4.0 Programme

Currently the project is done mostly for the project in my university.
You can see some of the detail in the code and even use it (just
do not copy completely if you do not understand, and you know, plagilarism)

This site is for ones who have contacted NachOS for a long time.



## Requirements

- Ubuntu 18.04 is recommended over 20.04.
- GCC 4.8.5 and G++ 4.8.5.
- A little of your luck (Honestly).
## How to compile

#### Step 1: make sure your directory is in folder /code

```http
  YourUserName@YourMachine/nachos/NaachOS-4.0/code:
```
#### Step 2: make a file for compiling

```http
  .../code: cd test
```

```http
  .../code/test: make
```
#### Step 3: then update the machine

```http
  .../code/test: cd ../build/linux
```

```http
  .../code/build.linux: make
```

#### Step 4: compile it (in this example, run 'halt' in folder /build.linux)

```http
  .../code/build.linux: ./nachos -x ../test/halt
```

If 'error' word does not exist, or it runs Machine Halting, you succeed in getting into the problems.
## About error

There is quite such some errors the compiler can occur.
There are some chances to meet them:

#### Check the version

If you use Ubuntu 20.04 and you have no experience about the
Linux operating system, you have to reinstall the older version.

#### Unexpected system call

The problem is inside the NachOS. Try to do all of these:

- Add a connection between a function and macro in `userprog/syscall.h` by adding some lines in `test/start.s`.
- Run the terminal and compile again.
- Check the typo of related lines.

Sometimes, your code is correct, but error can be made. In this
case, delete the syscall, then gradually add then, until you find the
problem.

#### Run 'make' in `/test` but the .c file does not create any.

Change some data in `test/Makefile`.

#### Parse error before ... in `test`

This is quite difficult. In the main function, do **declare variables**
at the first lines WITHOUT initalize anything. Before that, no calculation
is made. After that, no new variables appear, instead you can initalize and
use functions.

- For `char[]`: Recommend to use static array (such as `char array[10]`)
- For `bool`: this type is not available, so use `char` instead

#### Programme stop writing

If your programme stops like this, that means it is stuck in loop.

- Check any loop in the programme.
- You may forget to add `IncreasePC()` after syscall.
- You may add more than one `IncreasePC()`.
- Check the main function if it has `Halt()` or not.
- Sometimes, if you let the programme wait for inputting too long, it will also crash (!?)

#### Aborted(core dumped)

Problem with pointer.

#### And other problems you can meet!

Do this (or else) if you cannot see any problem above:

- Reinstall NachOS normally like the tutorial, keep the important code to somewhere else.
- Fix some codes (not recommend).
- Include libraries (Re-include).
- See how to fix by Google search.
- Wish your programme to compile successfully.
