// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

ABlinky::ABlinky()
{
	CurrentGridCoords = FVector2D(17, 10);
	DefaultSkin = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Materials/M_PlainRed.M_PlainRed'"));
}

void ABlinky::BeginPlay()
{
	Super::BeginPlay();

	//alterna scatter chase
	this->bIsLeaving = false;
	this->AlternateScatterChase();
}

AGridBaseNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

void ABlinky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();
	this->AlternateScatterChase();
}

void ABlinky::GoHome() {
	Super::GoHome();
}

//da modificare in modo che scelga il nodo dell'angolo pi� vicino in base alla propria posizione
void ABlinky::GoToHisCorner()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Scatter state on")));


	//nodo basso sx
	const AGridBaseNode* Target1 = *(TheGridGen->TileMap.Find(FVector2D(23, 18)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target1->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(23, 18))
	{
		//nodo alto sx
		const AGridBaseNode* Target2 = *(TheGridGen->TileMap.Find(FVector2D(28, 15)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target2->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}

		if (CurrentGridCoords == FVector2D(28, 15))
		{
			//nodo alto dx
			const AGridBaseNode* Target3 = *(TheGridGen->TileMap.Find(FVector2D(28, 26)));

			AGridBaseNode* PossibleNode3 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

			if (PossibleNode3)
			{
				this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode3->GetGridPosition() - this->GetLastNodeCoords()), true);
			}
			
			if (CurrentGridCoords == FVector2D(28, 15))
			{
				//nodo basso dx
				const AGridBaseNode* Target4 = *(TheGridGen->TileMap.Find(FVector2D(25, 21)));

				AGridBaseNode* PossibleNode4 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

				if (PossibleNode4)
				{
					this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode4->GetGridPosition() - this->GetLastNodeCoords()), true);
				}

				if (CurrentGridCoords == FVector2D(25, 21))
				{
					this->GoToHisCorner();
				}
			}
		}
	}
}
