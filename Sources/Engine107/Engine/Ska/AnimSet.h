#ifndef SE_INCL_ANIMSET_H
#define SE_INCL_ANIMSET_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif
#include <Engine/Templates/StaticArray.h>
#include <Engine/Base/Serial.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>

#define ANG_COMPRESIONMUL 182.041666666666666666666666666667f;

#define AN_LOOPING              (1UL<<0) // looping animation
#define AN_NORESTART            (1UL<<1) // dont restart anim
#define AN_PAUSED               (1UL<<2)
#define AN_CLEAR                (1UL<<3) // do new clear state before adding animation
#define AN_CLONE                (1UL<<4) // do new cloned state before adding animation
#define AN_NOGROUP_SORT         (1UL<<5) // dont sort animations by groups

#define CLEAR_STATE_LENGTH  0.2f
#define CLONED_STATE_LENGTH  0.2f

struct AnimPos
{
  UWORD ap_iFrameNum;  //frame number
  FLOAT3D ap_vPos;     //bone pos
};

struct AnimRotOpt
{
  UWORD aro_iFrameNum;  //frame number
  UWORD aro_ubH,aro_ubP;
  ANGLE aro_aAngle;
};

struct AnimRot
{
  UWORD ar_iFrameNum;  //frame number
  FLOATquat3D ar_qRot; //bone rot
};

struct Animation
{
  int an_iID;
  INDEX an_iFrames;
  FLOAT an_fSecPerFrame;
  FLOAT an_fTreshold;
  BOOL an_bCompresed;// are quaternions in animation compresed
  CStaticArray<struct MorphEnvelope> an_ameMorphs;
  CStaticArray<struct BoneEnvelope> an_abeBones;
  CTString an_fnSourceFile;// name of ascii aa file, used in Ska studio
  BOOL an_bCustomSpeed; // animation has custom speed set in animset list file, witch override speed from anim file
};

struct MorphEnvelope
{
  int me_iMorphMapID;
  CStaticArray<FLOAT> me_aFactors;
};

struct BoneEnvelope
{
  int be_iBoneID;
  Matrix12 be_mDefaultPos; // default pos
  CStaticArray<struct AnimPos> be_apPos;// array of compresed bone positions
  CStaticArray<struct AnimRot> be_arRot;// array if compresed bone rotations
  CStaticArray<struct AnimRotOpt> be_arRotOpt;// array if optimized compresed bone rotations
  FLOAT be_OffSetLen;
};

class ENGINE_API CAnimSet : public CSerial
{
public:
  CAnimSet();
  ~CAnimSet();
  void Optimize();
  void OptimizeAnimation(Animation &an, FLOAT fTreshold);
  void AddAnimation(Animation *pan);
  void RemoveAnimation(Animation *pan);
    
  void Read_t( CTStream *istrFile); // throw char *
  void Write_t( CTStream *ostrFile); // throw char *
  void Clear(void);
  SLONG GetUsedMemory(void);

  CStaticArray<struct Animation> as_Anims;
};

// if rotations are compresed does loader also fills array of uncompresed rotations
ENGINE_API void RememberUnCompresedRotatations(BOOL bRemember);
#endif  /* include-once check. */
