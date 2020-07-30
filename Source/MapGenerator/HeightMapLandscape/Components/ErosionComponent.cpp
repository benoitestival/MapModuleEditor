// Fill out your copyright notice in the Description page of Project Settings.


#include "ErosionComponent.h"
#include "MapGenerator/HeightMapLandscape/LandscapeChunk.h"
#include "MapGenerator/HeightMapLandscape/LandscapeManager.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "DrawDebugHelpers.h"

void UErosionComponent::ErodeTerrain() {


	NumberOfChunkImpacted = 1;//Temporary because easier when only using one chunk for testing
	
	MapSizeX = (Manager->ChunkSizeX + 1) * NumberOfChunkImpacted - 1;
	MapSizeY = (Manager->ChunkSizeY + 1) * NumberOfChunkImpacted - 1;

	Chunks.Empty();
	Chunks.Reserve(NumberOfChunkImpacted * NumberOfChunkImpacted);
	
	for (int x = 0; x < NumberOfChunkImpacted; x++) {
		for (int y = 0; y < NumberOfChunkImpacted; y++) {

			const int Index = x * NumberOfChunkImpacted + y;
			Chunks.Add(Manager->Chunks[Index]);
		}
	}

	
	for (int iteration = 0; iteration < NbrIterations; iteration++) {

		FDroplet Droplet = FDroplet();
		
		Droplet.X = FMath::RandRange(0.0f, (float)MapSizeX -1.0f);
		Droplet.Y = FMath::RandRange(0.0f, (float)MapSizeY -1.0f);;

		Droplet.Direction.X = 0;
		Droplet.Direction.Y = 0;

		float Speed = InitialSpeed;
		float Water = InitialWater;

		for (int DropletLifetime = 0; DropletLifetime < MaxDropletLifeTime; DropletLifetime++) {

			
			
			int NearestGridPointX = FMath::TruncToInt(Droplet.X);
			int NearestGridPointY = FMath::TruncToInt(Droplet.Y);

			int DropletCellIndex = NearestGridPointX * (MapSizeY + 1) + NearestGridPointY;
			
			TArray<float> CellCornerDensities = TArray<float>();
			GetCellDensities(DropletCellIndex, CellCornerDensities);
			
			Droplet.OffsetToGridX = Droplet.X - NearestGridPointX;
			Droplet.OffsetToGridY = Droplet.Y - NearestGridPointY;
			
			FVector2D DropletGradient = UErosionComponent::CalculateDropletGradient(CellCornerDensities, Droplet);
			const float DropletHeight = UErosionComponent::CalculateDropletHeight(CellCornerDensities, Droplet);

			if (DrawDroplets) {
				FVector DropletPos = FVector(Droplet.X * Manager->CellResolution, Droplet.Y * Manager->CellResolution, DropletHeight);
				DrawDebugPoint(UMapEditorUtils::GetActualWorld(), DropletPos, 10.0f, FColor::Red, false, 60);
			}
			
			Droplet.Direction = Droplet.Direction * Inertia - DropletGradient * (1 - Inertia);
			Droplet.Direction = Droplet.Direction.GetSafeNormal();

			Droplet.X += Droplet.Direction.X;
			Droplet.Y += Droplet.Direction.Y;
			
			if (!IsDropletInValid(Droplet)) {

				const int NewNearestGridPointX = FMath::TruncToInt(Droplet.X);
				const int NewNearestGridPointY = FMath::TruncToInt(Droplet.Y);
				
				//If the droplet is not in the same cell
				if (NewNearestGridPointX != NearestGridPointX || NewNearestGridPointY != NearestGridPointY) {

					NearestGridPointX = NewNearestGridPointX;
					NearestGridPointY = NewNearestGridPointY;
					
					DropletCellIndex = NearestGridPointX * (MapSizeY + 1) + NearestGridPointY;
					
					GetCellDensities(DropletCellIndex, CellCornerDensities);
					
					Droplet.OffsetToGridX = Droplet.X - NearestGridPointX;
					Droplet.OffsetToGridY = Droplet.Y - NearestGridPointY;
					
				}
				
				const float DropletNewHeight = UErosionComponent::CalculateDropletHeight(CellCornerDensities, Droplet);
				const float DeltaHeight = DropletNewHeight - DropletHeight;

				
				
				//TODO Erosion part

				//Speed = 
				
			}
			else {
				break;
			}
		}
	}
}


TArray<float>& UErosionComponent::GetCellDensities(int CellIndex, TArray<float>& OutArray) {
	
	const float SW = CellIndex;
	const float SE = CellIndex + 1;
	const float NW = CellIndex + MapSizeY;
	const float NE = CellIndex + MapSizeY + 1;

	OutArray.Empty();
	OutArray.Reserve(4);
	
	OutArray.Add(Chunks[0]->HeightMap[SW]);
	OutArray.Add(Chunks[0]->HeightMap[SE]);
	OutArray.Add(Chunks[0]->HeightMap[NW]);
	OutArray.Add(Chunks[0]->HeightMap[NE]);
	
	return OutArray;
}

FVector2D UErosionComponent::CalculateDropletGradient(TArray<float>& CellCorners, FDroplet& Droplet) {

	FVector2D ResultGradient = FVector2D();

	//SWGradiant is given by : (NW - SW, SE - SW)
	FVector2D SWGradiant = FVector2D(CellCorners[2] - CellCorners[0], CellCorners[1] - CellCorners[0]);
	//SEGradiant is given by : (NE - SE, SE - SW)
	FVector2D SEGradiant = FVector2D(CellCorners[3] - CellCorners[1], CellCorners[1] - CellCorners[0]);
	//NWGradiant is given by : (NW - SW, NE - NW)
	FVector2D NWGradiant = FVector2D(CellCorners[2] - CellCorners[0], CellCorners[3] - CellCorners[2]);
	//NEGradiant is given by : (NE - SE, NE - NW)
	FVector2D NEGradiant = FVector2D(CellCorners[3] - CellCorners[1], CellCorners[3] - CellCorners[2]);

	ResultGradient.X = UErosionComponent::BilinearInterpolation(SWGradiant.X, NWGradiant.X, SEGradiant.X, NEGradiant.X, Droplet.OffsetToGridX, Droplet.OffsetToGridY);
	ResultGradient.Y = UErosionComponent::BilinearInterpolation(SWGradiant.Y, SEGradiant.Y, NWGradiant.Y, NEGradiant.Y, Droplet.OffsetToGridY, Droplet.OffsetToGridX);
	
	return ResultGradient;
}


float UErosionComponent::CalculateDropletHeight(TArray<float>& CellCorners, FDroplet& Droplet) {
	return UErosionComponent::BilinearInterpolation(CellCorners[0], CellCorners[2], CellCorners[1], CellCorners[3], Droplet.OffsetToGridX, Droplet.OffsetToGridY);
}


bool UErosionComponent::IsDropletInValid(FDroplet& Droplet) const{
	return Droplet.X < 0 || Droplet.X >= MapSizeX || Droplet.Y < 0 || Droplet.Y >= MapSizeY || (Droplet.Direction.X == 0.0f && Droplet.Direction.Y == 0.0f);
}
