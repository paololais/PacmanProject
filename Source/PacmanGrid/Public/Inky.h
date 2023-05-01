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
	virtual AGridBaseNode* GetPlayerRelativeTarget() override;

public:
	AInky();

	virtual void RespawnGhostStartingPosition() override;

	virtual void GoHome() override;

	
protected:
	virtual void BeginPlay() override;
};
