#ifndef BATTLE_INFO_H
#define BATTLE_INFO_H

/**
 * Abstract base class for battle information that can be passed to tank algorithms
 * This allows different implementations to provide different types of battle data
 */
class BattleInfo {
public:
    virtual ~BattleInfo() {}
};

#endif // BATTLE_INFO_H
