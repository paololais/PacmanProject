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

	const FVector Location(1750, 1250, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 12))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 12))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);

	this->SetChaseState();
}

void AClyde::GoHome() {
	this->SetDeadState();

	const AGridBaseNode* Target = *(GridGenTMap.Find(FVector2D(17, 12)));

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(17, 12))
	{
		this->SetChaseState();
	}
}

void AClyde::GoToHisCorner()
{
}
