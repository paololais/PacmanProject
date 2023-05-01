// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinky.h"

AGridBaseNode* APinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

APinky::APinky()
{
	CurrentGridCoords = FVector2D(17, 14);
}

void APinky::BeginPlay()
{
	Super::BeginPlay();

	this->SetChaseState();
}

void APinky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	const FVector Location(1750, 1450, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 14))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 14))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);
	
	this->SetChaseState();
}