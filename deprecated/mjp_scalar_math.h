#if !defined(MJP_SCALAR_MATH_H)

#define PI 3.141592653589793
#define TAU 6.283185307179586

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


// NOTE (mjp): Vector types, operators and functions

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
ClampBiR32 (r32 x)
{
   r32 y = Clamp (-1.f, x, 1.f);
   return (y);
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

#define MJP_SCALAR_MATH_H
#endif
