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

	this->LastNode = (*(GridGenTMap.Find(FVector2D(17, 17))));
	this->SetNextNode(*(GridGenTMap.Find(FVector2D(17, 17))));
	this->CurrentGridCoords = FVector2D(17, 17);

	this->SetActorLocation(FVector(1750, 1750, this->GetActorLocation().Z));
	this->SetChaseState();
}