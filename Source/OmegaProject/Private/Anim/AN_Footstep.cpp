// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AN_Footstep.h"

#include "Character/OPlayer.h"
#include "Component/OFootstepComponent.h"

void UAN_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	check(MeshComp);

	AOPlayer* Character = MeshComp ? Cast<AOPlayer>(MeshComp->GetOwner()) : nullptr;
	if (Character)
	{
		if (UOFootstepComponent* FootstepsComp = Character->GetFootstepsComp())
		{
			FootstepsComp->HandleFootstep(Foot);
		}
	}
}
