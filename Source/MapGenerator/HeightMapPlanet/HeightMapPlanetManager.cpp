// Fill out your copyright notice in the Description page of Project Settings.


#include "HeightMapPlanetManager.h"
#include "HeightMapPlanet.h"
#include "Engine/World.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/GameplayStatics.h"

void UHeightMapPlanetManager::Enter() {
	UE_LOG(LogTemp, Warning, TEXT("Landscape : Enter"));
	if (!Initialized) {
		Initialized = true;

		const FVector Location = FVector();
		Planet = UMapEditorUtils::GetActualWorld()->SpawnActor<AHeightMapPlanet>(AHeightMapPlanet::StaticClass(), Location, FRotator(0.0f,0.0f,0.0f));
		Planet->Initialize();
	}

}

void UHeightMapPlanetManager::Leave() {

	UE_LOG(LogTemp, Warning, TEXT("Landscape : Leave"));
	if (Planet != nullptr) {
		UMapEditorUtils::GetActualWorld()->DestroyActor(Planet);
	}
	Initialized = false;
}

void UHeightMapPlanetManager::Construct() {

	UE_LOG(LogTemp, Warning, TEXT("Landscape : Construct"));
	Planet->Initialize();
}

void UHeightMapPlanetManager::Load() {

	Initialized = true;
	TArray<AActor*> Chunks = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(UMapEditorUtils::GetActualWorld(), AHeightMapPlanet::StaticClass(), Chunks);
	if (Chunks.Num() > 0) {
		Planet = Cast<AHeightMapPlanet>(Chunks[0]);
		if (Chunks.Num() > 1) {
			for (int i = 1 ; i < Chunks.Num(); i++) {
				UMapEditorUtils::GetActualWorld()->DestroyActor(Chunks[i]);
			}
		}
		//TODO Copy properties

		
	}

}