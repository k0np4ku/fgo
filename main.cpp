#include <Sneed.hpp>

#include "conf.h"
#include "fatego.h"

JavaVM *gJavaVM;
bool isHooked = false;

void *main_thread(void *) {
  try {
    if (isHooked)
      return NULL;

    while (gJavaVM == NULL)
      continue;

    isHooked = true;
    Sneed sneed("libil2cpp.so");

    JNIEnv *env;
    int envState = gJavaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (envState == JNI_EDETACHED) {
      if (gJavaVM->AttachCurrentThread(&env, NULL) != 0) {
        return NULL;
      }
    } else if (envState != JNI_OK) {
      return NULL;
    }

    if (loadConf(getDataPath(env))) {
      DEBUG("Sneed");

      sneed.hook("CodeStage.AntiCheat.Detectors", "ActDetectorBase", "OnCheatingDetected", ActDetectorBase::OnCheatingDetected, &ActDetectorBase::_OnCheatingDetected);
      sneed.hook("CodeStage.AntiCheat.Detectors", "ActDetectorBase", "Start", ActDetectorBase::Start, &ActDetectorBase::_Start);
      sneed.hook("CodeStage.AntiCheat.Detectors", "InjectionDetector", "OnCheatingDetected", InjectionDetector::OnCheatingDetected, &InjectionDetector::_OnCheatingDetected);
      sneed.hook("BattleLogic", "isTutorial", BattleLogic::isTutorial, &BattleLogic::_isTutorial);
      sneed.hook("BattleActorControl", "get_IsEnemy", BattleActorControl::get_isEnemy, &BattleActorControl::_get_isEnemy);
      sneed.hook("ServantEntity", "get_IsEnemy", ServantEntity::get_IsEnemy, &ServantEntity::_get_IsEnemy);
      sneed.hook("BattleServantData", "get_SvtEnt", BattleServantData::get_SvtEnt, &BattleServantData::_get_SvtEnt);
      sneed.hook("BattleServantData", "addNp", BattleServantData::addNp, &BattleServantData::_addNp);
      sneed.hook("BattleServantData", "setInitQuest", BattleServantData::setInitQuest, &BattleServantData::_setInitQuest);
      sneed.hook("BattleServantData", "setInitBattle", BattleServantData::setInitBattle, &BattleServantData::_setInitBattle);
      sneed.hook("BattleLogic", "sendWinBattleEvent", BattleLogic::sendWinBattleEvent, &BattleLogic::_sendWinBattleEvent);
      sneed.hook("BattleLogic", "checkEndBattle", BattleLogic::checkEndBattle, &BattleLogic::_checkEndBattle);
      sneed.hook("BattleLogicTask", "setActionCommand", BattleLogicTask::setActionCommand, &BattleLogicTask::_setActionCommand);
      sneed.hook("BattleLogicTask", "setActionSkill", BattleLogicTask::setActionSkill, &BattleLogicTask::_setActionSkill);
      sneed.hook("BattleLogicTask", "setAddAttackCommand", BattleLogicTask::setAddAttackCommand, &BattleLogicTask::_setAddAttackCommand);
      sneed.hook("BattleLogicTask", "setCommandSpell", BattleLogicTask::setCommandSpell, &BattleLogicTask::_setCommandSpell);
      sneed.hook("BattlePerformance", "popDamageObject", BattlePerformance::popDamageObject, &BattlePerformance::_popDamageObject);
      sneed.hook("BattlePerformance", "ShowDamageViewAfter", BattlePerformance::ShowDamageViewAfter, &BattlePerformance::_ShowDamageViewAfter);

      DEBUG("(Formerly Chuck's)");
    }
  } catch (...) {
    // ignore
  }

  if (gJavaVM != NULL) {
    gJavaVM->DetachCurrentThread();
    gJavaVM = NULL;
  }
  return NULL;
}

__attribute__((constructor)) void init() {
  try {
    if (validateIntegrity()) {
      pthread_t ptid;
      pthread_create(&ptid, NULL, &main_thread, NULL);
    }
  } catch (...) {
    // ignore
  }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
  gJavaVM = vm;
  return JNI_VERSION_1_6;
}