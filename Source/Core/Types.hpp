#pragma once
#ifndef KEDIT_TYPES_HPP
#define KEDIT_TYPES_HPP

using Void = void;

using Int8 = signed char;
using Int16 = signed short int;
using Int32 = signed int;
using Int64 = signed long int;

using Nat8 = unsigned char;
using Nat16 = unsigned short int;
using Nat32 = unsigned int;
using Nat64 = unsigned long int;

using Real32 = float;
using Real64 = double;

using Bit = bool;
using Bit8 =  Nat8;
using Bit16 = Nat16;
using Bit32 = Nat32;
using Bit64 = Nat64;

using Byte = char;
using Byte8 = char;
using Byte16 = char16_t;
using Byte32 = char32_t;
using Byte64 = Nat64;

#if defined(ARCH_86_64)
	using Int = Int64;
	using Nat = Nat64;
	using Real = Real64;
#else
	using Int = Int32;
	using Nat = Nat32;
	using Real = Real32;
#endif

#endif
