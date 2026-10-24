# Common implementation requirements

Use ANSI C89 and the exact public ABI supplied by the project.
Use descriptive names and consistent indentation. Keep functions focused.
Write one operation per line; avoid compressed blocks and unexplained constants.
Use short comments for ownership, invariants and non-obvious decisions, not every line.
Do not change published IID values, method order, signatures or calling conventions.
Obtain unrelated interfaces with QueryInterface, not pointer casts.
Every acquired interface reference needs a matching Release or an explicit ownership transfer.
On failure, clear interface output pointers, return an error and release temporary resources.
Keep the supported interface set static and return one canonical IEcoUnknown.
Support the aggregation explicitly required by ReuseContract.h; reject invalid aggregate creation requests. Keep task algorithms out of the shared framework.

## C89 local declaration syntax

Local declarations precede statements in EACH block. An early-return check is a statement. A new nested block may be used after guards. This unrelated syntax example is not part of the component and must not be copied into Reuse.c:
```c
static int c89_style_example(const int* input) {
    if (input == 0) return 0;
    {
        int value = *input;
        return value;
    }
}
```
Do not initialize a pointer by calling through an allocator before acquiring that allocator.
