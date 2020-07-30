// Fill out your copyright notice in the Description page of Project Settings.


#include "MapActor.h"
#include "MapGenerator/Objects/BaseMapManager.h"
#include "Utils/MapEditorUtils.h"
#include "HeightMapLandscape/LandscapeManager.h"
#include "HeightMapLandscape/Components/ErosionComponent.h"
#include "Objects/BaseLandscapeComponent.h"

// Sets default values
AMapActor::AMapActor(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void AMapActor::Init() {
	if (MapManager == nullptr) {
		SwitchMapMode(UMapEditorUtils::ActualMapType);
	}
	else {
		UMapEditorUtils::ActualMapType = MapManager->MapType;
	}
}

void AMapActor::SwitchMapMode(TEnumAsByte<EMapType> MapType) {

	UE_LOG(LogTemp, Warning, TEXT("Switch Mode"));
	UClass* ClassResult = *UMapEditorUtils::ManagerRegistry.Find(MapType);
	if (ClassResult->IsValidLowLevel()) {
		UBaseMapManager* Manager = Cast<UBaseMapManager>(ClassResult->GetDefaultObject());/*//SAME//*/
		if (Manager != nullptr) {
			UMapEditorUtils::ActualMapType = MapType;
			SwitchMapMode(Manager);
		}
	}
}

void AMapActor::SwitchMapMode(UBaseMapManager * NewManager) {
	if (NewManager != nullptr) {
		if (MapManager != nullptr) {
			MapManager->Leave();
		}
		MapManager = NewManager;
		MapManager->Enter();
	}
}

void AMapActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if(UMapEditorUtils::IsRunning) {
		if (MapManager != nullptr) {
			MapManager->Construct();
		}
	}
	else {
		if (MapManager != nullptr) {
			if (MapManager->Initialized) {
				MapManager->Construct();
			}
			else {
				MapManager->Load();
			}
		}
	}
}


void AMapActor::Erode() {

	if (MapManager->IsA(ULandscapeManager::StaticClass())) {
		ULandscapeManager* Manager = Cast<ULandscapeManager>(MapManager);
		UErosionComponent* Comp = nullptr;
		for (auto& Component : Manager->MapComponents) {
			if (Component->IsA(UErosionComponent::StaticClass())) {
				Comp = Cast<UErosionComponent>(Component);
			}
		}
		if (Comp != nullptr) {
			Comp->Manager = Manager;
			Comp->ErodeTerrain();
		}
	}

	
}
