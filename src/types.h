#pragma once

#include <stdbool.h>
#include <stddef.h>

#define KILOBYTES(x) (x * 1024)
#define MEGABYTES(x) (KILOBYTES(x) * 1024)

typedef unsigned char		byte;
typedef unsigned int		u32;
typedef int					s32;
typedef unsigned long long  u64;
typedef long long			s64;
typedef float				f32;
typedef double				f64;
