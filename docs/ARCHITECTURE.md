# Architecture

## 1. Frontend

```text
source.c
  -> Flex
  -> tokens
  -> Bison
  -> AST / semantic actions
```

Part 1 focuses on lexical analysis. Part 2 builds a parse-tree/AST representation. Part 3 performs semantic translation into machine-independent 3-address code.

## 2. TinyC 3AC

A quad has:

```text
op  arg1  arg2  result
```

Examples:

```text
t1 = i + 1
t2 = a[t1]
if t2 < n goto L1
goto L2
```

The same representation is useful for simulation, optimization and later target-specific code generation.

## 3. Backend

```text
3AC
  -> instruction selection
  -> simple stack-slot allocation
  -> x86-64 assembly
  -> assembler/linker
```

The current backend is intentionally educational and integer-only.

## 4. Vectorization extension

The planned BTP extension is:

```text
3AC
  -> loop analysis
  -> dependence analysis
  -> vectorization
  -> Vector IR
  -> target SIMD instructions
```

The first useful vectorization target is an element-wise loop such as `c[i] = a[i] + b[i]`.


### Array/pointer indexing in the current 3AC

The translator lowers `a[i]` and `p[i]` to address arithmetic followed by pointer load/store quads (`=*` / `*=`). This is semantically equivalent to indexed copy, and is convenient for the later dependence/vectorization work because the effective address is explicit.
