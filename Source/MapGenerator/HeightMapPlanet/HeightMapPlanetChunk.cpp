// Fill out your copyright notice in the Description page of Project Settings.


#include "HeightMapPlanetChunk.h"
#include "MapGenerator/HeightMapPlanet/HeightMapPlanetManager.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "MapGenerator/HeightMapPlanet/HeightMapPlanet.h"
#include "Providers/RuntimeMeshProviderStatic.h"




void AHeightMapPlanetChunk::Initialize(AHeightMapPlanet * _Owner, FVector _ChunkOrientation) {
	PlanetOwner = _Owner;
	ChunkOrientation = _ChunkOrientation;
	ChunkAxisA = FVector(ChunkOrientation.Z, ChunkOrientation.X, ChunkOrientation.Y);
	ChunkAxisB = FVector::CrossProduct(ChunkOrientation, ChunkAxisA);
}

void AHeightMapPlanetChunk::GenerateChunk() {

	ElemSize = PlanetOwner->ElemSize;
	ChunkSize = PlanetOwner->ChunkSize;
	ChunkTotalHalfSize = ChunkSize * ElemSize * 0.5f;//Potentially bug
	const FVector PlanetCenter = PlanetOwner->PlanetCenter;

	const float radius = PlanetOwner->Manager->PlanetRadius;
	const bool ShowCube = PlanetOwner->Manager->ShowCube;

	Scale = PlanetOwner->Manager->Scale;
	if (Scale == 0.0f) {
		Scale = 0.001f;
	}
	Noise.SetNoiseType(NoiseType::Perlin);

	Persistance = PlanetOwner->Manager->Persistance;
	Lacunarity = PlanetOwner->Manager->Lacunarity;
	const float Height = PlanetOwner->Manager->height;

	Curve = PlanetOwner->Manager->Curve;
	
	URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this);
	if (StaticProvider != nullptr) {
		//Vertices are added in local and not in world
		
		GetRuntimeMeshComponent()->Initialize(StaticProvider);

		TArray<FVector> Vertices;
		Vertices.Reserve((ChunkSize + 1) * (ChunkSize + 1));
		TArray<int> Triangles;
		Triangles.Reserve(ChunkSize * ChunkSize * 6);
		TArray<FColor> Colors;//No reserve because not using for now;
		TArray<FVector> Normals;
		Normals.SetNumUninitialized(ChunkSize * ChunkSize);
		TArray<FVector2D> UVs;
		UVs.Reserve((ChunkSize + 1) * (ChunkSize + 1));
		TArray<FRuntimeMeshTangent> Tangents;//No reserve because not using for now;

		TArray<int> VerticesBottomLineCache;
		VerticesBottomLineCache.Reserve(ChunkSize + 1);
		TArray<int> VerticesTopLineCache;
		VerticesBottomLineCache.Reserve(ChunkSize + 1);

		for (int y = 0; y < ChunkSize; y++) {
			const float Percent = ((float)y / (float)(ChunkSize - 1) - 0.5f) * 2.0f;//Percent between -1 and 1
			const FVector VerticeCube = ChunkOrientation + -1.0f * ChunkAxisA + Percent * ChunkAxisB;
			const FVector VerticeUnitSphere = (VerticeCube - PlanetCenter).GetSafeNormal(0.0f);
			const float _Noise = CalculateNoise(VerticeUnitSphere.X, VerticeUnitSphere.Y, VerticeUnitSphere.Z);
			const float NoiseHeight = _Noise * (Curve != nullptr ? Height * Curve->GetFloatValue(_Noise) : Height);
			const FVector Vertice = VerticeUnitSphere * (radius + NoiseHeight) + Offset * ChunkOrientation;
			VerticesBottomLineCache.Add(Vertices.Add(Vertice));
		}

		for (int x = 0; x < ChunkSize - 1; x++) {
			const float PercentX =  ((float)(x + 1) / (float)(ChunkSize - 1) - 0.5f) * 2.0f;//Percent between -1 and 1
			const FVector FirstVerticeCube = ChunkOrientation + PercentX * ChunkAxisA + -1.0f * ChunkAxisB;
			const FVector FirstVerticeUnitSphere = (FirstVerticeCube - PlanetCenter).GetSafeNormal(0.0f) ;
			const float _Noise = CalculateNoise(FirstVerticeUnitSphere.X, FirstVerticeUnitSphere.Y, FirstVerticeUnitSphere.Z);
			const float NoiseHeight = _Noise * (Curve != nullptr ? Height * Curve->GetFloatValue(_Noise) : Height);
			const FVector FirstVertice = FirstVerticeUnitSphere * (radius + NoiseHeight) + Offset * ChunkOrientation;
			VerticesTopLineCache.Add(Vertices.Add(FirstVertice));
			
			for (int y = 0; y < ChunkSize - 1; y++) {
				const float PercentXPrim = ((float)(x + 1)/ (float)(ChunkSize - 1) - 0.5f) * 2.0f;//Percent between -1 and 1
				const float PercentY = ((float)(y + 1 )/ (float)(ChunkSize - 1) - 0.5f) * 2.0f;//Percent between -1 and 1
				const FVector VerticeCube = ChunkOrientation + PercentY * ChunkAxisB + PercentXPrim * ChunkAxisA;
				const FVector VerticeUnitSphere = (VerticeCube - PlanetCenter).GetSafeNormal(0.0f);
				float _noise = CalculateNoise(VerticeUnitSphere.X, VerticeUnitSphere.Y, VerticeUnitSphere.Z);
				float noiseVal = _noise * (Curve != nullptr ? Height * Curve->GetFloatValue(_noise) : Height);
				//UE_LOG(LogTemp, Warning, TEXT("NoiseValue : %f"), noiseVal);
				const FVector Vertice =  VerticeUnitSphere * (radius + noiseVal) + Offset * ChunkOrientation;
				VerticesTopLineCache.Add(Vertices.Add(Vertice));

				Triangles.Add(VerticesBottomLineCache[y]);
				Triangles.Add(VerticesBottomLineCache[y + 1]);
				Triangles.Add(VerticesTopLineCache[y]);

				Triangles.Add(VerticesBottomLineCache[y + 1]);
				Triangles.Add(VerticesTopLineCache[y + 1]);
				Triangles.Add(VerticesTopLineCache[y]);

				const FVector AB = Vertices[VerticesTopLineCache[y]] - Vertices[VerticesBottomLineCache[y]];
				const FVector AC = Vertices[VerticesBottomLineCache[y + 1]] - Vertices[VerticesBottomLineCache[y]];
				const FVector NormalForBottom = FVector::CrossProduct(AB, AC);
				Normals[VerticesBottomLineCache[y]] = NormalForBottom.GetSafeNormal();
				
			}

			const FVector AB = Vertices[VerticesTopLineCache[ChunkSize - 2]] - Vertices[VerticesBottomLineCache[ChunkSize - 1]];
			const FVector AC = Vertices[VerticesTopLineCache[ChunkSize - 1]] - Vertices[VerticesBottomLineCache[ChunkSize - 1]];
			const FVector NormalForBottom = FVector::CrossProduct(AB, AC);
			Normals[VerticesBottomLineCache[ChunkSize - 1]] = NormalForBottom.GetSafeNormal();

			VerticesBottomLineCache = VerticesTopLineCache;
			VerticesTopLineCache.Empty();
			
		}

		const int x = ChunkSize - 2;
		for (int y = 0; y < ChunkSize - 1; y++) {
			int index = x * ChunkSize + y;
			const FVector AB = Vertices[VerticesBottomLineCache[y + 1]] - Vertices[VerticesBottomLineCache[y]];
			const FVector AC = Vertices[index] - Vertices[VerticesBottomLineCache[y]];
			const FVector NormalForBottom = FVector::CrossProduct(AB, AC);
			Normals[VerticesBottomLineCache[y]] = NormalForBottom.GetSafeNormal();
		}
		

		StaticProvider->CreateSectionFromComponents(0, 0, 0, Vertices, Triangles, Normals, UVs, Colors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
		
	}
}

float AHeightMapPlanetChunk::CalculateNoise(float x, float y, float z) {

	float NoiseValue = 0.0f;
	float Frequency = 1.0f;
	float Amplitude = 1.0f;
	
	for (int o = 0; o < 3; o++) {

		const float NewX = x / Scale * Frequency;
		const float NewY = y / Scale * Frequency;
		const float NewZ = z / Scale * Frequency;

		float Perlin = Noise.GetPerlin(NewX, NewY, NewZ);
		
		NoiseValue += Perlin * Amplitude;
		Amplitude *= Persistance;
		Frequency *= Lacunarity;
	}
	
	NoiseValue = (NoiseValue + 1) * 0.5f;
	
	return NoiseValue;
	
}
