# Virtual Machine
Basic virtual machine based on Von Neumann architecture. Implemented specific assembler that compiles code into binary file that could be executed by virtual machine.

## Usage
`vnvm assembler <code.asm> [out.bin]` - build binary file from assembler code
`vnvm exec <filename.bin>` - execute binary file

## Assembler
### List of assembler comands:

Commands take variables, constants or functions' names as arguments. In commands with two arguments (except `load`) first argument has only one byte to code address of variable and second argument has two bytes to code address. So first variable should be defined as global, otherwise an error is likely to appear.

- `push <src>` - push `src` to programm stack
- `pop <dst>` - pop the top of programm stack and write it to `dst`
- `call <function>` - push instruction pointer of next line to programm stack and call `function`
- `exec <function>` - just call `function` without pushing anything to stack
- `copy <src> <dst>` - copy from `src` to `dst`
- `load <src> <dst>` - copy from `src` to `dst`, however here `src` has larger address that `dst`
- `add <src> <dst>` - add `src` to `dst` and write result to `dst`
- `sub <src> <dst>` - substract `src` from `dst` and write result to `dst`
- `isequal <var1> <var2>` - push the result of `var1 == var2` to programm stack
- `if <flag>` - if `flag` is true (equals `1`), then execute next line, else skip next line
- `return` - pop the top of programm stack and return to the address written in popped top
- `exit` - exit programm, the return code is written to instruction pointer
- `print <var>` - print integer variable `var` on new line
- `printconst <const>` - print string constant `const` on new line
- `scan <var>` - read integer variable `var` from input

### Code structure:
```
var {
    var 0
    another_var 17
    ...
}
const {
    string Here's a string
    hello_world Hello, World!
    ...
}
def {
    function {
        static {
            static_var 10
            ...
        }
        <commands>
        ...
        push &static_var
        add var &static_var
        ...
        return
    }
    ...
    main {
        static {
            static_var 15
            ...
        }
        <commands>
        ...
        exit
    }
}
```
- `var { ... }` - definition of global integer variables, template for each line is `<name> <value>`
- `const { ... }` - definition of global string constants, template for each line is `<name> <string>`
-  `def { ... }` - definition of functions, including `main`, which is an entry point of programm
    - `<function_name> { ... }` - definition of function `function_name`
        - `static { ... }` - definition of static integer variables, template for each line is `<name> <value>`
        - the rest part of function is its code, static variables should be used in code with `&` before name
