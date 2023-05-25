// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinky.h"
#include "PacmanPawn.h"
#include "Kismet/GameplayStatics.h"

APinky::APinky()
{
	CurrentGridCoords = FVector2D(14, 13);
	DefaultSkin = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Materials/M_Pink.M_Pink'"));
}

void APinky::BeginPlay()
{
	Super::BeginPlay();
	PinkyLimit = 0;
	this->AlternateScatterChase();
	this->bIsLeaving = false;
	this->bIsHome = true;
	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
}

AGridBaseNode* APinky::GetPlayerRelativeTarget()
{
	if (IsValid(Player)) {

		FVector PlayerDirection = Player->GetLastValidDirection();

		FVector2D TargetCoords = Player->GetCurrentGridCoords();

		AGridBaseNode* Target = nullptr;

		//direzione verso l'alto
		if (PlayerDirection == FVector(1, 0, 0))
		{
			//gestione confini labirinto
			if (TargetCoords.X >= 24)
			{
				TargetCoords.X = 28;
				if (TargetCoords.Y <= 5)
				{
					TargetCoords.Y = 1;
				}
				else
				{
					TargetCoords.Y -= 4;
				}
			}
			else if (TargetCoords.Y <= 5)
			{
				TargetCoords.Y = 1;
				if (TargetCoords.X >= 24)
				{
					TargetCoords.X = 28;
				}
				else
				{
					TargetCoords.X += 4;
				}
			}
			else
			{
				TargetCoords += FVector2D(4, -4);
			}

			Target = (*(TheGridGen->TileMap.Find(TargetCoords)));

		}
		//verso il basso
		else if (PlayerDirection == FVector(-1, 0, 0))
		{
			if (TargetCoords.X <= 5)
			{
				TargetCoords.X = 1;
			}
			else
			{
				TargetCoords += FVector2D(-4, 0);
			}
			Target = (*(TheGridGen->TileMap.Find(TargetCoords)));
		}
		//verso dx
		else if (PlayerDirection == FVector(0, 1, 0))
		{
			//gestione offset se la tile è fuori dal labirinto
			if (TargetCoords.Y >= 22)
			{
				TargetCoords.Y = 26;
			}
			else
			{
				TargetCoords += FVector2D(0, 4);
			}

			Target = (*(TheGridGen->TileMap.Find(TargetCoords)));

		}
		//verso sx
		else if (PlayerDirection == FVector(0, -1, 0))
		{

			if (TargetCoords.Y <= 5)
			{
				TargetCoords.Y = 1;
			}
			else
			{
				TargetCoords += FVector2D(0, -4);
			}
			Target = (*(TheGridGen->TileMap.Find(TargetCoords)));
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target Coordinates: (%f,%f)"), TargetCoords.X, TargetCoords.Y));
		if (Target != nullptr)
		{
			return Target;
		}
		else
		{
			return Super::GetPlayerRelativeTarget();
		}
	}
	return Super::GetPlayerRelativeTarget();
}

void APinky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();
	this->ClearTimer();
	this->AlternateScatterChase();
	this->bIsLeaving = false;
	this->bIsHome = true;
}

void APinky::GoHome() {
	Super::GoHome();
}

void APinky::GoToHisCorner()
{
	//nodo basso sx
	const AGridBaseNode* Target1 = *(TheGridGen->TileMap.Find(FVector2D(23, 9)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target1->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(23, 9))
	{
		//nodo alto sx
		const AGridBaseNode* Target2 = *(TheGridGen->TileMap.Find(FVector2D(28, 12)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target2->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}

		if (CurrentGridCoords == FVector2D(28, 12))
		{
			//nodo alto dx
			const AGridBaseNode* Target3 = *(TheGridGen->TileMap.Find(FVector2D(28, 1)));

			AGridBaseNode* PossibleNode3 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

			if (PossibleNode3)
			{
				this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode3->GetGridPosition() - this->GetLastNodeCoords()), true);
			}

			if (CurrentGridCoords == FVector2D(28, 1))
			{
				//nodo basso dx
				const AGridBaseNode* Target4 = *(TheGridGen->TileMap.Find(FVector2D(25, 6)));

				AGridBaseNode* PossibleNode4 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

				if (PossibleNode4)
				{
					this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode4->GetGridPosition() - this->GetLastNodeCoords()), true);
				}

				if (CurrentGridCoords == FVector2D(25, 6))
				{
					this->GoToHisCorner();
				}
			}
		}
	}
}
