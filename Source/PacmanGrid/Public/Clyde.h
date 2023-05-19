// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Clyde.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API AClyde : public APhantomPawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	const int GhostIndex = 3;

public:
	AClyde();

	virtual AGridBaseNode* GetPlayerRelativeTarget() override;

	void RespawnGhostStartingPosition() override;

	void GoHome() override;

	void GoToHisCorner() override;
	
	virtual int MyIndex() const override { return GhostIndex; }
};
