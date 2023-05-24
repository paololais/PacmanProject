// Fill out your copyright notice in the Description page of Project Settings.


#include "Fruit.h"

AFruit::AFruit()
{
    EEatId = NotEaten;

    Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = Collider;
    UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(Collider);
    FVector BoxDimension = FVector(40.f, 40.f, 120.f);
    Collider->SetBoxExtent(BoxDimension);
    Collider->SetGenerateOverlapEvents(false);
}

void AFruit::BeginPlay()
{
    Super::BeginPlay();
    //registrazione degli eventi di collisione attraverso AddDynamic
    Collider->OnComponentBeginOverlap.AddDynamic(this, &AFruit::OnBeginOverlap);
}


void AFruit::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AFruit::ShowFruit()
{
    EEatId = NotEaten;
    this->SetActorHiddenInGame(false);
    Collider->SetGenerateOverlapEvents(true);

    GetWorld()->GetTimerManager().SetTimer(ShowFruitTimer, this, &AFruit::HideFruit, 10.0f, false);
}

void AFruit::HideFruit()
{
    Collider->SetGenerateOverlapEvents(false);
    this->SetActorHiddenInGame(true);
}
