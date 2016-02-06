# Unit tests for project 1

This repo contains a pretty comprehensive suite of unit tests for your project
1 implementation. If they all pass, you should feel really confident in what you
submit. =)

...and yes, they're written in Ruby using RSpec. This takes advantage of the
ability to pipe input to a binary (sound familiar?), like this:

```
echo 'echo yay for piping' | ./MyShell
```

## Installation

To be able to run these tests, you need to install a couple Ruby gems and update
your `$PATH` variable.

```
gem install --user-install rspec open4
```

Appends this to your `~/.bashrc` file:

```
if which ruby >/dev/null && which gem >/dev/null; then
  PATH="$(ruby -rubygems -e 'puts Gem.user_dir')/bin:$PATH"
fi
```

If you're running these commands on your own machine, you can install the gems
to the system directory if you want and skip the `.bashrc` change. Your call.

To check that everything's set up correctly, verify that running the following
command shows both rspec and open4:

```
gem list
```

Finally, you need to clone this repository somewhere. My commands below you
clone it in your `$HOME` directory, so make sure to update the commands
appropriately if you put it somewhere else.

## Running the tests

First, make sure you've built your shell program. Then, from within the root of
your repository, run the following command:

```
SHELL_DIR=$(pwd) rspec ~/project-1-specs/spec/shell_spec.rb -c
```

If your shell binary is called something other than 'MyShell', you also have to
prefix the above command with `SHELL_NAME=bin_name`, like this:

```
SHELL_NAME=shell SHELL_DIR=$(pwd) rspec ~/project-1-specs/spec/shell_spec.rb -c
```

Remember: if you cloned this repo somewhere other than `~/project-1-specs`, you
have to update the path to the repository for these commands to work.

## More options

If you want to use the documentation formatter instead (to give the same format
of output that I put in your deliverable feedback), the append the following to
the command to run the tests:

`-fd`

If you only want to run the tests for a specific deliverable, append one of the
following to the command:

```
# Only run tests for deliverable 1
--tag d1

# Only run tests for deliverable 2 that are not also part of deliverable 1
--tag d2

# Run all tests for deliverables 1 and 2
--tag d1 --tag d2

# only run tests for the final deliverable (not d1 or d2)
--tag df
```

Without a tag, all tests will be run.

# Disclaimer

Just like me, these tests are far from perfect. They make some assumptions that
may not be true in your shell, even though you're still adhering to the
requirements. I tried my best to make them general, but feel free to submit a
pull request, or comment out tests, if you find errors.

  1. Your prompt must start with the user's username, from the very first
     character (though I strip out ANSI escape sequences for color)

  2. Some tests depend on other functionality working correctly when testing
     other features. For example, my tests for `ls` require `pwd` to be working
     correctly, and the tests for the prompt assume that `ls /bad` correctly
     returns a non-zero status code.

  3. You have to return the correct exit codes from running child processes. See
     https://piazza.com/class/ii5r7hikwh0vu?cid=56 for how to do this.

If you submit code and there are failing tests, does that mean you automatically
lose points? Nope. It just means that the grader has to manually test a lot of
your shell code, which will probably be irritating if everyone elses' tests all
pass. Try your best to get them passing, but remember that there's a reasonable
human being who will actually be grading your work.

If there are failing tests in the code you submit, I encourage you to write a
quick explanation in your README if you think the test is misleading.
