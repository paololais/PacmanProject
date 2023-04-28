// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"

AGridBaseNode* AInky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

AInky::AInky()
{
	CurrentGridCoords = FVector2D(5, 26);
}

void AInky::BeginPlay()
{
	Super::BeginPlay();

	this->SetChaseState();
}

void AInky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	this->SetChaseState();

	this->LastNode = (*(GridGenTMap.Find(FVector2D(5, 26))));
	this->SetNextNode(*(GridGenTMap.Find(FVector2D(5, 26))));


	this->SetActorLocation(FVector(550, 2650, this->GetActorLocation().Z));
}