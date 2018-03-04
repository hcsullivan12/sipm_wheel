// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SiPMWheel_CoreIODict

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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Test.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *wheelevdcLcLTest_Dictionary();
   static void wheelevdcLcLTest_TClassManip(TClass*);
   static void *new_wheelevdcLcLTest(void *p = 0);
   static void *newArray_wheelevdcLcLTest(Long_t size, void *p);
   static void delete_wheelevdcLcLTest(void *p);
   static void deleteArray_wheelevdcLcLTest(void *p);
   static void destruct_wheelevdcLcLTest(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::wheelevd::Test*)
   {
      ::wheelevd::Test *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::wheelevd::Test));
      static ::ROOT::TGenericClassInfo 
         instance("wheelevd::Test", "Test.h", 6,
                  typeid(::wheelevd::Test), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &wheelevdcLcLTest_Dictionary, isa_proxy, 4,
                  sizeof(::wheelevd::Test) );
      instance.SetNew(&new_wheelevdcLcLTest);
      instance.SetNewArray(&newArray_wheelevdcLcLTest);
      instance.SetDelete(&delete_wheelevdcLcLTest);
      instance.SetDeleteArray(&deleteArray_wheelevdcLcLTest);
      instance.SetDestructor(&destruct_wheelevdcLcLTest);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::wheelevd::Test*)
   {
      return GenerateInitInstanceLocal((::wheelevd::Test*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::wheelevd::Test*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *wheelevdcLcLTest_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::wheelevd::Test*)0x0)->GetClass();
      wheelevdcLcLTest_TClassManip(theClass);
   return theClass;
   }

   static void wheelevdcLcLTest_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_wheelevdcLcLTest(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::wheelevd::Test : new ::wheelevd::Test;
   }
   static void *newArray_wheelevdcLcLTest(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::wheelevd::Test[nElements] : new ::wheelevd::Test[nElements];
   }
   // Wrapper around operator delete
   static void delete_wheelevdcLcLTest(void *p) {
      delete ((::wheelevd::Test*)p);
   }
   static void deleteArray_wheelevdcLcLTest(void *p) {
      delete [] ((::wheelevd::Test*)p);
   }
   static void destruct_wheelevdcLcLTest(void *p) {
      typedef ::wheelevd::Test current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::wheelevd::Test

namespace {
  void TriggerDictionaryInitialization_SiPMWheel_CoreIODict_Impl() {
    static const char* headers[] = {
"Test.h",
0
    };
    static const char* includePaths[] = {
"/home/hunter/Software/root-6.08.02/include",
"/home/hunter/Desktop/wheel/event_display/pyqt/core/io/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SiPMWheel_CoreIODict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace wheelevd{class __attribute__((annotate("$clingAutoload$Test.h")))  Test;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SiPMWheel_CoreIODict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Test.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"wheelevd::Test", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SiPMWheel_CoreIODict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SiPMWheel_CoreIODict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SiPMWheel_CoreIODict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SiPMWheel_CoreIODict() {
  TriggerDictionaryInitialization_SiPMWheel_CoreIODict_Impl();
}
