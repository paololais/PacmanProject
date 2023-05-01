// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"

AGridBaseNode* AInky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

AInky::AInky()
{
	CurrentGridCoords = FVector2D(17, 17);
}

void AInky::BeginPlay()
{
	Super::BeginPlay();

	this->SetChaseState();
}

void AInky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	const FVector Location(1750, 1750, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 17))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 17))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);

	this->SetChaseState();
}