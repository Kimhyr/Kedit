Please do not look at the commit history. It's embarassing. :)

# Kedit

Kedit is an experimental text editor written from scratch. Instead of a dynamic
array, rope, piece table, or a gapped array, Kedit uses a (what I call)
"gapped segmented doubly linked list" (I am still brainstorming names) for it's
buffer. BTW, I thought of the buffer myself (I'm trying to flex). Please see
the file `~/BIBLIOGRAPHY.md` for scholarly work that I have used for research.

Also, I know I broke many modern C++ rules. I do not care. But here are
some rules I broke:

* Using C casts to omit the const type qualifier;
* using the `new` keyword;
* using 8-width tabs (tabs > spaces, fight me);
* not making blank lines inside procedures to seperate code;
* ~~using the `hpp` file extension rather than `h` in discordant to the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rl-file-suffix)~~;
* not using inheritance ([I hate inheritance](#why-i-hate-inheritance));
* using raw pointers; and
* not using the C++ standard library (I use the C standard library, except
  `iostream`).

## What's with the buffer?

The process of inserting a string into a rope is too slow. First, you must
locate the requested position in the rope within a $O(\log{n})$ time complexity,
where $n$ is the length of the rope. Then, you must split the string, and
allocate space for both halfs of the string in the heap. Soley due to
allocating space on the heap per insertion is a no-go for me.

The data structure used for the buffer is a circular doubly linked list with
each node having a preallocated space for data. The data structure is also
accompanied by a cursor that is used for insertion, deletion, and traversing
the buffer.

Traversing the buffer has the time complexity of $O(\log{-i + n + 1})$ moving
forwards, and $O(\log{i + 1})$ moving backwards, where $i$ is the index that
the cursor is on, and $n$ is the mass of the buffer.

The time complexity of inserting a string that will not overflow the cursor's
segment when the cursor is hanging in it's segment is $O(n)$, where $n$ is the
length of the string being inserted.

Inserting a string that can overflow the cursor's segment when the cursor is
hanging in it's segment is $O(A \lceil - \frac{m - n - o}{m} \rceil + n)$, where
$A$ is an allocation in the heap, $m$ is the capacity of the segment that the
cursor is on, $n$ is the length of the string, and $o$ is the mass of the
segment.

## Tasks

* [ ] Refactor to using C++20 standard features;
* [x] implement writing and erasing operations;
* [ ] implement movement operations; and
* [ ] come up with an input system.

## Data structures and algorithms

TODO: This section.

As said in the introduction of this repository, the data structure for the
buffer is (what I call) 

## Building

**WARNING**: Ignore this warning for now. I'm only keeping it here for later.
This project is currently being developed under the pure support of UNIX-like
operating systems; this project may not work on Windows once usable.

1. First, you must have these installed:
	* a C++ compiler that is capable of all C++20 language features
	  (f.e. `clang++`, and `g++`), and
	* GNU Make.
2. Run `make`, and the executable, `~/kedit.exe`, will be generated.
