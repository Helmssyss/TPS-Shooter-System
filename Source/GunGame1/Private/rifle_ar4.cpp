#include "rifle_ar4.h"

Arifle_ar4::Arifle_ar4(){
    maxMermi = 250;
    sarjordekiMermi = 25;
    atilanMermi = 25;
}

void Arifle_ar4::GunReload(){
    if (atilanMermi < sarjordekiMermi){
		int32 eksikMermi = sarjordekiMermi - atilanMermi;
        int32 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}