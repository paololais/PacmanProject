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

	const FVector Location(1750, 1050, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 10))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 10))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);

	this->SetChaseState();
}

void ABlinky::GoHome() {
	this->SetDeadState();

	//target node (17,10)

	const AGridBaseNode* Target = *(GridGenTMap.Find(FVector2D(17, 10)));

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords==FVector2D(17,10))
	{
		this->SetChaseState();
	}
}