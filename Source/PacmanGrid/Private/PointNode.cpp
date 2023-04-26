// Fill out your copyright notice in the Description page of Project Settings.

#include "PointNode.h"
#include "TestGridGameMode.h"


APointNode::APointNode()
{
    EEatId = NotEaten;

    Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = Collider;
    UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(Collider);
    FVector BoxDimension = FVector(40.f, 40.f, 120.f);
    Collider->SetBoxExtent(BoxDimension);

    //get MyGameInstance reference
    GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void APointNode::BeginPlay()
{
    Super::BeginPlay();
    //registrazione degli eventi di collisione attraverso AddDynamic
    Collider->OnComponentBeginOverlap.AddDynamic(this, &APointNode::OnBeginOverlap);

}

void APointNode::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (this->CheckNotEaten())
    {
        if (OtherActor->IsA(AGridPawn::StaticClass()))
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("collision pointNode")));
            APlayerController* PacmanController = GetWorld()->GetFirstPlayerController();
            const auto Pacman = Cast<AGridPawn>(PacmanController->GetPawn());
            this->setEaten();
            SetActorHiddenInGame(true); 

            int new_value = (GameMode->getPunteggio()) + 10;

            GameMode->setPunteggio(new_value);

            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Score: %d"), GameMode->getPunteggio()));
        }
    }

}