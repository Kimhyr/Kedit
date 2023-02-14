TODO: Explain the adapted form of Big $O$ notation better, or look for a better
version of Big $O$ notation that satisfies my needs.

TODO: Define the variables for each expression.

# Kedit

Kedit is an experimental text editor written from scratch. Instead of a dynamic
array, rope, piece table, or a gapped array, Kedit uses a gapped doubly
circular linked list for it's buffer (I'm still working on the name of the
buffer).

## The buffer

The reason why I didn't use other data structures used for text buffers is
because they simply didn't satisfy me.

### Problems with other data structures:

The [rope data structure](https://en.wikipedia.org/wiki/Rope_(data_structure)
), which I assume is the most used, is too slow for me. Yes, it has
$O(\log{n})$ concatenation, insertion, appendation, and deletion, but it also
requires a heap allocation of a node containing the string everytime you want
to write into the buffer, and you have to search for the requested location
everytime you want to operate on the buffer. Solely because I'd have to
allocate in the heap for every write operation, the rope triggered me.

The [gap buffer](https://en.wikipedia.org/wiki/Gap_buffer) is array-based,
that's what triggered me.

I actually thought of a similar data structure to the [piece
table](https://en.wikipedia.org/wiki/Piece_table), but I had ideas to port this
text editor to a [Vulkan](https://www.vulkan.org/)-based GUI, and I wanted
a data structure that can performantly interop with a text renderer.

### Disclaimer before THE MATH

The mathematical notation used in the following equations does not use Big $O$
notation. Instead, they use an adapted form of Big $O$ notation to be more
explicit because Big $O$ notation omits important factors, such as, in this
case, an allocation on the heap.

### Time complexity

#### The time complexity of inserting a string:


$$
O\Bigg(\begin{cases}
  i^+ = o, & \begin{cases}
    n \leq m^-, & n \\
    n > m^-, & \lceil\frac{n - m^-}{m}\rceil A(N) + n\\
  \end{cases} \\
  i^+ < o, & \begin{cases}
    n \leq m^-, & A(N) + n \\
    n > m^-, & \lceil\frac{n}{m}\rceil A(N) + n
  \end{cases}
\end{cases}\Bigg)
$$

#### The time complexity of searching for a string:

$$
O\Bigg(\begin{cases}
  \overrightarrow{S}, & n - i^+ \\
  \overleftarrow{S}, & i^+ \\
  \overleftrightarrow{S}, & \frac{n}{2} \\
\end{cases}\Bigg) \\
$$

#### The time complexity for the cursor to jump to a found location:

$$
O(1)
$$

### Space complexity

$$
O(n + m - n \mod{m})
$$

## Building

**WARNING**: Ignore this warning for now. I'm only keeping it here for later.
This project is currently being developed under the pure support of UNIX-like
operating systems; this project may not work on Windows once usable.

1. First, you must have these installed:
	* a C++ compiler that is capable of all C++20 language features
	  (f.e. `clang++`, and `g++`), and
	* GNU Make.
2. Run `make`, and the executable, `~/kedit.exe`, will be generated.
