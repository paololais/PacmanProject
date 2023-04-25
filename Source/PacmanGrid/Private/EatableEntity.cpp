// Fill out your copyright notice in the Description page of Project Settings.


#include "EatableEntity.h"


// Sets default values
AEatableEntity::AEatableEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

FVector2D AEatableEntity::getPosition()
{
    return this->EatableEntityPosition;
}

FVector AEatableEntity::GetTileCoordinates()
{
    return EatableEntityCoordinatesPosition;
}

void AEatableEntity::SetFruitPosition(const double InX, const double InY)
{
    EatableEntityPosition.Set(InX, InY);
}

int AEatableEntity::getPointValue()
{
    return 0;
}

bool AEatableEntity::CheckNotEaten()
{

    if (this->EEatId == NotEaten) return true;
    else return false;
}


void AEatableEntity::setEaten()
{
    this->EEatId = Eaten;
}

// Called when the game starts or when spawned
void AEatableEntity::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AEatableEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


