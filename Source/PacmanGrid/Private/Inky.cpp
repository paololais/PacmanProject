// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacmanPawn.h"
#include <Kismet/GameplayStatics.h>

AGridBaseNode* AInky::GetPlayerRelativeTarget()
{
	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	if (IsValid(Player)) {

		FVector PlayerDirection = Player->GetLastValidDirection();

		FVector2D PlayerCoords = Player->GetLastNodeCoords();

		FVector2D TargetCoords = PlayerCoords;

		AGridBaseNode* Target = nullptr;

		//direzione verso l'alto
		if (PlayerDirection == FVector(1, 0, 0))
		{
			TargetCoords += FVector2D(4, -4);
			if (TargetCoords.Y < 0)
			{
				TargetCoords.Y += 3;
			}
			if (TargetCoords.X > 29)
			{
				TargetCoords.X -= 3;
			}

			Target = (*(GridGenTMap.Find(TargetCoords)));

		}
		//verso il basso
		else if (PlayerDirection == FVector(-1, 0, 0))
		{
			TargetCoords += FVector2D(-4, 0);
			if (TargetCoords.X < 0)
			{
				TargetCoords.X += 3;
			}
			Target = (*(GridGenTMap.Find(TargetCoords)));
		}
		//verso dx
		else if (PlayerDirection == FVector(0, 1, 0))
		{
			TargetCoords += FVector2D(0, 4);
			//gestione offset se la tile è fuori dal labirinto
			if (TargetCoords.Y > 27)
			{
				TargetCoords.Y -= 3;
			}

			Target = (*(GridGenTMap.Find(TargetCoords)));
		}
		//verso sx
		else if (PlayerDirection == FVector(0, -1, 0))
		{
			TargetCoords += FVector2D(0, -4);
			if (TargetCoords.Y < 0)
			{
				TargetCoords.Y += 3;
			}
			Target = (*(GridGenTMap.Find(TargetCoords)));
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target Coordinates: (%f,%f)"), TargetCoords.X, TargetCoords.Y));
		return Target;
	}

	return Super::GetPlayerRelativeTarget();
}

AInky::AInky()
{
	CurrentGridCoords = FVector2D(17, 17);
}

void AInky::BeginPlay()
{
	Super::BeginPlay();
	//alterna scatter chase
	AlternateScatterChase();

	//this->SetChaseState();
}

void AInky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	const FVector Location(1750, 1750, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 17))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 17))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);

	this->SetChaseState();
}

void AInky::GoHome() {
	//this->SetDeadState();

	const AGridBaseNode* Target = *(GridGenTMap.Find(FVector2D(17, 17)));

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	//const FVector Dimensions(60, 60, 20);
	//DrawDebugBox(GetWorld(), PossibleNode->GetTileCoordinates(), Dimensions, FColor::Red);

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(17, 17))
	{
		this->SetChaseState();
	}
}

void AInky::GoToHisCorner()
{
	//nodo basso sx
	const AGridBaseNode* Target1 = *(GridGenTMap.Find(FVector2D(23, 18)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target1->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(23, 18))
	{
		//nodo alto sx
		const AGridBaseNode* Target2 = *(GridGenTMap.Find(FVector2D(5, 21)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target2->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}

		if (CurrentGridCoords == FVector2D(5, 21))
		{
			//nodo alto dx
			const AGridBaseNode* Target3 = *(GridGenTMap.Find(FVector2D(5, 26)));

			AGridBaseNode* PossibleNode3 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

			if (PossibleNode3)
			{
				this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode3->GetGridPosition() - this->GetLastNodeCoords()), true);
			}

			if (CurrentGridCoords == FVector2D(5, 26))
			{
				//nodo basso dx
				const AGridBaseNode* Target4 = *(GridGenTMap.Find(FVector2D(1, 26)));

				AGridBaseNode* PossibleNode4 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target3->GetGridPosition(), -(this->GetLastValidDirection()));

				if (PossibleNode4)
				{
					this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode4->GetGridPosition() - this->GetLastNodeCoords()), true);
				}

				if (CurrentGridCoords == FVector2D(1, 26))
				{
					this->GoToHisCorner();
				}
			}
		}
	}
}
