#include "rifle_ak47.h"
#include "Components/SkeletalMeshComponent.h"

Arifle_ak47::Arifle_ak47(){
    maxMermi = 200;
    sarjordekiMermi = 30;
    atilanMermi = 30;

}
void Arifle_ak47::BeginPlay(){
    Super::BeginPlay();
    
}

void Arifle_ak47::GunReload(){
    if (atilanMermi < sarjordekiMermi){
		int32 eksikMermi = sarjordekiMermi - atilanMermi;
        int32 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}