// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Blinky.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API ABlinky : public APhantomPawn
{
	GENERATED_BODY()
	virtual AGridBaseNode* GetPlayerRelativeTarget() override;

private:
	const int GhostIndex = 0;

protected:
	virtual void BeginPlay() override;

public:
	ABlinky();

	virtual void RespawnGhostStartingPosition() override;

	void GoHome() override;

	void GoToHisCorner() override;
	
	virtual int MyIndex() const override{ return GhostIndex; }
};
