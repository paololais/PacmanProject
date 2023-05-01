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

public:
	ABlinky();

	virtual void RespawnGhostStartingPosition() override;

	virtual void GoHome() override;

	//my function that uses dijkstra algorithm to find shortest path to reach pacman
	//int FindShortestPath(AGridBaseNode* CurrentNode, AGridBaseNode* TargetPosition);

protected:
	virtual void BeginPlay() override;
};
