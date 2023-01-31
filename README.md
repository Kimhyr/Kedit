# Kedit

Kedit is an experimental text editor written from scratch (that is, without a
C++ standard library). Instead of a dynamic array, a rope, or a gapped array,
Kedit uses a (what I call) "pointed segmented doubly linked list" for it's buffer.

The buffer is inspired by paged file systems, and utilizes the idea of
preallocated memory per node. This ensures swift string manipulation per
in-segment appendation. For more specifics on the algorithms, go to the
[Algorithm](#algorithm) section.

## Algorithm

### Inserting

`Source/Buffer.cpp: Void BufferCursor::write(Byte datum) noexcept`

```cpp
if (this->segment_.full()) {
	this->segment_ = *new BufferSegment(this->segment_);
	this->index_ = 0;
} else if (this->index_ == 0) {
	this->segment_.prepend(*new BufferSegment());
	this->segment_ = *this->segment_.prior();
} else if (!this->atEndOfSegment())
	this->segment_.split(this->index_);
this->segment_.write(datum);
++this->index_;
if (datum == '\n') {
	++this->position_.row;
	this->position_.column = 0;
}
++this->position_.column;
this->column_ = this->position_.column;
```

#### Insert at the start of the pointed segment.

1. Attach a new segment before the pointed segment; then,
2. switch the pointed segment to the new segment; finally,
3. insert the char in the pointed segment (at index `0`).

```
segment = (h, i, !, 0, 0, 0, 0, 0)
insert_start(Z) = (Z, 0, 0, 0, 0, 0, 0, 0)->(h, i, !, 0, 0, 0, 0, 0)
```

#### Insert within the pointed segment.

1. Split the pointed segment from the pointed segment index forwards; then,
2. append the char to the pointed segment.

```
segment = (h, i, !, 0, 0, 0, 0, 0)
insert_within(Z) = (h, Z, 0, 0, 0, 0, 0, 0)->(i, !, 0, 0, 0, 0, 0, 0)
```

#### Insert at the end of the pointed segment.

1. Append the char to the pointed segment.

```
segment = (h, i, !, 0, 0, 0, 0, 0)
insert_end(Z) = (h, i, !, Z, 0, 0, 0, 0)
```

#### Insert a full pointed segment.

1. Attach a new segment after the pointed segment; then,
2. switch the pointed segment to the new segment; then,
3. update the pointed segment index to 0; finally,
4. insert the char in the pointed segment.

```
segment = (h, i, !, !, !, !, !, !)
insert_full(Z) = (h, i, !, !, !, !, !, !)->(Z, 0, 0, 0, 0, 0, 0, 0)
```

### Deleting

`Source/Buffer.cpp: Bool BufferCursor::erase(Byte eraser)`

```cpp
	
```

### Traversing

## Building

**WARNING**: This project is currently being developed under the pure support
of UNIX operating systems; this project won't work on Windows once usable.

1. First, you must have these installed:
	* a C++ compiler that is capable of all C++20 language features
	  (f.e. `clang++`, and `g++`), and
	* GNU Make.

2. Run `make`.
