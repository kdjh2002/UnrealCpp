// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/ActionCharacter.h"
#include "Common/CommonEnums.h"
#include "WeaponActor.generated.h"


UCLASS()
class UNREALCPP_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual void OnWeaponActivate() {};
	virtual void OnWeaponDeactivate() {};

public:
	//무기 자체를 활성화/비활성화 하는 함수(비지빌리티)
	UFUNCTION(BlueprintCallable)
	void WeaponActivate(bool bActivate);

	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);	//콜리전 바꾸기

	//공격을 했을떄 실행되어야 할 함수
	UFUNCTION(BlueprintCallable)
	virtual void OnAttack() {};

	//이 무기로 공격할 수 있는 지 확인하는 함수
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CanAttack() { return true; }

	//무기를 획득했을떄
	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponPickuped(int InCount);

	virtual void PostInitializeComponents() override;

	inline EItemCode GetWeaponID() const { return WeaponID; }

	inline void SetWeaponOwner(AActionCharacter* InOwner) {WeaponOwner = InOwner;}
	//UFUNCTION(BlueprintCallable)
	//inline void SetWeaponOwner(AActionCharacter* InOwner) { WeaponOwner = InOwner; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemCode WeaponID = EItemCode::BasicWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UDamageType> DamageType = nullptr;

private:
	TWeakObjectPtr<AActionCharacter> WeaponOwner = nullptr;

};
