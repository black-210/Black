# Black language model

Black is not a general-purpose application language. A Black file is a system specification whose declarations lower to C, with explicit low-level operations rather than hidden runtime services.

## Core forms

- `kernel { ... }` marks kernel-oriented declarations.
- `fn name(args) -> type { ... }` declares a function.
- `let x: type = expression;` declares storage.
- `struct Name { field: type; ... }` declares a record layout.
- `enum Name { A, B, C }` declares an enum.
- `extern fn ...;` and `extern name: type;` describe linker-visible symbols.
- `target("x86_64");` and `abi("sysv");` carry build intent into generated C comments.
- `section(".name") fn ...` lowers to a real compiler section attribute.
- `asm("...");` lowers to GCC/Clang extended inline assembly syntax.

## Low-level type forms

`u8/u16/u32/u64`, `i8/i16/i32/i64`, `usize/isize`, `bool`, `void`, pointers `*T`, arrays `[N]T`, named structs, and function types `fn(T1, T2) -> R` are represented in the AST and checked before C generation.

## Safety model

There is no garbage collector, VM, mandatory allocator, or mandatory libc. Pointer dereference, address-of, pointer arithmetic, MMIO, and inline assembly are explicit source operations. `volatile` is preserved in the generated C type/qualifiers. `unsafe` is represented on functions and documents that the function intentionally crosses low-level safety boundaries.

## Architecture strategy

The front end is architecture-neutral. `target`/`abi` metadata and C compiler attributes provide the first target boundary. Architecture-specific startup, linker scripts, interrupt tables, page tables, and boot assembly remain separate build inputs, which prevents the core language from becoming a pile of architecture-specific conditionals.
