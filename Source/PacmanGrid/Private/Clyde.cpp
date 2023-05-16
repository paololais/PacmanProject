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

	//alterna scatter chase
	AlternateScatterChase(MyIndex);

	//this->SetChaseState();
}

void AClyde::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	const FVector Location(1750, 1250, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 12))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 12))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);

	this->AlternateScatterChase(MyIndex);
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
		this->AlternateScatterChase(MyIndex);
	}
}

void AClyde::GoToHisCorner()
{
	//nodo basso sx
	const AGridBaseNode* Target1 = *(GridGenTMap.Find(FVector2D(5, 6)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target1->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(5, 6))
	{
		//nodo alto sx
		const AGridBaseNode* Target2 = *(GridGenTMap.Find(FVector2D(5, 1)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target2->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}

		if (CurrentGridCoords == FVector2D(5, 1))
		{
			//nodo alto dx
			const AGridBaseNode* Target3 = *(GridGenTMap.Find(FVector2D(1, 1)));

			AGridBaseNode* PossibleNode3 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

			if (PossibleNode3)
			{
				this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode3->GetGridPosition() - this->GetLastNodeCoords()), true);
			}

			if (CurrentGridCoords == FVector2D(1, 1))
			{
				//nodo basso dx
				const AGridBaseNode* Target4 = *(GridGenTMap.Find(FVector2D(1, 6)));

				AGridBaseNode* PossibleNode4 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

				if (PossibleNode4)
				{
					this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode4->GetGridPosition() - this->GetLastNodeCoords()), true);
				}

				if (CurrentGridCoords == FVector2D(1, 6))
				{
					this->GoToHisCorner();
				}
			}
		}
	}
}
