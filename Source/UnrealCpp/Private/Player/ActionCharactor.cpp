// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharactor.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	SpringArm->bUsePawnControlRotation = true;		//스프링암의 회전을 컨트롤러에 맞춤

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	bUseControllerRotationYaw = false;	//컨트롤러의 Yaw회전을 사용함 -> 컨트롤러의 yaw회전을 캐릭터에 적용
	GetCharacterMovement()->bOrientRotationToMovement = true;	//이동방향을 바라보게회전
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);

}

// Called when the game starts or when spawned
void AActionCharactor::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();  //ABP 객체 가져오기
	CurrentStamina = MAXStamina; //시작에 스테미나 맥스로 채우기
	UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina); 

	
}

// Called every frame
void AActionCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckMove();
}

// Called to bind functionality to input
void AActionCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) //입력컴포넌트
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhanced)	//입력 컴포넌트가 향상된 입력 컴포넌트 일때
	{
		//IA_Move
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharactor::OnMoveInput);
		//AActioncharacter(주소):: 함수

		//IA_Sprint
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started,
				[this](const FInputActionValue& _) {
					SetSprintMode();
				});
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Triggered, //누르고있을떄
				[this](const FInputActionValue& _) {
					CurrentStamina -= 1.0f; //매 프레임마다 1만큼 깎이기
					UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina);

				});
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed,
				[this](const FInputActionValue& _) {
					SetWalkMode();
				});

			//IA_Roll
		enhanced->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AActionCharactor::OnRollInput);
	}
}

void AActionCharactor::OnMoveInput(const FInputActionValue& InValue)
{
	FVector2D inputDirection = InValue.Get<FVector2D>();
	//UE_LOG(LogTemp, Log, TEXT("Dir : (%.1f, %.1f)"), inputDirection.X, inputDirection.Y);
	//UE_LOG(LogTemp, Log, TEXT("Dir : (%s)"), *inputDirection.ToString());

	//실습1. 좌우이동 메쉬움직이기
	FVector moveDirection(inputDirection.Y, inputDirection.X, 0.0f);

	///실습3 카메라대로 움직이게하기
	FQuat controlYawRotation = FQuat(FRotator(0, GetControlRotation().Yaw, 0)); //컨트롤러의 yaw회전을 따로 뽑아와스
	moveDirection = controlYawRotation.RotateVector(moveDirection); //이동 방향에 적용

	/*FRotator a;
	a.RotateVector*/


	AddMovementInput(moveDirection);
}

void AActionCharactor::OnRollInput(const FInputActionValue& InValue)
{
	if(CurrentStamina < 20.0f)
	{	
	SetWalkMode();
	}
	else 
	{
		if (AnimInstance.IsValid())
		{
			if (!AnimInstance->IsAnyMontagePlaying())
			{
				if (!GetLastMovementInputVector().IsNearlyZero()) //입력을 하는 중에만 즉시 회전
				{
					SetActorRotation(GetLastMovementInputVector().Rotation()); //구르기 컨트롤  //마지막 입력방향으로 회전 시키기
				}
				PlayAnimMontage(RollMontage);
				CurrentStamina -= 10.0f; //매 프레임마다 1만큼 깎이기
				UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina);

			}
		}
	}
}

void AActionCharactor::SetSprintMode()
{

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	UE_LOG(LogTemp, Warning, TEXT("달리기 모드"));
	UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina);

}

void AActionCharactor::SetWalkMode()
{
	UE_LOG(LogTemp, Warning, TEXT("걷기 모드"));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


}

void AActionCharactor::CheckMove()
{
	CurrentStamina = FMath::Clamp(CurrentStamina, -0.0f, MAXStamina);

	if (CurrentStamina <= 0)
	{
		SetWalkMode();
		CanMove = false;
	}
	else
	{
		CanMove = true;
	}
	CurrentStamina += 0.5f; //매 프레임마다 1만큼 깎이기
	UE_LOG(LogTemp, Warning, TEXT("CanMove : %d"), CanMove);
	UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina);
}


