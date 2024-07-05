#ifndef _WIN_64
static_assert(true, "Mars only supports 64-bit Windows");
#endif

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8  = char;
using i16 = short;
using i32 = int;
using i64 = long long;

using f32 = float;
using f64 = double;

using Size = size_t;

static_assert(sizeof(u8)  == 1, "Expected to be 1 byte");
static_assert(sizeof(u16) == 2, "Expected to be 2 bytes");
static_assert(sizeof(u32) == 4, "Expected to be 4 bytes");
static_assert(sizeof(u64) == 8, "Expected to be 8 bytes");

static_assert(sizeof(i8)  == 1, "Expected to be 1 byte");
static_assert(sizeof(i16) == 2, "Expected to be 2 bytes");
static_assert(sizeof(i32) == 4, "Expected to be 4 bytes");
static_assert(sizeof(i64) == 8, "Expected to be 8 bytes");

static_assert(sizeof(f32) == 4, "Expected to be 4 bytes");
static_assert(sizeof(f64) == 8, "Expected to be 8 bytes");

static_assert(sizeof(Size) == 8, "Expected to be 8 bytes");

#ifdef MARS_EXPORT
#define MARS_API __declspec(dllexport)
#else
#define MARS_API __declspec(dllimport)
#endif
