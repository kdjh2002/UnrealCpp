// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/ResourceComponent.h"
#include "Weapon/WeaponActor.h"


// Sets default values
AActionCharacter::AActionCharacter()
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

	Resource = CreateDefaultSubobject<UResourceComponent>(TEXT("PlayerResource"));

	bUseControllerRotationYaw = false;	//컨트롤러의 Yaw회전을 사용함 -> 컨트롤러의 yaw회전을 캐릭터에 적용
	GetCharacterMovement()->bOrientRotationToMovement = true;	//이동방향을 바라보게회전
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);

}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();  //ABP 객체 가져오기
	}
	if (Resource)
	{
		Resource->OnStaminaEmpty.AddDynamic(this, &AActionCharacter::SetWalkMode);
	}
	//게임 진행 중에 자주 변경되는 값은 시작 시점에서 리셋을 해줌
	bIsSprint = true;
	//UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina); 
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Velocity : %s"), GetVelocity().ToString());


	//CheckMove();

	StandRunStamina(DeltaTime);
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) //입력컴포넌트
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhanced)	//입력 컴포넌트가 향상된 입력 컴포넌트 일때
	{
		//IA_Move
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveInput);
		//AActioncharacter(주소):: 함수

		//IA_Sprint
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started,
				[this](const FInputActionValue& _) {
					SetSprintMode();
				});
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed,
				[this](const FInputActionValue& _) {
					SetWalkMode();
				});

		//IA_Roll,Attack, kick
		enhanced->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AActionCharacter::OnRollInput);
		enhanced->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AActionCharacter::OnAttackInput);
		enhanced->BindAction(IA_Kick, ETriggerEvent::Triggered, this, &AActionCharacter::OnKickInput);

	}
}

void AActionCharacter::OnAttackEnable(bool bEnable)
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->AttackEnable(bEnable);
	}
}

void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
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

void AActionCharacter::OnRollInput(const FInputActionValue& InValue)
{
		if (AnimInstance.IsValid())
		{
			if (!AnimInstance->IsAnyMontagePlaying() //&& CurrentStamina > RollStaminaCost()
				&& Resource->HasEnoughStamina(RollStaminaCost))	// 몽타주 재생중이 아니고 충분한 스태미너가 있을 때만 작동
			{
				//if (!GetLastMovementInputVector().IsNearlyZero()) //입력을 하는 중에만 즉시 회전
				//{
				//	SetActorRotation(GetLastMovementInputVector().Rotation()); //구르기 컨트롤  //마지막 입력방향으로 회전 시키기
				//}
				//Current -= RollStaminaCost
				//StaminaRegenTimerSet();
				Resource->AddStamina(-RollStaminaCost);// -= 10.0f; //매 프레임마다 1만큼 깎이기
				PlayAnimMontage(RollMontage);
			//	UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina);

			}
		}
}

void AActionCharacter::OnAttackInput(const FInputActionValue& InValue)
{
	if (AnimInstance.IsValid()
		&&Resource->HasEnoughStamina(AttackStaminaCost))	// 애님 인스턴스가 있고 스태미너도 충분할때
	{
		if (!AnimInstance->IsAnyMontagePlaying() )//&& CurrentStamina > RollStaminaCost()
		{
			//
			/*ActionCharacter->AttackEnable(false);*/
			//첫 번째 공격
			PlayAnimMontage(AttackMontage);
			Resource->AddStamina(-AttackStaminaCost);// -= 10.0f; //스테미너 감소
		}
		else if (AnimInstance->GetCurrentActiveMontage() == AttackMontage)	
			// 몽타주가 재생 중인데, AttackMontage가 재생중이면
		{
			// 콤보 공격
			SectionJumpForCombo();
		}
	}

}

void AActionCharacter::OnKickInput(const FInputActionValue& InValue)
{
	if (AnimInstance.IsValid()
		&& Resource->HasEnoughStamina(KickStaminaCost))	// 애님 인스턴스가 있고 스태미너도 충분할때

	{
		if (!AnimInstance->IsAnyMontagePlaying())//&& CurrentStamina > RollStaminaCost()
		{
			//첫 번째 공격
			PlayAnimMontage(KickMontage);
			Resource->AddStamina(-KickStaminaCost);// -= 10.0f; //스테미너 감소
		}
		else if (AnimInstance->GetCurrentActiveMontage() == KickMontage)
			// 몽타주가 재생 중인데, AttackMontage가 재생중이면
		{
			// 콤보 공격
			//SectionKickForCombo();

		}
	}

}


void AActionCharacter::SetSprintMode()
{

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	UE_LOG(LogTemp, Warning, TEXT("달리기 모드"));
	//UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina);
	bIsSprint = true;

}

void AActionCharacter::SetWalkMode()
{
	UE_LOG(LogTemp, Warning, TEXT("걷기 모드"));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprint = false;


}

void AActionCharacter::SectionJumpForCombo()
{
	if (SectionJumpNotify.IsValid() && bComboReady)  //SectionJumpNotify가 있고 콤보가 가능한 상태이면
	{
		UAnimMontage* current = AnimInstance->GetCurrentActiveMontage();	
		AnimInstance->Montage_SetNextSection(		//다음섹션으로 점프하기
			AnimInstance->Montage_GetCurrentSection(current),	//현재 섹션
			SectionJumpNotify->GetNexSectionName(),	//다음 섹션의  이름
			current);

		bComboReady = false;	//중복 실행 방지
		Resource->AddStamina(-AttackStaminaCost);// -= 10.0f; //스테미너 감소
	}
}


void AActionCharacter::StandRunStamina(float DeltaTime)
{
	if ((bIsSprint && !GetVelocity().IsNearlyZero())	//달리기 상태이고 움직이지 않고 있다.
		&& (AnimInstance.IsValid() && !AnimInstance->IsAnyMontagePlaying()))	//어떤 몽타쥬도 재생중이지 않다.(루트모션 때문에 Velocity 변경있음)
	{
		Resource->AddStamina(-SprintStaminaCost * DeltaTime);	// 스태미너 감소

	}

	//GetWorld()->GetFirst
}

//void AActionCharactor::CheckMove()
//{
//	CurrentStamina = FMath::Clamp(CurrentStamina, -0.0f, MAXStamina);
//
//	if (CurrentStamina <= 0)
//	{
//		SetWalkMode();
//		bIsSprint = false;
//	}
//	else
//	{
//		bIsSprint = true;
//	}
//	CurrentStamina += 0.5f; //매 프레임마다 1만큼 깎이기
//	UE_LOG(LogTemp, Warning, TEXT("IsSprint : %d"), bIsSprint);
//	UE_LOG(LogTemp, Warning, TEXT("CurrentStamina : %.1f"), CurrentStamina);
//}





