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

	virtual AGridBaseNode* GetPlayerRelativeTarget() override;

public:
	AClyde();

	void RespawnGhostStartingPosition() override;

	void GoHome() override;

	void GoToHisCorner() override;

protected:
	virtual void BeginPlay() override;

private:
	const int MyIndex = 3;
};
