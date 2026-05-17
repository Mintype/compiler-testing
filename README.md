A small compiler project I’m building while learning:
- C
- x86-64 assembly
- compiler design
- parsing and lexing
- low-level systems programming

The goal is to gradually build a compiled programming language from scratch.


---

# Building

Requirements:
- GCC
- NASM
- Make

Build compiler:

```bash
make
```

Run compiler:

```bash
./compiler examples/test.min > out.asm
```

Assemble:

```bash
nasm -f elf64 out.asm -o out.o
```

Link executable:

```bash
gcc out.o -o out
```

Run generated program:

```bash
./out
```

Check return value:

```bash
echo $?
```