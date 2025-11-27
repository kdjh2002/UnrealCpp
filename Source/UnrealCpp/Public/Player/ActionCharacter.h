// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "Common/CommonEnums.h"
#include "InventoryOwner.h"
#include "ActionCharacter.generated.h"	//ㅁㅈㄱ 마지막


class UInputAction; 
//class USpringArmComponent;
class UResourceComponent;
class UStatusComponent;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter, public IInventoryOwner
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 아이템 추가 인터페이스 함수 구현
	virtual void AddItem_Implementation(EItemCode Code, int32 ItemCount) override;
	virtual void AddWeapon_Implementation(EWeaponCode Code, int32 UseCount) override;

	// 무기를 장비하는 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(EWeaponCode WeaponCode);


	//노티파이가 공격을 가능하게 만들라는 신호가 왔을떄 실행될 함수
	void OnAttackEnable(bool bEnable);

	//노티파이어가 무기의 트레일을 표시하라고 신호가 왔을 떄 실행될 함수->WeaponTrail
	void OnWeaponTrailEnable(bool bEnable);

	// 노티파이가 범위 공격을 하라고 신호가 왔을 때 실행될 함수
	void OnAreaAttack();

	
	UResourceComponent* GetResourceComponent() { return Resource; }
	UStatusComponent* GetStatusComponent() { return Status; }

	inline void SetSectionJumpNotify(UAnimNotifyState_SectionJump* InSectionJumpNotify)
	{
		SectionJumpNotify = InSectionJumpNotify;		//콤보 하나만될때 결정적 오류 주의
		bComboReady = InSectionJumpNotify != nullptr;
	}

	//테스트용 함수
	UFUNCTION(BlueprintCallable)
	void TestDropUsedWeapon();

	UFUNCTION(BlueprintCallable)
	void TestDropCurrentWeapon();


protected:
	//입력 방향 입력 받기
	void OnMoveInput(const FInputActionValue& InValue);

	//구르기 입력 받기
	void OnRollInput(const FInputActionValue& InValue);

	//공격 입력받기
	void OnAttackInput(const FInputActionValue& InValue);

	//발차기 입력받기
	void OnKickInput(const FInputActionValue& InValue);

	//달리기 모드 설정
	void SetSprintMode();


	//걷기 모드 설정(다이나믹에서 )
	UFUNCTION()
	void SetWalkMode();

	
	//걷기 모드 설정(다이나믹에서 )
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	//스테미너 확인
	//void CheckMove();
	//-----------------------------------
private:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//콤보용 섹션 점프 함수
	void SectionJumpForCombo();

	//달리기용 스테미너 소비함수
	void SpendRunStamina(float DeltaTime);

	// 다 쓴 무기를 버리는 함수
	void DropWeapon(EWeaponCode WeaponCode);

	//사용 중이던 무기를 버리는 함수
	void DropCurrentWeapon(EWeaponCode WeaponCode);


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	TObjectPtr<class UCameraComponent> PlayerCamera = nullptr;
	//TObjectPtr<USpringArmComponent> a = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	TObjectPtr<class UResourceComponent> Resource = nullptr;
	
	////실습 - statusComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Status")
	TObjectPtr<class UStatusComponent> Status = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TObjectPtr<class USceneComponent> DropLocation = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TObjectPtr<class UWeaponManagerComponent> WeaponManager = nullptr;


	//IA_인풋 액션들 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Sprint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Roll = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Kick = nullptr;

	//---------------------------------------------------

	//달리기 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float SprintSpeed = 1200.0f;//BS_Move에서 그래프 최대 길이로 맞추기
	//걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float WalkSpeed = 600.0f;//BS_Move에서 그래프 반길이로 맞춤

	//----------------------------------------------------------------
	//구르기 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	//UAnimMontage* //이것도 가능
	TObjectPtr<UAnimMontage>RollMontage = nullptr;

	//공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	//UAnimMontage* //이것도 가능
	TObjectPtr<UAnimMontage>AttackMontage = nullptr;

	//킥 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	//UAnimMontage* //이것도 가능
	TObjectPtr<UAnimMontage>KickMontage = nullptr;

	//---------------------------------------------------

	//달리기 상태일 떄 초당 스테미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float SprintStaminaCost = 20.0f;

	//달리기 상태일 떄 초당 스테미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float RollStaminaCost = 50.0f;


	// 공격을 하기 위해 필요한 스태미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float AttackStaminaCost = 15.0f;

	// 킥을 하기 위해 필요한 스태미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float KickStaminaCost = 20.0f;

	//움직이기 T/F
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprint = false;//canmove가 True여야 가능

	//플레이어가 현재 가지고 있는 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TWeakObjectPtr<class AWeaponActor>CurrentWeapon = nullptr;


	//----------------------------------------------------
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//AWeaponActor* Weapon;


	//---------------------------------------------------
private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance>AnimInstance = nullptr;

	//현재 진행중인 섹션점프 노티파이 스테이트
	//점프콤보
	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify;
	//헤더 넣어줌

	////플레이어가 현재 가지고 있는지 아닌지
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	//bool bWeaponUseEnded = false;

	//콤보가 가능한 상황인지 확인하기 위한 플래그
	bool bComboReady = false;

};
