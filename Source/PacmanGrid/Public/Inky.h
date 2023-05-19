// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Inky.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API AInky : public APhantomPawn
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:
	const int GhostIndex = 2;

public:
	AInky();

	void RespawnGhostStartingPosition() override;

	virtual AGridBaseNode* GetPlayerRelativeTarget() override;
	
	void GoHome() override;

	void GoToHisCorner() override;

	virtual int MyIndex() const override { return GhostIndex; }
};
