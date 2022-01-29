# CLI Input Handling and Checking

## Description

Check an entry array as input program.
That module will check if a program input array is well formed.

## Steps of checking

### Step 1: Reduce program argument array

The argument array that is received by the program has the belowing form:

```['-g', 'test', '--name=test']```

Which is an array of string. But in order to efficently scan and interpret arguments, we need to create an normalized array first. So take all arguments with the form: `--LABEL=CONTENT` and split them, then take them back to new array to finally have:

```['-g', 'test', '--name', 'test']```

### Step 2: Check syntax

The argument have a syntax that need to be respected.
This syntax is a question of string chain.

Correct syntax:
```['-g', 'test', '--name=test']```

Invalid syntax:
```['-g', 'test', 'test2', '--name=test']```

`test2` is not placed right because not attached to any label.

### Step 3: Check business errors

All program have their own business syntax and rules.
For example the argument `-g` may have only discret element like `component` and `class`. So we need to warn the user that the argument content is not correct.