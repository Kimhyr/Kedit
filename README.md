# Kedit

Kedit is an experimental text editor written from scratch (that is, without a
C++ standard library). Instead of a dynamic array, rope, or a gapped array,
Kedit uses a segmented doubly linked list for it's buffer.

The buffer is inspired by paged file systems, and utilizes the idea of
preallocated memory per node. This ensures swift string manipulation because
you don't need to reallocate data per insertion like a gapped or
dynamic array. Like a rope, insertion 

## Synopsis

### Insertion

#### Inserting at the start of the pointed segment.
1. Attach a new segment before the pointed segment; then,
2. switch the pointed segment to the new segment; finally,
3. insert the char in the pointed segment (at index `0`).

```
segment = (h, i, !, 0, 0, 0, 0, 0)
insert_start(Z) = (Z, 0, 0, 0, 0, 0, 0, 0)->(h, i, !, 0, 0, 0, 0, 0)
```

#### Inserting within the pointed segment.
1. Split the pointed segment from the pointed segment index forwards; then,
2. append the char to the pointed segment.

```
segment = (h, i, !, 0, 0, 0, 0, 0)
insert_within(Z) = (h, Z, 0, 0, 0, 0, 0, 0)->(i, !, 0, 0, 0, 0, 0, 0)
```

#### Inserting at the end of the pointed segment.
1. Append the char to the pointed segment.

```
segment = (h, i, !, 0, 0, 0, 0, 0)
insert_end(Z) = (h, i, !, Z, 0, 0, 0, 0)
```

#### Inserting a full pointed segment.
1. Attach a new segment after the pointed segment; then,
2. switch the pointed segment to the new segment; then,
3. update the pointed segment index to 0; finally,
4. insert the char in the pointed segment.

```
segment = (h, i, !, !, !, !, !, !)
insert_full(Z) = (h, i, !, !, !, !, !, !)->(Z, 0, 0, 0, 0, 0, 0, 0)
```

### Deletion

## Building

**WARNING**: This project *probably* only works on Linux for right now.

1. First, you must have these installed:
	* a C++ compiler that is capable of all C++20 language features
	  (f.e. `clang++`, and `g++`), and
	* GNU Make.

2. Run `make`.
