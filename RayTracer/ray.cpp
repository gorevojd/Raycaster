#include <stdio.h>
#include <stdlib.h>
#include "ray.h"
#include <time.h>

#include <Windows.h>

INTERNAL_FUNCTION u32 
GetTotalPixelSie(image_u32 Image) {
	u32 Result = Image.Width * Image.Height * sizeof(u32);
	return(Result);
}

INTERNAL_FUNCTION u32* GetPixelPointer(image_u32* Image, u32 X, u32 Y) {
	u32* Result = Image->Pixels + Y * Image->Width + X;
	return(Result);
}

INTERNAL_FUNCTION image_u32 AllocateImage(u32 Width, u32 Height) {
	image_u32 Image = {};

	u32 OutputPixelsCount = Width * Height;
	Image.Pixels = (u32*)malloc(OutputPixelsCount * 4);
	Image.Width = Width;
	Image.Height = Height;

	return(Image);
}

INTERNAL_FUNCTION void DeallocateImage(image_u32 Image) {
	free(Image.Pixels);
}

INTERNAL_FUNCTION void WriteImage(image_u32 Image, char* FileName) {
	u32 OutputPixelsSize = GetTotalPixelSie(Image);

	bitmap_header Header = {};
	Header.FileType = 0x4D42;
	Header.FileSize = sizeof(Header) + OutputPixelsSize;
	Header.BitmapOffset = sizeof(Header);
	Header.Size = sizeof(Header) - 14;
	Header.Width = Image.Width;
	Header.Height = Image.Height;
	Header.Planes = 1;
	Header.BitsPerPixel = 32;
	Header.Compression = 0;
	Header.SizeOfBitmap = OutputPixelsSize;

	FILE* fp = fopen(FileName, "wb");
	if (fp) {

		fwrite(&Header, sizeof(bitmap_header), 1, fp);
		fwrite(Image.Pixels, 1, OutputPixelsSize, fp);

		fclose(fp);
	}
}

inline f32 
RandomUnilateral() {
	f32 Result = rand() / (f32)RAND_MAX;
	return(Result);
}

inline f32
RandomBilateral() {
	f32 Result = -1.0f + 2 * RandomUnilateral();
	return(Result);
}

INTERNAL_FUNCTION f32
ExactlinearToSRGB(f32 L) {
	if (L < 0.0f) {
		L = 0.0f;
	}

	if (L > 1.0f) {
		L = 1.0f;
	}
	
	f32 S = L * 12.92f;
	if (L >  0.0031308f) {
		S = 1.055f * pow(L, 1.0f / 2.4f) - 0.055f;
	}

	return(S);
}

INTERNAL_FUNCTION u64 LockedAddWithPreviousReturn(u64 volatile* Value, u64 Addend) {
	u64 Result = InterlockedExchangeAdd64((volatile LONGLONG *)Value, Addend);
	return(Result);
}

b32 RenderTile(work_queue* Queue)
{
	u32 WorkOrderIndex = LockedAddWithPreviousReturn(&Queue->NextWorkOrderIndex, 1);
	if (WorkOrderIndex >= Queue->WorkOrderCount) {
		return(false);
	}

	work_order* Order = Queue->WorkOrders + WorkOrderIndex;
	world* World = Order->World;
	image_u32* Image = &Order->Image;
	u32 XMin = Order->XMin;
	u32 OnePastXMax = Order->OnePastXMax;
	u32 YMin = Order->YMin;
	u32 OnePastYMax = Order->OnePastYMax;

	f32 FilmDist = 1.0f;
	f32 FilmW = 1.0f;
	f32 FilmH = 1.0f;
	v3 FilmCenter = World->CameraP - FilmDist * World->CameraZ;

	if (Image->Width > Image->Height) {
		FilmH = FilmW * ((f32)Image->Height / (f32)Image->Width);
	}
	else if (Image->Height > Image->Width) {
		FilmW = FilmH * ((f32)Image->Width / (f32)Image->Height);
	}
	f32 HalfDimW = 0.5f * FilmW;
	f32 HalfDimH = 0.5f * FilmH;

	f32 HalfPixW = 0.5f / (f32)Image->Width;
	f32 HalfPixH = 0.5f / (f32)Image->Height;

	u32 RaysPerPixel = Queue->RaysPerPixel;
	f32 ContributionValue = 1.0f / (f32)RaysPerPixel;
	for (u32 VerticalIndex = YMin; VerticalIndex < OnePastYMax; VerticalIndex++) {

		u32* Out = GetPixelPointer(Image, XMin, VerticalIndex);

		f32 FilmY = 2.0f * ((f32)VerticalIndex / (f32)Image->Height) - 1.0f;
		for (u32 HorizontalIndex = XMin; HorizontalIndex < OnePastXMax; HorizontalIndex++) {

			f32 FilmX = 2.0f * ((f32)HorizontalIndex / (f32)Image->Width) - 1.0f;

			v3 Color = {};
			for (u32 RayIndex = 0;
				RayIndex < RaysPerPixel;
				RayIndex++)
			{
				f32 OffX = FilmX + RandomBilateral() * HalfPixW;
				f32 OffY = FilmY + RandomBilateral() * HalfPixH;
				v3 FilmP = FilmCenter + OffX * HalfDimW * World->CameraX + OffY * HalfDimH * World->CameraY;

				v3 RayOrigin = World->CameraP;
				v3 RayDirection = NOZ(FilmP - World->CameraP);


				/*RAYCAST BEGINS HERE*/
				v3 ResultRaycastColor = {};

				f32 MinHitDistance = 0.0001f;
				f32 Tolerance = 0.0001f;

				LockedAddWithPreviousReturn(&Queue->TotalRaysCast, 1);

				v3 Attenuation = V3(1.0f, 1.0f, 1.0f);
				for (u32 BounceCount = 0;
					BounceCount < Queue->MaxBounceCount;
					++BounceCount)
				{
					f32 HitDistance = 10000000.0f;

					u32 HitMaterialIndex = 0;

					v3 NextNormal = {};

					LockedAddWithPreviousReturn(&Queue->BouncesComputed, 1);

					for (u32 PlaneIndex = 0;
						PlaneIndex < World->PlaneCount;
						++PlaneIndex)
					{
						plane Plane = World->Planes[PlaneIndex];

						f32 Denominator = Dot(Plane.N, RayDirection);
						if ((Denominator < -Tolerance) || (Denominator > Tolerance)) {
							f32 ThisDistance = (-Plane.d - Dot(Plane.N, RayOrigin)) / Denominator;
							if ((ThisDistance > MinHitDistance) && (ThisDistance < HitDistance)) {
								HitDistance = ThisDistance;
								HitMaterialIndex = Plane.MatIndex;

								NextNormal = Plane.N;
							}
						}
					}

					for (u32 SphereIndex = 0;
						SphereIndex < World->SphereCount;
						SphereIndex++)
					{
						sphere Sphere = World->Spheres[SphereIndex];

						v3 SphereRelativeRayOrigin = RayOrigin - Sphere.P;
						float ACoef = Dot(RayDirection, RayDirection);
						float BCoef = 2.0f * Dot(SphereRelativeRayOrigin, RayDirection);
						float CCoef = Dot(SphereRelativeRayOrigin, SphereRelativeRayOrigin) - Sphere.r * Sphere.r;

						float Discriminant = BCoef * BCoef - 4.0f * ACoef * CCoef;
						float RootTerm = Sqrt(Discriminant);

						float Denom = 2.0f * ACoef;
						if (RootTerm > Tolerance) {
							f32 Solution1 = (-BCoef - RootTerm) / Denom;
							f32 Solution2 = (-BCoef + RootTerm) / Denom;

							f32 t = Solution1;
							if ((Solution2 > MinHitDistance) && (Solution2 < Solution1)) {
								t = Solution2;
							}

							if ((t > MinHitDistance) && (t < HitDistance)) {
								HitDistance = t;
								HitMaterialIndex = Sphere.MatIndex;

								NextNormal = NOZ(HitDistance * RayDirection + SphereRelativeRayOrigin);
							}
						}
					}

					if (HitMaterialIndex) {
						material Mat = World->Materials[HitMaterialIndex];

						ResultRaycastColor += Hadamard(Attenuation, Mat.EmitColor);
						f32 AttenuationCos = 1.0f;
#if 1
						AttenuationCos = Dot(-RayDirection, NextNormal);
						if (AttenuationCos < 0.0f) {
							AttenuationCos = 0.0f;
						}
#endif
						Attenuation = Hadamard(Attenuation, AttenuationCos * Mat.RefColor);

						RayOrigin += HitDistance * RayDirection;
						v3 ReflectedBounce = RayDirection - 2.0f * Dot(NextNormal, RayDirection) * NextNormal;
						v3 RandomVector = V3(RandomBilateral(), RandomBilateral(), RandomBilateral());
						v3 RandomBounce = NOZ(NextNormal + RandomVector);
						RayDirection = NOZ(Lerp(RandomBounce, ReflectedBounce, Mat.Specular));
					}
					else {
						material Mat = World->Materials[HitMaterialIndex];
						ResultRaycastColor += Hadamard(Attenuation, Mat.EmitColor);

						break;
					}
				}
				/*RAYCAST ENDS HERE*/

				Color += ContributionValue * ResultRaycastColor;
			}

#if 0
			v4 BMPColor = Linear1ToSRGB255(V4(Color, 1.0f));
#else
			v4 BMPColor = {
				255.0f * ExactlinearToSRGB(Color.x),
				255.0f * ExactlinearToSRGB(Color.y),
				255.0f * ExactlinearToSRGB(Color.z),
				255.0f };
#endif
			u32 BMPValue = BGRAPack4x8(BMPColor);

			*Out++ = BMPValue;
		}
	}

	LockedAddWithPreviousReturn(&Queue->RenderedTileCount, 1);

	return(true);
}

INTERNAL_FUNCTION DWORD WINAPI
WorkerThread(void* Param) {
	work_queue* Queue = (work_queue*)Param;

	while (RenderTile(Queue)) {}

	return(0);
}

INTERNAL_FUNCTION u32
GetCPUCoreCount() {
	u32 Result = 0;

	SYSTEM_INFO Info;
	GetSystemInfo(&Info);
	Result = Info.dwNumberOfProcessors;

	return(Result);
}

INTERNAL_FUNCTION void CreateWorkThread(void* Param) {
	DWORD ThreadID;
	HANDLE ThreadHandle = CreateThread(0, 0, WorkerThread, Param, 0, &ThreadID);
	CloseHandle(ThreadHandle);
}

int main(int ArgsCount, char** Args) {

	material Materials[5] = {};
	Materials[0].EmitColor = V3(0.3f, 0.4f, 0.5f);
	Materials[1].RefColor = V3(0.5f, 0.5f, 0.5f);
	Materials[2].RefColor = V3(0.7f, 0.5f, 0.3f);
	Materials[3].RefColor = V3(0.7f, 0.0f, 0.0f);
	Materials[4].RefColor = V3(0.0f, 0.5f, 0.9f);
	Materials[4].Specular = 0.9f;

	plane Planes[1] = {};
	Planes[0].N = V3(0, 0, 1);
	Planes[0].d = 0;
	Planes[0].MatIndex = 1;

	sphere Spheres[3] = {};
	Spheres[0].P = V3(0, 0, 0);
	Spheres[0].r = 1.0f;
	Spheres[0].MatIndex = 2;

	Spheres[1].P = V3(3, -2, 0);
	Spheres[1].r = 1.0f;
	Spheres[1].MatIndex = 3;
	
	Spheres[2].P = V3(-2, -1, 2);
	Spheres[2].r = 1.0f;
	Spheres[2].MatIndex = 4;

#define ArrayCount(arr) ((sizeof(arr)) / (sizeof((arr)[0])))
	world World = {};
	World.MaterialCount = ArrayCount(Materials);
	World.Materials = Materials;
	World.PlaneCount = ArrayCount(Planes);
	World.Planes = Planes;
	World.Spheres = Spheres;
	World.SphereCount = ArrayCount(Spheres);

	World.CameraP = {0, -10, 1};
	World.CameraZ = NOZ(World.CameraP);
	World.CameraX = NOZ(Cross(World.CameraZ, V3(0, 0, 1)));
	World.CameraY = NOZ(Cross(World.CameraX, World.CameraZ));

	image_u32 Image = AllocateImage(1366, 768);

	u32 CoreCount = GetCPUCoreCount();
	
	u32 TileWidth = 128;
	u32 TileHeight = TileWidth;
	u32 TileYCount = (Image.Height + (TileHeight - 1)) / TileHeight;
	u32 TileXCount = (Image.Width + (TileWidth - 1)) / TileWidth;

	u32 TotalTileCount = TileYCount * TileXCount;

	work_queue Queue = {};
	Queue.WorkOrders = (work_order*)malloc(TotalTileCount * sizeof(work_order));
	Queue.RaysPerPixel = 32;
	Queue.MaxBounceCount = 8;

	work_order* WorkOrder = Queue.WorkOrders;

	printf("Configuration: %d cores with %d %dx%d (%dk/tile) tiles\n",
		CoreCount, TotalTileCount, 
		TileWidth, TileHeight, 
		TileWidth * TileHeight * 4 / 1024);

	printf("Quality: %d rays/pixel, %d max bounces per ray\n",
		Queue.RaysPerPixel, Queue.MaxBounceCount);

	for (u32 TileY = 0;
		TileY < TileYCount;
		TileY++)
	{
		u32 MinY, MaxY;
			
		MinY = TileY * TileHeight;
		MaxY = (TileY + 1) * TileHeight;
	
		if (TileY == (TileYCount - 1)) {
			MaxY = Image.Height;
		}
		
		for (u32 TileX = 0;
			TileX < TileXCount;
			TileX++)
		{
			u32 MinX, MaxX;
			
			MinX = TileX * TileWidth;
			MaxX = (TileX + 1) * TileWidth;

			if (TileX == (TileXCount - 1)) {
				MaxX = Image.Width;
			}

			work_order* Order = Queue.WorkOrders + Queue.WorkOrderCount++;
			Order->Image = Image;
			Order->World = &World;
			Order->XMin = MinX;
			Order->OnePastXMax = MaxX;
			Order->YMin = MinY;
			Order->OnePastYMax = MaxY;

		}
	}
	Assert(Queue.WorkOrderCount == TotalTileCount);

	LockedAddWithPreviousReturn(&Queue.NextWorkOrderIndex, 0);


	clock_t BeginClock = clock();

#if 1
	for (u32 CoreIndex = 1;
		CoreIndex < CoreCount;
		++CoreIndex)
	{
		CreateWorkThread(&Queue);
	}
#endif

	while (Queue.RenderedTileCount < TotalTileCount) {
		if (RenderTile(&Queue)) {
			printf("\r%d tiles rendered", Queue.RenderedTileCount);

			fflush(stdout);
		}
	}

	WriteImage(Image, "test.bmp");
	DeallocateImage(Image);

	printf("\nDone\n");

	clock_t ClocksElapsed = clock() - BeginClock;
	float TimeElapsed = (float)ClocksElapsed / (float)CLOCKS_PER_SEC;
	printf("%.2fs elapsed\n", TimeElapsed);

	printf("Total rays casted: %llu\n", Queue.TotalRaysCast);
	printf("Bounces computed: %llu\n", Queue.BouncesComputed);
	printf("Perfomance: %fms/bounce\n", (f64)TimeElapsed * 1000.0f / (f64)Queue.BouncesComputed);

	system("pause");
	return(0);
}