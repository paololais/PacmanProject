// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"
#include "PacmanPawn.h"
#include "cmath"
#include "Kismet/GameplayStatics.h"

AClyde::AClyde()
{
	CurrentGridCoords = FVector2D(14, 16);
	DefaultSkin = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Materials/M_PlainOrange.M_PlainOrange'"));
}

void AClyde::BeginPlay()
{
	Super::BeginPlay();
	ClydeLimit = 60;
	this->AlternateScatterChase();
	this->bIsLeaving = false;
	this->bIsHome = true;
	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
}

AGridBaseNode* AClyde::GetPlayerRelativeTarget()
{
	if (IsValid(Player)) {
		FVector2D PlayerCoords = Player->GetCurrentGridCoords();
		EuclideDistance = sqrt(pow(this->CurrentGridCoords.X - PlayerCoords.X, 2) + pow(this->CurrentGridCoords.Y - PlayerCoords.Y, 2));

		if (EuclideDistance > 7)
		{
			TargetCoords = PlayerCoords;
			return Super::GetPlayerRelativeTarget();
		}
		else {
			AGridBaseNode* Target = nullptr;
			//va nel suo angolo
			if (!(CurrentGridCoords.X<5 && CurrentGridCoords.Y <6))
			{
				TargetCoords = (FVector2D(5, 6));
			}
			else if (CurrentGridCoords == FVector2D(5, 6))
			{
				TargetCoords = (FVector2D(5, 1));
			}
			else if (CurrentGridCoords == FVector2D(5, 1))
			{
				TargetCoords = (FVector2D(1, 1));
			}
			else
			{
				TargetCoords = (FVector2D(5, 6));
			}

			Target = *(TheGridGen->TileMap.Find(TargetCoords));

			if (Target!= nullptr)
			{
				return Target;
			}
			else
			{
				return Super::GetPlayerRelativeTarget();
			}
		}
	}
	return Super::GetPlayerRelativeTarget();
}

void AClyde::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();
	this->ClearTimer();
	this->AlternateScatterChase();
	this->bIsLeaving = false;
	this->bIsHome = true;
}

void AClyde::GoHome() {
	Super::GoHome();
}

void AClyde::GoToHisCorner()
{
	const AGridBaseNode* Target1 = *(TheGridGen->TileMap.Find(FVector2D(5, 6)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target1->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(5, 6))
	{
		const AGridBaseNode* Target2 = *(TheGridGen->TileMap.Find(FVector2D(5, 1)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target2->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}

		if (CurrentGridCoords == FVector2D(5, 1))
		{
			const AGridBaseNode* Target3 = *(TheGridGen->TileMap.Find(FVector2D(1, 1)));

			AGridBaseNode* PossibleNode3 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

			if (PossibleNode3)
			{
				this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode3->GetGridPosition() - this->GetLastNodeCoords()), true);
			}

			if (CurrentGridCoords == FVector2D(1, 1))
			{
				const AGridBaseNode* Target4 = *(TheGridGen->TileMap.Find(FVector2D(1, 6)));

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
