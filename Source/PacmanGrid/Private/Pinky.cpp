// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinky.h"
#include "PacmanPawn.h"
#include "Kismet/GameplayStatics.h"

AGridBaseNode* APinky::GetPlayerRelativeTarget()
{
	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	if (IsValid(Player)) {

		FVector PlayerDirection = Player->GetLastValidDirection();

		FVector2D PlayerCoords = Player->GetLastNodeCoords();

		FVector2D TargetCoords = PlayerCoords;

		AGridBaseNode* Target = nullptr;

		//direzione verso l'alto
		if (PlayerDirection == FVector(1,0,0))
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
	
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target Coordinates: (%f,%f)"), TargetCoords.X, TargetCoords.Y));
		return Target;
	}

	return Super::GetPlayerRelativeTarget();
}

APinky::APinky()
{
	CurrentGridCoords = FVector2D(17, 14);
}

void APinky::BeginPlay()
{
	Super::BeginPlay();

	this->SetChaseState();
}

void APinky::RespawnGhostStartingPosition()
{
	Super::RespawnGhostStartingPosition();

	const FVector Location(1750, 1450, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(FVector2D(17, 14))));
	SetNextNode(*(GridGenTMap.Find(FVector2D(17, 14))));
	SetTargetNode(NextNode);

	SetActorLocation(Location);
	
	this->SetChaseState();
}

void APinky::GoHome() {
	this->SetDeadState();

	//target node (17,14)

	const AGridBaseNode* Target = *(GridGenTMap.Find(FVector2D(17, 14)));

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(17, 14))
	{
		this->SetChaseState();
	}
}

void APinky::GoToHisCorner()
{
}
