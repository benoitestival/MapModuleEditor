// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorGameMode.h"

#include "MapGenerator/MapActor.h"

#include "Editor.h"
#include "Kismet/GameplayStatics.h"


UEditorGameMode* UEditorGameMode::EditorGameMode = nullptr;


UEditorGameMode::UEditorGameMode() {
}

UEditorGameMode::~UEditorGameMode() {
	
}

void UEditorGameMode::Init() {
	MapActor = nullptr;
	MapActor = GetMapActor();
	//SettingsObject = GetSettingsObject();
}

void UEditorGameMode::Delete() {
	MapActor->Destroy();
	MapActor = nullptr;
	//SettingsObject = nullptr;
}

bool UEditorGameMode::IsInEditor() {
	//TODO
	return true;
}


AMapActor * UEditorGameMode::GetMapActor() {
	if(MapActor == nullptr) {
		MapActor = FindMapActorInMap();
	}
	return MapActor;
}
AMapActor * UEditorGameMode::FindMapActorInMap() const {

	AMapActor* _MapActor = nullptr;
	TArray<AActor*> Out = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMapActor::StaticClass(), Out);
	if (Out.Num() > 0) {
		_MapActor = Cast<AMapActor>(Out[0]);
	}
	if(_MapActor == nullptr) {
		const FVector Location = FVector(0.0f, 0.0f, 0.0f);
		const FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
		_MapActor = GetWorld()->SpawnActor<AMapActor>(AMapActor::StaticClass(), Location, Rotation);
	}
	_MapActor->Init();
	return _MapActor;
}


UEditorGameMode * UEditorGameMode::Get() {
	if (EditorGameMode == nullptr) {
		EditorGameMode = new UEditorGameMode();
		EditorGameMode->Init();
	}
	return EditorGameMode;
}

void UEditorGameMode::Destroy() {
	EditorGameMode->Delete();
	delete EditorGameMode;
	EditorGameMode = nullptr;
}

UWorld * UEditorGameMode::GetWorld() {
	UWorld* World = nullptr;
	if (GEditor->bIsSimulatingInEditor) {
		World = GEditor->GetPIEWorldContext()->World();
	}
	else {
		World = GEditor->GetEditorWorldContext().World();
	}
	if(World == nullptr) {
		//TODO
	}
	return World;
}