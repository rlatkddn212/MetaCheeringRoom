
#pragma once

#include "CoreMinimal.h"
#include <FbxImExPort/InterData.h>
#include <Engine/Texture.h>
#include <MeshDescription.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>

namespace FileIO
{
/**
 * 
 */
//RUNTIMELOADFBX_API
struct  ForSeperateMesh
{
    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FVector> Tangents;
    TArray<FVector2D> UV0;
    TArray<FColor> VertexClrs;

    void Init(const MeshIO::MeshPtVector& pts);
    int32 BigToSub(int32 subMeshVertexIndexInBig, ForSeperateMesh& subMeshRender, TMap<int32, int32>& bigVIndex2SubVIndex)const;
    bool addMeshToSub(const MeshIO::InterSubMesh& subMesh, ForSeperateMesh& subMeshRender, TArray<int32>& Triangles)const;
};

class FileMats
{
    TMap<FString, UTexture*> uTextureCreated;
    TMap<int, UMaterialInterface*> umatsCreated;
    MeshIO::FMaterials mats;
    TMap<FString, UMaterialInterface*> _userMats;//用户设置用户比较材质名称的材质映射

    //从图片创建纹理
    UTexture* GetOrCreateTexture(const FString& imagePath);

    //创建材质
    UMaterialInterface* CreateMaterial(MeshIO::MeshMaterial* mat);
    
    FCriticalSection _mutex;
public:
    FileMats();
    FileMats(const MeshIO::FMaterials& mats);
    void setMat_p(const MeshIO::FMaterials& mats);
    void addUserMats(const TArray<UMaterialInterface*>& userMats);
    //获得材质
    UMaterialInterface* getMat(int nMatId);    
};

FMeshDescription BuildMeshDescriptionFromInterMesh(MeshIO::InterMesh* mesh, FileMats* fileMats, TArray<UMaterialInterface*>& sectionsMat);


FBox GetActorBoundWithChildren(AActor* pActor, bool bNonColliding = false, bool bIncludeFromChildActors = true
    , TSubclassOf<AActor> ClassIgnore = nullptr);

FBox GetActorBoundWithChildren(const TSet<AActor*>& actors, bool bNonColliding = false, bool bIncludeFromChildActors = true
    , TSubclassOf<AActor> ClassIgnore = nullptr);

FBox GetActorBoundWithChildren(const TArray<AActor*>& actors, bool bNonColliding = false, bool bIncludeFromChildActors = true
    , TSubclassOf<AActor> ClassIgnore = nullptr);

}


