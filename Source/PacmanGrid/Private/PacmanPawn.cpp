// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPawn.h"
#include "PointNode.h"
#include "PowerNode.h"
#include "MyGameInstance.h"
#include "TestGridGameMode.h"

APacmanPawn::APacmanPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//// vettore di direzione inizializzato con zero (il pawn non si muove allo start del gioco fino a che non
	//   viene premuto uno dei tasti W-A-S-D )
	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);
	////posizione iniziale  del pawn nelle coordinate di griglia (1,1)
	CurrentGridCoords = FVector2D(1, 1);
	
	// get the game instance reference
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void APacmanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APacmanPawn::BeginPlay()
{
	Super::BeginPlay();
	//// posizione iniziale del pawn
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];

	GameMode = (ATestGridGameMode*)(GetWorld()->GetAuthGameMode());

}

void APacmanPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	APlayerController* PacmanController = GetWorld()->GetFirstPlayerController();
	SetNextNodeByDir(LastInputDirection);
}

void APacmanPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);
}

void APacmanPawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (AGridBaseNode* CurrTile = Cast<AGridBaseNode>(Hit.GetActor()))
	{
		FVector2D CurrCoords = CurrTile->GetGridPosition();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrCoords.X, CurrCoords.Y));
	}
}

void APacmanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Actions here
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APacmanPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APacmanPawn::SetHorizontalInput);
	// bind the OnClick function to InputComponent for test purpose
	InputComponent->BindAction("Click", IE_Pressed, this, &APacmanPawn::OnClick);
}



void APacmanPawn::HandleMovement()
{
	Super::HandleMovement();
	// si muove autonomamente fino a che il next node è walkable
	if (!TargetNode && !NextNode)
	{
		if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			SetLastValidDirection(LastInputDirection);
		}
		SetNodeGeneric(LastValidInputDirection);
	}
}

void APacmanPawn::SetTargetNode(AGridBaseNode* Node)
{
	Super::SetTargetNode(Node);
}

void APacmanPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APowerNode::StaticClass())) {
		const auto Point = Cast<APowerNode>(OtherActor);
		if (IsValid(Point) && Point->CheckNotEaten())
		{
			//punto mangiato viene settato a Eaten e nascosto
			Point->setEaten();
			Point->SetActorHiddenInGame(true);

			//Power Mode
			this->PowerModeOn();

			//Score System
			if (IsValid(GameInstance))
			{
				int new_value = (GameInstance->getScore()) + 50;

				GameInstance->setScore(new_value);
			}

			//decrement food count
			int new_foodcount = (TheGridGen->GetCountFood()) - 1;
			TheGridGen->SetCountFood(new_foodcount);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("There are %d points left"), new_foodcount));

			//check if pacman has eaten all the food
			if (TheGridGen->GetCountFood() == 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("WIN! you ate all the points")));
			}
		}
	}

	if (OtherActor->IsA(APointNode::StaticClass()))
	{
		const auto Point = Cast<APointNode>(OtherActor);
		if (IsValid(Point) && Point->CheckNotEaten())
		{
			//punto mangiato viene settato a Eaten e nascosto
			Point->setEaten();
			Point->SetActorHiddenInGame(true);

			//Score System
			if (IsValid(GameInstance))
			{
				int new_value = (GameInstance->getScore()) + 10;
				GameInstance->setScore(new_value);
			}

			//decrement food count
			int new_foodcount = (TheGridGen->GetCountFood()) - 1;
			TheGridGen->SetCountFood(new_foodcount);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("There are %d points left"), new_foodcount));
			
			//check if pacman has eaten all the food
			if (TheGridGen->GetCountFood() == 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("WIN! you ate all the points")));
			}
		}
	}
}

void APacmanPawn::PowerModeOn()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Power Mode on")));
	CurrentMovementSpeed = 800.0f;
	float PowerModeTime = 10;
	// set timer to call UFUNCTION that resets speed to default value
	// // should also enter in frightened mode, will do later
	GetWorld()->GetTimerManager().SetTimer(PowerModeTimer, this, &APacmanPawn::PowerModeOff, PowerModeTime, false);
}

void APacmanPawn::PowerModeOff()
{
	CurrentMovementSpeed = NormalMovementSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Power Mode Off")));
}