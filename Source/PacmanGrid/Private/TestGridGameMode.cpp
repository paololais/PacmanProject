// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGridGameMode.h"
#include "GridPlayerController.h"
#include "GridPawn.h"
#include "PacmanPawn.h"
#include "GameOverWIdget.h"
#include "GameWinWidget.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"

ATestGridGameMode::ATestGridGameMode()
{
	PlayerControllerClass = AGridPlayerController::StaticClass();
	DefaultPawnClass = APacmanPawn::StaticClass();
}

ATestGridGameMode::~ATestGridGameMode() {
}


void ATestGridGameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	//IsLevelWin = false;

	
	// con questa posizione la tile di coordinate griglia (0,0) ha come
	// coordinate nello spazio dello spigolo inferiore sinistro (0,0) 
	// 
	// es. coordinate nello spazio della tile di coordinate di griglia (0, 0) 
	// 
	// (100,0) -------------- (100,100)
    //         |            |
	//		   |            |
	//         |  (50, 50)  |
	//         |            |
	//         |            |
	//  (0,0)  -------------- (0, 100)
	//

	FVector GridPos(50.0f, 50.0f, 0.0f);
	// N.B. tutte le funzioni andrebbero normalizzate se la posizione
	// della griglia non fosse pi� questa ma una qualsiasi nello spazio tridimensionale
	if (GridGeneratorClass != nullptr)
	{
		// spawn della griglia
		GField = GetWorld()->SpawnActor<AGridGenerator>(GridGeneratorClass, GridPos, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	};

	//PacmanPawn =  GetWorld()->SpawnActor<APacmanPawn>(PacmanClass, FVector((500) + 50, (900) + 50, 5.0f), FRotator(0, 0, 0));

	BlinkyPawn = GetWorld()->SpawnActor<ABlinky>(BlinkyClass, FVector((100 * 17) + 50, (100 * 10) + 50, 5.0f), FRotator(0, 0, 0));
	PinkyPawn = GetWorld()->SpawnActor<APinky>(PinkyClass, FVector((100 * 14) + 50, (100 * 13) + 50, 5.0f), FRotator(0, 0, 0));
	InkyPawn = GetWorld()->SpawnActor<AInky>(InkyClass, FVector((100 * 14) + 50, (100 * 11) + 50, 5.0f), FRotator(0, 0, 0));
	ClydePawn = GetWorld()->SpawnActor<AClyde>(ClydeClass, FVector((100 * 14) + 50, (100 * 16) + 50, 5.0f), FRotator(0, 0, 0));
}

void ATestGridGameMode::GameOver()
{
	IsGameOver = true;

	GetWorld()->GetTimerManager().ClearTimer(StopTimer);

	if (IsValid(BlinkyPawn))
	{
		BlinkyPawn->ClearTimer();
		BlinkyPawn->Destroy();
	}
	if (IsValid(InkyPawn)) {
		InkyPawn->ClearTimer();
		InkyPawn->Destroy();
	}
	if (IsValid(PinkyPawn)) {
		PinkyPawn->ClearTimer();
		PinkyPawn->Destroy();
	}

	if (IsValid(ClydePawn)) {
		ClydePawn->ClearTimer();
		ClydePawn->Destroy();
	}

	PacmanPawn = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	if (IsValid(PacmanPawn)) {
		PacmanPawn->ClearTimer();
		PacmanPawn->Destroy();
	}

	ShowGameOverScreen();
}


void ATestGridGameMode::GameWin()
{
	GetWorld()->GetTimerManager().ClearTimer(StopTimer);

	if (IsValid(BlinkyPawn))
	{
		BlinkyPawn->ClearTimer();
		BlinkyPawn->Destroy();
	}
	if (IsValid(InkyPawn)) {
		InkyPawn->ClearTimer();
		InkyPawn->Destroy();
	}
	if (IsValid(PinkyPawn)) {
		PinkyPawn->ClearTimer();
		PinkyPawn->Destroy();
	}

	if (IsValid(ClydePawn)) {
		ClydePawn->ClearTimer();
		ClydePawn->Destroy();
	}

	PacmanPawn = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	if (IsValid(PacmanPawn)) {
		PacmanPawn->ClearTimer();
		PacmanPawn->Destroy();
	}

	ShowGameWinScreen();
}

void ATestGridGameMode::RespawnPositions()
{
	if (IsValid(BlinkyPawn))
	{
		BlinkyPawn->RespawnGhostStartingPosition();
	}
	if (IsValid(InkyPawn)) {
		InkyPawn->RespawnGhostStartingPosition();
	}
	if (IsValid(PinkyPawn)) {
		PinkyPawn->RespawnGhostStartingPosition();
	}

	if (IsValid(ClydePawn)) {
		ClydePawn->RespawnGhostStartingPosition();
	}

	PacmanPawn = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	if (IsValid(PacmanPawn)) {
		PacmanPawn->RespawnStartingPosition();
	}
}

void ATestGridGameMode::StopMovement(float StopTime)
{
	if (IsValid(BlinkyPawn))
	{
		BlinkyPawn->PrimaryActorTick.bCanEverTick = false;
	}
	if (IsValid(InkyPawn)) {
		InkyPawn->PrimaryActorTick.bCanEverTick = false;
	}
	if (IsValid(PinkyPawn)) {
		PinkyPawn->PrimaryActorTick.bCanEverTick = false;
	}

	if (IsValid(ClydePawn)) {
		ClydePawn->PrimaryActorTick.bCanEverTick = false;
	}

	PacmanPawn = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	if (IsValid(PacmanPawn)) {
		PacmanPawn->SetPreviousDirection(PacmanPawn->GetLastValidDirection());
		PacmanPawn->SetLastValidDirection(FVector::ZeroVector);
		PacmanPawn->SetLastInputDirection(FVector::ZeroVector);
		PacmanPawn->RespawnStartingPosition();
	}
	GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &ATestGridGameMode::ResumeMovement, StopTime, false);
}

void ATestGridGameMode::ResumeMovement()
{
	if (IsValid(BlinkyPawn))
	{
		BlinkyPawn->PrimaryActorTick.bCanEverTick = true;
	}
	if (IsValid(InkyPawn)) {
		InkyPawn->PrimaryActorTick.bCanEverTick = true;
	}
	if (IsValid(PinkyPawn)) {
		PinkyPawn->PrimaryActorTick.bCanEverTick = true;
	}

	if (IsValid(ClydePawn)) {
		ClydePawn->PrimaryActorTick.bCanEverTick = true;
	}

	PacmanPawn = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	if (IsValid(PacmanPawn)) {
		PacmanPawn->SetLastInputDirection(PacmanPawn->GetPreviousDirection());
		PacmanPawn->RespawnStartingPosition();
	}
}

//use if you implement other levels
/*
void ATestGridGameMode::SetLevelWin()
{
	this->RespawnPositions();

	TMap<FVector2D, AEatableEntity*> EatableMap = GField->GetEatableEntityMap();

	for (auto It = EatableMap.CreateIterator(); It; ++It)
	{	
		It->Value->setNotEaten();
		It->Value->SetActorHiddenInGame(false);
	}
}
*/

void ATestGridGameMode::ShowGameOverScreen()
{
	// Verifica se il widget "GameOverWidget" � valido
	if (IsValid(GameOverWidgetClass))
	{
		GameOverWidget = Cast<UGameOverWIdget>(CreateWidget(GetWorld(), GameOverWidgetClass));

		if (GameOverWidget != nullptr)
		{
			// Mostra il widget sulla viewport
			GameOverWidget->AddToViewport();
		}
	}

	//play sound
	UGameplayStatics::PlaySound2D(this, GameOverSound);
}

void ATestGridGameMode::ShowGameWinScreen()
{
	if (IsValid(GameWinWidgetClass))
	{
		GameWinWidget = Cast<UGameWinWidget>(CreateWidget(GetWorld(), GameWinWidgetClass));

		if (GameWinWidget != nullptr)
		{
			// Mostra il widget sulla viewport
			GameWinWidget->AddToViewport();
		}
	}
	UGameplayStatics::PlaySound2D(this, GameWinSound);
}
