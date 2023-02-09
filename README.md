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
locate the requested position in the rope within a *O(log(n))* time complexity;
then, you must split the string, and allocate space for both halfs of the
string in the heap. Soley due to allocating space on the heap per insertion is
a no-go for me.

The data structure used for the buffer is a circular doubly linked list with
each node having a preallocated space (perferably 160 bytes+) for data. The
data structure is also accompanied by a cursor that is used for insertion,
deletion, and traversing the buffer.

The use of the cursor and the buffer being circular means that traversing will
never take a time complexity of $O(n)$, where $n$ is the mass of the buffer,
instead it will take $O(\log{n - i + 1}$ moving forwards, and $O(\log{i + 1})$
moving backwards, where $n$ is the mass of the bfufer, and $i$ is the index
that the cursor is on.

The time complexity of inserting a string when the cursor is hanging in it's
segment is $O(n)$, where $n$ is the length of the string being inserted.
Inserting a string that can overflow the cursor's segment when the cursor is
hanging in it's segment is
$O(m - o + (n - (m - o)) + A * \lceil \frac{n - (m - o)}{m} \rceil )$, where $m$ is the
capacity of the segment that the cursor is on, $o$ is the mass of the segment
that the cursor is on, $n$ is the length of the string being inserted, and $A$
is a heap allocation. Inserting within the cursor's segment's mass is 


## Tasks

* [ ] Refactor to using C++20 standard features;
* [x] implement writing and erasing operations;
* [ ] implement movement operations; and
* [ ] come up with an input system.

## Why I hate inheritance

I don't need to give a reason why. If you don't know why, you probably work for
a megacorp., or use some scripting/GC language that abstracts everything
(e.g. Python, TypeScript, and C#). I am pure; I like imperative, Rust-like
OOP, and funtional (never logical) programming languages.

I understand that I'm using C++ (an object oriented language), but that doesn't
mean I have to use some object oriented features.

I like C, but I hate the way I need to name things. For example,
`buffer_segment_write` or `write_to_buffer_segment` for procedure names (I
prefer the second name), and `BUFFER_FLAG_WRITABLE` or `WRITABLE_BUFFER_FLAG`
for enum names (second one also). I hate the naming. And you can't specify the
type size of an enum. And there are no namespaces so library procedures are
like `lib_poop_your_pants`. And the fact that there is no standard in naming
things.

If you read my code, yes, I understand that the way I name things are pretty
bad. It's even worse knowing that I spend a quite a bit of time figuring out
some of the names I used.

## Data structures and algorithms

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
