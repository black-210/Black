# Black
<<<<<<< HEAD
Black is a specification-oriented systems programming language and compiler for kernels, operating systems, firmware, bare-metal systems, compilers, and low-level tooling.
=======

Black is a specification-oriented systems programming language and compiler. Its job is to describe low-level system behavior cleanly, then lower that description into ordinary C11 suitable for a freestanding C toolchain.

## Scope

Black targets kernels, operating systems, firmware, boot code, drivers, kernel modules, bare-metal programs, memory managers, MMIO, ABI boundaries, linker-facing declarations, and compiler/toolchain construction. It deliberately does not target web, GUI, or ordinary application development.

## Pipeline

`Black source -> lexer -> parser -> AST -> semantic/type checking -> C backend -> C compiler -> object/binary`

The Black compiler itself does not invoke a VM, garbage collector, libc runtime, or OS service.

## Build

```sh
make
make test
```

The compiler is written in portable C11. GCC/Clang are expected for the final C lowering step when building a kernel or firmware image.

## Syntax

```black
kernel {
    fn main() {
        let video: *u8 = 0xB8000;
        *video = 65;
    }
}
```

Functions use `fn`, variables use `let`, and types are explicit. Pointers are written `*T`. Dereference is `*expr` and address-of is `&expr`. Arrays are `[N]T`.

System-oriented qualifiers include `const`, `volatile`, `static`, `extern`, `inline`, and `unsafe`. `target("...")` and `abi("...")` record target/toolchain intent in the generated translation unit.

## Kernel model

`kernel { ... }` marks a program as kernel-oriented metadata. The current C backend emits no libc calls and includes only integer/size definitions. Actual bootstrapping, linker scripts, startup code, page tables, interrupt tables, and architecture assembly belong to the target build description and can be added without changing the core language model.

## Compatibility

The old C12 public API names remain available as aliases/wrappers so existing callers do not immediately break. New code should use the `black_*` API.
>>>>>>> 36a5e66 (Initial Black compiler)
