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

	const float radius = 10000.0f;
	
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
			const FVector Vertice = (VerticeCube - PlanetCenter).GetSafeNormal(0.0f) * radius;
			VerticesBottomLineCache.Add(Vertices.Add(Vertice));
		}

		for (int x = 0; x < ChunkSize - 1; x++) {
			const float PercentX = ((float)(x + 1) / (float)(ChunkSize - 1) - 0.5f) * 2.0f;//Percent between -1 and 1
			const FVector FirstVerticeCube = ChunkOrientation + PercentX * ChunkAxisA + -1.0f * ChunkAxisB;
			const FVector FirstVertice = (FirstVerticeCube - PlanetCenter).GetSafeNormal(0.0f) * radius /** radius*/;
			VerticesTopLineCache.Add(Vertices.Add(FirstVertice));
			
			for (int y = 0; y < ChunkSize - 1; y++) {
				const float PercentXPrim = ((float)(x + 1)/ (float)(ChunkSize - 1) - 0.5f) * 2.0f;//Percent between -1 and 1
				const float PercentY = ((float)(y + 1 )/ (float)(ChunkSize - 1) - 0.5f) * 2.0f;//Percent between -1 and 1
				const FVector VerticeCube = ChunkOrientation + PercentY * ChunkAxisB + PercentXPrim * ChunkAxisA;
				const FVector Vertice = (VerticeCube - PlanetCenter).GetSafeNormal(0.0f) * radius/*radius*/;
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
