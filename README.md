# Kedit

Kedit is an experimental text editor written from scratch. Instead of a dynamic
array, a rope, or a gapped array, Kedit uses a (what I call) "pointed segmented
doubly linked list" for it's buffer.

The buffer is inspired by paged file systems, and utilizes the idea of
preallocated memory per node. This ensures swift string manipulation per
in-segment appendation. For more specifics on the algorithms, go to the
[Algorithm](#algorithm) section.

Also, I know I broke many modern C++ rules. I don't give a shit. But here are
some rules I broke:

* Using C casts to omit the const type qualifier;
* using the "new" keyword
* using 8-width tabs (tabs > spaces, fight me);
* using the `hpp` as a file extension rather than `h`;
* not using inheritance ([I fucking hate inheritance](#why-i-hate-inheritance));
* using raw pointers; and
* not using the C++ standard library (I use the C standard library).

## Why I hate inheritance

I don't need to give a reason why. If you don't know why, you probably work for
a megacorp., or use some scripting/GC language that abstracts everything
(e.g. Python, TypeScript, and C#). I am pure; I like imperative, Rust-like
OOP, and funtional (never logical) programming langauges.

I understand that I'm using C++ (an OOP language), but that doesn't mean I
can't not use all it's features.

I like C, but I hate the way I need to name things. For example,
"buffer_segment_write" or "write_to_buffer_segment" for procedure names (I
prefer the right one), and "BUFFER_FLAG_WRITABLE" or "WRITABLE_BUFFER_FLAG" for
enum names. I fucking hate the naming. And you can't specify the type size of
an enum. And there are no namespaces so library procedures are like
"lib_shit_your_pants". And the fact that there is not standard in naming shit.

## Building

**WARNING**: This project is currently being developed under the pure support
of UNIX-like operating systems; this project won't work on Windows once usable.

1. First, you must have these installed:
	* a C++ compiler that is capable of all C++20 language features
	  (f.e. `clang++`, and `g++`), and
	* GNU Make.
2. Run `make`.
