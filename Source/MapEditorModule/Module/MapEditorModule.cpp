#include "MapEditorModule.h"
#include "EditorModeRegistry.h"
#include "Textures/SlateIcon.h"
#include "../MapEditorMode.h"
#include "MapEditorModule/DetailsCustomisation/MapActorDetailCustomization.h"

IMPLEMENT_GAME_MODULE(FMapEditorModule, MapEditorModule)

DEFINE_LOG_CATEGORY(MapEditorModule)

#define LOCTEXT_NAMESPACE "MapEditorModule"

void FMapEditorModule::StartupModule() {

	FEditorModeRegistry::Get().RegisterMode<FMapEditorMode>(FMapEditorMode::EM_MapEditorID, FText::FromString("Map Editor"), FSlateIcon(), true);
	MapActorDetailCustomization::RegisterDetailsCustomization();
}

void FMapEditorModule::ShutdownModule() {
	
}

#undef LOCTEXT_NAMESPACE