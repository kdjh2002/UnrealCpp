// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharactor.h"
#include "EnhancedInputComponent.h"

// Sets default values
AActionCharactor::AActionCharactor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActionCharactor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AActionCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) //입력컴포넌트
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhanced)	//입력 컴포넌트가 향상된 입력 컴포넌트 일때
	{
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharactor::OnMoveInput);
		//AActioncharacter(주소):: 함수
	}

}

void AActionCharactor::OnMoveInput(const FInputActionValue& InValue)
{
	FVector2D inputDirection = InValue.Get<FVector2D>();
	UE_LOG(LogTemp, Log, TEXT("Dir : (%.1f, %.1f)"), inputDirection.X, inputDirection.Y);
	UE_LOG(LogTemp, Log, TEXT("Dir : (%s)"), *inputDirection.ToString());

}