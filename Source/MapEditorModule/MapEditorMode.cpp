#include "MapEditorMode.h"
#include "MapGenerator/Utils/MapEditorUtils.h"
#include "Editor/UnrealEd/Public/Toolkits/ToolkitManager.h"
#include "MapEditorModule/ToolKits/MapEditorToolKit.h"
#include "EditorModeManager.h" 

FEditorModeID FMapEditorMode::EM_MapEditorID = TEXT("EM_MapEditorID");


void FMapEditorMode::Enter() {
	FEdMode::Enter();
	UMapEditorUtils::InitMapEditor();
	if (!Toolkit.IsValid()) {
		Toolkit = MakeShareable(new MapEditorToolKit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
	UE_LOG(LogTemp, Warning, TEXT("Enter Mode"));
}

void FMapEditorMode::Exit() {
	UE_LOG(LogTemp, Warning, TEXT("Exit Mode"));
	if (Toolkit.Get()) {
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	UMapEditorUtils::DestroyMapEditor();
	FEdMode::Exit();
}

bool FMapEditorMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const {
	return false;
}
