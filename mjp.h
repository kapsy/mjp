#if !defined(MJP_H)

/*
Outline:
   mjp.h 
   v1.00 

   This is my version of Sean Barrett's highly popular stb single header files.

   Contains useful, reusable utilities, types and functions.

   Many ideas, conventions borrowed directly from stb.h, and the Handmade Hero
   series.

Documentation:
   Unit tests:    

Version History
   1.00 Initial version, gathers sporadic useful types and functions from across my projects.

*/

  //////////////////////////////////////////////////////////////////////////////
 //// SECTION: TYPEDEFS ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Here is a change

// Basic typedefs
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef float     r32;
typedef double    r64;

typedef s32       b32;

typedef size_t    size;
typedef size_t    memory_index;
typedef intptr_t  intptr;
typedef uintptr_t uintptr;
typedef uintptr_t umm;

// Vector intrinsics types
// TODO (MJP): SSE support check
typedef __m128 m128;
// TODO (MJP): Remove this
typedef __m128 r128;
typedef __m128i m128i;
typedef __m256 m256;
typedef __m256i m256i;


// Macros and defines
#define function static
#define internal static
#define local_persist static
#define global_variable static

#define R32_MAX FLT_MAX
#define R32_MIN FLT_MAX
#define U32_MAX 0xFFFFFFFF
#define S32_MAX

// TODO (MJP): Is there a way to get these from the system?
#define L2_CACHE_SIZE 64
#define SYSTEM_PAGE_SIZE 4096
#define KiB(s) ((s)*(1LL << 10))
#define MiB(s) ((s)*(1LL << 20))


// Assert defines
#if ASSERTS_ENABLED
#define Assert(Expression) if(!(Expression)) { abort(); }
#define AssertPrint(expr, msg) if (!(expr)) { printf ("%s\n", (msg)); abort(); }
#define AssertNAN(x) Assert(!(x != x));
#define AssertInf(x) Assert((*(u32 *)((void *)&x) != 0xff800000) && (*(u32 *)((void *)&x) != 0x7f800000))
#define AssertUniPolar(x) Assert(((x) >= 0.f) && ((x) <= 1.f))
#define AssertNAN64(x) Assert(!(x != x));
#define AssertInf64(x) Assert((*(u64 *)((void *)&x) != 0xfff0000000000000) && (*(u64 *)((void *)&x) != 0x7ff0000000000000))
#else
#define Assert(exp)
#define AssertPrint(exp, msg)
#define AssertNAN(x)
#define AssertInf(x)
#define AssertUniPolar(x)
#define AssertNAN64(x)
#endif

#define FatalAssert(Expression) if(!(Expression)) { abort(); }

// Zero functions 
inline void
ZeroSize (memory_index Size, void *Ptr)
{
  u8 *Byte = (u8 *)Ptr;
  while(Size--)
  {
    *Byte++ = 0;
  }
}

#define SizeOf(t) sizeof(t)
#define ZeroArray(Array) ZeroSize(SizeOf(Array), Array);
#define ZeroStruct(Instance) ZeroSize(SizeOf(Instance), &(Instance))

#define ArrayCount(Array) (SizeOf(Array)/SizeOf((Array)[0]))

#define InvalidCodePath Assert(!"InvalidCodePath");
#define InvalidDefaultCase default: {InvalidCodePath;} break

inline u32
PointerToU32(void *Pointer)
{
    u32 Result = (u32)((u64)Pointer);
    return(Result);
}

// Flag defines
#define GetFlag(Value, Flag) \
   ((Value) & (Flag))

#define SetFlag(Value, Flag, Switch) \
   (Value) = ((Value) & ~(Flag)) | (Flag)*Switch

#define GetBit(Value, Position) \
   ((Value) & (1 << Position))


// TODO: (Kapsy) Implement the ASM version at github.com/itfrombit/osx_handmade
// See: http://clang.llvm.org/docs/LanguageExtensions.html#builtin-functions
#define __rdtsc __builtin_readcyclecounter

  //////////////////////////////////////////////////////////////////////////////
 //// SECTION STD LIB, INTRINSIC WRAPPERS /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void
MemCopy(void *Dest, void *Source, u32 Size)
{
  memcpy(Dest, Source, Size);
}

// TODO (MJP): If we roll our own can figure out direction simply comparing pointer positions.
inline void
MemMove(void *Dest, void *Source, u32 Size)
{
  memmove(Dest, Source, Size);
}

// NOTE: (Kapsy) Copies in reverse so we can move data right without overwriting
// TODO (MJP) Should really become a MemMove
inline void
MemCopyRev (void *from, void *to, u32 size)
{
    // move one byte back
    // although, this is ugly, will rewrite with simd at some point
    u8 *frombytes = ((u8 *) from) + size - 1;
    u8 *tobytes = ((u8 *) to) + size - 1;

    for(u32 i=0 ; i<size ; i++)
    {
        *tobytes-- = *frombytes--;
    }
}

// TODO: (Kapsy) Switch to this!
inline void
Copy(void *From, void *To, u32 Size)
{
    u8 *FromBytes = (u8 *)From;
    u8 *ToBytes = (u8 *)To;

    while(Size--) { *ToBytes++ = *FromBytes++; }
}

inline s32
SignOf(s32 Value)
{
    s32 Result = (Value >= 0) ? 1 : -1;
    return(Result);
}

inline r32
SquareRoot(r32 A)
{
    r32 Result = sqrtf(A);
    return(Result);
}

inline r64
Pow(r64 Base, r64 Exponent)
{
   r64 Result = pow(Base, Exponent);
   return(Result);
}

inline r32
AbsoluteValue(r32 A)
{
    r32 Result = fabs(A);
    return(Result);
}

//// inline s32
//// AbsoluteValue (s32 x)
//// {
////    return (abs (x));
//// }

inline u32
RotateLeft(u32 Value, s32 Amount)
{
#if COMPILER_MSVC
    u32 Result = _rotl(Value, Amount);
#else
    // TODO: (Kapsy) Other compiler support!
    // u32 Result = rol32(Value, Amount);
    Amount &= 31;
    u32 Result = ((Value << Amount) | (Value >> (32 - Amount)));
#endif
    return(Result);
}

inline u32
RotateRight(u32 Value, s32 Amount)
{
#if COMPILER_MSVC
    u32 Result = _rotr(Value, Amount);
#else
    // TODO: (Kapsy) Other compiler support!
    // u32 Result = ror32(Value, Amount);
    Amount &= 31;
    u32 Result = ((Value >> Amount) | (Value << (32 - Amount)));
#endif
    return(Result);
}

inline r32
Round(r32 A)
{
    r32 Result = roundf(A);
    return(Result);
}

inline r32
RoundR32DownLeaning(r32 A)
{
    A-=0.000001f;
    r32 Result = roundf(A);
    return(Result);
}

inline s32
RoundR32ToS32(r32 A)
{
    s32 Result = (s32)roundf(A);
    return(Result);
}

inline u32
RoundR32ToUS32(r32 A)
{
    u32 Result = (u32)roundf(A);
    return(Result);
}

inline s32
FloorR32ToS32(r32 A)
{
    // TODO: (Kapsy) Should be calling the builtin floorf - although I don't know how to check!
    s32 Result = (s32)__builtin_floorf(A);
    return(Result);
}

inline u32
FloorR32Tou32(r32 A)
{
    u32 Result = (u32)__builtin_floorf(A);
    return(Result);
}

inline r32
FloorR32(r32 A)
{
    // TODO: (Kapsy) Should be calling the builtin floorf - although I don't know how to check!
    r32 Result = __builtin_floorf(A);
    return(Result);
}

inline s32
CeilR32ToS32(r32 A)
{
    s32 Result = (s32)__builtin_ceilf(A);
    return(Result);
}

inline u32
CeilR32Tou32(r32 A)
{
    u32 Result = (u32)__builtin_ceilf(A);
    return(Result);
}

inline s32
TruncateR32ToS32(r32 A)
{
    s32 Result = (s32)A;
    return(Result);
}

inline r32
Sin(r32 Angle)
{
    r32 Result = sinf(Angle);
    return(Result);
}

inline r32
Cos(r32 Angle)
{
    r32 Result = cosf(Angle);
    return(Result);
}


inline r32
ATan2(r32 Y, r32 X)
{
    r32 Result = atan2f(Y, X);
    return(Result);
}

struct bit_scan_result
{
    b32 Found;
    u32 Index;
};

inline bit_scan_result
FindLeastSignificantSetBit(u32 Value)
{
    bit_scan_result Result = {};

#if COMPILER_MSVC
    Result.Found = _BitScanForward(&Result.Index, Value);
// TODO: (Kapsy) LLVM!
#else
    for(u32 Test = 0;
            Test < 32;
            ++Test)
    {
        if(Value & (1 << Test))
        {
            Result.Index = Test;
            Result.Found = true;
            break;
        }
    }
#endif

    return(Result);
}

inline u16
ReverseEndianWord(u16 Word)
{
    u8 *Bytes = (u8 *)&Word;
    u16 Result = (Bytes[1] << 0) | (Bytes[0] << 8);
    return(Result);
}

inline u32
ReverseEndianDWord(u32 DWord)
{
    u8 *Bytes = (u8 *)&DWord;
    u32 Result = (Bytes[3] << 0) | (Bytes[2] << 8) | (Bytes[1] << 16) | (Bytes[0] << 24);
    return(Result);
}

//// old inline r32
//// old Min(r32 A, r32 B)
//// old {
//// old     r32 Result = fminf(A, B);
//// old     return(Result);
//// old }
//// old 
//// old inline r32
//// old Max(r32 A, r32 B)
//// old {
//// old     r32 Result = fmaxf(A, B);
//// old     return(Result);
//// old }


  ////////////////////////////////////////////////////////////////////////////////
 //// SECTION MATH HELPERS //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Trig constants
#define PI 3.141592653589793
#define TAU 6.283185307179586

// Small time scalar conversions
#define SPerMS  (1e-03)
#define SPerUS  (1e-06)
#define SPerNS  (1e-09)

#define MSPerS  (1e03)
#define MSPerUS (1e-03)
#define MSPerNS (1e-06)

#define USPerS  (1e06)
#define USPerMS (1e03)
#define USPerNS (1e-03)

#define NSPerS  (1e09)
#define NSPerMS (1e06)
#define NSPerUS (1e03)

#define SToMS(Value)  ((Value)*MSPerS)
#define SToUS(Value)  ((Value)*USPerS)
#define SToNS(Value)  ((Value)*NSPerS)

#define MSToS(Value)  ((Value)*SPerMS)
#define MSToUS(Value) ((Value)*USPerMS)
#define MSToNS(Value) ((Value)*NSPerMS)

#define USToS(Value)  ((Value)*SPerUS)
#define USToMS(Value) ((Value)*MSPerUS)
#define USToNS(Value) ((Value)*NSPerUS)

#define NSToS(Value)  ((Value)*SPerNS)
#define NSToMS(Value) ((Value)*MSPerNS)
#define NSToUS(Value) ((Value)*USPerNS)


// TODO (MJP): Get rid of these
#define ZERO _mm_set1_ps(0.f)
#define ONE _mm_set1_ps(1.f)
#define TWO _mm_set1_ps(2.f)
#define THREE _mm_set1_ps(3.f)
#define FOUR _mm_set1_ps(4.f)
#define EIGHT _mm_set1_ps(8.f)
#define NEGONE _mm_set1_ps(-1.f)
#define NEGTWO _mm_set1_ps(-2.f)
#define NEGFOUR _mm_set1_ps(-4.f)
#define HALF _mm_set1_ps(0.5f)
#define NEGHALF _mm_set1_ps(-0.5f)
#define QUART _mm_set1_ps(0.25f)
#define THIRD _mm_set1_ps(0.333333333f)

#define ABSMASK _mm_set1_epi32(0x7FFFFFFF)
#define PI128 _mm_set1_ps(PI_R64)
#define TAU128 _mm_set1_ps(TAU_R64)
#define NEGTAU128 _mm_set1_ps (TAU_R64)


inline r32
Square(r32 A)
{
    r32 Result = A*A;
    return(Result);
}

inline r32
Lerp(r32 A, r32 t, r32 B)
{
   r32 Result = (1.f - t)*A + t*B;
   return(Result);
}


// move these somewhere useful
//#define FMA(x, a, y) _mm_add_ps(_mm_mul_ps(x, a), y)
#define AbsR128(a) (_mm_and_ps ((a), ABSMASK))
// shoving this here
//
// #define ShapedPanR128(x) (THIRD*((x)*(FOUR - (x))))
// #define ShapedPan(x) { 0.333333f*((x)*(4.f - (x))) }

inline r32
ShapedPan (r32 x)
{
   r32 y = 0.333333f*(x*(4.f - x));
   return (y);
}


inline r128
ShapedPanR128 (r128 x)
{
   r128 y = THIRD*(x*(FOUR - x));
   return (y);
}

// which law is this???
internal r32 ShapedXfade (r32 t)
{
   r32 ts = 0.333333f*(t*(4.f - t));
   return (ts);
}

// t is bipolar
internal r32 
ShapedMix (r32 t, r32 x1, r32 x2)
{
   r32 a = ShapedXfade (1.f - t);
   r32 b = ShapedXfade (t + 1.f);

   r32 y = x1*a + x2*b;
   return (y);
}

// which law is this???
internal r128
ShapedXfade (r128 t)
{
   r128 ts = THIRD*(t*(FOUR - t));
   return (ts);
}

// t is bipolar
internal r128 
ShapedMix (r128 t, r128 x1, r128 x2)
{
   r128 a = ShapedXfade (1.f - t);
   r128 b = ShapedXfade (t + 1.f);

   r128 y = x1*a + x2*b;
   return (y);
}

internal r128
BiToUni (r128 x)
{
   return ((x + ONE)*HALF);
}

internal r32
BiToUni (r32 x)
{
   return ((x + 1.f)*0.5f);
}


#if 0
inline r128
ClampUniR128 (r128 x)
{
   r128 ltm = _mm_cmplt_ps (x, ZERO);
   r128 gtm = _mm_cmpgt_ps (x, ONE);
   r128 a1 = _mm_andnot_ps (ltm, x);
   r128 a = _mm_andnot_ps (gtm, a1);
   r128 b = _mm_and_ps (ltm, ZERO);
   r128 c = _mm_and_ps (gtm, ONE);
   r128 y = a + b + c;
   return (y);
}
#endif



inline r128
LerpR128 (r128 t, r128 a, r128 b)
{
   r128 y = (ONE - t)*a + t*b;
   // r128 y = _mm_fmadd_ps ((ONE - t), a, t*b);
   return (y);
}

inline m128 
Lerp(m128 t, m128 a, m128 b)
{
   // m128 y = (ONE - t)*a + t*b;
   m128 one = _mm_set1_ps(1.f);
   m128 y = _mm_fmadd_ps((one - t), a, t*b);
   return (y);
}

inline m256 
Lerp(m256 t, m256 a, m256 b)
{
   // m256 y = (ONE - t)*a + t*b;
   m256 one = _mm256_set1_ps(1.f);
   m256 y = _mm256_fmadd_ps((one - t), a, t*b);
   return (y);
}





// NOTE (mjp): Clamping functions 

inline r32
Clamp(r32 Min, r32 Value, r32 Max)
{
   r32 Result = Value;
   if(Result < Min)
   {
      Result = Min;
   }
   else if(Result > Max)
   {
      Result = Max;
   }
   return(Result);
}

inline s32
Clamp(s32 Min, s32 Value, s32 Max)
{
   s32 Result = Value;

   if (Result < Min)
   {
      Result = Min;
   }
   else if (Result > Max)
   {
      Result = Max;
   }

   return(Result);
}

inline u32
Clamp(u32 Min, u32 Value, u32 Max)
{
   u32 Result = Value;

   if (Result < Min)
   {
      Result = Min;
   }
   else if (Result > Max)
   {
      Result = Max;
   }

   return(Result);
}

inline r32
Clamp01(r32 Value)
{
   r32 Result = Clamp(0.0f, Value, 1.0f);
   return(Result);
}



inline r32
ClampBiR32 (r32 x)
{
   r32 y = Clamp (-1.f, x, 1.f);
   return (y);
}

#if 1
// sse4, but takes about half the time of above implementation
inline r128
ClampUniR128 (r128 x)
{
   r128 y = x;
   y = _mm_blendv_ps (y, ZERO, _mm_cmplt_ps (x, ZERO));
   y = _mm_blendv_ps (y, ONE, _mm_cmpgt_ps (x, ONE));
   return (y);
}
#endif

inline r128
ClampBiR128 (r128 x)
{
   r128 ltm = _mm_cmplt_ps (x, NEGONE);
   r128 gtm = _mm_cmpgt_ps (x, ONE);
   r128 a1 = _mm_andnot_ps (ltm, x);
   r128 a = _mm_andnot_ps (gtm, a1);
   r128 b = _mm_and_ps (ltm, NEGONE);
   r128 c = _mm_and_ps (gtm, ONE);
   r128 y = a + b + c;
   return (y);
}

inline r128
ClampR128 (r128 x, r128 min, r128 max)
{
   r128 ltm = _mm_cmplt_ps (x, min);
   r128 gtm = _mm_cmpgt_ps (x, max);
   r128 a1 = _mm_andnot_ps (ltm, x);
   r128 a = _mm_andnot_ps (gtm, a1);
   r128 b = _mm_and_ps (ltm, min);
   r128 c = _mm_and_ps (gtm, max);
   r128 y = a + b + c;
   return (y);
}

inline m128
ClampUni(m128 x)
{
   m128 ZERO256 = _mm_setzero_ps();
   m128 ONE256 = _mm_set1_ps(1.f);

   m128 y = x;
   y = _mm_blendv_ps(y, ZERO256, _mm_cmp_ps(x, ZERO256, _CMP_LT_OS));
   y = _mm_blendv_ps(y, ONE256, _mm_cmp_ps(x, ONE256, _CMP_GT_OS));
   return (y);
}


inline m256
ClampUni256(m256 x)
{
   m256 ZERO256 = _mm256_setzero_ps();
   m256 ONE256 = _mm256_set1_ps(1.f);

   m256 y = x;
   y = _mm256_blendv_ps(y, ZERO256, _mm256_cmp_ps(x, ZERO256, _CMP_LT_OS));
   y = _mm256_blendv_ps(y, ONE256, _mm256_cmp_ps(x, ONE256, _CMP_GT_OS));
   return (y);
}


















// TODO (MJP): Rename to MapToUnilateralAndClamp
inline r32
MapToUnilateralAndClamp(r32 Min, r32 t, r32 Max)
{
   r32 Result = 0.f;
   r32 Range = Max - Min;
   if (Range != 0.f)
   {
      Result = Clamp01((t - Min)/Range);
   }
   return(Result);
}

inline r32
SafeRatioN(r32 Numerator, r32 Divisor, r32 N)
{
   r32 Result = N;
   if (Divisor != 0.f)
   {
      Result = Numerator/Divisor;
   }
   return(Result);
}

inline r32
SafeRatio0(r32 Numerator, r32 Divisor)
{
   return SafeRatioN(Numerator, Divisor, 0.f);
}

inline r32
SafeRatio1(r32 Numerator, r32 Divisor)
{
   return SafeRatioN(Numerator, Divisor, 1.f);
}



// Not sure what this is, was in props. Guess it's an approximation?
// Only really accurate within -0.5 < x < 0.5
inline r32
TanApprox(r32 x)
{
    // TODO: (Kapsy) Not sure that this naming is right, as 2e3 actually means 2x10^3.
    r32 xe2 = x*x;
    r32 xe3 = xe2*x;
    r32 xe5 = xe2*xe3;
    r32 Result = x + (xe5*0.133333f + xe3*0.333333f);
    return(Result);
}

inline m128
TanApprox(m128 x)
{
    m128 xe2 = x*x;
    m128 xe3 = xe2*x;
    m128 xe5 = xe2*xe3;
    m128 Result = x + (xe5*0.133333f + xe3*0.333333f);
    return(Result);
}



  //////////////////////////////////////////////////////////////////////////////
 //// SECTION: VECTORS ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

union v2
{
    struct
    {
        r32 x, y;
    };
    struct
    {
        r32 u, v;
    };
    r32 E[2];
};

union v3
{
    struct
    {
        r32 x, y, z;
    };
    struct
    {
        r32 u, v, w;
    };
    struct
    {
        r32 r, g, b;
    };
    struct
    {
        v2 xy;
        r32 Ignored0_;
    };
    struct
    {
        r32 Ignored1_;
        v2 yz;
    };
    struct
    {
        v2 uv;
        r32 Ignored2_;
    };
    struct
    {
        r32 Ignored3_;
        v2 vw;
    };
    r32 E[3];
};

union v4
{
    struct
    {
        union
        {
            v3 xyz;
            struct
            {
                r32 x, y, z;
            };
        };
        r32 w;
    };

    struct
    {
        union
        {
            v3 rgb;
            struct
            {
                r32 r, g, b;
            };
        };
        r32 a;
    };

    struct
    {
        v2 xy;
        r32 Ignored0_;
        r32 Ignored1_;
    };
    struct
    {
        r32 Ignored2_;
        v2 yz;
        r32 Ignored3_;
    };
    struct
    {
        r32 Ignored4_;
        r32 Ignored5_;
        v2 zw;
    };
    r32 E[4];
};

inline v2
V2(r32 A)
{
   v2 Result;

   Result.E[0] = Result.E[1] = A;

   return(Result);
}

inline v2
V2(r32 X, r32 Y)
{
   v2 Result;

   Result.x = X;
   Result.y = Y;

   return(Result);
}

inline v2
V2i(s32 X, s32 Y)
{
   v2 Result;

   Result.x = (r32)X;
   Result.y = (r32)Y;

   return(Result);
}

inline v2
V2i(u32 X, u32 Y)
{
   v2 Result;

   Result.x = (r32)X;
   Result.y = (r32)Y;

   return(Result);
}

inline v3
V3(r32 A)
{
   v3 Result;

   Result.E[0] = Result.E[1] = Result.E[2] = A;
   
   return(Result);
}

inline v3
V3(r32 X, r32 Y, r32 Z)
{
   v3 Result;

   Result.x = X;
   Result.y = Y;
   Result.z = Z;

   return(Result);
}

inline v4
V4(r32 X, r32 Y, r32 Z, r32 W)
{
   v4 Result;

   Result.x = X;
   Result.y = Y;
   Result.z = Z;
   Result.w = W;

   return(Result);
}

inline v3
V3(v2 XY, r32 Z)
{
   v3 Result;

   Result.x = XY.x;
   Result.y = XY.y;
   Result.z = Z;

   return(Result);
}

inline v4
V4(v3 XYZ, r32 W)
{
   v4 Result;

   Result.xyz = XYZ;
   Result.w = W;

   return(Result);
}

inline v4
V4(r32 Value)
{
   v4 Result;

   Result.r = Value;
   Result.b = Value;
   Result.g = Value;
   Result.a = Value;

   return(Result);
}


// NOTE (mjp): v2 operators

inline v2
Perp(v2 A)
{
    v2 Result = { -A.y, A.x };

    return(Result);
}

inline v2
operator*(r32 A, v2 B)
{
   v2 Result;

   Result.x = A*B.x;
   Result.y = A*B.y;

   return(Result);
}

inline v2
operator*(v2 B, r32 A)
{
   v2 Result = A*B;

   return(Result);
}

inline v2
operator*(v2 A, v2 B)
{
   v2 Result;

   Result.x = A.x*B.x;
   Result.y = A.y*B.y;

   return(Result);
}

inline v2 &
operator*=(v2 &B, r32 A)
{
   B = A * B;

   return(B);
}

inline v2
operator/(v2 A, r32 B)
{
   v2 Result;

   Result.x = A.x/B;
   Result.y = A.y/B;

   return(Result);
}

inline v2
operator/(v2 A, v2 B)
{
   v2 Result;

   Result.x =  A.x/B.x;
   Result.y =  A.y/B.y;

   return(Result);
}


inline
v2 operator-(v2 A)
{
   v2 Result;

   Result.x = -A.x;
   Result.y = -A.y;

   return(Result);
}

inline
v2 operator+(v2 A, v2 B)
{
   v2 Result;

   Result.x = A.x + B.x;
   Result.y = A.y + B.y;

   return(Result);
}

inline
v2 operator+(v2 A, r32 B)
{
   v2 Result = A + V2(B, B);

   return(Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
   A = A + B;
   return(A);
}

inline
v2 operator-(v2 A, v2 B)
{
   v2 Result;

   Result.x = A.x - B.x;
   Result.y = A.y - B.y;

   return(Result);
}

inline
v2 operator-=(v2 &A, v2 B)
{
   A = A - B;
   return(A);
}

inline v2
Hadamard(v2 A, v2 B)
{
   v2 Result = {A.x*B.x, A.y*B.y};

   return(Result);
}

inline r32
Inner(v2 A, v2 B)
{
   r32 Result = A.x*B.x + A.y*B.y;
   return(Result);
}

inline r32
LengthSq(v2 A)
{
   r32 Result = Inner(A, A);
   return(Result);
}

inline r32
Length(v2 A)
{
   r32 Result = SquareRoot(LengthSq(A));
   return(Result);
}

inline v2
Unit (v2 A)
{
   v2 Result = A/Length (A);
   return(Result);
}

inline v2
Lerp (v2 A, r32 t, v2 B)
{
   v2 Result = (1.f - t)*A + t*B;
   return(Result);
}

inline v2
Clamp01(v2 A)
{
   v2 Result = V2(Clamp01(A.x), Clamp01(A.y));
   return(Result);
}

// NOTE (mjp): v3 operators
// TODO (mjp): Should probably generate these or something

inline v3
operator*(r32 A, v3 B)
{
   v3 Result;

   Result.x = A*B.x;
   Result.y = A*B.y;
   Result.z = A*B.z;

   return(Result);
}

inline v3
operator*(v3 B, r32 A)
{
   v3 Result = A*B;

   return(Result);
}

inline v3 &
operator*=(v3 &B, r32 A)
{
   B = A * B;

   return(B);
}

inline v3
operator-(v3 A)
{
   v3 Result;

   Result.x = -A.x;
   Result.y = -A.y;
   Result.z = -A.z;

   return(Result);
}

inline v3
operator+(v3 A, v3 B)
{
   v3 Result;

   Result.x = A.x + B.x;
   Result.y = A.y + B.y;
   Result.z = A.z + B.z;

   return(Result);
}

inline v3 &
operator+=(v3 &A, v3 B)
{
   A = A + B;

   return(A);
}

inline v3
operator-(v3 A, v3 B)
{
   v3 Result;

   Result.x = A.x - B.x;
   Result.y = A.y - B.y;
   Result.z = A.z - B.z;

   return(Result);
}

inline v3
Hadamard(v3 A, v3 B)
{
   v3 Result = {A.x*B.x, A.y*B.y, A.z*B.z};

   return(Result);
}

inline r32
Inner(v3 A, v3 B)
{
   r32 Result = A.x*B.x + A.y*B.y + A.z*B.z;

   return(Result);
}

inline r32
LengthSq(v3 A)
{
   r32 Result = Inner(A, A);

   return(Result);
}

inline r32
Length(v3 A)
{
   r32 Result = SquareRoot(LengthSq(A));
   return(Result);
}

inline v3
Normalize(v3 A)
{
   v3 Result = A*(1.0f / Length(A));

   return(Result);
}

inline v3
Clamp01(v3 Value)
{
   v3 Result;

   Result.x = Clamp01(Value.x);
   Result.y = Clamp01(Value.y);
   Result.z = Clamp01(Value.z);

   return(Result);
}

inline v3
Lerp(v3 A, r32 t, v3 B)
{
   v3 Result = (1.f - t)*A + t*B;

   return(Result);
}

// NOTE (mjp): v4 operators
// TODO (mjp): Should probably generate these or something

inline v4
operator*(r32 A, v4 B)
{
   v4 Result;

   Result.x = A*B.x;
   Result.y = A*B.y;
   Result.z = A*B.z;
   Result.w = A*B.w;

   return(Result);
}

inline v4
operator*(v4 B, r32 A)
{
   v4 Result = A*B;

   return(Result);
}

inline v4 &
operator*=(v4 &B, r32 A)
{
   B = A * B;

   return(B);
}

inline v4
operator-(v4 A)
{
   v4 Result;

   Result.x = -A.x;
   Result.y = -A.y;
   Result.z = -A.z;
   Result.w = -A.w;

   return(Result);
}

inline v4
operator+(v4 A, v4 B)
{
   v4 Result;

   Result.x = A.x + B.x;
   Result.y = A.y + B.y;
   Result.z = A.z + B.z;
   Result.w = A.w + B.w;

   return(Result);
}

inline v4 &
operator+=(v4 &A, v4 B)
{
   A = A + B;

   return(A);
}

inline v4
operator-(v4 A, v4 B)
{
   v4 Result;

   Result.x = A.x - B.x;
   Result.y = A.y - B.y;
   Result.z = A.z - B.z;
   Result.w = A.w - B.w;

   return(Result);
}

inline v4
Hadamard(v4 A, v4 B)
{
   v4 Result = {A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w};

   return(Result);
}

inline r32
Inner(v4 A, v4 B)
{
   r32 Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

   return(Result);
}

inline r32
LengthSq(v4 A)
{
   r32 Result = Inner(A, A);

   return(Result);
}

inline r32
Length(v4 A)
{
   r32 Result = SquareRoot(LengthSq(A));
   return(Result);
}

inline v4
Clamp01(v4 Value)
{
   v4 Result;

   Result.x = Clamp01(Value.x);
   Result.y = Clamp01(Value.y);
   Result.z = Clamp01(Value.z);
   Result.w = Clamp01(Value.w);

   return(Result);
}

inline v4
Lerp(v4 A, r32 t, v4 B)
{
   v4 Result = (1.f - t)*A + t*B;

   return(Result);
}

// NOTE (mjp):
// In Photoshop, 255, 0, 0 will become:
// 127, 127, 127 at 0% saturation.
// 159, 95, 95 at 25% saturation.
// 191, 64, 64 at 50% saturation.
// 223, 32, 32 at 75% saturation.
// 255, 255, 255 is unchanged at any saturation value.
inline v4
ColorWithSaturation(v4 C, r32 S)
{
   r32 Avg = (C.r + C.g + C.b)/3.f;
   r32 Weight = 0.6f;
   Avg = Weight + Avg*(1.f-Weight);
   v4 Result = V4(Lerp(V3(Avg, Avg, Avg), S, C.rgb), C.a);
   return(Result);
}

  //////////////////////////////////////////////////////////////////////////////
 //// SECTION: RECTANGLES /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// NOTE (mjp): Rectangle types and functions

struct rectangle2
{
   v2 Min;
   v2 Max;
};

typedef rectangle2 rect2;

inline v2
GetMinCorner(rectangle2 Rect)
{
   v2 Result = Rect.Min;
   return(Result);
}

inline v2
GetMaxCorner(rectangle2 Rect)
{
   v2 Result = Rect.Max;
   return(Result);
}

inline v2
GetDim(rectangle2 Rect)
{
   v2 Result = Rect.Max - Rect.Min;
   return(Result);
}

inline v2
GetCenter(rectangle2 Rect)
{
   v2 Result = 0.5f *(Rect.Min + Rect.Max);
   return(Result);
}

inline rectangle2
RectMinMax(v2 Min, v2 Max)
{
   rectangle2 Result;

   Result.Min = Min;
   Result.Max = Max;

   return(Result);
}

inline rectangle2
RectMinDim(v2 Min, v2 Dim)
{
   rectangle2 Result;

   Result.Min = Min;
   Result.Max = Min + Dim;

   return(Result);
}


inline rectangle2
RectCenterHalfDim(v2 Center, v2 HalfDim)
{
   rectangle2 Result;

   Result.Min = Center - HalfDim;
   Result.Max = Center + HalfDim;

   return(Result);
}

inline rectangle2
RectCenterDim(v2 Center, v2 Dim)
{
   rectangle2 Result = RectCenterHalfDim(Center, 0.5f*Dim);

   return(Result);
}

#if 1
inline rectangle2
RectTopLeftDim(v2 TopLeft, v2 Dim)
{
   rectangle2 Result = RectMinDim(V2(TopLeft.x, TopLeft.y - Dim.y), Dim);

   return(Result);
}
#endif

inline rectangle2 &
RoundRect(rectangle2 &A)
{
   Round(A.Min.x);
   Round(A.Min.y);
   Round(A.Max.x);
   Round(A.Max.y);

   return(A);
}

inline rectangle2
RectUnion(rectangle2 A, rectangle2 B)
{
   rectangle2 Result = RectMinMax(
         V2(((A.Min.x < B.Min.x) ? A.Min.x : B.Min.x),
            ((A.Min.y < B.Min.y) ? A.Min.y : B.Min.y)),
         V2(((A.Max.x > B.Max.x) ? A.Max.x : B.Max.x),
            ((A.Max.y > B.Max.y) ? A.Max.y : B.Max.y))
         );

   return(Result);
}

// NOTE (mjp): Assumes Y down
inline b32
IsInRectangle(rectangle2 Rectangle, v2 Test)
{
   b32 Result =
      ((Test.x >= Rectangle.Min.x) &&
       (Test.y >= Rectangle.Min.y) &&
       (Test.x < Rectangle.Max.x) &&
       (Test.y < Rectangle.Max.y));

   return(Result);
}

inline b32
IsInRectangleRev(rectangle2 Rectangle, v2 Test)
{
   b32 Result =
      ((Test.x >= Rectangle.Min.x) &&
       (Test.y < Rectangle.Min.y) &&
       (Test.x < Rectangle.Max.x) &&
       (Test.y >= Rectangle.Max.y));

   return(Result);
}

inline b32
IsInRectangle(rectangle2 Rectangle, rectangle2 Test)
{
   b32 Result =
      ((Test.Min.x >= Rectangle.Min.x) &&
       (Test.Max.x <= Rectangle.Max.x) &&
       (Test.Min.y >= Rectangle.Min.y) &&
       (Test.Max.y <= Rectangle.Max.y));

   return(Result);
}

inline b32
RectanglesIntersect(rectangle2 A, rectangle2 B)
{
   b32 Result =
      !((A.Min.x > B.Max.x) ||
            (A.Max.x < B.Min.x) ||
            (A.Min.y > B.Max.y) ||
            (A.Max.y < B.Min.y));

   return(Result);
}


inline rectangle2
ClipRect(rectangle2 A, rectangle2 B)
{
   rectangle2 Result =
      RectMinMax(V2(((A.Min.x < B.Min.x) ? B.Min.x : A.Min.x),
               ((A.Min.y < B.Min.y) ? B.Min.y : A.Min.y)),
            V2(((A.Max.x > B.Max.x) ? B.Max.x : A.Max.x),
               ((A.Max.y > B.Max.y) ? B.Max.y : A.Max.y)));
   return(Result);
}


inline v2
GetBarycentric(rectangle2 A, v2 P)
{
   v2 Result;

   Result.x = SafeRatio0(P.x - A.Min.x, A.Max.x - A.Min.x);
   Result.y = SafeRatio0(P.y - A.Min.y, A.Max.y - A.Min.y);

   return(Result);
}

inline v2
GetPointRelativeMin(rectangle2 A, v2 P)
{
   v2 Result = P - A.Min;

   return(Result);
}

// NOTE (mjp): rectangle3

struct rectangle3
{
   v3 Min;
   v3 Max;
};

inline v3
GetMinCorner(rectangle3 Rect)
{
   v3 Result = Rect.Min;
   return(Result);
}

inline v3
GetMaxCorner(rectangle3 Rect)
{
   v3 Result = Rect.Max;
   return(Result);
}

inline v3
GetDim(rectangle3 Rect)
{
   v3 Result = Rect.Max - Rect.Min;
   return(Result);
}

inline v3
GetCenter(rectangle3 Rect)
{
   v3 Result = 0.5f*(Rect.Min + Rect.Max);
   return(Result);
}

inline rectangle3
RectMinMax(v3 Min, v3 Max)
{
   rectangle3 Result;

   Result.Min = Min;
   Result.Max = Max;

   return(Result);
}

inline rectangle3
RectMinDim(v3 Min, v3 Dim)
{
   rectangle3 Result;

   Result.Min = Min;
   Result.Max = Min + Dim;

   return(Result);
}

inline rectangle3
RectCenterHalfDim(v3 Center, v3 HalfDim)
{
   rectangle3 Result;

   Result.Min = Center - HalfDim;
   Result.Max = Center + HalfDim;

   return(Result);
}

inline rectangle3
AddRadiusTo(rectangle3 A, v3 Radius)
{
   rectangle3 Result;

   Result.Min = A.Min - Radius;
   Result.Max = A.Max + Radius;

   return(Result);
}

inline rectangle3
Offset(rectangle3 A, v3 Offset)
{
   rectangle3 Result;

   Result.Min = A.Min + Offset;
   Result.Max = A.Max + Offset;

   return(Result);
}

inline rectangle3
RectCenterDim(v3 Center, v3 Dim)
{
   rectangle3 Result = RectCenterHalfDim(Center, 0.5f*Dim);

   return(Result);
}

inline b32
IsInRectangle(rectangle3 Rectangle, v3 Test)
{
   b32 Result = ((Test.x >= Rectangle.Min.x) &&
         (Test.y >= Rectangle.Min.y) &&
         (Test.z >= Rectangle.Min.z) &&
         (Test.x < Rectangle.Max.x) &&
         (Test.y < Rectangle.Max.y) &&
         (Test.z < Rectangle.Max.z));

   return(Result);
}

inline b32
RectanglesIntersect(rectangle3 A, rectangle3 B)
{
   b32 Result = !((B.Max.x <= A.Min.x) ||
         (B.Min.x >= A.Max.x) ||
         (B.Max.y <= A.Min.y) ||
         (B.Min.y >= A.Max.y) ||
         (B.Max.z <= A.Min.z) ||
         (B.Min.z >= A.Max.z));
   return(Result);
}

inline v3
GetBarycentric(rectangle3 A, v3 P)
{
   v3 Result;

   Result.x = SafeRatio0(P.x - A.Min.x, A.Max.x - A.Min.x);
   Result.y = SafeRatio0(P.y - A.Min.y, A.Max.y - A.Min.y);
   Result.z = SafeRatio0(P.z - A.Min.z, A.Max.z - A.Min.z);

   return(Result);
}

inline rectangle2
ToRectangleXY(rectangle3 A)
{
   rectangle2 Result;

   Result.Min = A.Min.xy;
   Result.Max = A.Max.xy;

   return(Result);
}

// NOTE (mjp): rectangle2i

struct rectangle2i
{
   s32 MinX, MinY;
   s32 MaxX, MaxY;
};

inline rectangle2i
Intersect(rectangle2i A, rectangle2i B)
{
   rectangle2i Result;

   Result.MinX = (A.MinX < B.MinX) ? B.MinX : A.MinX;
   Result.MinY = (A.MinY < B.MinY) ? B.MinY : A.MinY;
   Result.MaxX = (A.MaxX > B.MaxX) ? B.MaxX : A.MaxX;
   Result.MaxY = (A.MaxY > B.MaxY) ? B.MaxY : A.MaxY;

   return(Result);
}

inline rectangle2i
Union(rectangle2i A, rectangle2i B)
{
   rectangle2i Result;

   Result.MinX = (A.MinX < B.MinX) ? A.MinX : B.MinX;
   Result.MinY = (A.MinY < B.MinY) ? A.MinY : B.MinY;
   Result.MaxX = (A.MaxX > B.MaxX) ? A.MaxX : B.MaxX;
   Result.MaxY = (A.MaxY > B.MaxY) ? A.MaxY : B.MaxY;

   return(Result);
}

inline s32
GetClampedRectArea(rectangle2i A)
{
   s32 Width = (A.MaxX - A.MinX);
   s32 Height = (A.MaxY - A.MinY);
   s32 Result = 0;
   if((Width > 0) && (Height > 0))
   {
      Result = Width*Height;
   }

   return(Result);
}

inline b32
HasArea(rectangle2i A)
{
   b32 Result = ((A.MinX < A.MaxX) && (A.MinY < A.MaxY));

   return(Result);
}

inline rectangle2i
InvertedInfinityRectangle(void)
{
   rectangle2i Result;

   Result.MinX = Result.MinY = INT_MAX;
   Result.MaxX = Result.MaxY = -INT_MAX;

   return(Result);
}

//////////////////////////////////////////////////////////////////////////////
//// line2 ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

struct line2
{
   v2 A;
   v2 B;
};

inline line2
Line(v2 A, v2 B)
{
   line2 Result;

   Result.A = A;
   Result.B = B;

   return(Result);
}



//////////////////////////////////////////////////////////////////////////////
//// mat2 ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// All matrices are column major

union mat4
{
   v4 cols[4];
};

inline mat4
Mat4 (v4 a1, v4 a2, v4 a3, v4 a4)
{
   mat4 A;

   A.cols[0] = a1;
   A.cols[1] = a2;
   A.cols[2] = a3;
   A.cols[3] = a4;

   return (A);
}

// rename to mat3x3?
union mat3
{
   v3 cols[3];
   r32 e[3][3];
};


// TODO just make these element wise...? not much point in defining vectors...
inline mat3
Mat3 (v3 a1, v3 a2, v3 a3)
{
   mat3 A;

   A.cols[0] = a1;
   A.cols[1] = a2;
   A.cols[2] = a3;

   return (A);
}


inline mat3
Mat3 ()
{
   mat3 A = {0};
   return(A);
}


// TODO
// probably best to simdfy mat4s and just use those
inline mat3
operator*(mat3 A, mat3 B)
{
   mat3 C = Mat3 ();
   u32 Cj, Ci, Bj, Ai;
   for (Cj = 0; Cj < 3; ++Cj)
      for (Ci = 0; Ci < 3; ++Ci) {

         // whole colum of C
         // C[0][0] = A[0][0]*B[0][0] + A[1][0]*B[0][1] + A[2][0]*B[0][2];
         // C[0][1] = A[0][1]*B[0][0] + A[1][1]*B[0][1] + A[2][1]*B[0][2];
         // C[0][2] = A[0][2]*B[0][0] + A[1][2]*B[0][1] + A[2][2]*B[0][2];

         Ai = Ci;
         Bj = Cj;
         C.e[Cj][Ci] = 
            A.e[0][Ai]*B.e[Bj][0] +
            A.e[1][Ai]*B.e[Bj][1] +
            A.e[2][Ai]*B.e[Bj][2];
      }

   return (C);
}




union mat2
{
   v2 cols[2];
   r32 e[2][2];
};

inline mat2
Mat2(v2 a1, v2 a2)
{
   mat2 A;

   A.cols[0] = a1;
   A.cols[1] = a2;

   return (A);
}

inline mat2
Mat2()
{
   mat2 A = {0};

   return (A);
}


// TODO probably best to simdfy mat4s and just use those
inline mat2
operator*(mat2 A, mat2 B)
{
   mat2 C = Mat2 ();
   u32 Cj, Ci, Bj, Ai;
   for (Cj = 0; Cj < 2; ++Cj)
      for (Ci = 0; Ci < 2; ++Ci) {

         Ai = Ci;
         Bj = Cj;
         C.e[Cj][Ci] = 
            A.e[0][Ai]*B.e[Bj][0] +
            A.e[1][Ai]*B.e[Bj][1];
      }

   return (C);
}


inline v2
operator*(mat2 A, v2 B)
{
   v2 r = {};
   // pretty sure this is right, normal rotation should be acw
   r.x = A.e[0][0]*B.x + A.e[1][0]*B.y;
   r.y = A.e[0][1]*B.x + A.e[1][1]*B.y;

   //// r.x = A.e[0][0]*B.x + A.e[0][1]*B.y;
   //// r.y = A.e[1][0]*B.x + A.e[1][1]*B.y;
   return (r);
}


// NOTE (mjp): sRGB functions

inline v4
SRGB255ToLinear1(v4 C)
{
   v4 Result;

   r32 Inv255 = 1.f / 255.f;

   Result.r = Square(Inv255*C.r);
   Result.g = Square(Inv255*C.g);
   Result.b = Square(Inv255*C.b);
   Result.a = Inv255*C.a;

   return(Result);
}

inline v4
Linear1ToSRGB255(v4 C)
{
   v4 Result;

   r32 One255 = 255.f;

   Result.r = One255*SquareRoot(C.r);
   Result.g = One255*SquareRoot(C.g);
   Result.b = One255*SquareRoot(C.b);
   Result.a = One255*C.a;

   return(Result);
}



inline r32
GetSign (r32 x)
{
   // naive for now, but should check MSB
   if (x >= 0.f) return 1.f;
   else return -1.f;
}


inline u32
GetSignBit (r32 x)
{
   return ((*((u32 *) &x)) >> 31);
}



// NOTE: (Kapsy) THIS IS MIT LICENSE CODE, WILL HAVE TO REMOVE AT SOME POINT.
#define EXP_POLY_DEGREE 3

#define POLY0(x, c0) _mm_set1_ps(c0)
#define POLY1(x, c0, c1) _mm_add_ps(_mm_mul_ps(POLY0(x, c1), x), _mm_set1_ps(c0))
#define POLY2(x, c0, c1, c2) _mm_add_ps(_mm_mul_ps(POLY1(x, c1, c2), x), _mm_set1_ps(c0))
#define POLY3(x, c0, c1, c2, c3) _mm_add_ps(_mm_mul_ps(POLY2(x, c1, c2, c3), x), _mm_set1_ps(c0))
#define POLY4(x, c0, c1, c2, c3, c4) _mm_add_ps(_mm_mul_ps(POLY3(x, c1, c2, c3, c4), x), _mm_set1_ps(c0))
#define POLY5(x, c0, c1, c2, c3, c4, c5) _mm_add_ps(_mm_mul_ps(POLY4(x, c1, c2, c3, c4, c5), x), _mm_set1_ps(c0))

inline
m128 exp2f4(m128 x)
{
   m128i ipart;
   m128 fpart, expipart, expfpart;

   x = _mm_min_ps (x, _mm_set1_ps ( 129.00000f));
   x = _mm_max_ps (x, _mm_set1_ps (-126.99999f));

   /* ipart = int(x - 0.5) */
   ipart = _mm_cvtps_epi32 (_mm_sub_ps (x, _mm_set1_ps (0.5f)));

   /* fpart = x - ipart */
   fpart = _mm_sub_ps (x, _mm_cvtepi32_ps (ipart));

   /* expipart = (float) (1 << ipart) */
   expipart = _mm_castsi128_ps (_mm_slli_epi32 (_mm_add_epi32 (ipart, _mm_set1_epi32 (127)), 23));

   /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
#if EXP_POLY_DEGREE == 5
   expfpart = POLY5(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
#elif EXP_POLY_DEGREE == 4
   expfpart = POLY4(fpart, 1.0000026f, 6.9300383e-1f, 2.4144275e-1f, 5.2011464e-2f, 1.3534167e-2f);
#elif EXP_POLY_DEGREE == 3
   expfpart = POLY3(fpart, 9.9992520e-1f, 6.9583356e-1f, 2.2606716e-1f, 7.8024521e-2f);
#elif EXP_POLY_DEGREE == 2
   expfpart = POLY2(fpart, 1.0017247f, 6.5763628e-1f, 3.3718944e-1f);
#else
#error
#endif

   return(_mm_mul_ps(expipart, expfpart));
}


  //////////////////////////////////////////////////////////////////////////////
 //// Fast Random Functions ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

union random_union
{
    r32 R32;
    u32 u32;
};

typedef u32 random_seed;

inline r32
RandomUnilateral (random_seed *seed)
{
    random_union rnd = {};

    *seed = (*seed)*1103515245 + 12345;
    rnd.u32 = ((*seed) & 0x7fffff) | 0x3f800000;

    r32 result = rnd.R32 - 1.f;
    return (result);
}

inline r32
RandomBilateral (random_seed *seed)
{
    r32 result = 2.f*RandomUnilateral (seed) - 1.f;
    return (result);
}

// may want to rename "between", as implies min < max, not min <= max

// values min <= max are possible
inline r32
RandomBetweenR32 (random_seed *Seed, r32 Min, r32 Max)
{
   r32 Result = Lerp(Min, RandomUnilateral(Seed), Max);
   return(Result);
}

// values min <= max are possible
inline u32
RandomBetweenU32 (random_seed *seed, u32 min, u32 max)
{
    Assert (min <= max);

    r32 range = (r32) ((max - min) + 1);
    u32 result = min + (u32) (range*RandomUnilateral (seed));

    return (result);
}

inline u64
RandomBetweenU64(random_seed *Seed, u64 Min, u64 Max)
{
   Assert(Min <= Max);
   r32 Range = (r32)((Max - Min) + 1);
   u64 Result = Min + (u64)(Range*RandomUnilateral(Seed));
   return(Result);
}


// values min <= max are possible
inline s32
RandomBetweenS32 (random_seed *seed, s32 min, s32 max)
{
    //Assert (min < max);

    r32 range = (r32) ((max - min) + 1);
    s32 result = min + (s32) (range*RandomUnilateral (seed));

    return (result);
}













// typedef r128 random_seed_4;

//// union random_union_4
//// {
////     r128 R;
////     u128 U;
//// };
//// 
//// #define S1 set1_s128(1103515245)
//// #define S2 set1_s128(12345)
//// 
//// #define B1 set1_s128(0x7fffff)
//// #define B2 set1_s128(0x3f800000)
//// 
//// inline r128
//// RandomUnilateral (random_seed_4 *seed)
//// {
////     random_union_4 rnd = {};
////     *seed = (*seed)*S1 + S2;
////     rnd.U = or_s128 (and_s128 (*seed, B1), B2);
////     // rnd.U = ((*seed) & 0x7fffff) | 0x3f800000;
////     r128 y = rnd.R - ONE;
////     return (y);
//// }

  //////////////////////////////////////////////////////////////////////////////
 //// SECTION: LINKED LISTS ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// f:first/free l:last b:base n:node

// Sentinel DLL macros

#define SDLLInsertAfter_NP(b, n, Next, Prev) ((n)->Next = (b)->Next, \
                                              (n)->Prev = (b), \
                                              (b)->Next->Prev = (n), \
                                              (b)->Next = (n))
#define SDLLInsertAfter(b, n) SDLLInsertAfter_NP (b, n, Next, Prev)
#define SDLLInsertBefore_NP(b, n, Next, Prev) SDLLInsertAfter_NP (b, n, Prev, Next)
#define SDLLInsertBefore(b, n) SDLLInsertAfter_NP (b, n, Prev, Next)

#define SDLLRemove_NP(n, Next, Prev) ((n)->Next->Prev = (n)->Prev, \
                                      (n)->Prev->Next = (n)->Next, \
                                      (n)->Next = 0, \
                                      (n)->Prev = 0)
#define SDLLRemove(n) SDLLRemove_NP(n, Next, Prev)

#define SDLLInit_NP(s, Next, Prev) ((s)->Next = (s), \
                                    (s)->Prev = (s))
#define SDLLInit(s) SDLLInit_NP(s, Next, Prev)


#define SDLLAlloc_N(n, f, alloc, Next) ((f)->Next == (f) ? \
                                        ((n) = (alloc)) : \
                                        ((n) = (f)->Next, \
                                         SDLLRemove (n)))
#define SDLLAlloc(n, f, alloc) SDLLAlloc_N(n, f, alloc, Next)

// Don't like this, removal from list should be separate to free
// #define SDLLFree(n, f) (SDLLRemove ((n)), \
//                         SDLLInsertAfter ((f), (n))) \

// DLL macros

#define DLLPushBack_NP(f, l, n, Next, Prev) ((f) == 0 ? \
                                             ((f) = (l) = (n), (n)->Next = (n)->Prev = 0) : \
                                             ((n)->Prev = (l), (l)->Next = (n), (l) = (n), (n)->Next = 0))
#define DLLPushFront_NP(f, l, n, Next, Prev) DLLPushBack_NP (l, f, n, Prev, Next)
#define DLLPushBack(f, l, n) DLLPushBack_NP (f, l, n, Next, Prev)
#define DLLPushFront(f, l, n) DLLPushBack_NP (l, f, n, Prev, Next)

#define DLLPushBackL_NP(l, n, Prev, Next) DLLPushBack_NP ((l)->first, (l)->last, n, Prev, Next)


#define DLLInsertAfter_NP(f, l, b, n, Next, Prev) ((l) == (b) ? \
                                                   (DLLPushBack_NP (f, l, n, Next, Prev)) : \
                                                   ((n)->Next = (b)->Next, \
                                                    (n)->Prev = (b), \
                                                    (b)->Next->Prev = (n), \
                                                    (b)->Next = (n)))
#define DLLInsertAfter(f, l, b, n) DLLInsertAfter_NP (f, l, b, n, Next, Prev)
#define DLLInsertBefore(f, l, b, n) DLLInsertAfter_NP (l, f, b, n, Prev, Next)

#define DLLRemove_NP(f, l, n, Next, Prev) (((f) == (n) ? \
                                            ((l) == (n) ? \
                                             ((f) = (l) = 0) : \
                                             ((f) = (f)->Next, (f)->Prev = 0)) : \
                                            ((l) == (n) ? \
                                             ((l) = (l)->Prev, (l)->Next = 0) : \
                                             ((n)->Next->Prev = (n)->Prev, \
                                              (n)->Prev->Next = (n)->Next))), (n)->Next = 0, (n)->Prev = 0)
#define DLLRemove(f, l, n) DLLRemove_NP (f, l, n, Next, Prev)
#define DLLRemoveL_NP(l, n, Next, Prev) DLLRemove_NP ((l)->first, (l)->last, n, Next, Prev)

#define DLLAlloc_N(n, f, l, alloc) ((f) == 0 ? \
                                    ((n) = (alloc)) : \
                                    ((n) = (f), \
                                     DLLRemove (f, l, n)))
#define DLLAlloc(f, l, n, alloc)  DLLAlloc_N (n, f, l, alloc)

#define DLLEmpty(f, l) (((f) == 0) && ((l) == 0))


// SLL queue macros

#define SLLQueuePush_N(f, l, n, Next) ((f) == 0 ? \
                                       (f) = (l) = (n) : \
                                       ((l)->Next = (n), (l) = (n)), \
                                       (n)->Next = 0)
#define SLLQueuePush(f, l, n) SLLQueuePush_N (f, l, n, Next)

#define SLLQueuePushFront_N(f, l, n, Next) ((f) == 0 ? \
                                            ((f) = (l) = (n), (n)->Next = 0) : \
                                            ((n)->Next = (f), (f) = (n)))
#define SLLQueuePushFront(f, l, n) SLLQueuePushFront_N (f, l, n, Next)

#define SLLQueuePop_N(f, l, Next) ((f) == (l) ? \
                                   (f) = (l) = 0 : \
                                   (f) = (f)->Next)
#define SLLQueuePop(f, l) SLLQueuePop_N (f, l, Next)


// SLL stack macros

#define SLLStackPush_N(f, n, Next) ((f) == 0 ? \
                                    ((n)->Next = 0, (f) = (n)) : \
                                    ((n)->Next = (f), (f) = (n)))
#define SLLStackPush(f, n) SLLStackPush_N (f, n, Next)

#define SLLStackPop_N(f, Next) ((f) == 0 ? 0 : \
                                (f) = (f)->Next)
#define SLLStackPop(f) SLLStackPop_N (f, Next)

#define SLLStackAlloc_N(n, f, alloc, Next) ((f) ? \
                                            ((n) = (f), SLLStackPop(f), (n)->Next = 0) : \
                                            ((n) = (alloc), (n)->Next = 0))
#define SLLStackAlloc(n, f, alloc) SLLStackAlloc_N (n, f, alloc, Next)

#define SLLStackFree_N(n, f, Next) ((f) ? \
                                    (SLLStackPush (f, n)) : \
                                    ((f) = (n), (n)->Next = 0))
#define SLLStackFree(n, f) SLLStackFree_N (n, f, Next)

// TODO: Should go in kapsy.h
#define DLLAllocL(list, elem, alloc) DLLAlloc ((list).first, (list).last, (elem), (alloc))
#define DLLPushBackL(list, elem) DLLPushBack ((list).first, (list).last, (elem))
#define DLLPushBackLP(list, elem) DLLPushBack ((list)->first, (list)->last, (elem))
#define DLLInsertBeforeL(list, before, elem) DLLInsertBefore ((list).first, (list).last, (before), (elem))
#define DLLRemoveL(list, elem) DLLRemove ((list).first, (list).last, (elem))


////////////////////////////////////////////////////////////////////////////
// SECTION: MIDI
//
// Comments go here


// 1.0/127.0 (not 128, because we want 127 = 1.0)
#define MIDI_VAL 127.0
#define INV_MIDI_VAL 0.007874015748031
#define MIDI_VEL 127.0
#define INV_MIDI_VEL 0.007874015748031



/*
 * SECTION: ATOMICS
 *
 * Implementation notes go here...
 *
 */
 
// MARK: - HMH LLVM atomic operations
// // TODO: (Kapsy) Does LLVM have specific barrier calls?
#define CompletePreviousReadsBeforeFuturesReads asm volatile("" ::: "memory")
#define CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory") 
// TODO: (Kapsy) From the GCC docs (might be different for LLVM):
// These functions are implemented in terms of the ‘__atomic’ builtins (see
// __atomic Builtins). They should not be used for new code which should use
// the ‘__atomic’ builtins instead.
// bool __sync_bool_compare_and_swap (type *ptr, type oldval, type newval, ...)
// type __sync_val_compare_and_swap (type *ptr, type oldval, type newval, ...)
// These built-in functions perform an atomic compare and swap. That is, if the
// current value of *ptr is oldval, then write newval into *ptr.  The “bool”
// version returns true if the comparison is successful and newval is written.
// The “val” version returns the contents of *ptr before the operation.
// __sync_synchronize (...) TODO: (Kapsy) Need a reliable way of testing atomic
// functions.  TODO: (Kapsy) Do we need to manually add barriers?

inline u32
AtomicCompareExchangeU32_U32(u32 volatile *TheValue, u32 OldValue, u32 NewValue)
{
   u32 Result = __sync_val_compare_and_swap(TheValue, OldValue, NewValue);
   return(Result);
}

function B32
AtomicCompareExchangeU32_B32(u32 volatile *TheValue, u32 OldValue, u32 NewValue)
{
   B32 Result = __sync_bool_compare_and_swap(TheValue, OldValue, NewValue);
   return(Result);
}

function B32
AtomicCompareExchangeU64_B32(u64 volatile *TheValue, u64 OldValue, u64 NewValue)
{
   B32 Result = __sync_bool_compare_and_swap(TheValue, OldValue, NewValue);
   return(Result);
}



// TODO: (KAPSY) What does this guy actually do?
// What value is actually returned? Does it always perform the operation?
function u64
AtomicExchangeU64_U64(u64 volatile *TheValue, u64 NewValue)
{
    u64 Result = __sync_lock_test_and_set(TheValue, NewValue);
    return(Result);
}

function u64
AtomicAddU64_U64(u64 volatile *TheValue, u64 Addend)
{
    // NOTE: (Kapsy) Returns the original value, prior to adding.
    u64 Result = __sync_fetch_and_add(TheValue, Addend);
    return(Result);
}

function u32
AtomicAddU32(u32 volatile *TheValue, u32 Addend)
{
    // NOTE: (Kapsy) Returns the original value, prior to adding.
    // NOTE: (KAPSY) Testing (see cmpxchg_test_a) proves that the add is always
    // performed, however this is not the case for
    // __sync_bool_compare_and_swap.
    u32 Result = __sync_fetch_and_add(TheValue, Addend);
    return(Result);
}



function u32
AtomicIncrementU32(u32 volatile *TheValue)
{
    u32 Result = AtomicAddU32(TheValue, 1);
    return(Result);
}

function u32
AtomicDecrementU32(u32 volatile *TheValue)
{
    u32 Result = __sync_fetch_and_add(TheValue, -1);
    return(Result);
}

// NOTE (MJP): Simplified atomic functions
function b32
AtomicCompareAndSwapBool(u64 volatile *TheValue, u64 OldValue, u64 NewValue)
{
   b32 Result = __sync_bool_compare_and_swap(TheValue, OldValue, NewValue);
   return(Result);
}

function u64
AtomicLoad(u64 volatile *TheValue)
{
   // NOTE: (Kapsy) Returns the original value, prior to adding.
   u64 Result = __sync_fetch_and_add(TheValue, 0);
   return(Result);
}

function u64
AtomicAdd(u64 volatile *TheValue, u64 Addend)
{
    // NOTE: (Kapsy) Returns the original value, prior to adding.
    u64 Result = __sync_fetch_and_add(TheValue, Addend);
    return(Result);
}


/*
 * SECTION: MEMORY ARENAS
 *
 *
 */




// 
// SECTION: STRINGS
// 
//




// TODO Replace with String8 like implementation
inline int
StringLength(char *String)
{
    int Count = 0;
    while(*String++)
    {
        ++Count;
    }
    return(Count);
}

// TODO (MJP): Use RJF String8 like functions
inline u32
CatStrings (char *a, char *b, char *out)
{
    char *outat = out;

    char *aat = a;
    u32 alen = StringLength(a);

    char *bat = b;
    u32 blen = StringLength(b);

    for (u32 i=0 ; i<alen ; i++)
        *outat++ = *aat++;
        
    for (u32 i=0 ; i<blen ; i++)
        *outat++ = *bat++;

    u32 res = (u32) (outat - out);

    return (res);
}

//
// SECTION: CHUNK ALLOCATOR
//
// This is a basic heap allocator, that operates with a fixed chunk size to
// reduce fragmentation and the realloc frequency. 
//
// Allocates from a Arena (that should be expandable using virtual alloc etc) for now.
//
// TODO (MJP): To be honest, I don't know if it's faster than 8 aligned
// allocations, and might well be. Need to make the chunk/alignment size
// adjustable, so implementer can decide based on use case.
//

// NOTE (MJP): Chunk allocator types
#define CHUNK_ALLOCATOR_CHUNK_SIZE 64
#define CHUNK_ALLOCATOR_CHUNK_COUNT (1 << 18)

struct chunk_region
{
   chunk_region *Next;
   chunk_region *Prev;
   u32  StartIndex;
   u32  Count;
};

struct chunk_allocator
{
   // NOTE (MJP): Ordered list for tracking free regions, not to be confused
   // with FreeListSentinel
   chunk_region *FreeRegionsSentinel;
   chunk_region *AllocatedRegionsSentinel;

   // NOTE (MJP): For recycling chunk_regions, NOT tracking free regions
   chunk_region *FreeListSentinel;

   M_Arena *Arena;

   // TODO (MJP): For now, will allocate up front from same arena, but would
   // like to be expandable, so will probably have a different arena for chunk
   // memory and allocation meta members.
   u8 *ChunkMemory;
};

function u32
GetChunkCount(u32 SizeBytes)
{
   u32 ChunkCount =
      (SizeBytes + (CHUNK_ALLOCATOR_CHUNK_SIZE - 1))/CHUNK_ALLOCATOR_CHUNK_SIZE;
   return(ChunkCount);
}

function u32
GetSizeBytes(u32 ChunkCount)
{
   u32 SizeBytes = ChunkCount*CHUNK_ALLOCATOR_CHUNK_SIZE;
   return(SizeBytes);
}

function void
InsertRegionInOrder(chunk_region *Sentinel, chunk_region *ChunkRegion)
{
   chunk_region *NextRegion = Sentinel->Next;
   for (; NextRegion != Sentinel;
          NextRegion = NextRegion->Next)
   {
      if (NextRegion->StartIndex > ChunkRegion->StartIndex)
      {
         break;
      }
   }
   SDLLInsertBefore(NextRegion, ChunkRegion);
}

function chunk_region *
NewChunkRegion(chunk_allocator *ChunkAllocator, u32 StartIndex, u32 Count)
{
   chunk_region *ChunkRegion = 0;
   SDLLAlloc(ChunkRegion, ChunkAllocator->FreeListSentinel,
             RJF_PushArray(ChunkAllocator->Arena, chunk_region, 1));
   Assert(ChunkRegion);
   ZeroStruct(*ChunkRegion);

   ChunkRegion->StartIndex = StartIndex;
   ChunkRegion->Count = Count;

   return(ChunkRegion);
}

function void
RemoveChunkRegion(chunk_allocator *ChunkAllocator, chunk_region *ChunkRegion)
{
   SDLLRemove(ChunkRegion);
   SDLLInsertAfter(ChunkAllocator->FreeListSentinel, ChunkRegion);
}


function void *
GetMemoryAddress(chunk_allocator *Allocator, u32 ChunkIndex)
{
   void *Address =
      (void *) (Allocator->ChunkMemory +
       ChunkIndex*CHUNK_ALLOCATOR_CHUNK_SIZE);
   Assert(Address);
   return(Address);
}

function void *
GetMemoryAddress(chunk_allocator *Allocator, chunk_region *ChunkRegion)
{
   void *Address =
      (void *) (Allocator->ChunkMemory +
       ChunkRegion->StartIndex*CHUNK_ALLOCATOR_CHUNK_SIZE);
   Assert(Address);
   return(Address);
}

function chunk_region *
GetAllocatedRegion(chunk_allocator *ChunkAllocator, void *RegionStartAddress)
{
   chunk_region *FoundAllocatedRegion = 0;

   u32 ChunkStartIndex = 
      (u32)(((memory_index)RegionStartAddress -
             (memory_index)ChunkAllocator->ChunkMemory)/
             CHUNK_ALLOCATOR_CHUNK_SIZE);

   // TODO (MJP): Dictionary is probably better than iterating like this every time.
   for (chunk_region *AllocatedRegion = ChunkAllocator->AllocatedRegionsSentinel->Next;
                      AllocatedRegion != ChunkAllocator->AllocatedRegionsSentinel;
                      AllocatedRegion = AllocatedRegion->Next)
   {
      if (AllocatedRegion->StartIndex == ChunkStartIndex)
      {
         FoundAllocatedRegion = AllocatedRegion;
         break;
      }
   }
   return(FoundAllocatedRegion);
}

function void
MergeNeighboringFreeRegions(chunk_allocator *ChunkAllocator)
{

}

function void
SetupAllocator(chunk_allocator *ChunkAllocator)
{
   ChunkAllocator->Arena = M_ArenaAlloc((1 << 20)*20);

   u32 ChunkMemorySize =
      CHUNK_ALLOCATOR_CHUNK_SIZE*CHUNK_ALLOCATOR_CHUNK_COUNT;
   ChunkAllocator->ChunkMemory =
      (u8 *)M_ArenaPushAligned(ChunkAllocator->Arena,
                               ChunkMemorySize, L2_CACHE_SIZE);

   ChunkAllocator->AllocatedRegionsSentinel = 
      RJF_PushArray(ChunkAllocator->Arena, chunk_region, 1);
   ChunkAllocator->AllocatedRegionsSentinel->Next = 
   ChunkAllocator->AllocatedRegionsSentinel->Prev = 
   ChunkAllocator->AllocatedRegionsSentinel;

   ChunkAllocator->FreeRegionsSentinel = 
      RJF_PushArray(ChunkAllocator->Arena, chunk_region, 1);
   ChunkAllocator->FreeRegionsSentinel->Next = 
   ChunkAllocator->FreeRegionsSentinel->Prev = 
   ChunkAllocator->FreeRegionsSentinel;

   ChunkAllocator->FreeListSentinel = 
      RJF_PushArray(ChunkAllocator->Arena, chunk_region, 1);
   ChunkAllocator->FreeListSentinel->Next = 
   ChunkAllocator->FreeListSentinel->Prev = 
   ChunkAllocator->FreeListSentinel;

   // Setup the first free region
   chunk_region *FreeRegion =
      NewChunkRegion(ChunkAllocator, 0, CHUNK_ALLOCATOR_CHUNK_COUNT);
   InsertRegionInOrder(ChunkAllocator->FreeRegionsSentinel, FreeRegion);
}

function void
CheckForClashingRegions(chunk_allocator *ChunkAllocator)
{
   for (chunk_region *AllocatedRegion = ChunkAllocator->AllocatedRegionsSentinel->Next;
                      AllocatedRegion != ChunkAllocator->AllocatedRegionsSentinel;
                      AllocatedRegion = AllocatedRegion->Next)
   {
      u32 AllocatedRegionChunkStartIndex = AllocatedRegion->StartIndex;
      u32 AllocatedRegionChunkOPEIndex = AllocatedRegion->StartIndex + AllocatedRegion->Count;

      for (chunk_region *TestRegion = AllocatedRegion->Next;
            TestRegion != ChunkAllocator->AllocatedRegionsSentinel;
            TestRegion = TestRegion->Next)
      {
         u32 TestRegionChunkStartIndex = TestRegion->StartIndex;
         u32 TestRegionChunkOPEIndex = TestRegion->StartIndex + TestRegion->Count;

         Assert(((TestRegionChunkStartIndex >= AllocatedRegionChunkOPEIndex) &&
                 (TestRegionChunkOPEIndex >= AllocatedRegionChunkOPEIndex)) ||
                ((TestRegionChunkStartIndex <= AllocatedRegionChunkStartIndex) &&
                 (TestRegionChunkOPEIndex <= AllocatedRegionChunkStartIndex)));
      }

      for (chunk_region *TestRegion = ChunkAllocator->FreeRegionsSentinel->Next;
                         TestRegion != ChunkAllocator->FreeRegionsSentinel;
                         TestRegion = TestRegion->Next)
      {
         u32 TestRegionChunkStartIndex = TestRegion->StartIndex;
         u32 TestRegionChunkOPEIndex = TestRegion->StartIndex + TestRegion->Count;

         Assert(((TestRegionChunkStartIndex >= AllocatedRegionChunkOPEIndex) &&
                 (TestRegionChunkOPEIndex >= AllocatedRegionChunkOPEIndex)) ||
                ((TestRegionChunkStartIndex <= AllocatedRegionChunkStartIndex) &&
                 (TestRegionChunkOPEIndex <= AllocatedRegionChunkStartIndex)));
      }
   }

   for (chunk_region *FreeRegion = ChunkAllocator->FreeRegionsSentinel->Next;
                      FreeRegion != ChunkAllocator->FreeRegionsSentinel;
                      FreeRegion = FreeRegion->Next)
   {
      u32 FreeRegionChunkStartIndex = FreeRegion->StartIndex;
      u32 FreeRegionChunkOPEIndex = FreeRegion->StartIndex + FreeRegion->Count;

      for (chunk_region *TestRegion = FreeRegion->Next;
                         TestRegion != ChunkAllocator->FreeRegionsSentinel;
                         TestRegion = TestRegion->Next)
      {
         u32 TestRegionChunkStartIndex = TestRegion->StartIndex;
         u32 TestRegionChunkOPEIndex = TestRegion->StartIndex + TestRegion->Count;

         Assert(((TestRegionChunkStartIndex >= FreeRegionChunkOPEIndex) &&
                 (TestRegionChunkOPEIndex >= FreeRegionChunkOPEIndex)) ||
                ((TestRegionChunkStartIndex <= FreeRegionChunkStartIndex) &&
                 (TestRegionChunkOPEIndex <= FreeRegionChunkStartIndex)));
      }
   }
}

function chunk_region *
AllocateChunkRegion(chunk_allocator *ChunkAllocator, u32 ChunkCount)
{
   Assert(ChunkCount);
   Assert(ChunkAllocator->ChunkMemory);

   void *RegionStartAddress = 0;

   chunk_region *AllocatedRegion = 0;
   for (chunk_region *FreeRegion = ChunkAllocator->FreeRegionsSentinel->Next;
                      FreeRegion != ChunkAllocator->FreeRegionsSentinel;
                      FreeRegion = FreeRegion->Next)
   {
      if (ChunkCount <= FreeRegion->Count)
      {
         SDLLRemove(FreeRegion);
         AllocatedRegion = FreeRegion;
         InsertRegionInOrder(ChunkAllocator->AllocatedRegionsSentinel, AllocatedRegion);

         u32 RemainingChunks = AllocatedRegion->Count - ChunkCount;
         if (RemainingChunks)
         {
            u32 NewFreeRegionStartIndex = 
                  (AllocatedRegion->StartIndex + ChunkCount);
            chunk_region *NewFreeRegion =
               NewChunkRegion(ChunkAllocator, NewFreeRegionStartIndex, RemainingChunks);
            Assert(NewFreeRegion->StartIndex != AllocatedRegion->StartIndex);
            InsertRegionInOrder(ChunkAllocator->FreeRegionsSentinel, NewFreeRegion);
            // MergeNeighboringFreeRegions(ChunkAllocator);
         }

         AllocatedRegion->Count = ChunkCount;
         break;
      }
   }
   Assert(AllocatedRegion);

   return(AllocatedRegion);
}

#if 0 // old
void *
NewAllocationVoid(chunk_allocator *ChunkAllocator, u32 SizeBytes)
{
   Assert(ChunkAllocator->ChunkMemory);

   u32 ChunkCount = GetChunkCount(SizeBytes);
   chunk_region *AllocatedRegion =
      AllocateChunkRegion(ChunkAllocator, ChunkCount);

   void *RegionStartAddress = 
      GetMemoryAddress(ChunkAllocator, AllocatedRegion);

   // TODO (MJP): Asserting here for now, but really up to caller to check if allocation suceeded.
   // However, when we support expandable arenas, will want to assert here.
   Assert(RegionStartAddress);
   return(RegionStartAddress);
}
#endif

function void *
ResizeAllocationVoid(chunk_allocator *ChunkAllocator, void *RegionStartAddress, u32 SizeBytes)
{
   chunk_region *SourceRegion =
      GetAllocatedRegion(ChunkAllocator, RegionStartAddress);

   Assert(ChunkAllocator->ChunkMemory);
   void *NewRegionStartAddress = 0x0;
   u32 NewChunkCount = GetChunkCount(SizeBytes);

   if (NewChunkCount)
   {
      // TODO (MJP): Check neighboring regions and if free, and have enough
      // space, expand the region to avoid copying

      chunk_region *DestRegion =
         AllocateChunkRegion(ChunkAllocator, NewChunkCount);

      if (SourceRegion)
      {
         // Copy and free
         void *SourceMemory =
            GetMemoryAddress(ChunkAllocator, SourceRegion);
         void *DestMemory =
            GetMemoryAddress(ChunkAllocator, DestRegion);

         u32 CopySizeBytes =
            GetSizeBytes(Min(SourceRegion->Count, DestRegion->Count));
         u32 DestSizeBytes = GetSizeBytes(DestRegion->Count);
         Assert(CopySizeBytes <= DestSizeBytes);
         MemCopy(DestMemory, SourceMemory, CopySizeBytes);

         // Move region to free regions
         SDLLRemove(SourceRegion);
         InsertRegionInOrder(ChunkAllocator->FreeRegionsSentinel, SourceRegion);

         Assert(GetMemoryAddress(ChunkAllocator, DestRegion) == DestMemory);
      }

      NewRegionStartAddress = GetMemoryAddress(ChunkAllocator, DestRegion);
   }
   else
   {
      if (SourceRegion)
      {
         SDLLRemove(SourceRegion);
         InsertRegionInOrder(ChunkAllocator->FreeRegionsSentinel, SourceRegion);
      }
      NewRegionStartAddress = 0x0;
   }

   MergeNeighboringFreeRegions(ChunkAllocator);

   // CheckForClashingRegions(ChunkAllocator);
   return(NewRegionStartAddress);
}

#define NULL_ALLOCATION 0x0
// NOTE (MJP): User facing API
#define NewAllocation(Allocator, Type, Count) (Type *)ResizeAllocationVoid(Allocator, (void *)NULL_ALLOCATION, SizeOf(Type)*Count)
#define ResizeAllocation(Allocator, Allocation, Type, Count) (Type *)ResizeAllocationVoid((Allocator), (void *)(Allocation), SizeOf(Type)*(Count))


#define MJP_H
#endif
