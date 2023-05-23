// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Pinky.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API APinky : public APhantomPawn
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	const int GhostIndex = 1;

public:
	APinky();

	void RespawnGhostStartingPosition() override;
	
	virtual AGridBaseNode* GetPlayerRelativeTarget() override;
	
	void GoHome() override;

	void GoToHisCorner() override;

	virtual int MyIndex() const override { return GhostIndex; }

	int PinkyCounter;
	int PinkyLimit;
	virtual int PointGhostCounter() const override { return PinkyCounter; }
	virtual int PointGhostLimit() const override { return PinkyLimit; }
	virtual void IncrementPointCounter() override { PinkyCounter++; }
};
