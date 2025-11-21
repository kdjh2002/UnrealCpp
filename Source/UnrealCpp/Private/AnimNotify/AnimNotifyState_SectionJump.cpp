// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "Player/ActionCharacter.h"

void UAnimNotifyState_SectionJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	//UE_LOG(LogTemp, Warning, TEXT("NotifyBegin Fired! %s"), *NextSectionName.ToString());
	//콤보에 NextSectionName 이름넣기!!!	

	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
		//UE_LOG(LogTemp, Warning, TEXT("NotifyBegin2"));

	}

	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->SetSectionJumpNotify(this);
		//UE_LOG(LogTemp, Warning, TEXT("NotifyBegin3 "));

	}
}

void UAnimNotifyState_SectionJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharacter.IsValid())
	{
		//OwnerCharacter 설정 해제
		OwnerCharacter->SetSectionJumpNotify(nullptr);
		OwnerCharacter = nullptr;
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
