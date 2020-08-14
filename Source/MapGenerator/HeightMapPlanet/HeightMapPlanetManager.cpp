// Fill out your copyright notice in the Description page of Project Settings.


#include "HeightMapPlanetManager.h"
#include "HeightMapPlanet.h"
#include "Engine/World.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/GameplayStatics.h"

UHeightMapPlanetManager::UHeightMapPlanetManager() {

	MapType = EMapType::ProceduralHMPlanet;
	static ConstructorHelpers::FObjectFinder<UCurveFloat> FalloffCurve(TEXT("CurveFloat'/Game/Helpers/FallofCurvePlanet.FallofCurvePlanet'"));
	if (FalloffCurve.Succeeded()) {
		Curve = FalloffCurve.Object;
	}
}


void UHeightMapPlanetManager::Enter() {
	UE_LOG(LogTemp, Warning, TEXT("Landscape : Enter"));
	if (!Initialized) {
		Initialized = true;
		if (Offsets.Num() == 0) {
			for (int i = 0; i < 6; i++) {
				Offsets.Add(0.0f);
			}
		}
		const FVector Location = FVector();
		Planet = UMapEditorUtils::GetActualWorld()->SpawnActor<AHeightMapPlanet>(AHeightMapPlanet::StaticClass(), Location, FRotator(0.0f,0.0f,0.0f));
		Planet->Initialize();
	}
}

void UHeightMapPlanetManager::Leave() {

	UE_LOG(LogTemp, Warning, TEXT("Landscape : Leave"));
	if (Planet != nullptr) {
		Planet->Remove();
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
	if (Offsets.Num() == 0) {
		for (int i = 0; i < 6; i++) {
			Offsets.Add(0.0f);
		}
	}
	TArray<AActor*> Chunks = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(UMapEditorUtils::GetActualWorld(), AHeightMapPlanet::StaticClass(), Chunks);
	if (Chunks.Num() > 0) {
		Planet = Cast<AHeightMapPlanet>(Chunks[0]);
		if (Chunks.Num() > 1) {
			for (int i = 1 ; i < Chunks.Num(); i++) {
				AHeightMapPlanet* PlanetToDelete = Cast<AHeightMapPlanet>(Chunks[i]);
				PlanetToDelete->Remove();
				UMapEditorUtils::GetActualWorld()->DestroyActor(PlanetToDelete);
			}
		}
		//TODO Copy properties
		
	}

}