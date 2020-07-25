#pragma once


#include "Engine.h"
#include "Modules/ModuleInterface.h"


DECLARE_LOG_CATEGORY_EXTERN(MapEditorModule, All, All)

class FMapEditorModule : public IModuleInterface {

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};