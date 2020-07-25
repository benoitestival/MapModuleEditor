#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class MAPEDITORMODULE_API FMapEditorMode : public FEdMode {

public:

	virtual void Enter() override;

	virtual void Exit() override;

	bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;

	static FEditorModeID EM_MapEditorID;
};