// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"

AGridBaseNode* AClyde::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

AClyde::AClyde()
{
	CurrentGridCoords = FVector2D(17, 12);
}

void AClyde::BeginPlay()
{
	Super::BeginPlay();

	this->SetChaseState();
}

void AClyde::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	this->LastNode = (*(GridGenTMap.Find(FVector2D(17, 12))));
	this->SetNextNode(*(GridGenTMap.Find(FVector2D(17, 12))));
	this->SetTargetNode(this->GetPlayerRelativeTarget());

	this->CurrentGridCoords = FVector2D(17, 12);
	this->SetActorLocation(FVector(1750, 1250, this->GetActorLocation().Z));

	this->SetChaseState();
}