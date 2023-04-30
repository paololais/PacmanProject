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

	this->LastNode = (*(GridGenTMap.Find(FVector2D(17, 10))));
	this->SetNextNode(*(GridGenTMap.Find(FVector2D(17, 10))));
	this->SetTargetNode(this->GetPlayerRelativeTarget());

	this->CurrentGridCoords = FVector2D(17, 10);
	this->SetActorLocation(FVector(1750, 1050, this->GetActorLocation().Z));
	
	this->SetChaseState();
}