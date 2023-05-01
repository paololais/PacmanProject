// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

AGridBaseNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

ABlinky::ABlinky()
{
	CurrentGridCoords = FVector2D(17, 10);
}

void ABlinky::BeginPlay()
{
	Super::BeginPlay();

	this->SetChaseState();
}

void ABlinky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	const FVector Location(1750, 1050, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 10))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 10))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);

	this->SetChaseState();
}