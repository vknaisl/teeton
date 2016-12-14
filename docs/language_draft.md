# Language draft

## Types
- int
- char
- bool
- list - it can includes just 1D list of integers, chars or booleans

## Representation of types
```
obj {
	type: [int|char|bool|list]
	props: .....
	value: .....
}
```
## Example representations

### Integer
```
obj {
	type: int
	props: {}
	value: 10
}
```
### Char
```
obj {
	type: char
	props: {}
	value: 'a'
}
```
### Boolean
```
obj {
	type: bool
	props: {}
	value: 1 // True
}
```

### List
```
obj {
	type: list
	props: {
		length: 4
		type: char
	}
	value: ['j','o','h','n']
}
```

## Type definition
```
var a = 4
var b = scan_int()
```
## Build-in functions
```
scan_int() 	   : () -> int
scan_char()    : () -> char
scan_string()  : () -> list of chars
print(...) 	   : () -> nothing
println(...)   : () -> nothing
type() 	  	   : () -> type of variable
list()		   : (...) -> list
create_list()  : (size, fill_value) -> list
len(a)         : (list) -> int
```

## Build-in operators
```
+   : (int, int) -> int, (list, list) -> list
-   : (int, int) -> int
*   : (int, int) -> int
/   : (int, int) -> int
!   : (bool) -> bool
<   : (int, int) -> bool
>   : (int, int) -> bool
<=  : (int, int) -> bool
>=  : (int, int) -> bool
==  : (int, int) -> bool, (char, char) -> bool, (list, list) -> bool, (string, string) -> bool
=== : (int, int) -> bool, (char, char) -> bool, (list, list) -> bool, (string, string) -> bool
```

## If statement
```
if (a == 'b') {

} else {

}
```

## Loop
```
while (a == 'b') {
	if (a == 'c') {
		break
	}
}
```

## Comments
```
# create list 
var a = ['j','o','h','n']
```

## Example of code

### Bad definition of type
```
var a // error
```

### Basic computing
```
var a = 1
var b = 2
var c = a + b
print(c) // 3
```

### Work with strings
```
var a = list('j', 'o', 'h', 'n')
var b = "john"
print(a == b) // True
print(a === b) // False
```

### Find out type of variable
```
var a = 2
print(type(a)) // int
```

### Work with list
```
var array = create_list(5, 0)
array[2] = 5
print(array[0]) // 0
print(array[2]) // 5
```