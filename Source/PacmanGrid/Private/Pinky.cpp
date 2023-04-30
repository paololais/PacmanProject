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

	this->LastNode = (*(GridGenTMap.Find(FVector2D(17, 14))));
	this->SetNextNode(*(GridGenTMap.Find(FVector2D(17, 14))));
	this->SetTargetNode(this->GetPlayerRelativeTarget());

	this->CurrentGridCoords = FVector2D(17, 14);
	this->SetActorLocation(FVector(1750, 1450, this->GetActorLocation().Z));

	this->SetChaseState();
}