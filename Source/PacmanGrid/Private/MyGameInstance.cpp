// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::IncrementScore()
{
	Score++;
}

int32 UMyGameInstance::GetScore()
{
	return Score;
}
