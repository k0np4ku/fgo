#pragma once

#include <koa/utils.h>

typedef enum class ActorType {
  NONE,
  PLAYER_MASTER,
  PLAYER_SERVANT,
  ENEMY_SERVANT,
  COMMAND,
  FIELD
};

typedef enum class WinResult {
  None,
  Normal,
  TimeLimit,
  Lose
};

struct ActDetectorBase {
  static void (*_OnCheatingDetected)(...);
  static void OnCheatingDetected(ActDetectorBase *);

  static void (*_Start)(...);
  static void Start(ActDetectorBase *);
};

struct InjectionDetector {
  static void (*_OnCheatingDetected)(...);
  static void OnCheatingDetected(InjectionDetector *, Any);
};

struct ServantEntity {
  static bool (*_get_IsEnemy)(...);
  static bool get_IsEnemy(ServantEntity *self);
};

struct BattleActorControl {
  static bool (*_get_isEnemy)(...);
  static bool get_isEnemy(BattleActorControl *self);
};

struct BattleServantData {
  static int (*_addNp)(...);
  static int addNp(BattleServantData *self, int intp, bool flg);

  static void (*_setInitQuest)(...);
  static void setInitQuest(BattleServantData *self);

  static void (*_setInitBattle)(...);
  static void setInitBattle(BattleServantData *self);

  static ServantEntity *(*_get_SvtEnt)(...);
  static ServantEntity *get_SvtEnt(BattleServantData *self);
};

struct BattleLogicTask {
  char _pad[0x8];
  ActorType actorType;

  static void (*_setActionCommand)(...);
  static void setActionCommand(BattleLogicTask *self, Any combo, Any command, Any index);

  static void (*_setActionSkill)(...);
  static void setActionSkill(BattleLogicTask *self, Any skillInfo, Any targetList, Any ptTarget, Any checkAlive, Any checkRevengeId);

  static void (*_setAddAttackCommand)(...);
  static void setAddAttackCommand(BattleLogicTask *self, Any combo, Any command);

  static void (*_setCommandSpell)(...);
  static void setCommandSpell(BattleLogicTask *self, Any skillId, Any targetList, Any ptTarget);
};

struct BattleLogic {
  static bool (*_isTutorial)(...);
  static bool isTutorial(BattleLogic *self);

  static void (*_sendWinBattleEvent)(...);
  static void sendWinBattleEvent(BattleLogic *self, WinResult winResult);

  static void (*_checkEndBattle)(...);
  static void checkEndBattle(BattleLogic *self, Any endproc, Any isEndOfTurn);
};

struct BattlePerformance {
  static void *(*_popDamageObject)(...);
  static void *popDamageObject(BattlePerformance *self, BattleActorControl *actor, int damage, Any damageData, Any damageMotion, Any isRandomDamage, Any damageNumberPosition, Any isNoDamageMotion, Any isRandomTiming, Any randomTime, Any npCharge, Any randomPos, Any isTreasureDevice);

  static void (*_ShowDamageViewAfter)(...);
  static void ShowDamageViewAfter(BattlePerformance *self, Any atkSvtData, Any defSvtData, BattleActorControl *defActor, Any index, int damage, Any damageData);
};
