TODO: Explain the adapted form of Big $O$ notation better, or look for a better
version of Big $O$ notation that satisfies my needs.

TODO: Define the variables for each expression.

# Kedit

Kedit is an experimental text editor written from scratch. Instead of a dynamic
array, rope, piece table, or a gapped array, Kedit uses a gapped doubly
circular linked list for it's buffer.

## The buffer

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
