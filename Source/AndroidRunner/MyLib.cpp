// Fill out your copyright notice in the Description page of Project Settings.

#include "MyLib.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

MyLib::MyLib()
{
}

MyLib::~MyLib()
{
}

AEndlessGM* MyLib::GetEndlessGM(UWorld* world) {
	return Cast<AEndlessGM>(UGameplayStatics::GetGameMode(world));
}