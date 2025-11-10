// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharactor.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AActionCharactor::AActionCharactor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SocketOffset = FVector(0, 0, 250);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));



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


	//실습1. 좌우이동 메쉬움직이기
	FVector moveDirection(inputDirection.Y, inputDirection.X, 0.0f);
	AddMovementInput(moveDirection);
}