// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME UCint
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackData.h"
#include "/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackExtrapolationResult.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TrackExtrapolationResult(void *p = 0);
   static void *newArray_TrackExtrapolationResult(Long_t size, void *p);
   static void delete_TrackExtrapolationResult(void *p);
   static void deleteArray_TrackExtrapolationResult(void *p);
   static void destruct_TrackExtrapolationResult(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TrackExtrapolationResult*)
   {
      ::TrackExtrapolationResult *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TrackExtrapolationResult >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TrackExtrapolationResult", ::TrackExtrapolationResult::Class_Version(), "TrackExtrapolationResult.h", 7,
                  typeid(::TrackExtrapolationResult), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TrackExtrapolationResult::Dictionary, isa_proxy, 4,
                  sizeof(::TrackExtrapolationResult) );
      instance.SetNew(&new_TrackExtrapolationResult);
      instance.SetNewArray(&newArray_TrackExtrapolationResult);
      instance.SetDelete(&delete_TrackExtrapolationResult);
      instance.SetDeleteArray(&deleteArray_TrackExtrapolationResult);
      instance.SetDestructor(&destruct_TrackExtrapolationResult);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TrackExtrapolationResult*)
   {
      return GenerateInitInstanceLocal((::TrackExtrapolationResult*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TrackExtrapolationResult*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TrackData(void *p = 0);
   static void *newArray_TrackData(Long_t size, void *p);
   static void delete_TrackData(void *p);
   static void deleteArray_TrackData(void *p);
   static void destruct_TrackData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TrackData*)
   {
      ::TrackData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TrackData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TrackData", ::TrackData::Class_Version(), "TrackData.h", 9,
                  typeid(::TrackData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TrackData::Dictionary, isa_proxy, 4,
                  sizeof(::TrackData) );
      instance.SetNew(&new_TrackData);
      instance.SetNewArray(&newArray_TrackData);
      instance.SetDelete(&delete_TrackData);
      instance.SetDeleteArray(&deleteArray_TrackData);
      instance.SetDestructor(&destruct_TrackData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TrackData*)
   {
      return GenerateInitInstanceLocal((::TrackData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TrackData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TrackExtrapolationResult::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackExtrapolationResult::Class_Name()
{
   return "TrackExtrapolationResult";
}

//______________________________________________________________________________
const char *TrackExtrapolationResult::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackExtrapolationResult*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TrackExtrapolationResult::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackExtrapolationResult*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrackExtrapolationResult::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackExtrapolationResult*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrackExtrapolationResult::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackExtrapolationResult*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TrackData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackData::Class_Name()
{
   return "TrackData";
}

//______________________________________________________________________________
const char *TrackData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TrackData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrackData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrackData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TrackExtrapolationResult::Streamer(TBuffer &R__b)
{
   // Stream an object of class TrackExtrapolationResult.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TrackExtrapolationResult::Class(),this);
   } else {
      R__b.WriteClassBuffer(TrackExtrapolationResult::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TrackExtrapolationResult(void *p) {
      return  p ? new(p) ::TrackExtrapolationResult : new ::TrackExtrapolationResult;
   }
   static void *newArray_TrackExtrapolationResult(Long_t nElements, void *p) {
      return p ? new(p) ::TrackExtrapolationResult[nElements] : new ::TrackExtrapolationResult[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrackExtrapolationResult(void *p) {
      delete ((::TrackExtrapolationResult*)p);
   }
   static void deleteArray_TrackExtrapolationResult(void *p) {
      delete [] ((::TrackExtrapolationResult*)p);
   }
   static void destruct_TrackExtrapolationResult(void *p) {
      typedef ::TrackExtrapolationResult current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TrackExtrapolationResult

//______________________________________________________________________________
void TrackData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TrackData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TrackData::Class(),this);
   } else {
      R__b.WriteClassBuffer(TrackData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TrackData(void *p) {
      return  p ? new(p) ::TrackData : new ::TrackData;
   }
   static void *newArray_TrackData(Long_t nElements, void *p) {
      return p ? new(p) ::TrackData[nElements] : new ::TrackData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrackData(void *p) {
      delete ((::TrackData*)p);
   }
   static void deleteArray_TrackData(void *p) {
      delete [] ((::TrackData*)p);
   }
   static void destruct_TrackData(void *p) {
      typedef ::TrackData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TrackData

namespace ROOT {
   static TClass *vectorlETrackExtrapolationResultgR_Dictionary();
   static void vectorlETrackExtrapolationResultgR_TClassManip(TClass*);
   static void *new_vectorlETrackExtrapolationResultgR(void *p = 0);
   static void *newArray_vectorlETrackExtrapolationResultgR(Long_t size, void *p);
   static void delete_vectorlETrackExtrapolationResultgR(void *p);
   static void deleteArray_vectorlETrackExtrapolationResultgR(void *p);
   static void destruct_vectorlETrackExtrapolationResultgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TrackExtrapolationResult>*)
   {
      vector<TrackExtrapolationResult> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TrackExtrapolationResult>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TrackExtrapolationResult>", -2, "vector", 386,
                  typeid(vector<TrackExtrapolationResult>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETrackExtrapolationResultgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TrackExtrapolationResult>) );
      instance.SetNew(&new_vectorlETrackExtrapolationResultgR);
      instance.SetNewArray(&newArray_vectorlETrackExtrapolationResultgR);
      instance.SetDelete(&delete_vectorlETrackExtrapolationResultgR);
      instance.SetDeleteArray(&deleteArray_vectorlETrackExtrapolationResultgR);
      instance.SetDestructor(&destruct_vectorlETrackExtrapolationResultgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TrackExtrapolationResult> >()));

      ::ROOT::AddClassAlternate("vector<TrackExtrapolationResult>","std::vector<TrackExtrapolationResult, std::allocator<TrackExtrapolationResult> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TrackExtrapolationResult>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETrackExtrapolationResultgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TrackExtrapolationResult>*)0x0)->GetClass();
      vectorlETrackExtrapolationResultgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETrackExtrapolationResultgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETrackExtrapolationResultgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TrackExtrapolationResult> : new vector<TrackExtrapolationResult>;
   }
   static void *newArray_vectorlETrackExtrapolationResultgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TrackExtrapolationResult>[nElements] : new vector<TrackExtrapolationResult>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETrackExtrapolationResultgR(void *p) {
      delete ((vector<TrackExtrapolationResult>*)p);
   }
   static void deleteArray_vectorlETrackExtrapolationResultgR(void *p) {
      delete [] ((vector<TrackExtrapolationResult>*)p);
   }
   static void destruct_vectorlETrackExtrapolationResultgR(void *p) {
      typedef vector<TrackExtrapolationResult> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TrackExtrapolationResult>

namespace {
  void TriggerDictionaryInitialization_UCint_Impl() {
    static const char* headers[] = {
"/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackData.h",
"/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackExtrapolationResult.h",
0
    };
    static const char* includePaths[] = {
"/localhome/henkel/Desktop/VMMSoftware/root_builddir/include",
"/home/henkel/Desktop/VMMData/prm_2023_B/include",
"/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include",
"/home/henkel/Desktop/VMMSoftware/root_builddir/include/",
"/home/henkel/Desktop/VMMData/prm_2023_B/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "UCint dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
struct __attribute__((annotate(R"ATTRDUMP(file_name@@@/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackExtrapolationResult.h)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TrackExtrapolationResult.h")))  __attribute__((annotate("$clingAutoload$/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackData.h")))  TrackExtrapolationResult;
struct __attribute__((annotate(R"ATTRDUMP(file_name@@@/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackData.h)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackData.h")))  TrackData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "UCint dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackData.h"
#include "/home/henkel/Desktop/VMMData/prm_2023_B/include/ROOT_include/TrackExtrapolationResult.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"", payloadCode, "@",
"TrackData", payloadCode, "@",
"TrackData::fgIsA", payloadCode, "@",
"TrackExtrapolationResult", payloadCode, "@",
"TrackExtrapolationResult::fgIsA", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("UCint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_UCint_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_UCint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_UCint() {
  TriggerDictionaryInitialization_UCint_Impl();
}
