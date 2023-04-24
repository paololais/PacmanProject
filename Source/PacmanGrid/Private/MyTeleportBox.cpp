// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTeleportBox.h"
#include "GridPawn.h"

AMyTeleportBox::AMyTeleportBox()
{
	//when an actor begins to overlap with this teleport box the EnterTeleporter function will be called
	OnActorBeginOverlap.AddDynamic(this, &AMyTeleportBox::EnterTeleporter);
	//same thing happens when the overlapping ends
	OnActorEndOverlap.AddDynamic(this, &AMyTeleportBox::ExitTeleporter);

	teleporting = false;
}

void AMyTeleportBox::BeginPlay() {
	Super::BeginPlay();
}

void AMyTeleportBox::EnterTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this) {
		if (otherTele) {
			
			AGridPawn* Pawn = Cast<AGridPawn>(otherActor);

			//check se assegnazione corretta e se il pawn non si è appena teletrasportato dall'altro portale
			if (Pawn && !otherTele->teleporting) {
				teleporting = true;
				FVector NewLocation = otherTele->GetActorLocation();
				Pawn->SetActorLocation(NewLocation);
			}
		}
	}
}

void AMyTeleportBox::ExitTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this) {
		if (otherTele && !teleporting) {

			//this avoids indefinite loops
			otherTele->teleporting = false;
		}
	}
}
