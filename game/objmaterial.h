#pragma once

void SetObjectMaterial(uintptr_t libGTASA, uintptr_t rwObject, uintptr_t* pMaterialTextures);
uintptr_t LoadTexture(uintptr_t libGTASA, const char* texname);