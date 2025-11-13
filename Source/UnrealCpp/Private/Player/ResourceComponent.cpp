// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ResourceComponent.h"
#include "GameFramework/Actor.h"



// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentStamina = MaxStamina; //시작에 스테미나 맥스로 채우기

}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
		// 내가 시간 누적을 직접 하는 경우
	//TimeSinceLastStaminaUse += DeltaTime;
	//if (TimeSinceLastStaminaUse > StaminaRegenCoolTime && CurrentStamina <= MaxStamina)
	//{
	//	CurrentStamina = FMath::Min(CurrentStamina + StaminaRegenAmount * DeltaTime, MaxStamina);
	//	UE_LOG(LogTemp, Warning, TEXT("Stamina Regen : %.1f"), CurrentStamina);
	//}

	// 타이머로 조건만 설정하는 경우
	//if (bRegenStamina)
	//{
	//	CurrentStamina += StaminaRegenAmount * DeltaTime;
	//	if (CurrentStamina > MaxStamina)
	//	{
	//		bRegenStamina = false;
	//		CurrentStamina = MaxStamina;
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("Stamina Regen : %.1f"), CurrentStamina);
	//}
}

void UResourceComponent::AddHealth(float InValue)
{
	float health = CurrentHealth + InValue;
	CurrentHealth = FMath::Clamp(health, 0, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (!IsAlive())
	{
		OnDie.Broadcast();
	}
}

void UResourceComponent::AddStamina(float InValue)
{
	//여기서 타이머셋
		// 스태미너 변경 처리
	//CurrentStamina += InValue;

	//TimeSinceLastStaminaUse = 0;	//시간을 직접 제어할때 쓰던 코드(예시 확인용)
	// 
		// 스태미너 변경 처리
	SetCurrentStamina(FMath::Clamp(CurrentStamina + InValue, 0, MaxStamina));

	//스테미너를 소비하고 일정 시간 뒤에 자동생성되게 타이머 세팅
	StaminaAutoRegenCoolTimerSet();


	//CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
	//OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	if (CurrentStamina <= 0)
	{
		//델리게이트로 스태미너가 떨어졌음을 알림
		OnStaminaEmpty.Broadcast();

	}


	//UE_LOG(LogTemp, Warning, TEXT("Stamina : %.1f"), CurrentStamina);
}


void UResourceComponent::StaminaAutoRegenCoolTimerSet()
{
	UWorld* world = GetWorld();
	FTimerManager& timerManager = world->GetTimerManager();

//GetWorldTimerManager().ClearTimer(StaminaCoolTimer);	// 해서 나쁠 것은 없음(SetTimer할 때 이미 내부적으로 처리하고 있다)
	timerManager.SetTimer(
		StaminaAutoRegenCoolTimer,	//StaminaAutoRegenCoolTimer핸들에 연결된 타이머세팅. (StaminaRegenCoolTime초 후에 한번만 람다식을 실행하는 타이머
		[this]() {
			//bRegenStamina = true;
			UE_LOG(LogTemp, Log, TEXT("리젠 타이머 실행"));

			//StaminaRegenTickTImer핸들에 연결될 타이머 세팅(StamainaRegenCool)
			//	StaminaTickInterval초를 첨에 한번 기다리고,
			//	StaminaTickInterval시간간격으로
			//	이 클래스의 staminaRegenPerTick함수를 실행하는 타이머

			UWorld* world = GetWorld();
			FTimerManager& timerManager = world->GetTimerManager();
			timerManager.SetTimer(
				StaminaRegenTickTimer,	
				this,
				&UResourceComponent::StaminaRegenPerTick,
				StaminaTickInterval,	// 실행 간격
				true,	// 반복 재생 여부
				StaminaTickInterval);	// 첫 딜레이
		},
		StaminaRegenCoolTime,
		false);
}

void UResourceComponent::StaminaRegenPerTick()
{
	float stamina = CurrentStamina + StaminaRegenAmountPerTick;

	//CurrentStamina += StaminaRegenAmountPerTick;	// 틱당 10
	//CurrentStamina += MaxStamina * StaminaRegenRatePerTick;	// 틱당 최대 스태미너의 10%

	if (stamina > MaxStamina)
	{
		stamina = MaxStamina;
		UWorld* world = GetWorld();
		FTimerManager& timerManager = world->GetTimerManager();
		timerManager.ClearTimer(StaminaRegenTickTimer);
	}
	SetCurrentStamina(stamina);


	//UE_LOG(LogTemp, Warning, TEXT("Stamina Regen : %.1f"), CurrentStamina);

}

