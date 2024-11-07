// Copyright 2021 ls Sun, Inc. All Rights Reserved.


#include "FileImporterAsync.h"
#include "Engine/World.h"
#include <HAL/FileManager.h>
#include <FbxImExPort/FbxSetting.h>
#include <FbxImExPort/LoadFbxC.h>
#include <FbxImExPort/InterData.h>
#include "CommonImport.h"

namespace FileIO
{
    class AsyncData
    {
		static void RenameActorRootComponent(AActor* actor, const TCHAR* NewName)
		{
			USceneComponent* rootComp = actor->GetRootComponent();
			if (rootComp)
			{
				rootComp->Rename(NewName);
			}
		}

        static void CreateActorWithMatrixAndComponent(TWeakObjectPtr<UWorld> world,MeshIO::InterMesh* mesh, AActor*& pActorCreated
            , TSubclassOf<AActor> actorClass)
        {

            //FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([world, &pActorCreated, mesh, actorClass]()
                {
                FName fname(UTF8_TO_TCHAR(mesh->strNode));
                FString fstrName = fname.ToString();
                if (fstrName.StartsWith(TEXT("UCX_")))
                    return;

                    bool bHaveMeshComponent = mesh->psubmeshs?true:false;
                    pActorCreated = CreateActor(world, !bHaveMeshComponent, actorClass);

                    
                    //如果有mesh则创建组件
                    if (bHaveMeshComponent)
                    {
                        //FBX右手矩阵转左手矩阵参照UE4源码：FbxUtilsImport.cpp/FFbxDataConverter::ConvertMatrix
                        //{
                        FPlane inx(mesh->matrix[0], -mesh->matrix[1], mesh->matrix[2], mesh->matrix[3]);
                        FPlane iny(-mesh->matrix[4], mesh->matrix[5], -mesh->matrix[6], -mesh->matrix[7]);
                        FPlane inz(mesh->matrix[8], -mesh->matrix[9], mesh->matrix[10], mesh->matrix[11]);
                        FPlane inw(mesh->matrix[12], -mesh->matrix[13], mesh->matrix[14], mesh->matrix[15]);
                        FMatrix fMat(inx, iny, inz, inw);
                        FTransform transform(fMat);

                        //}

                        UProceduralMeshComponent* ProMeshCmp = NewObject<UProceduralMeshComponent>(pActorCreated, fname);
                        USceneComponent* parentComponent = pActorCreated->GetRootComponent();
                        if (parentComponent)
                        {
                            ProMeshCmp->AttachToComponent(parentComponent, FAttachmentTransformRules::KeepRelativeTransform);
                        }
                        else
                        {
                            pActorCreated->SetRootComponent(ProMeshCmp);
                        }
                        ProMeshCmp->RegisterComponent();
                        pActorCreated->SetActorTransform(transform);

                    }
					else
					{
						RenameActorRootComponent(pActorCreated, *fstrName);
					}
                }
        }

        AActor* GameThreadCreateActor(MeshIO::InterMesh* mesh)
        {
            AActor* pActorCreated = 0;
            TWeakObjectPtr<UWorld> world = _world;
            TSubclassOf<AActor> actorClassuse = _actorClass;
            if (!ReadFileTask.IsValid())
            {
                CreateActorWithMatrixAndComponent(_world,mesh, pActorCreated, actorClassuse);
            }
            else
            {
                FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([world,mesh,&pActorCreated, actorClassuse]()
                    {
                        CreateActorWithMatrixAndComponent(world,mesh, pActorCreated, actorClassuse);
                    }, TStatId(), NULL, ENamedThreads::GameThread);

                FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
            }
            return pActorCreated;
        }

        static void GameThreadSetMeshMat(UProceduralMeshComponent* ProMeshCmp, int i, UMaterialInterface* pMat)
        {
            if (IsInGameThread())
            {
                ProMeshCmp->SetMaterial(i, pMat);
            }
            else
            {
                FFunctionGraphTask::CreateAndDispatchWhenReady([ProMeshCmp, i, pMat]()
                    {
                        ProMeshCmp->SetMaterial(i, pMat);
                    }, TStatId(), NULL, ENamedThreads::GameThread);
            }
        }

        void createFileActor(TArray<AActor*>& meshActors, const MeshIO::MeshVector& meshs, bool bRoot = false)
        {
            //float fPos = 0;
            int nNumMesh = meshs.count;
            for (int i = 0; i < meshs.count; ++i)
            {
                MeshIO::InterMesh* mesh = meshs.pMeshs[i];
                bool hasMesh = mesh->submeshCount > 0;
                AActor* pchild = createAMesh(mesh);
                if(pchild)
                    meshActors.Add(pchild);
                if (_fprogress&& hasMesh)
                {
                    //fPos = float(i+1) / nNumMesh;

                    if (!IsInGameThread())
                    {
                        FGraphEventRef FinishTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
                            {
                                float fPos = (++_curMeshIndex * 1.0f) / _numMeshAll;
                                _fprogress->CalProgress(fPos);
                            }, TStatId(), NULL, ENamedThreads::GameThread);
                        FTaskGraphInterface::Get().WaitUntilTaskCompletes(FinishTask);
                    }
                    else
                    {
                        float fPos = (++_curMeshIndex * 1.0f) / _numMeshAll;
                        _fprogress->CalProgress(fPos);
                    }
                }
            }
        }
        AActor* createAMesh(MeshIO::InterMesh* mesh)
        {
            AActor* actor = GameThreadCreateActor(mesh);
            if (!actor)return nullptr;
            //actor->Rename(UTF8_TO_TCHAR(mesh->strNode));
            //创建自身
            {
                if (mesh->submeshCount > 0)
                {
                    CreateUMesh(actor, mesh);
                }
            }
            //创建子
            TArray<AActor*> children;
            createFileActor(children, mesh->vChildrend);

            if (IsInGameThread())
            {
                for (AActor* child : children)
                {
                    child->AttachToActor(actor, FAttachmentTransformRules::KeepWorldTransform);
                }
            }
            else
            {
                FGraphEventRef AttachTask = FFunctionGraphTask::CreateAndDispatchWhenReady([actor, children]()
                    {
                        for (AActor* child : children)
                        {
                            child->AttachToActor(actor, FAttachmentTransformRules::KeepWorldTransform);
                        }
                    }, TStatId(), NULL, ENamedThreads::GameThread);

                _AllParseTasks.Add(AttachTask);
            }
           

            return actor;
        }

        void CreateUMesh(TWeakObjectPtr<AActor> weakActor, MeshIO::InterMesh* mesh)
        {
            AActor* actor = weakActor.Get();
            if (!actor || !mesh || mesh->submeshCount < 1)return;

            UProceduralMeshComponent* ProMeshCmp = Cast<UProceduralMeshComponent>(actor->GetComponentByClass(UProceduralMeshComponent::StaticClass()));
            if (!ProMeshCmp)return;
            //////////////////////////////////////////////////////////////////////////
            ForSeperateMesh TotalVertex;
            TotalVertex.Init(mesh->pts);
            //////////////////////////////////////////////////////////////////////////
            _AllParseTasks.Reserve(mesh->submeshCount * 2);
            bool bCreateCollision = _calCollision > eNoCollision;
            for (int i = 0; i < mesh->submeshCount; ++i)
            {
                const MeshIO::InterSubMesh& subMesh = mesh->psubmeshs[i];
                TArray<int32> Triangles;
                //////////////////////////////////////////////////////////////////////////
                TSharedPtr<ForSeperateMesh> subMeshVertex = MakeShared<ForSeperateMesh>();
                if (!TotalVertex.addMeshToSub(subMesh, *subMeshVertex, Triangles))
                    continue;
                int nMatId = subMesh.MatID;
                FileMats* fileMats = &_fileMats;
                if (IsInGameThread())
                {
                    TArray<FProcMeshTangent> Tangents;
                    ProMeshCmp->CreateMeshSection(i, subMeshVertex->Vertices, Triangles
                        , subMeshVertex->Normals, subMeshVertex->UV0
                        , subMeshVertex->VertexClrs, Tangents
                        , bCreateCollision);

                    UMaterialInterface* pMat = fileMats->getMat(nMatId);
                    GameThreadSetMeshMat(ProMeshCmp, i, pMat);
                }
                else
                {
                    FGraphEventRef task1 = FFunctionGraphTask::CreateAndDispatchWhenReady([ProMeshCmp, i, subMeshVertex, Triangles, bCreateCollision]()
                        {

                            TArray<FProcMeshTangent> Tangents;
                            ProMeshCmp->CreateMeshSection(i, subMeshVertex->Vertices, Triangles
                                , subMeshVertex->Normals, subMeshVertex->UV0
                                , subMeshVertex->VertexClrs, Tangents
                                , bCreateCollision);
                        }, TStatId(), NULL, ENamedThreads::GameThread);
                    _AllParseTasks.Add(task1);

                    //////////////////////////////////////////////////////////////////////////
                    
                    FGraphEventRef task2 = FFunctionGraphTask::CreateAndDispatchWhenReady([ProMeshCmp, i, nMatId, fileMats]()
                        {
                            UMaterialInterface* pMat = fileMats->getMat(nMatId);
                            GameThreadSetMeshMat(ProMeshCmp, i, pMat);
                        }, TStatId(), NULL, ENamedThreads::AnyBackgroundThreadNormalTask);

                    _AllParseTasks.Add(task2);
                }
                
            }
        }


    public:
        FGraphEventRef ReadFileTask;//任务
        TWeakObjectPtr < AActor> _rootActor;
        TWeakObjectPtr<UWorld> _world;
        FString _filePath;
        bool _CalTangentIfNone;
        TEnumAsByte<enum CalculateCollisionMethod> _calCollision;
        TSubclassOf<AActor> _actorClass;
        URLFProgress* _fprogress;
        void* _scene;
		unsigned int _numMeshAll;
		unsigned int _curMeshIndex;
        FileMats _fileMats;
        FGraphEventArray _AllParseTasks;//所有的任务

        AsyncData() : _rootActor(0), _CalTangentIfNone(false), _scene(0), _numMeshAll(0), _curMeshIndex(0)
        {

        }
        ~AsyncData()
        {

        }

        bool valid()const
        {
            MeshIO::FbxFullData* fulldata = (MeshIO::FbxFullData*)(_scene);
            if (!fulldata || fulldata->meshs.count < 1)return false;
            return true;
        }



        void FileLoadedAsync()
        {
            FbxIOSetting ios;
            ios.ConvertToAxis = 2;//转为右手Z向上，下面Mesh顶点和uv反转，参照了UnFbx::FFbxImporter::BuildStaticMeshFromGeometry
            ios.ConvertToUnit = 2;
            ios.CalTangentIfNone = _CalTangentIfNone;

            //ios.progress = progressFunction;
            loadFbxMemory(TCHAR_TO_UTF8(*_filePath), _scene, &ios);

            MeshIO::FbxFullData* fulldata = (MeshIO::FbxFullData*)(_scene);
            if (!fulldata || fulldata->meshs.count < 1)return;

            MeshIO::MeshVector meshs = fulldata->meshs;
            _fileMats.setMat_p(fulldata->mats);
			_numMeshAll = fulldata->mats.numMeshAll;
			if (_numMeshAll == 0)
				_numMeshAll = 1;

            TArray<AActor*> meshActors;
            createFileActor(meshActors, meshs,true);

            if (ReadFileTask.IsValid())//Async
                FTaskGraphInterface::Get().WaitUntilTasksComplete(_AllParseTasks);
            FString strRootName = UTF8_TO_TCHAR(fulldata->strRootName);
            freeFbxMemory(_scene);

            TWeakObjectPtr < AActor> root = _rootActor;
            //根节点归底归中
            if (root.IsValid())
            {
                FBox box = FileIO::GetActorBoundWithChildren(meshActors);
                FVector vCenter = box.GetCenter();
                vCenter.Z = box.Min.Z;
                root->SetActorLocation(vCenter);

                if (ReadFileTask.IsValid())//Async
                {
                    FGraphEventRef AttachTask = FFunctionGraphTask::CreateAndDispatchWhenReady([root, meshActors, strRootName]()
                        {
                            RenameActorRootComponent(root.Get(), *strRootName);

                            for (AActor* actor : meshActors)
                            {
                                if (root.IsValid())
                                    actor->AttachToActor(root.Get(), FAttachmentTransformRules::KeepWorldTransform);
                            }
                        }, TStatId(), NULL, ENamedThreads::GameThread);
                    FTaskGraphInterface::Get().WaitUntilTaskCompletes(AttachTask);
                }
                else
                {
                    RenameActorRootComponent(root.Get(), *strRootName);
                    for (AActor* actor : meshActors)
                    {
                        if (root.IsValid())
                            actor->AttachToActor(root.Get(), FAttachmentTransformRules::KeepWorldTransform);
                    }
                }
               
            }
            if (_fprogress)
            {
                if (!IsInGameThread())
                {
                    FGraphEventRef FinishTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
                        {
                            _fprogress->CalFinish(_rootActor.Get());
                        }, TStatId(), NULL, ENamedThreads::GameThread);
                    FTaskGraphInterface::Get().WaitUntilTaskCompletes(FinishTask);
                }
                else
                {
                    _fprogress->CalFinish(_rootActor.Get());
                }                
                _fprogress->RemoveFromRoot();
            }

            //_rootActor->GetRootComponent()->SetMobility(EComponentMobility::Type::Static);

        }
        static AActor* CreateActor(TWeakObjectPtr<UWorld> world, bool bCreateRoot = false, TSubclassOf<AActor> actorClass = nullptr)
        {
            if (!world.IsValid())
                return nullptr;
            //TSubclassOf<AActor> actorClassUse = actorClass ? actorClass : AActor::StaticClass();
            TSubclassOf<AActor> actorClassUse = actorClass;
            if (!actorClass)actorClassUse = AActor::StaticClass();

            AActor* aactor = world->SpawnActor<AActor>(actorClassUse);
            if (bCreateRoot)
            {
                USceneComponent* SceneComponent = NewObject<USceneComponent>(aactor, "SceneComponent");
                aactor->SetRootComponent(SceneComponent);
            }

            return aactor;
        }
    };

    TWeakObjectPtr<AActor> FileIO::LoadFile2ProceduralMesh(const FString& filePath
        , bool bAsync /*= true*/
        , URLFProgress* fprogress
        , TEnumAsByte<enum CalculateCollisionMethod> calCollision
        , bool CalTangentIfNone
        , TSubclassOf<AActor> actorClass
        , UWorld* world)
    {
        if (!IFileManager::Get().FileExists(*filePath))
        {
            if (fprogress)
                fprogress->RemoveFromRoot();
            return nullptr;
        }

        //FString fileName = FPaths::GetBaseFilename(filePath);


        TSharedPtr<AsyncData> asyncdata = MakeShared<AsyncData>();
        asyncdata->_filePath = filePath;
        //asyncdata->_actorClass = actorClass ? actorClass : AActor::StaticClass();
        asyncdata->_actorClass = actorClass;
		if (!asyncdata->_actorClass)asyncdata->_actorClass = AActor::StaticClass();

        asyncdata->_rootActor = AsyncData::CreateActor(world, true, asyncdata->_actorClass);
        asyncdata->_world = world;
        asyncdata->_calCollision = calCollision;
        asyncdata->_fprogress = fprogress;
        asyncdata->_CalTangentIfNone = CalTangentIfNone;
        if (bAsync)
        {
            asyncdata->ReadFileTask = FFunctionGraphTask::CreateAndDispatchWhenReady([asyncdata]()
                {
                    asyncdata->FileLoadedAsync();
                }, TStatId(), NULL, ENamedThreads::AnyBackgroundThreadNormalTask);
        }
        else
        {
            asyncdata->FileLoadedAsync();
        }


        return asyncdata->_rootActor;
    }
}
