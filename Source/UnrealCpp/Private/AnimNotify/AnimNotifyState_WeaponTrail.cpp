// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_WeaponTrail.h"
#include "Player/ActionCharacter.h"


void UAnimNotifyState_WeaponTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!OwnerCharacter.IsValid())	//캐릭터 못 찾음
	{
		OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
		//
	}
	if (OwnerCharacter.IsValid())  //캐릭터 찾음
	{
		OwnerCharacter->OnWeaponTrailEnable(true);	//무기 이팩트 켜기
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("UAnimNotifyState_WeaponTrail::NotifyBegin : OwnerCharacter가 유효하지 않습니다."));
	}
}

void UAnimNotifyState_WeaponTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharacter.IsValid())	//캐릭터 찾으면 
	{
		OwnerCharacter->OnWeaponTrailEnable(false);		//무기 이팩트 끄기
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("UAnimNotifyState_WeaponTrail::NotifyBegin : OwnerCharacter가 유효하지 않습니다."));

	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

//액션 캐릭터에 함수 만들기 