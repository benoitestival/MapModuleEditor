// Fill out your copyright notice in the Description page of Project Settings.


#include "MapActorDetailCustomization.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Modules/ModuleManager.h"


TSharedRef<IDetailCustomization> MapActorDetailCustomization::MakeInstance() {
	return MakeShareable(new MapActorDetailCustomization);
}

void MapActorDetailCustomization::RegisterDetailsCustomization() {
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("MapActor", FOnGetDetailCustomizationInstance::CreateStatic(&MapActorDetailCustomization::MakeInstance));

}

void MapActorDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {
	//TODO

	
}
