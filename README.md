Please do not look at the commit history. It's embarassing. :)

$\lceil \frac{n - (m - o)}{m} \rceil$

$\log{n - i + 1}$

# Kedit

Kedit is an experimental text editor written from scratch. Instead of a dynamic
array, rope, piece table, or a gapped array, Kedit uses a (what I call)
"gapped segmented doubly linked list" (I am still brainstorming names) for it's
buffer. BTW, I thought of the buffer myself (I'm trying to flex). Please see
the file `~/BIBLIOGRAPHY.md` for scholarly work that I have used for research.

The buffer is inspired by the rope data structure. The problem with the rope is
that it requires a heap allocation per insertion of a string. 

TODO: Finish this paragraph.

The buffer is inspired by paged file systems, and the rop data structure, and
utilizes the idea of preallocated memory per node. This ensures swift string manipulation per
in-segment appendation. For more specifics on the algorithms, go to the
"[Data structures and algorithms](#data-structures-and-algorithms)" section.

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
