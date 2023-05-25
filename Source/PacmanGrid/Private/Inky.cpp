// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacmanPawn.h"
#include "Blinky.h"
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

	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	Blinky = Cast<ABlinky>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlinky::StaticClass()));
}

AGridBaseNode* AInky::GetPlayerRelativeTarget()
{	
	if (IsValid(Player) && IsValid(Blinky)) {

		FVector PlayerDirection = Player->GetLastValidDirection();
		FVector2D PlayerCoords = Player->GetCurrentGridCoords();
		
		FVector BlinkyDirection = Blinky->GetLastValidDirection();
		FVector2D BlinkyCoords = Blinky->GetCurrentGridCoords();

		TargetCoords = Player->GetCurrentGridCoords();

		Target = nullptr;

		//direzione verso l'alto
		if (PlayerDirection == FVector(1, 0, 0))
		{
			TargetCoords += FVector2D(2, -2);
			TargetCoords += 2 * (BlinkyCoords - TargetCoords).GetAbs();
		}
		//verso il basso
		else if (PlayerDirection == FVector(-1, 0, 0))
		{
			TargetCoords.X -= 2; //posizione offset tile
			TargetCoords -= 2 * (BlinkyCoords - TargetCoords).GetAbs();
		}
		//verso dx
		else if (PlayerDirection == FVector(0, 1, 0))
		{
			TargetCoords.Y += 2;
			TargetCoords += 2 * (BlinkyCoords - TargetCoords).GetAbs();
		}
		//verso sx
		else if (PlayerDirection == FVector(0, -1, 0))
		{

			TargetCoords.Y -= 2;
			TargetCoords -= 2 * (BlinkyCoords - TargetCoords).GetAbs();
		}

		//gestione offset fuori dal labirinto
		if (TargetCoords.X > 28)
		{
			TargetCoords.X = 28;
		}
		else if (TargetCoords.X<1)
		{
			TargetCoords.X = 1;
		}

		if (TargetCoords.Y > 26)
		{
			TargetCoords.Y = 26;
		}
		else if (TargetCoords.Y < 1)
		{
			TargetCoords.Y = 1;
		}

		//assegno target
		Target = (*(TheGridGen->TileMap.Find(TargetCoords)));

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
