// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacmanPawn.h"
#include <Kismet/GameplayStatics.h>

AInky::AInky()
{
	CurrentGridCoords = FVector2D(14, 11);
	DefaultSkin = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Materials/M_Cyan.M_Cyan'"));
}

void AInky::BeginPlay()
{
	Super::BeginPlay();
	InkyLimit = 30;
	this->AlternateScatterChase();
	this->bIsLeaving = false;
	this->bIsHome = true;
}

AGridBaseNode* AInky::GetPlayerRelativeTarget()
{
	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
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
			if (TargetCoords.X <=5)
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
			
			if (TargetCoords.Y <=5)
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


void AInky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();
	this->AlternateScatterChase();
	this->bIsLeaving = false;
	this->bIsHome = true;
}

void AInky::GoHome() {
	Super::GoHome();
}

void AInky::GoToHisCorner()
{
	const AGridBaseNode* Target1 = *(TheGridGen->TileMap.Find(FVector2D(5, 21)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target1->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(5, 21))
	{
		const AGridBaseNode* Target2 = *(TheGridGen->TileMap.Find(FVector2D(5, 26)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target2->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}

		if (CurrentGridCoords == FVector2D(5, 26))
		{
			const AGridBaseNode* Target3 = *(TheGridGen->TileMap.Find(FVector2D(1, 26)));

			AGridBaseNode* PossibleNode3 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

			if (PossibleNode3)
			{
				this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode3->GetGridPosition() - this->GetLastNodeCoords()), true);
			}

			if (CurrentGridCoords == FVector2D(1, 26))
			{
				const AGridBaseNode* Target4 = *(TheGridGen->TileMap.Find(FVector2D(1, 21)));

				AGridBaseNode* PossibleNode4 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

				if (PossibleNode4)
				{
					this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode4->GetGridPosition() - this->GetLastNodeCoords()), true);
				}

				if (CurrentGridCoords == FVector2D(1, 21))
				{
					this->GoToHisCorner();
				}
			}
		}
	}
}
