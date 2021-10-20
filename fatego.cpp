#include "fatego.h"
#include "conf.h"

void (*ActDetectorBase::_OnCheatingDetected)(...);
void ActDetectorBase::OnCheatingDetected(ActDetectorBase *) {}

void (*ActDetectorBase::_Start)(...);
void ActDetectorBase::Start(ActDetectorBase *) {}

void (*InjectionDetector::_OnCheatingDetected)(...);
void InjectionDetector::OnCheatingDetected(InjectionDetector *, Any) {}

bool (*BattleLogic::_isTutorial)(...);
bool BattleLogic::isTutorial(BattleLogic *self) {
  return BattleLogic::_isTutorial(self);
}

bool (*BattleActorControl::_get_isEnemy)(...);
bool BattleActorControl::get_isEnemy(BattleActorControl *self) {
  return BattleActorControl::_get_isEnemy(self);
};

bool (*ServantEntity::_get_IsEnemy)(...);
bool ServantEntity::get_IsEnemy(ServantEntity *self) {
  return ServantEntity::_get_IsEnemy(self);
}

ServantEntity *(*BattleServantData::_get_SvtEnt)(...);
ServantEntity *BattleServantData::get_SvtEnt(BattleServantData *self) {
  return BattleServantData::_get_SvtEnt(self);
}

/**
 * Always max NP
 */

BattleServantData *changeNoblePhantasmGauge(BattleServantData *self) {
  ServantEntity *entity = BattleServantData::get_SvtEnt(self);
  if (conf.Player.alwaysMaxNoblePhantasm && !ServantEntity::get_IsEnemy(entity)) {
    BattleServantData::_addNp(self, 10000, false);
  }
  return self;
}

int (*BattleServantData::_addNp)(...);
int BattleServantData::addNp(BattleServantData *self, int intp, bool flg) {
  return BattleServantData::_addNp(changeNoblePhantasmGauge(self), intp, flg);
}

void (*BattleServantData::_setInitQuest)(...);
void BattleServantData::setInitQuest(BattleServantData *self) {
  return BattleServantData::_setInitQuest(changeNoblePhantasmGauge(self));
}

void (*BattleServantData::_setInitBattle)(...);
void BattleServantData::setInitBattle(BattleServantData *self) {
  return BattleServantData::_setInitBattle(changeNoblePhantasmGauge(self));
}

/**
 * Always turn
 * Could have just NOP BattleLogicEnemyAi::taskAIAttack ~ idk
 */

#define doActions !conf.Battle.alwaysTurn || self->actorType != ActorType::ENEMY_SERVANT

void (*BattleLogicTask::_setActionCommand)(...);
void BattleLogicTask::setActionCommand(BattleLogicTask *self, Any combo, Any command, Any index) {
  if (doActions) {
    BattleLogicTask::_setActionCommand(self, combo, command, index);
  }
}

void (*BattleLogicTask::_setActionSkill)(...);
void BattleLogicTask::setActionSkill(BattleLogicTask *self, Any skillInfo, Any targetList, Any ptTarget, Any checkAlive, Any checkRevengeId) {
  if (doActions) {
    BattleLogicTask::_setActionSkill(self, skillInfo, targetList, ptTarget, checkAlive, checkRevengeId);
  }
}

void (*BattleLogicTask::_setAddAttackCommand)(...);
void BattleLogicTask::setAddAttackCommand(BattleLogicTask *self, Any combo, Any command) {
  if (doActions) {
    BattleLogicTask::_setAddAttackCommand(self, combo, command);
  }
}

void (*BattleLogicTask::_setCommandSpell)(...);
void BattleLogicTask::setCommandSpell(BattleLogicTask *self, Any skillId, Any targetList, Any ptTarget) {
  if (doActions) {
    BattleLogicTask::_setCommandSpell(self, skillId, targetList, ptTarget);
  }
}

/**
 * One turn win
 */
void (*BattleLogic::_sendWinBattleEvent)(...);
void BattleLogic::sendWinBattleEvent(BattleLogic *self, WinResult winResult) {
  BattleLogic::_sendWinBattleEvent(self, winResult);
}

void (*BattleLogic::_checkEndBattle)(...);
void BattleLogic::checkEndBattle(BattleLogic *self, Any endproc, Any isEndOfTurn) {
  if (conf.Battle.oneTurnWin && !BattleLogic::isTutorial(self)) {
    return BattleLogic::sendWinBattleEvent(self, WinResult::Normal);
  }
  BattleLogic::_checkEndBattle(self, endproc, isEndOfTurn);
}

/**
 * ATK multiplier/divisor
 */

int changeDamageValue(BattleActorControl *self, int damage) {
  return BattleActorControl::get_isEnemy(self) ? damage * conf.Player.atkMultiplier : damage / conf.Enemy.atkDivisor;
}

void *(*BattlePerformance::_popDamageObject)(...);
void *BattlePerformance::popDamageObject(BattlePerformance *self, BattleActorControl *actor, int damage, Any damageData, Any damageMotion, Any isRandomDamage, Any damageNumberPosition, Any isNoDamageMotion, Any isRandomTiming, Any randomTime, Any npCharge, Any randomPos, Any isTreasureDevice) {
  return BattlePerformance::_popDamageObject(self, actor, changeDamageValue(actor, damage), damageData, damageMotion, isRandomDamage, damageNumberPosition, isNoDamageMotion, isRandomTiming, randomTime, npCharge, randomPos, isTreasureDevice);
}

void (*BattlePerformance::_ShowDamageViewAfter)(...);
void BattlePerformance::ShowDamageViewAfter(BattlePerformance *self, Any atkSvtData, Any defSvtData, BattleActorControl *defActor, Any index, int damage, Any damageData) {
  BattlePerformance::_ShowDamageViewAfter(self, atkSvtData, defSvtData, defActor, index, changeDamageValue(defActor, damage), damageData);
}
