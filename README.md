# Simple makefile

If writing a `makefile` isn't your strong suit, you can use the one from this
repo. It's simple and takes care of the following:

 - `make` will compile everything under the `src/` directory.
    - By default, the binary is called `simulator` and is placed in the root
      of your repository.
    - You can have any number of subdirectories, called whatever you want.
    - Dependencies are automatically wired up, so if you change a file, `make`
      will only rebuild the things that changed as a result.

 - All the intermediate objects and artifacts are placed in an auto-generated
   `bin` directory.

 - `make clean` will remove the binary (`simulator`) and the `bin/` directory.

 - Uses C++11 by default.

 - Compiles with debugging symbols (`-g`) so you can debug your program.

There's also a `.gitignore` file that ignores the appropriate things. Feel free
to use it. Or not.

Hope it helps!
