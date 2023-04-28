// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

AGridBaseNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

ABlinky::ABlinky()
{
	CurrentGridCoords = FVector2D(27, 26);
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

	this->LastNode = (*(GridGenTMap.Find(FVector2D(27, 26))));
	this->SetNextNode(*(GridGenTMap.Find(FVector2D(27, 26))));


	this->SetActorLocation(FVector(2750, 2650, this->GetActorLocation().Z));
}