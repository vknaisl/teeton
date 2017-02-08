# Introduction

# Installation

Once you have copy of this repository, you can run `make build` to build teeton executable.
Then it is a good idea to run `make test` to ensure that everything works correctly.

If everything is fine you can use `make run` to open teeton console. By running `make install`
the teeton executable is copied into `/usr/local/bin` and can be used directly in terminal.
If you want to get rid of it, simply run `make uninstall`.

# Usage

You can use teeton either in console mode or to run teeton programs.

## Console mode

Console mode can be used by running teeton executable without any arguments.

```
$ teeton
TEETON console 
use ctrl + C to exit
T> 1 + 2

3

```

Every statement is evaluated after 2 newlines.

## Running programs

Once you have written your program, you can run it with teeton. Simply give teeton executable
program path as an argument.

```
$ teeton my_program.ttn
```

You can see some programs in `examples` folder.


# Language

## Types

Teeton supports 4 types.

### Int

Int represents positive and negative whole numbers. Teeton does not support real numbers.

```
x = 10
y = -5
```

### Char

Char represents single character. Must be enclosed in single quotes.

```
a = 'a'
```

### Bool

Represents boolean value. Two keywords - `True` and `False` are used.

```
a = True
b = False
```

### Lists

Lists can consist of other types. There are functions for manipulating the lists (see below).
You can create empty list using `[]`:

```
my_list = []
```

Strings are represented as a list of chars. You can create a list of chars using double quotes.

```
my_list_of_chars = "Hello Teeton!"
```

## Variables

Variables are defined with variable name and expression that will be reduced to a value.

```
<variable name> = <expression>
```

Same name can be used to redefine the variable. You can even change type.

```
x = 1
x = 'a'
```

## Operators

Teeton has various arithmetic, comparison and logical operators. Here is a table of available
operators and types that can be used with them.

```
+   : (int int) -> int, (list list) -> list
-   : (int int) -> int
*   : (int int) -> int
/   : (int int) -> int
<   : (int int) -> bool, (char char) -> bool, (list list) -> bool
>   : (int int) -> bool, (char char) -> bool, (list list) -> bool
<=  : (int int) -> bool, (char char) -> bool, (list list) -> bool
>=  : (int int) -> bool, (char char) -> bool, (list list) -> bool
==  : (int int) -> bool, (char char) -> bool, (list list) -> bool, (bool bool) -> bool
!=  : (int int) -> bool, (char char) -> bool, (list list) -> bool, (bool bool) -> bool 
!   : (bool) -> bool
&&  : (bool bool) -> bool
||  : (bool bool) -> bool
=== : (any any) -> bool
```

Remember that you can't mix types for binary operators with only exception for `===` which
is reference comparison.

All binary operators are used with infix notation.

```
x = 1 + 2
y = x > 3
```


## Lists

There are several functions for manipulating lists.

```
append : (list any) -> void
get    : (list int) -> any
set    : (list int any) -> void
len    : (list) -> int
```

### append

Appends an element at the end of the list.

```
xs = []
append(xs 2)
append(xs (2 + 8 * 2))
```

### get

Return the element at the given index.

```
word = "hello"
get(word 1) # -> 'e'
```

### set

Set an element at the given index to a new value.

```
word = "hello"
set(word 1 'x') # word now contains "hxllo"
```

### len

Return the length of a list.

```
word = "hello"
len(word) # -> 5
```

## If Else block

You can use if else block in Teeton as follows:

```
if (x < 10) {
    # do something
} else {
    # do something else
}
```

The expression used in if condition must resolve in Bool type. Else branch is compulsory.

## Loops

Teeton supports only while loop:

```
i = 0
while (i < 10) {
    # do something
    i = i + 1
}
```

## User Input

Teeton has few statements to read user input.

### scan_int

Scan int from user input.

```
a = scan_int

# You can use it in any expression
if (scan_int < 10) {
    # ...    
} else {
    # ...
}
```

### scan_char

Scan single character from user input.

```
a = scan_char
```

### scan_string

Scan list of characters from user input.

```
str = scan_string
```

## Output

Teeton has `print` and `println` functions that print to standard output.

```
# print with the final newline
println("Hello Teeton!")

# or without the final newline
x = 2
print("The number is: ")
print(x)
```

## Comments

The hash character `#` is used for commends. Basically everything from `#` to
the end of line is comment.

# Examples

You can find some examples in `examples` folder.

- `hello.ttn` - Simple hello world
- `reverse_string.ttn` - Example of reading string from user input and reversing it
- `is_palindrome.ttn` - Program that detects whether the given string is palindrome
- `bubble_sort.ttn` - Example of bubble sort algorithm in teeton
