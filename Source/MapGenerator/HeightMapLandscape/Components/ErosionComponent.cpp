// Fill out your copyright notice in the Description page of Project Settings.


#include "ErosionComponent.h"
#include "MapGenerator/HeightMapLandscape/LandscapeChunk.h"
#include "MapGenerator/HeightMapLandscape/LandscapeManager.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "DrawDebugHelpers.h"

void UErosionComponent::ErodeTerrain() {


	NumberOfChunkImpacted = 1;//Temporary because easier when only using one chunk for testing
	
	MapSizeX = Manager->ChunkSizeX + 1 /** NumberOfChunkImpacted - 1;*/;
	MapSizeY = Manager->ChunkSizeY + 1 /** NumberOfChunkImpacted - 1;*/;

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
		
		Droplet.X = FMath::RandRange(0.0f, (float)MapSizeX -2.0f);
		Droplet.Y = FMath::RandRange(0.0f, (float)MapSizeY -2.0f);;

		Droplet.Direction.X = 0;
		Droplet.Direction.Y = 0;

		Droplet.Speed = InitialSpeed;
		Droplet.Water = InitialWater;
		Droplet.Sediment = 0.0f;

		for (int DropletLifetime = 0; DropletLifetime < MaxDropletLifeTime; DropletLifetime++) {

			
			
			const int NearestGridPointX = FMath::TruncToInt(Droplet.X);
			const int NearestGridPointY = FMath::TruncToInt(Droplet.Y);

			const int DropletCellIndex = NearestGridPointX * MapSizeY + NearestGridPointY;
			
			TArray<float> CellCornerDensities = TArray<float>();
			GetCellDensities(DropletCellIndex, CellCornerDensities);
			
			Droplet.OffsetToGridX = Droplet.X - NearestGridPointX;
			Droplet.OffsetToGridY = Droplet.Y - NearestGridPointY;
			
			FVector2D DropletGradient = UErosionComponent::CalculateDropletGradient(CellCornerDensities, Droplet);
			const float DropletHeight = UErosionComponent::CalculateDropletHeight(CellCornerDensities, Droplet);

			if (DrawDroplets) {
				FVector DropletPos = FVector(Droplet.X * Manager->CellResolution, Droplet.Y * Manager->CellResolution, Chunks[0]->ApplyHeightMultiplicator(DropletHeight));
				DrawDebugPoint(UMapEditorUtils::GetActualWorld(), DropletPos, 10.0f, FColor::Red, false, 60);
			}
			
			Droplet.Direction = Droplet.Direction * Inertia - DropletGradient * (1 - Inertia);
			Droplet.Direction = Droplet.Direction.GetSafeNormal();

			Droplet.X += Droplet.Direction.X;
			Droplet.Y += Droplet.Direction.Y;
			
			if (IsDropletValid(Droplet)) {

				const int DropletActualIndex = FMath::TruncToInt(Droplet.X) * MapSizeY + FMath::TruncToInt(Droplet.Y);
				if (DropletActualIndex != DropletCellIndex) {
					GetCellDensities(DropletActualIndex, CellCornerDensities);
				}
				
				const float DropletNewHeight = UErosionComponent::CalculateDropletHeight(CellCornerDensities, Droplet);
				const float DeltaHeight = DropletNewHeight - DropletHeight;

				
				
				//TODO Erosion part
				//////////////////
				const float SedimentCapacity = FMath::Max(-DeltaHeight, MinSedimentCapacity) * Droplet.Speed * Droplet.Water * SedimentCapacityFactor;
				
				if (Droplet.Sediment > SedimentCapacity || DeltaHeight > 0) {
					//Add Sediment
					
					/*float SedimentDeposit = (Droplet.Sediment - SedimentCapacity) * DepositSpeed;
					
					if (Droplet.Sediment < SedimentCapacity) {
						SedimentDeposit = FMath::Min(DeltaHeight, Droplet.Sediment);
					}*/

					float SedimentDeposit = FMath::Min(DeltaHeight, Droplet.Sediment);
					if (Droplet.Sediment > SedimentCapacity) {
						SedimentDeposit = (Droplet.Sediment - SedimentCapacity) * DepositSpeed;
					}
					
					Chunks[0]->HeightMap[DropletCellIndex] += SedimentDeposit * (1 - Droplet.OffsetToGridX) * (1 - Droplet.OffsetToGridY);
					Chunks[0]->HeightMap[DropletCellIndex + 1] += SedimentDeposit * Droplet.OffsetToGridX * (1 - Droplet.OffsetToGridY);
					Chunks[0]->HeightMap[DropletCellIndex + MapSizeY] += SedimentDeposit * (1 - Droplet.OffsetToGridX) * Droplet.OffsetToGridY;
					Chunks[0]->HeightMap[DropletCellIndex + MapSizeY + 1] += SedimentDeposit * Droplet.OffsetToGridX * Droplet.OffsetToGridY;

					Droplet.Sediment -= SedimentDeposit;
					
				}
				else {
					//Erode Terrain
					TArray<int> Indexes;
					TArray<float> Weights;
					float RadiusWeightSum = 0.0f;
					
					for (int x = NearestGridPointX - ErosionRadius; x < NearestGridPointX + ErosionRadius; x++) {
						for (int y = NearestGridPointY - ErosionRadius; y < NearestGridPointY + ErosionRadius; y++) {

							const float SquaredDist = (x - NearestGridPointX) * (x - NearestGridPointX) + (y - NearestGridPointY) * (y - NearestGridPointY);
							//const float Dist = FVector2D(x - Droplet.X, y - Droplet.Y).Size();

							if (SquaredDist < ErosionRadius * ErosionRadius) {
							//if (Dist < ErosionRadius) {

								if (x >= 0 && x < MapSizeX && y >= 0 && y < MapSizeY) {
									const int Index = x * MapSizeY + y;
									const float PointWeight = 1 - FMath::Sqrt(SquaredDist) / ErosionRadius;
									//const float PointWeight = FMath::Max(0.0f, ErosionRadius - Dist);
									Indexes.Add(Index);
									Weights.Add(PointWeight);
									RadiusWeightSum += PointWeight;
								}
							}
						}
					}

					const float AmoutToErode = FMath::Min((SedimentCapacity - Droplet.Sediment) * ErodeSpeed, - DeltaHeight);
					
					for (int i = 0; i < Indexes.Num(); i++) {

						float Weight = AmoutToErode * (Weights[i] / RadiusWeightSum);
						if (Weight > Chunks[0]->HeightMap[Indexes[i]]) {
							Weight = Chunks[0]->HeightMap[Indexes[i]];
						}
						Chunks[0]->HeightMap[Indexes[i]] -= Weight;
						Droplet.Sediment += Weight;
						
					}

				}
				
				Droplet.Speed = FMath::Sqrt(Droplet.Speed * Droplet.Speed + DeltaHeight * Gravity);
				Droplet.Water *= (1 - EvaporateSpeed);
				///////////////////
			}
			else {
				break;
			}
		}
	}

	for (auto Chunk :  Chunks) {
		Chunk->GenerateMesh(false);
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


bool UErosionComponent::IsDropletValid(FDroplet& Droplet) const{
	return Droplet.X >= 0 && Droplet.X < MapSizeX - 1 && Droplet.Y >= 0 && Droplet.Y < MapSizeY - 1 && (Droplet.Direction.X != 0.0f && Droplet.Direction.Y != 0.0f);
}