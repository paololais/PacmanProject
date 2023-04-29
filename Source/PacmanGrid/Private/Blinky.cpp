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

	this->SetChaseState();

	CurrentGridCoords = FVector2D(17, 10);
	LastNode = (*(GridGenTMap.Find(FVector2D(17, 10))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 10))));
	SetTargetNode(this->GetPlayerRelativeTarget());

	SetActorLocation(FVector(1750, 1050, this->GetActorLocation().Z));
}