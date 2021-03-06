#ifndef RAYTRACER_H

#define INTERNAL_FUNCTION static
#define Assert(cond) if(!(cond)){*(int*)0 = 0;}

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef int s32;
typedef short s16;
typedef char s8;

typedef float f32;
typedef double f64;

#include "ray_math.h"

#pragma pack(push, 1)
struct bitmap_header {
	u16 FileType; /*File type, always 4d42 ("BM")*/
	u32 FileSize; /*Size of the file in bytes*/
	u16 Reserved1; /*Always 0*/
	u16 Reserved2; /*Always 0*/
	u32 BitmapOffset; /*Starting position of image data in bytes*/

	u32 Size; /*Size of header in bytes*/
	s32 Width; /*Image width in pixels*/
	s32 Height; /*Image height in pixels*/
	u16 Planes; /*Number of color planes*/
	u16 BitsPerPixel; /*Number of bits per pixel*/

	u32 Compression; /*Compression methods used*/
	u32 SizeOfBitmap; /*Size of bitmap in bytes*/
	s32 HorzResolution;/*Horizontal resolution in pixels per meter*/
	s32 VertResolution;/*Vertical resolution in pixels per meter*/
	u32 ColorsUsed;/*Number of colors in the image*/
	u32 ColorsImportant;/*Minimum number of important colors*/
};
#pragma pack(pop)

struct image_u32 {
	u32 Width;
	u32 Height;
	u32* Pixels;
};

struct material {
	f32 Specular;
	v3 EmitColor;
	v3 RefColor;
};

struct plane {
	v3 N;
	f32 d;
	u32 MatIndex;
};

struct sphere {
	v3 P;
	f32 r;
	u32 MatIndex;
};


struct world {
	u32 MaterialCount;
	material* Materials;

	u32 PlaneCount;
	plane* Planes;

	u32 SphereCount;
	sphere* Spheres;

	v3 CameraP;
	v3 CameraZ;
	v3 CameraX;
	v3 CameraY;
};

struct work_order {
	world* World;
	image_u32 Image;
	u32 XMin;
	u32 OnePastXMax;
	u32 YMin;
	u32 OnePastYMax;
};

struct work_queue {
	u32 WorkOrderCount;
	work_order* WorkOrders;

	u32 RaysPerPixel;
	u32 MaxBounceCount;
	volatile u64 NextWorkOrderIndex;
	volatile u64 BouncesComputed;
	volatile u64 TotalRaysCast;
	volatile u64 RenderedTileCount;
};


#define RAYTRACER_H
#endif