// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPawn.h"
#include "PointNode.h"
#include "PowerNode.h"
#include "TestGridGameMode.h"
#include "Sound/SoundCue.h"
#include "Math/Rotator.h"

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
	CurrentGridCoords = FVector2D(5, 12);

	//speed
	CurrentMovementSpeed = NormalMovementSpeed;
	
	// get the game instance reference
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void APacmanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int APacmanPawn::GetNumberOfGhostsKilled()
{
	return NumberOfGhostsKilled;
}

void APacmanPawn::SetNumberOfGhostsKilled(int n)
{
	this->NumberOfGhostsKilled = n;
}

void APacmanPawn::BeginPlay()
{
	Super::BeginPlay();
	//// posizione iniziale del pawn
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];

	GameMode = (ATestGridGameMode*)(GetWorld()->GetAuthGameMode());

	//get ghosts references
	Blinky = Cast<APhantomPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlinky::StaticClass()));
	Inky = Cast<APhantomPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AInky::StaticClass()));
	Pinky = Cast<APhantomPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APinky::StaticClass()));
	Clyde = Cast<APhantomPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AClyde::StaticClass()));
	
	if (IsValid(Pinky))
	{
		CurrentPreferredGhost = Pinky;
	}
	GetWorld()->GetTimerManager().SetTimer(CurrentPreferredGhost->GhostLeaveTimer, [this]() {
		CurrentPreferredGhost->ExitOnTimer();
		}, CurrentPreferredGhost->GhostLeaveTime, false);

}

void APacmanPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	APlayerController* PacmanController = GetWorld()->GetFirstPlayerController();
	SetNextNodeByDir(LastInputDirection);

	// Calcola la rotazione desiderata in base all'input
	float YawRotation = 0.0f;
	float PitchRotation = 0.0f;
	if (LastInputDirection.X > 0) // Va verso l'alto
	{
		YawRotation = 0.f;
	}
	else if (LastInputDirection.X < 0) // Va verso il basso
	{
		YawRotation = 180.f;
	}

	StaticMesh->SetWorldRotation(FRotator(PitchRotation, YawRotation, 0));

}

void APacmanPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);

	// Calcola la rotazione desiderata in base all'input
	float YawRotation = 0.0f;
	float PitchRotation = 0.0f;
	if (LastInputDirection.Y > 0) // Va verso destra
	{
		YawRotation = 90.0f;
		PitchRotation = 0.f;
	}
	else if (LastInputDirection.Y < 0) // Va verso sinistra
	{
		YawRotation = 90.f;;
		PitchRotation = 180.f;
	}

	StaticMesh->SetWorldRotation(FRotator(PitchRotation, YawRotation, 0));
}

void APacmanPawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (AGridBaseNode* CurrTile = Cast<AGridBaseNode>(Hit.GetActor()))
	{
		FVector2D CurrCoords = CurrTile->GetGridPosition();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrCoords.X, CurrCoords.Y));
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
	//eating power pill
	if (OtherActor->IsA(APowerNode::StaticClass())) {
		const auto Point = Cast<APowerNode>(OtherActor);
		if (IsValid(Point) && Point->CheckNotEaten())
		{
			//punto mangiato viene settato a Eaten e nascosto
			Point->setEaten();
			Point->SetActorHiddenInGame(true);

			//play sound
			UGameplayStatics::PlaySound2D(this, EatingPowerSound);

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

			if (IsValid(CurrentPreferredGhost))
			{
				//increment counter and check if can exit house
				CurrentPreferredGhost->CanExitHouse();
			}

			//check if pacman has eaten all the food
			if (TheGridGen->GetCountFood() == 0)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("WIN! you ate all the points")));
				this->Destroy();

				if (IsValid(Blinky))
				{
					Blinky->Destroy();
				}
				if (IsValid(Inky))
				{
					Inky->Destroy();
				}
				if (IsValid(Pinky))
				{
					Pinky->Destroy();
				}
				if (IsValid(Clyde))
				{
					Clyde->Destroy();
				}
				GameMode->ShowGameWinScreen();
			}
		}
	}

	//eating point
	if (OtherActor->IsA(APointNode::StaticClass()))
	{
		const auto Point = Cast<APointNode>(OtherActor);
		if (IsValid(Point) && Point->CheckNotEaten())
		{
			//punto mangiato viene settato a Eaten e nascosto
			UGameplayStatics::PlaySound2D(this, ConsumptionSound);

			Point->setEaten();
			Point->SetActorHiddenInGame(true);


			if (IsValid(CurrentPreferredGhost))
			{
				//reset GhostLeaveTimer when eating
				if (GetWorld()->GetTimerManager().IsTimerActive(CurrentPreferredGhost->GhostLeaveTimer)) {
					GetWorld()->GetTimerManager().ClearTimer(CurrentPreferredGhost->GhostLeaveTimer);
				}
				GetWorld()->GetTimerManager().SetTimer(CurrentPreferredGhost->GhostLeaveTimer, [this]() {
					CurrentPreferredGhost->ExitOnTimer();
					}, CurrentPreferredGhost->GhostLeaveTime, false);
				//increment counter and check if can exit house
				CurrentPreferredGhost->CanExitHouse();
			}
		
			//Score System
			if (IsValid(GameInstance))
			{
				int new_value = (GameInstance->getScore()) + 10;
				GameInstance->setScore(new_value);
			}

			//decrement food count
			int new_foodcount = (TheGridGen->GetCountFood()) - 1;
			TheGridGen->SetCountFood(new_foodcount);

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("There are %d points left"), new_foodcount));
			
			//check if pacman has eaten all the food
			if (TheGridGen->GetCountFood() == 0)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("WIN! you ate all the points")));
				GameMode->GameWin();
			}
		}
	}
}

void APacmanPawn::OnNodeReached()
{
	Super::OnNodeReached();
	if (CurrentGridCoords == (FVector2D(17, 13)) && (LastInputDirection.X < 0 || LastValidInputDirection.X < 0)) {
		// Ignora l'input e prosegue per la direzione valida
		LastValidInputDirection = PreviousDirection;
		SetNextNodeByDir(LastValidInputDirection, true);
		SetTargetNode(NextNode);
	}
}

void APacmanPawn::PowerModeOn()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Power Mode on")));
	CurrentMovementSpeed = PowerSpeed;

	float PowerModeTime = 10.0f;
	GetWorld()->GetTimerManager().SetTimer(PowerModeTimer, this, &APacmanPawn::PowerModeOff, PowerModeTime, false);
	//flash ghost skin when power mode is finishing
	float FlashGhostTime = 7.0f;
	GetWorld()->GetTimerManager().SetTimer(FlashGhostTimer, [this]() {
		if (Blinky)
			Blinky->FlashSkin();
		if (Inky)
			Inky->FlashSkin();
		if (Pinky)
			Pinky->FlashSkin();
		if (Clyde)
			Clyde->FlashSkin();
		}, FlashGhostTime, false);

	//set ghosts in frightened mode
	if (IsValid(Blinky))
	{
		Blinky->SetFrightenedState();
		//disattivo il timer della funzione alternateScatterChase
		Blinky->ClearTimer();
	}
	if (IsValid(Inky))
	{
		Inky->SetFrightenedState();
		Inky->ClearTimer();
	}
	if (IsValid(Pinky))
	{
		Pinky->SetFrightenedState();
		Pinky->ClearTimer();
	}
	if (IsValid(Clyde))
	{
		Clyde->SetFrightenedState();
		Clyde->ClearTimer();
	}
}

void APacmanPawn::PowerModeOff()
{
	CurrentMovementSpeed = NormalMovementSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Power Mode Off")));

	//set ghosts in Chase mode
	if (IsValid(Blinky) && (Blinky->IsFrightenedState()))
	{
		Blinky->AlternateScatterChase(0);
	}
	if (IsValid(Inky) && (Inky->IsFrightenedState()))
	{
		Inky->AlternateScatterChase(2);
	}
	if (IsValid(Pinky) && (Pinky->IsFrightenedState()))
	{
		Pinky->AlternateScatterChase(1);
	}
	if (IsValid(Clyde) && (Clyde->IsFrightenedState()))
	{
		Clyde->AlternateScatterChase(3);
	}

	this->SetNumberOfGhostsKilled(0);
}

void APacmanPawn::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(PowerModeTimer);
	GetWorld()->GetTimerManager().ClearTimer(FlashGhostTimer);
}

void APacmanPawn::RespawnStartingPosition() {
	this->LastNode = (*(GridGenTMap.Find(FVector2D(5, 12))));
	this->SetNextNode(*(GridGenTMap.Find(FVector2D(5, 12))));
	this->SetTargetNode(NextNode);
	this->LastInputDirection = FVector(0, 0, 0);
	this->LastValidInputDirection = FVector(0, 0, 0);
	this->SetActorLocation(FVector(550, 1250, GetActorLocation().Z));

	ResetPreferredGhost();

	if (IsValid(CurrentPreferredGhost))
	{
		GetWorld()->GetTimerManager().SetTimer(CurrentPreferredGhost->GhostLeaveTimer, [this]() {
			CurrentPreferredGhost->ExitOnTimer();
			}, CurrentPreferredGhost->GhostLeaveTime, false);
	}
}

void APacmanPawn::SetNextPreferredGhost()
{
	if (CurrentPreferredGhost == Pinky)
	{
		CurrentPreferredGhost = Inky;
	}
	else if (CurrentPreferredGhost == Inky)
	{
		CurrentPreferredGhost = Clyde;
	}
	else
	{
		CurrentPreferredGhost = nullptr;
	}
}

void APacmanPawn::ResetPreferredGhost() {
	if (IsValid(Pinky)) {
		CurrentPreferredGhost = Pinky;
	}
}