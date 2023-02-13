# Kedit

Kedit is an experimental text editor written from scratch. Instead of a dynamic
array, rope, piece table, or a gapped array, Kedit uses a (what I call)
"gapped segmented circular doubly linked list" (I am still brainstorming names)
for it's buffer. BTW, I thought of the buffer myself (I'm trying to flex).
Please see the file `~/BIBLIOGRAPHY.md` for scholarly work that I have used for
research.

## The "gapped segmented circular doubly linked list"

Note that the "circular" part is still not implemented as well as a lot of the
features.

The data structure used for the buffer is a circular doubly linked list with
each node having a preallocated space for data. The data structure is also
accompanied by a cursor that is used for insertion, deletion, and traversing
the buffer.

When an insertion is made, several paths can happen based on the cursor's
position.

* If the cursor is within a segment's mass, the cursor will split the segment
  from it's index onwards.
* If the cursor is holding onto a segment's mass ($-1$th index of the
  segment's mass), the cursor will drop to the prior segment if the prior
  segment is empty; otherwise, the cursor will create a new prior segment, and
  drop to the new prior segment.

TODO: Other paths.

For every path, the cursor simply appends the segment's mass after an other
procedure.

Delting also has several paths.

TODO: Paths.
   
TODO: THIS ENTIRE SECTION

**WARNING**: My big O notation is not very good, nor me explaining things.

Since the time complexity of heap allocations are important, the time
complexity of a heap allocation will be denoted with the constant $A$, and, for
simplicity, previous heap allocations will not be considered.

### Time complexity

TODO: Base the time complexities off of movement. F.e. include the time it
takes to drop down to a segment after prepending a segment.

TODO: Account for gaps when inserting, deleting, and traversing.

#### Time time complexity of traversing.

* Traversing the buffer has the time complexity of $O(\log{-i + n + 1})$ moving
  forwards, and
* $O(\log{i + 1})$ moving backwards.

#### The time complexity of inserting.

* inserting a string when the cursor is hanging in it's is
  $O(A\lceil\frac{n-m+o}{m}\rceil+n)$;
* inserting a string within the mass of the cursor's segment is
  $O(A\lceil\frac{n-m+o}{m}\rceil+o-i-1+n)$; and
* inserting a string when the cursor is holding onto it's segment is
  $O(A\lceil\frac{n-m+o}{m}\rceil+A+n)$;

#### The time complexity of deleting.

* Deleting a string has the time complexity of $O(n)$. 

### Space complexity

$O(n \mod{m} + n)$, where $m$ is the capacity of a segment.

TDOJF:LKSJD;fk

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
